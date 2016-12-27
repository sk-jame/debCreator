#include "ChangelogSettingsWidget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGridLayout>
#include "QSavedProperties.h"
#include <QDateTime>

ChangelogSettingsWidget::ChangelogSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir, parent){
    teChangeLog = new QTextEdit(this);
    label = new QLabel(tr(
"Заполните текст изменений.\nФормат записи:\n\
    package (version) distribution(s); urgency=urgency\n\
            [optional blank line(s), stripped]\n\
        * change details\n\
        more change details\n\
            [blank line(s), included in output of dpkg-parsechangelog]\n\
        * even more change details\n\
            [optional blank line(s), stripped]\n\
       -- maintainer name <email address>[two spaces]  date\n\
Формат даты:\n\
    day-of-week, dd month yyyy hh:mm:ss +zzzz\n\
Пример:\n\
    supersh (1.0-1) stable; urgency=medium\n\
        * Testing.\n\
       -- aai <a.aleksandrov@neroelectronics.by> Sun, 28 Sep 2016 18:34:46 +0300"), this);

    helpLabel = new QLabel(this);
    helpLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbBack = new QPushButton(tr("Назад"),this);
    pbBack->setShortcut(QKeySequence("ALT+B"));
    pbExit = new QPushButton(tr("Выход"),this);

    QGridLayout* lay = new QGridLayout(this);
    lay->addWidget(label,  0, 0, 1, 3);
    lay->addWidget(helpLabel,  1, 0, 1, 3);
    lay->addWidget(teChangeLog, 2, 0, 1, 3 );
    lay->addWidget(pbExit, 3, 0, 1, 1 );
    lay->addWidget(pbBack, 3, 1, 1, 1 );
    lay->addWidget(pbNext, 3, 2, 1, 1 );



    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
}

ChangelogSettingsWidget::~ChangelogSettingsWidget(){
    delete teChangeLog;
    delete pbNext;
    delete pbBack;
    delete pbExit;
    delete helpLabel;
    delete label;
}

void ChangelogSettingsWidget::updateWidgetsData(){
    QString res = this->tryToReadDataFromFile("changelog");
    if ( res.isEmpty() == false ){
        teChangeLog->setPlainText( res );
    }
    QString temp = tr("Для удобства текущее время и создатель пакета(можно скопировать):\n\t-- ");
    QDateTime dt = QDateTime::currentDateTime();
    QLocale locale = QLocale(QLocale::English, QLocale::UnitedStates); // set the locale you want here

    temp.append( this->getMaintainer() );
    temp.append( locale.toString(dt.date(), " ddd, dd MMM yyyy "));
    temp.append( locale.toString(dt.time(), "hh:mm:ss +0300")); //TODO timezone
    helpLabel->setText(temp);
}

void ChangelogSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = openFileForSave("changelog", ok, 644);
    if ( ok == false ) return;
    stream << teChangeLog->toPlainText();
    closeFile();
    emit finished(true);
}

