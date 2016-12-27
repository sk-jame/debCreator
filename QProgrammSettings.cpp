#include <QApplication>
#include "QProgrammSettings.h"
#include <QVariant>
#include "QSavedProperties.h"
#include <QFileDialog>
#include <QMessageBox>

QWidget* QProgrammSettings::createDebOptsWidgets(QWidget* wid){
    debWidget = new QWidget(wid);
    cbUseMail = new QCheckBox(tr("Подставлять заданные почту и имя создателя пакета?"), debWidget);
    mailLabel = new QLabel(tr("Введите e-mail:"), debWidget);
    maintainerLabel = new QLabel(tr("Введите ФИО:"), debWidget);;
    leMaintainer = new QLineEdit(debWidget);
    leMail = new QLineEdit(debWidget);

    QGridLayout* lay = new QGridLayout(debWidget);
    lay->addWidget(cbUseMail,       0, 0, 1, 3);
    lay->addWidget(maintainerLabel, 1, 0, 1, 1);
    lay->addWidget(leMaintainer,    1, 1, 1, 2);
    lay->addWidget(mailLabel,       2, 0, 1, 1);
    lay->addWidget(leMail,          2, 1, 1, 2);

    cbUseMail->setChecked(settings.value(SAVED_USE_MAIL).toBool());

    if (!settings.value(SAVED_USE_MAIL).toBool()){
        leMail->setDisabled(true);
        leMaintainer->setDisabled(true);
    }

    leMail->setText(settings.value(SAVED_MAIL).toString());
    leMaintainer->setText(settings.value(SAVED_MAINTAINER).toString());

    connect( cbUseMail, SIGNAL(toggled(bool)),
             leMail,    SLOT(setEnabled(bool)));

    connect( cbUseMail,     SIGNAL(toggled(bool)),
             leMaintainer,  SLOT(setEnabled(bool)));

    return debWidget;
}

QWidget *QProgrammSettings::createSaveWidgets(QWidget* wid){
    saveWidget = new QWidget(wid);
    sbSysrootCnt = new QSpinBox(saveWidget);
    sbSysrootCnt->setValue(settings.value(SAVED_SYSROOT_SHOW_CNT).toUInt());
    sysrootLabel = new QLabel(tr("Кол-во отображаемых sysroot'ов:"));

    leSaveDebPath = new QLineEdit(settings.value(SAVED_DEB_PATH).toString(), saveWidget);
    cbSaveDebPath = new QCheckBox(tr("Использовать данный путь для сохранения пакетов?\nпо-умолчанию пакеты сохраняются рядом с выбранным sysroot'ом"), saveWidget);
    cbSaveDebPath->setChecked(settings.value(SAVED_USE_DEB_PATH).toBool());
    pbSavePathOpen = new QPushButton(tr("Открыть"), saveWidget);

    QGridLayout* lay = new QGridLayout(saveWidget);
    lay->addWidget( sysrootLabel,   0, 0, 1, 2 );
    lay->addWidget( sbSysrootCnt,   0, 2, 1, 1 );
    lay->addWidget( cbSaveDebPath,  1, 0, 1, 3 );
    lay->addWidget( leSaveDebPath,  2, 0, 1, 2 );
    lay->addWidget( pbSavePathOpen, 2, 2, 1, 1 );

    cbSaveDebPath->setChecked(settings.value(SAVED_USE_DEB_PATH).toBool());
    if (!settings.value(SAVED_USE_DEB_PATH).toBool()){
        leSaveDebPath->setDisabled(true);
        pbSavePathOpen->setDisabled(true);
    }

    connect( cbSaveDebPath, SIGNAL(toggled(bool)),
             leSaveDebPath, SLOT(setEnabled(bool)));
    connect( cbSaveDebPath, SIGNAL(toggled(bool)),
             pbSavePathOpen, SLOT(setEnabled(bool)));

    connect( pbSavePathOpen, SIGNAL(released()),
             this,           SLOT(openPath()));
    return saveWidget;
}

QProgrammSettings::QProgrammSettings(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    this->setWindowTitle(tr("Настройки"));
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createSaveWidgets(tabWidget), tr("Настройки сохранений"));
    tabWidget->addTab(createDebOptsWidgets(tabWidget), tr("Настройки пакетов"));

    dialButtons = new QDialogButtonBox(QDialogButtonBox::Apply |
                                       QDialogButtonBox::Close,
                                       Qt::Horizontal, this);

    connect( dialButtons->button(QDialogButtonBox::Apply), SIGNAL(released()),
             this,  SLOT(apply()));

    connect( dialButtons->button(QDialogButtonBox::Close), SIGNAL(released()),
             this,  SLOT(close()));

    mainLay = new QGridLayout(this);
    mainLay->addWidget(tabWidget, 0, 0, 4, 4);
    mainLay->addWidget(dialButtons, 4, 2, 1, 2);

}

QProgrammSettings::~QProgrammSettings(){

}

/**
 * @brief QProgrammSettings::initializeSettings
 */
void QProgrammSettings::initializeSettings(){
    QSettings set;
    if (set.value(SAVED_INITIALIZED).isValid())
        return;
    set.setValue(SAVED_MAIL, "");
    set.setValue(SAVED_MAINTAINER, "");
    set.setValue(SAVED_SYSROOT_PATH, QStringList());
    set.setValue(SAVED_USE_MAIL, false);
    set.setValue(SAVED_SYSROOT_SHOW_CNT, 10);

    set.setValue(SAVED_DEB_PATH, QDir::currentPath());
    set.setValue(SAVED_USE_DEB_PATH, false);

    set.setValue(SAVED_INITIALIZED, true);
}

void QProgrammSettings::apply(){
    settings.setValue(SAVED_USE_MAIL, cbUseMail->isChecked());
    if ( cbUseMail->isChecked() ){
        settings.setValue(SAVED_MAIL, leMail->text());
        settings.setValue(SAVED_MAINTAINER, leMaintainer->text());
    }

    settings.setValue(SAVED_SYSROOT_SHOW_CNT, sbSysrootCnt->value());

    if (cbSaveDebPath->isChecked()){
        QString temp = leSaveDebPath->text();
        if (!QDir(temp).exists()){
            QMessageBox::critical(this, tr("Ошибка"), tr("Указанный Вами каталог не существует!\nДанные сохранены не будут!"));
            return;
        }
        settings.setValue(SAVED_DEB_PATH, temp);
        settings.setValue(SAVED_USE_DEB_PATH, true);
    }
    QMessageBox::information(this, tr("Успех"), tr("Настройки успешно применены."));
    this->close();
}

void QProgrammSettings::openPath(){
    QPushButton* pSender = qobject_cast<QPushButton*>(sender());
    if ( pbSavePathOpen == pSender ){
        QString path = QFileDialog::getExistingDirectory(this, tr("Выберете каталог для сохранения"),
                                                         settings.value(SAVED_DEB_PATH).toString());
        if (path.isEmpty())
            return;
        leSaveDebPath->setText(path);
        return;
    }
}
