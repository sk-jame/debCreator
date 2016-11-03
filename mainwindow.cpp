#include <QApplication>
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

void MainWindow::createLintianWidget(){
    lintianWidget = new QWidget(0);
    lintianLabel = new QLabel(lintianWidget);
    //TODO scrollbar
    lintianLabel->setTextFormat(Qt::RichText);
    lintianLabel->setStyleSheet("background-color: black");
    lintianLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    pbExit = new QPushButton(tr("Выход"),lintianWidget);
    QVBoxLayout* linlay = new QVBoxLayout(lintianWidget);
    linlay->addWidget(lintianLabel);
    linlay->addSpacing( 5 );
    linlay->addWidget( pbExit );
    connect( pbExit, SIGNAL(clicked()), this, SLOT(close()));
    lintianWidget->hide();
}

void MainWindow::createMainWidget(){
    startWidget = new QWidget(this);
    this->setCentralWidget( startWidget );
    QVBoxLayout* mainLay = new QVBoxLayout( startWidget );
    QHBoxLayout* openLay = new QHBoxLayout;
    openLay->setSpacing( 5 );
    mainLay->setSpacing( 5 );

    label = new QLabel(tr("Необходимо ввести путь папке содержащую sysroot пакета"), startWidget);
    lePath = new QLineEdit(startWidget);
    pbOpenSave = new QPushButton(tr("Открыть"),startWidget);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close,
                                     Qt::Horizontal,
                                     startWidget
                                     );
    // настройка layouts
    openLay->addWidget(lePath);
    openLay->addWidget(pbOpenSave);

    mainLay->addWidget( label );
    mainLay->addItem( openLay );
    mainLay->addWidget( buttonBox );

    // connections
    connect( buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
             this,                                    SLOT(okOpenClicked()));

    connect( buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()),
             this,                                       SLOT(close()));

    connect( pbOpenSave, SIGNAL(clicked()),
             this,   SLOT(openClicked()));

    connect( startWidget, SIGNAL(destroyed()),
             openLay,     SLOT(deleteLater()));

    connect( startWidget, SIGNAL(destroyed()),
             mainLay,     SLOT(deleteLater()));

}

inline void MainWindow::checkInstalledSoft(QString softName){
    if ( process->execute("which " + softName) != 0 ){
        QMessageBox::critical(this, tr("Необходимо наличие программы"),
                              "Установите "+softName+"\nsudo apt-get install " + softName);
        this->close();
        return;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    this->setWindowTitle(QString("debCreator ") + QString(VERSION));
    createMainWidget();
    createLintianWidget();

    for(int i = 0; i < DebSettingsCommon::eEndOfWidgets; i++ ){
        switch (i) {
        case DebSettingsCommon::eControlWidget:{
            ControlSettingsWidget* widget = new ControlSettingsWidget();
            settingsWidgets << widget;
            connect(widget, SIGNAL(someData(QString,QString,QString)),
                    this,   SLOT(createFileName(QString,QString,QString)));
            }
            break;
        case DebSettingsCommon::eChangeLogWidget:{
            ChangelogSettingsWidget* widget = new ChangelogSettingsWidget();
            settingsWidgets << widget;
            }
            break;
        case DebSettingsCommon::eDirWidget:{
            DirSettingsWidget* widget = new DirSettingsWidget();
            settingsWidgets << widget;
            }
            break;
        case DebSettingsCommon::eScriptWidget:{
            ScriptSettingsWidget* widget = new ScriptSettingsWidget();
            settingsWidgets << widget;
            }
            break;
        case DebSettingsCommon::eCopyrightWidget:{
            CopyrightsSettingsWidget* widget = new CopyrightsSettingsWidget();
            settingsWidgets << widget;
            }
            break;
        }
        connect( settingsWidgets.last(), SIGNAL(finished(bool)),
                 this, SLOT(settingsWidgetFinished(bool)));
        connect( settingsWidgets.last(), SIGNAL(tryGoBack()),
                 this, SLOT(settingsWidgetGoBack()));
    }

    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, SIGNAL(readyReadStandardOutput()),
            this,    SLOT(readDatafromStdOut()));

    connect(process, SIGNAL(readyReadStandardError()),
            this,    SLOT(readDataFromStdError()));

    testDeb = false;
    writeMd5Sum = false;
}

MainWindow::~MainWindow(){
    while(settingsWidgets.count()){
        delete settingsWidgets.takeLast();
    }
    delete lePath;
    delete label;
    delete buttonBox;
    delete pbOpenSave;
    delete startWidget;
}

void MainWindow::readDatafromStdOut(){
    if ( writeMd5Sum ){
        QString str = process->readAllStandardOutput();
        QTextStream stream(&md5sumFile);
        str.replace("./", "");
        stream << str;
        stream.flush();
    }
    else if ( testDeb ){
        QString out = process->readAllStandardOutput();
        QString str = lintianLabel->text()+"</font><br>"+out;
        str.replace("E:", "<font color='red'>Error: ");
        str.replace("W:", "<font color='yellow'>Warning: ");
        lintianLabel->setText( str );
    }
}

void MainWindow::readDataFromStdError(){
    if ( fakeRootCheck ){
            fakeRootOut.clear();
            fakeRootOut.append( process->readAllStandardOutput() );
            fakeRootOut.append( "\n" );
            fakeRootOut.append( process->readAllStandardError() );
        }
}

void MainWindow::createFileName(QString name, QString version, QString arch){
    resultFileName = name + "_" + version + "_" + arch + ".deb";
}

/**
 * @brief MainWindow::okClicked
 * проверяем существование подпапок, проверяем существования папки DEBIAN
 * если что предлагаем переписать.
 */
void MainWindow::okOpenClicked(){
    checkInstalledSoft("dpkg");
    checkInstalledSoft("debconf");
    checkInstalledSoft("hashdeep");
    checkInstalledSoft("fakeroot");

    if ( lePath->text().isEmpty() ){
        QMessageBox::critical(this, tr("Пустой путь"),
                              tr("Введён пустой путь.\nВведите путь к каталогу содержащему sysroot"));
        return;
    }
    workDir = QDir(lePath->text());
    if ( workDir.count() == 0 ){
        QMessageBox::critical(this, tr("Пустой каталог"),
                              tr("Выбран пустой каталог.\nКаталог должен содержать sysroot"));
        return;
    }

    QFileInfoList infoList = workDir.entryInfoList();

    foreach(QFileInfo info, infoList){
        if ( info.fileName() == "." || info.fileName() == ".." ){
            infoList.removeOne(info);
        }
    }

    if ( infoList.count() == 1 && infoList.first().fileName() == "DEBIAN" ){
        QMessageBox::critical(this, tr("Пустой каталог"),
                              tr("Выбран каталог без sysroot.\nКаталог должен содержать sysroot"));
        return;
    }

    bool hasDebian = false;
    foreach( QFileInfo info, infoList ){
        if ( info.fileName() == "DEBIAN" ){
            hasDebian = true;
            break;
        }
    }

    if ( hasDebian ){
        int result = QMessageBox::question(this, tr("Затереть DEBIAN"),
                              tr("Выбранный каталог уже содержит папку DEBIAN\n\
                                 Содержимое каталога будет перезаписано.\nПродолжать?"),
                                 QMessageBox::Ok, QMessageBox::Cancel);

        if ( result != QMessageBox::Ok)
            return;
    }

    startWidget->hide();
    startWidget->setParent(0);
    settingsWidgets.first()->setWorkDir(workDir);
    settingsWidgets.first()->updateWidgetsData();
    settingsWidgets.first()->setParent(this);
    this->setCentralWidget(settingsWidgets.first());
}

/**
 * @brief MainWindow::openClicked
 * открывает окно QGetOpenFileDialog и значение сохраняет в leEditPath
 */
void MainWindow::openClicked(){
    QString path = QFileDialog::getExistingDirectory(this, "Выберете папку содержащую sysroot");
    if ( path.isEmpty() ) return;
    lePath->setText(path);
}


/**
 * @brief MainWindow::settingsWidgetFinished
 * @param shouldContinue
 */
void MainWindow::settingsWidgetFinished(bool shouldContinue){
    DebSettingsCommon* debWidget = qobject_cast<DebSettingsCommon*>(sender());
    debWidget->hide();
    if ( shouldContinue == false ){
        this->close();
        return;
    }
    debWidget->setParent(0);//now it is saved
    int index = settingsWidgets.indexOf(debWidget);
    // если дальше есть ещё виджеты - показываем их
    if ( ++index < DebSettingsCommon::eEndOfWidgets ){
        settingsWidgets.at(index)->show();
        settingsWidgets.at(index)->updateWidgetsData();
        settingsWidgets.at(index)->setParent( this );
        this->setCentralWidget(settingsWidgets.at(index));
        return;
    }
    // создаём deb архив

    process->execute("touch "+ workDir.absolutePath() + "/DEBIAN/conffiles");
    process->execute("chmod 644 "+ workDir.absolutePath() + "/DEBIAN/conffiles");
    process->execute("touch "+ workDir.absolutePath() + "/DEBIAN/md5sum");
    process->execute("touch "+ workDir.absolutePath() + "/DEBIAN/menu");
    process->execute("chmod 644 "+ workDir.absolutePath() + "/DEBIAN/menu");
    process->execute("touch "+ workDir.absolutePath() + "/DEBIAN/rules");
    process->execute("chmod 644 "+ workDir.absolutePath() + "/DEBIAN/rules");
    process->execute("touch "+ workDir.absolutePath() + "/DEBIAN/watch");
    process->execute("chmod 644 "+ workDir.absolutePath() + "/DEBIAN/watch");

    // md5sum
    md5sumFile.setFileName(workDir.absolutePath()+"/DEBIAN/md5sum");
    if ( !md5sumFile.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        QMessageBox::critical(this, tr("Ошибка записи"),
                              tr("Не смог создать(перезаписать) файл.\nПроверьте права доступа к каталогу"));
        this->close();
        return;
    }
    writeMd5Sum = true;
    QDir::setCurrent(workDir.absolutePath());
    foreach( QString path, workDir.entryList()){
        if ( path != "." && path != ".." && path != "DEBIAN"){
            qDebug()<<"md5deep -lrs ./" + path + " ";
            process->start("md5deep -lrs " + path + " ");
            process->waitForFinished();
        }
    }
    writeMd5Sum = false;
    md5sumFile.close();
    process->execute("chmod 644 "+ workDir.absolutePath() + "/DEBIAN/md5sum");
    // !md5sum

    QDir temp = workDir.absolutePath();
    temp.cdUp();
    QDir::setCurrent(temp.absolutePath());

    // fakeroot ( create deb )
    fakeRootCheck = true;
    process->start("fakeroot dpkg-deb --build " + workDir.absolutePath());
    process->waitForFinished();
    fakeRootCheck = false;
    if (!fakeRootOut.isEmpty()){
        QMessageBox::critical(this, tr("Ошибка создания пакета"),
                              tr("Не смог создать deb-пакет.\nВывод fakeroot:\n") + fakeRootOut);
        this->close();
        return;
    }
    // !fakeroot
    process->execute("mv " + workDir.absolutePath() + ".deb " + workDir.absolutePath() + "/" + resultFileName);

    if ( !QFile::exists(workDir.absolutePath() + "/" + resultFileName)){
        QMessageBox::critical(this, tr("Ошибка создания пакета"),
                              tr("Не смог создать deb-пакет."));
        this->close();
        return;
    }

    int res = QMessageBox::question(this, "Закончено", "Пакет " + resultFileName +
                             "создан в каталоге " + workDir.absolutePath() + "\nХотите проверить пакет?",
                                    QMessageBox::Ok, QMessageBox::No);
    if ( res == QMessageBox::No ){
        this->close();
        return;
    }

    checkInstalledSoft("lintian");
    testDeb = true;
    lintianWidget->setParent( this );
    this->setCentralWidget(lintianWidget);
    lintianWidget->show();
    process->start("lintian "+workDir.absolutePath() + "/" + resultFileName);
    process->waitForFinished();
    testDeb = false;
}

void MainWindow::settingsWidgetGoBack(){
    DebSettingsCommon* debWidget = qobject_cast<DebSettingsCommon*>(sender());
    int index = settingsWidgets.indexOf( debWidget );
    debWidget->setParent(0);//now it is saved
    debWidget->hide();
    if ( --index == -1 ){
        startWidget->show();
        startWidget->setParent( this );
        this->setCentralWidget( startWidget );
    }
    else{
        settingsWidgets.at(index)->show();
        settingsWidgets.at(index)->updateWidgetsData();
        settingsWidgets.at(index)->setParent( this );
        this->setCentralWidget(settingsWidgets.at(index));
    }
}
