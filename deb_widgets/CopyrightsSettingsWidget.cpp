#include "CopyrightsSettingsWidget.h"
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>

CopyrightsSettingsWidget::CopyrightsSettingsWidget(QDir _workDir, QWidget *parent)
    : DebSettingsCommon(_workDir, parent )
{

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    label = new QLabel(tr("Заполните текст лицензии.\nОзнакомится с примером можно http://dep.debian.net/deps/dep5 "), this);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    teCopyRights = new QTextEdit(this);

    mainLay->addWidget(label);
    mainLay->addWidget(teCopyRights);

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

    mainLay->addItem( tempLay );


    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
}

CopyrightsSettingsWidget::~CopyrightsSettingsWidget(){
    delete label;
    delete teCopyRights;
    delete pbBack;
    delete pbNext;
    delete pbExit;
}

void CopyrightsSettingsWidget::updateWidgetsData(){
    QString res = this->tryToReadDataFromFile("copyright");
    if ( res.isEmpty() == false ){
        teCopyRights->setPlainText( res );
    }
}

void CopyrightsSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = this->openFileForSave("copyright", ok, 644);

    if ( ok == false ) return;

    stream << teCopyRights->toPlainText();
    closeFile();
    emit finished(true);
}
