#include "ChangelogSettingsWidget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGridLayout>
#include "QSavedProperties.h"
#include <QDateTime>

ChangelogSettingsWidget::ChangelogSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir, parent){
    teNewChanges = new QTextEdit(this);
    teOldChanges = new QTextEdit(this);
    label = new QLabel(tr("Введите список новых изменений."
                          "\nКаждое изменение должно быть с новой строки."
                          "\nЕсли дле перечислений нужно использовать +."
                          "\nК примеру:\n"
                          "* Changes prompted by lintian:\n"
                          "+ debian/copyright: move license grants into Comment sections\n"
                          "+ Bump build dependency on dpkg-dev to 1.17.14 for build profile support"
                            "(Closes: #813811)\n"
                          "+ Call ldconfig in libperl5.22 postrm script.\n"
                          "+ Override lintian warning about file conflict over /usr/bin/perldoc.\n"
                          "+ Install manual pages for perl5.22-<arch> and cpan5.22-<arch>.\n"
                          "+ Fix POD errors in Memoize, Encode-Unicode and ok.\n"), this);

    cbUrgency = new QComboBox(this);
    QStringList items;
    items << "low" << "medium" << "high" << "emergency" << "critical";
    cbUrgency->addItems(items);
    urgencyLabel = new QLabel(tr("Необходимость:"), this);

    cbDistribution = new QComboBox(this);
    items.clear();
    items << "stable" << "unstable" << "experimental";
    cbDistribution->addItems(items);
    distributionLabel = new QLabel(tr("Распространение:"), this);

    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbBack = new QPushButton(tr("Назад"),this);
    pbBack->setShortcut(QKeySequence("ALT+B"));
    pbExit = new QPushButton(tr("Выход"),this);


    QGridLayout* lay = new QGridLayout(this);
    lay->addWidget(cbUrgency,  0, 0, 1, 3);
    lay->addWidget(teOldChanges, 2, 0, 1, 3 );
    lay->addWidget(pbExit, 3, 0, 1, 1 );
    lay->addWidget(pbBack, 3, 1, 1, 1 );
    lay->addWidget(pbNext, 3, 2, 1, 1 );



    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
}

ChangelogSettingsWidget::~ChangelogSettingsWidget(){
    delete teOldChanges;
    delete pbNext;
    delete pbBack;
    delete pbExit;
    //delete helpLabel;
    delete label;
}

void ChangelogSettingsWidget::updateWidgetsData(){
    QString res = this->tryToReadDataFromFile("changelog");
    if ( res.isEmpty() == false ){
        teOldChanges->setPlainText( res );
    }
    QString temp = tr("Для удобства текущее время и создатель пакета(можно скопировать):\n\t-- ");
    QDateTime dt = QDateTime::currentDateTime();
    QLocale locale = QLocale(QLocale::English, QLocale::UnitedStates); // set the locale you want here

    temp.append( this->getMaintainer() );
    temp.append( locale.toString(dt.date(), " ddd, dd MMM yyyy "));
    temp.append( locale.toString(dt.time(), "hh:mm:ss +0300")); //TODO timezone
//    helpLabel->setText(temp);
}

void ChangelogSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = openFileForSave("changelog", ok, 755);
    if ( ok == false ) return;
    stream << teOldChanges->toPlainText();
    closeFile();
    emit finished(true);
}

