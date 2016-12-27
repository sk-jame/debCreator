#include "ControlSettingsWidget.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include "QSavedProperties.h"

void ControlSettingsWidget::parseControlFile(QStringList &data, QString key, QWidget *widget){
    if ( data.isEmpty()) return;
    int index = -1;
    foreach(QString string, data){
        if (string.startsWith(key)){
            index = data.indexOf( string );
        }
    }
    if ( index == -1 ) return;
    QString temp = data.takeAt(index);
    if ( key.endsWith(": ") == false ) key.append(": ");
    temp.remove(key);

    if ( key.contains("Description")){

        descriptionShort->setText( temp );
        QString teRes;
        while(data.isEmpty() == false ){
            temp = data.at(index);
            if ( temp.startsWith(" ")
                 && temp.count() > 1
                 && temp != " .")
            {
                temp.remove(0,1);
                teRes.append(temp);
                teRes.replace(".","\n");
                data.removeAt(index);
            }
            else break;
        }

        descriptionLong->setPlainText( teRes );
        return;
    }
    else if ( key.contains("Maintainer") ){
        this->setMaintainer(temp);
        index = temp.indexOf("<");
        if ( index == -1 ) return;
        QString name;
        for(int i = 0; i < index; i++ ){
            name += temp.at(i);
        }
        maintName->setText(name);
        temp.remove(0,index);
        temp.remove("<");
        temp.remove(">");
        maintMail->setText(temp);
        return;
    }
    else if ( key.contains("Section") || key.contains("Architecture")){
        qobject_cast<QComboBox*>(widget)->setCurrentText(temp);
        return;
    }

    qobject_cast<QLineEdit*>(widget)->setText( temp );
}

ControlSettingsWidget::ControlSettingsWidget(QDir workDir, QWidget *parent) : DebSettingsCommon(workDir,parent){
    QGridLayout* lay = new QGridLayout(this);

    packName = new QLineEdit(this);
    version = new QLineEdit(this);
    depends = new QLineEdit(this);
    maintName = new QLineEdit(this);
    maintMail = new QLineEdit(this);
    predepends = new QLineEdit(this);
    conflicts = new QLineEdit(this);
    replaces = new QLineEdit(this);
    recommends = new QLineEdit(this);
    suggests = new QLineEdit(this);


    descriptionShort = new QLineEdit(this);
    descriptionLong = new QTextEdit(this);

    section = new QComboBox(this);
    arch = new QComboBox(this);

    lName = new QLabel(tr("Имя пакета"), this);
    lVersion = new QLabel(tr("Версия пакета"), this);
    lSection = new QLabel(tr("Тип пакета(секция)"), this);
    lArch = new QLabel(tr("Архитектура"), this);
    lDepends = new QLabel(tr("Зависимости\nПосле имени пакета можно в круглых скобках указать ограничение на версию,\nиспользуя операторы: <<, =, >>, <=, >=.\nЕсли оператор не указан — используется >="), this);
    lPredepends = new QLabel(tr("Зависимости во время устновки\nСписок пакетов, которые требуются в процессе установки этого пакета.\nЭти зависимости могут потребоваться для скриптов установки пакета:\nнапример, пакет flash-installer требует wget\nМожно использовать ограничения на версию (см. Depends)."));
    lConflicts = new QLabel(tr("Конфликты с пакетами\nСписок пакетов, которые не могут быть установлены одновременно с этим.\nУстановка не удастся, если хоть один из перечисленных пакетов уже будет установлен."));
    lReplaces = new QLabel(tr("Заменяет пакеты\nСписок пакетов, файлы которых модифицируются этим пакетом.\nТребуется в случае создания «пакета-патча», изменяющего что-либо:\nв противном случае при замене файлов чужого пакета возникнет ошибка при установке. "));
    lRecommends = new QLabel(tr("Рекомендуемые\nСписок пакетов, рекомендуемых к установке\nЭти пакеты не обязательны, но обычно используются вместе с текущим"));
    lSuggests = new QLabel(tr("Предлагаемые\nСписок пакетов, предлагаемых к установке.\nЭти пакеты не обязательны"));

    lMainName = new QLabel(tr("Имя создателя пакета"), this);
    lMainMail = new QLabel(tr("Почтовый ящик\nсоздателя пакета"), this);
    lDescrShort = new QLabel(tr("Краткое описание пакета(70 символов)"),this);
    lDescrLong = new QLabel(tr("Подробное описание пакета\nТочка '.' является переносом строки!\nТочки в местах переноса строки устанавливаются автоматически!"),this);

    pbNext = new QPushButton(tr("Дальше"),this);
    pbNext->setShortcut(QKeySequence("ALT+N"));
    pbExit = new QPushButton(tr("Выход"),this);
//    pbBack = new QPushButton(tr("Назад"),this);
//    pbBack->setShortcut(QKeySequence("ALT+B"));

    const int descrLongRowCnt = 3;
    int layRow = 0;
    lay->addWidget(lName,       layRow++, 0, 1, 1 );
    lay->addWidget(lVersion,    layRow++, 0, 1, 1 );
    lay->addWidget(lSection,    layRow++, 0, 1, 1 );
    lay->addWidget(lArch,       layRow++, 0, 1, 1 );
    lay->addWidget(lDepends,    layRow++, 0, 1, 1 );
    lay->addWidget(lPredepends, layRow++, 0, 1, 1 );
    lay->addWidget(lConflicts,  layRow++, 0, 1, 1 );
    lay->addWidget(lReplaces,   layRow++, 0, 1, 1 );
    lay->addWidget(lRecommends, layRow++, 0, 1, 1 );
    lay->addWidget(lSuggests,   layRow++, 0, 1, 1 );
    lay->addWidget(lMainName,   layRow++, 0, 1, 1 );
    lay->addWidget(lMainMail,   layRow++, 0, 1, 1 );
    lay->addWidget(lDescrShort, layRow++, 0, 1, 1 );
    lay->addWidget(lDescrLong,  layRow++, 0, 3, 1 );

    layRow = 0;
    lay->addWidget(packName,    layRow++, 1, 1, 3 );
    lay->addWidget(version,     layRow++, 1, 1, 3 );
    lay->addWidget(section,     layRow++, 1, 1, 3 );
    lay->addWidget(arch,        layRow++, 1, 1, 3 );
    lay->addWidget(depends,     layRow++, 1, 1, 3 );
    lay->addWidget(predepends,  layRow++, 1, 1, 3 );
    lay->addWidget(conflicts,   layRow++, 1, 1, 3 );
    lay->addWidget(replaces,    layRow++, 1, 1, 3 );
    lay->addWidget(recommends,  layRow++, 1, 1, 3 );
    lay->addWidget(suggests,    layRow++, 1, 1, 3 );
    lay->addWidget(maintName,   layRow++, 1, 1, 3 );
    lay->addWidget(maintMail,   layRow++, 1, 1, 3 );
    lay->addWidget(descriptionShort, layRow++, 1, 1, 3 );
    lay->addWidget(descriptionLong,  layRow++, 1, 3, 3 );


    lay->addWidget(pbExit, layRow + descrLongRowCnt, 2, 1, 1);
//    lay->addWidget(pbBack, layRow + descrLongRowCnt, 2, 1, 1);
    lay->addWidget(pbNext, layRow + descrLongRowCnt, 3, 1, 1);

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



//    connect( pbBack, SIGNAL(clicked()), this, SIGNAL(tryGoBack()));


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

    delete predepends;
    delete conflicts;
    delete replaces;
    delete recommends;
    delete suggests;
    delete lPredepends;
    delete lConflicts;
    delete lReplaces;
    delete lRecommends;
    delete lSuggests;

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
//    delete pbBack;
    delete pbExit;
}

void ControlSettingsWidget::updateWidgetsData(){
    QString res = this->tryToReadDataFromFile("control");
    if ( !res.isEmpty() ){
        QStringList temp = res.split("\n");
        parseControlFile(temp, "Package", packName );
        parseControlFile(temp, "Version", version );
        parseControlFile(temp, "Section", section );
        parseControlFile(temp, "Architecture", arch );
        parseControlFile(temp, "Depends", depends );
        parseControlFile(temp, "Pre-Depends", predepends );
        parseControlFile(temp, "Conflicts", conflicts );
        parseControlFile(temp, "Replaces", replaces );
        parseControlFile(temp, "Recommends", recommends );
        parseControlFile(temp, "Suggests", suggests );
        QSettings settings;
        if ( settings.value(SAVED_USE_MAIL).toBool()){
            maintName->setText(settings.value(SAVED_MAINTAINER).toString());
            maintMail->setText(settings.value(SAVED_MAIL).toString());
            this->setMaintainer(settings.value(SAVED_MAINTAINER).toString() +
                                " <" +
                                settings.value(SAVED_MAIL).toString() +
                                ">");
        }
        else{
            parseControlFile(temp, "Maintainer", maintName );
        }
        parseControlFile(temp, "Description", descriptionShort );
    }
}

void ControlSettingsWidget::saveChangesAndGoNext(){
    bool ok;
    QTextStream& stream = this->openFileForSave("control", ok, 644);

    if ( ok == false ) return;

    stream << "Package: " + packName->text() << "\n";
    stream << "Version: " + version->text() << "\n";
    stream << "Section: " + section->currentText() << "\n";
    stream << "Architecture: " + arch->currentText() << "\n";
    if ( !depends->text().isEmpty() ) stream << "Depends: " + depends->text() << "\n";
    if ( !predepends->text().isEmpty() ) stream << "Pre-Depends: " + predepends->text() << "\n";
    if ( !conflicts->text().isEmpty() ) stream << "Conflicts: " + conflicts->text() << "\n";
    if ( !replaces->text().isEmpty() ) stream << "Replaces: " + replaces->text() << "\n";
    if ( !recommends->text().isEmpty() ) stream << "Recommends: " + recommends->text() << "\n";
	if ( !maintName->text().isEmpty() ) stream << "Maintainer: " + maintName->text().simplified() + " <" + maintMail->text() << ">\n";
    if ( !maintMail->text().isEmpty() ) stream << "Suggests: " + suggests->text() << "\n";
    if ( !descriptionShort->text().isEmpty() ) stream << "Description: " + descriptionShort->text() << "\n ";
    QString temp = descriptionLong->toPlainText();
    if ( !temp.isEmpty() ){
        if ( temp.endsWith("\n") ) temp.remove(temp.lastIndexOf("\n"), 1 );//temp.append("\n");
        temp.replace("\n", ".\n ");
        temp.append(".\n");
        stream << temp;
    }
    closeFile();
    emit someData(packName->text(), version->text(), arch->currentText());

    emit finished(true);
}
