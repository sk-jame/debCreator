#ifndef QPROGRAMMSETTINGS_H
#define QPROGRAMMSETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>

/**
 * @brief The QProgrammSettings class
 * Позволяет настроить:
 *  - Колличество предлагаемых sysroot'ов, место сохранения дебпакетов, или выбрать режим сохранения возле sysroot'a
 *  - Maintainer'a и e-mail по-умолчанию. Приоритет из настроек или из пакета
 */
class QProgrammSettings : public QDialog
{
    Q_OBJECT
private:
    /* widgets */
    QDialogButtonBox* dialButtons;
    QGridLayout* mainLay;
    QTabWidget* tabWidget;

    QWidget* debWidget;
    QCheckBox* cbUseMail;
    QLabel* mailLabel;
    QLabel* maintainerLabel;
    QLineEdit* leMaintainer;
    QLineEdit* leMail;

    QWidget* saveWidget;
    QSpinBox* sbSysrootCnt;
    QLabel* sysrootLabel;
    QLineEdit* leSaveDebPath;
    QCheckBox* cbSaveDebPath;
    QLabel* debpathLabel;
    QPushButton* pbSavePathOpen;

    /* init functions */
    QWidget *createDebOptsWidgets(QWidget *wid);
    QWidget *createSaveWidgets(QWidget *wid);
    void createTabWidget();
    /* other */
    QSettings settings;
public:
    explicit QProgrammSettings(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~QProgrammSettings();

    static void initializeSettings();
signals:

public slots:
    void apply();
private slots:
    void openPath();
};

#endif // QPROGRAMMSETTINGS_H
