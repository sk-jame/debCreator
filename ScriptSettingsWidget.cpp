#include "ScriptSettingsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>

ScriptSettingsWidget::ScriptSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir, parent){
    QVBoxLayout* lay = new QVBoxLayout( this );
    lay->setSpacing( 5 );
    for(int i = 0; i < endOfScripts; i++ ){
        ChoosePath *str = new ChoosePath;
        QString text;
        switch (i) {
        case preInst:
            text = QString(tr("Выберете скрипт выполняемый перед установкой"));
            break;
        case postInst:
            text = QString(tr("Выберете скрипт выполняемый после установки"));
            break;
        case preRm:
            text = QString(tr("Выберете скрипт выполняемый перед удалением"));
            break;
        case postRm:
            text = QString(tr("Выберете скрипт выполняемый после удаления"));
            break;
        }
        str->grBox = new QGroupBox(text, this);
        str->lePath = new QLineEdit(str->grBox);
        str->pbOpen = new QPushButton(tr("Выбрать"), str->grBox);
        QHBoxLayout* tempLay = new QHBoxLayout(str->grBox);
        tempLay->addWidget(str->lePath);
        tempLay->addSpacing(5);
        tempLay->addWidget(str->pbOpen);

        connect(str->pbOpen, SIGNAL(clicked()), this, SLOT(pbOpenClicked()));
        lay->addWidget(str->grBox);
        widgetsList << str;
    }

    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbBack = new QPushButton(tr("Назад"),this);
    pbBack->setShortcut(QKeySequence("ALT+B"));
    pbExit = new QPushButton(tr("Выход"),this);
    QHBoxLayout* tempLay = new QHBoxLayout;
    tempLay->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding));
    tempLay->addWidget(pbExit);
    tempLay->addSpacing(5);
    tempLay->addWidget(pbBack);
    tempLay->addSpacing(5);
    tempLay->addWidget(pbNext);

    lay->addItem( tempLay );


    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));


    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
}

ScriptSettingsWidget::~ScriptSettingsWidget(){
    while(!widgetsList.isEmpty()){
        delete widgetsList.takeAt(0);
    }
    delete pbNext;
    delete pbExit;

}

void ScriptSettingsWidget::pbOpenClicked(){
    QString path = QFileDialog::getOpenFileName(this, "Выберете файл скрипта", workDir.absolutePath());
    if ( path.isEmpty() ) return;
    QPushButton* pb = qobject_cast<QPushButton*>(sender());
    foreach(ChoosePath* str, widgetsList){
        if ( str->pbOpen == pb ){
            str->lePath->setText(path);
            return;
        }
    }
}

void ScriptSettingsWidget::saveChangesAndGoNext(){
    for(int i = 0; i < widgetsList.count(); i++ ){
        ChoosePath* str = widgetsList.at(i);
        QString fileName;
        switch (i) {
        case preInst:
            fileName = QString("preinst.sh");
            break;
        case postInst:
            fileName = QString("postinst.sh");
            break;
        case preRm:
            fileName = QString("prerm.sh");
            break;
        case postRm:
            fileName = QString("postrm.sh");
            break;
        }
        if ( str->lePath->text().isEmpty() ){
            // create Empty files
            bool ok;
            openFileForSave(fileName, ok, 777);
            closeFile();
        }
        else{
            QString target;
            if ( workDir.absolutePath().endsWith("/DEBIAN")){
                target = QString( workDir.absolutePath() + "/" + fileName );
            }
            else if ( workDir.absolutePath().endsWith("/DEBIAN/")){
                target = QString( workDir.absolutePath() + fileName );
            }
            else{
                target = QString( workDir.absolutePath() + "/DEBIAN/" + fileName );
            }
            if ( QFile::exists(target) ){
                QFile::remove(target);
            }
            QFile::copy(str->lePath->text(), target);
        }
    }

    emit finished(true);
}
