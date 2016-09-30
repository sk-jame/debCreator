#include "DirSettingsWidget.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

DirSettingsWidget::DirSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir, parent){
    teDirs = new QTextEdit(this);
    label = new QLabel(tr("Введите папки используемые пакетом\nПапки, в которых содержатся исполняемые файлы также необходимы"), this);
    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbExit = new QPushButton(tr("Выход"),this);

    QGridLayout* lay = new QGridLayout(this);
    lay->addWidget(label,  0, 0, 1, 3, Qt::AlignCenter);
    lay->addWidget(teDirs, 1, 0, 1, 3 );
    lay->addWidget(pbNext, 2, 2, 1, 1 );
    lay->addWidget(pbExit, 2, 0, 1, 1 );

    pbBack = new QPushButton(tr("Назад"),this);
    pbBack->setShortcut(QKeySequence("ALT+B"));
    lay->addWidget(pbBack, 2, 1, 1, 1 );

    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));


    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
}

DirSettingsWidget::~DirSettingsWidget(){
    delete teDirs;
    delete label;
    delete pbNext;
    delete pbExit;
}

void DirSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = openFile("changelog", ok, 644);
    if ( ok == false ) return;

    stream << teDirs->toPlainText();
    closeFile();
    emit finished(true);
}
