#include "ControlSettingsWidget.h"
#include <QGridLayout>
#include <QMessageBox>

ControlSettingsWidget::ControlSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir,parent){
    QGridLayout* lay = new QGridLayout(this);

    packName = new QLineEdit(this);
    version = new QLineEdit(this);
    depends = new QLineEdit(this);
    maintName = new QLineEdit(this);
    maintMail = new QLineEdit(this);

    descriptionShort = new QLineEdit(this);
    descriptionLong = new QTextEdit(this);

    section = new QComboBox(this);
    arch = new QComboBox(this);

    lName = new QLabel(tr("Имя пакета"), this);
    lVersion = new QLabel(tr("Версия пакета"), this);
    lSection = new QLabel(tr("Тип пакета(секция)"), this);
    lArch = new QLabel(tr("Архитектура"), this);
    lDepends = new QLabel(tr("Зависимости\nПосле имени пакета можно в круглых скобках указать ограничение на версию,\nиспользуя операторы: <<, =, >>, <=, >=.\nЕсли оператор не указан — используется >="), this);
    lMainName = new QLabel(tr("Имя создателя пакета"), this);
    lMainMail = new QLabel(tr("Почтовый ящик\nсоздателя пакета"), this);
    lDescrShort = new QLabel(tr("Краткое описание пакета(70 символов)"),this);
    lDescrLong = new QLabel(tr("Подробное описание пакета\nКаждая строка должна, начинаться с пробела.\n\В расширенном описании все переводы строки игнорируются.\nДля вставки \\n используется одиночная точка."),this);

    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbExit = new QPushButton(tr("Выход"),this);

    lay->addWidget(lName,       0, 0, 1, 1 );
    lay->addWidget(lVersion,    1, 0, 1, 1 );
    lay->addWidget(lSection,    2, 0, 1, 1 );
    lay->addWidget(lArch,       3, 0, 1, 1 );
    lay->addWidget(lDepends,    4, 0, 1, 1 );
    lay->addWidget(lMainName,   5, 0, 1, 1 );
    lay->addWidget(lMainMail,   6, 0, 1, 1 );
    lay->addWidget(lDescrShort, 7, 0, 1, 1 );
    lay->addWidget(lDescrLong,  8, 0, 3, 1 );


    lay->addWidget(packName,    0, 1, 1, 2 );
    lay->addWidget(version,     1, 1, 1, 2 );
    lay->addWidget(section,     2, 1, 1, 2 );
    lay->addWidget(arch,        3, 1, 1, 2 );
    lay->addWidget(depends,     4, 1, 1, 2 );
    lay->addWidget(maintName,   5, 1, 1, 2 );
    lay->addWidget(maintMail,   6, 1, 1, 2 );
    lay->addWidget(descriptionShort, 7, 1, 1, 2 );
    lay->addWidget(descriptionLong,  8, 1, 3, 2 );


    lay->addWidget(pbNext, 11, 2, 1, 1);
    lay->addWidget(pbExit, 11, 1, 1, 1);

    // настройка комбо боксов
    {
    QStringList sectionsList;
    sectionsList<<"admin"<<"base"<<"comm"<<"contrib"<<"devel"<<"doc"
                <<"editors"<<"electronics"<<"embedded"<<"games"<<"gnome"
                <<"graphics"<<"hamradio"<<"interpreters"<<"kde"<<"libs"
                <<"libdevel"<<"mail"<<"math"<<"misc"<<"net"<<"news"<<"non-free"
                <<"oldlibs"<<"otherosfs"<<"perl"<<"python"<<"science"<<"shells"
                <<"sound"<<"tex"<<"text"<<"utils"<<"web"<<"x11";
    section->addItems( sectionsList );
    section->setCurrentIndex(sectionsList.indexOf("embedded"));
    }

    {
        QStringList archList;
        archList << "i386" << "amd64" << "all" << "source" << "armhf";
        arch->addItems( archList );
        arch->setCurrentIndex( archList.indexOf("armhf") );
    }

    connect( pbNext, SIGNAL(clicked()), this, SLOT(saveChangesAndGoNext()));
    connect( pbExit, SIGNAL(clicked()), this, SLOT(exitClicked()));
}

ControlSettingsWidget::~ControlSettingsWidget(){
    delete packName;
    delete version;
    delete section;
    delete arch;
    delete depends;
    delete maintName;
    delete maintMail;
    delete descriptionShort;
    delete descriptionLong;

    delete lName;
    delete lVersion;
    delete lSection;
    delete lArch;
    delete lDepends;
    delete lMainName;
    delete lMainMail;
    delete lDescrShort;
    delete lDescrLong;

    delete pbNext;
    delete pbExit;
}

void ControlSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = this->openFile("control", ok);

    if ( ok == false ) return;

    stream << "Package: " + packName->text() << "\n";
    stream << "Version: " + version->text() << "\n";
    stream << "Section: " + section->currentText() << "\n";
    stream << "Architecture: " + arch->currentText() << "\n";
    stream << "Depends: " + depends->text() << "\n";
    stream << "Maintainer: " + maintName->text() + " <" + maintMail->text() << ">\n";
    stream << "Description: " + descriptionShort->text() << "\n " << descriptionLong->toPlainText() <<".\n";
    closeFile();
    emit someData(packName->text(), version->text(), arch->currentText());

    emit finished(true);
}
