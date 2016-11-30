#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QDir>
#include <QLabel>
#include <QFileInfoList>
#include "ControlSettingsWidget.h"
#include "DirSettingsWidget.h"
#include "ChangelogSettingsWidget.h"
#include "ScriptSettingsWidget.h"
#include "CopyrightsSettingsWidget.h"
#include <QProcess>

#define VERSION "v1.0.0"
#define SYSROOT_SAVED_PATH "sysroot/pathList"

class QCompleter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QProcess* process;

    QDir workDir;

    QLineEdit* lePath;
    QCompleter* completer;
    QLabel* label;
    QDialogButtonBox* buttonBox;
    QPushButton* pbOpenSave;

    ControlSettingsWidget* controlWidget;
    DirSettingsWidget* dirWidget;
    ChangelogSettingsWidget* changelogWidget;
    ScriptSettingsWidget* scriptWidget;

    QList<DebSettingsCommon*> settingsWidgets;

    void createLintianWidget();
    QWidget* lintianWidget;
    QLabel* lintianLabel;
    QPushButton* pbExit;

    void createMainWidget();
    QWidget* startWidget;

    QString currentCheck;
    QString resultFileName;
    QString fakeRootOut;

    void checkInstalledSoft(QString softName);
    bool writeMd5Sum;
    bool fakeRootCheck;
    bool testDeb;

    QFile md5sumFile;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void readDatafromStdOut();
    void readDataFromStdError();
    void createFileName(QString name, QString version, QString arch);
    void okOpenClicked();
    void openClicked();
    void settingsWidgetFinished(bool shouldContinue);
    void settingsWidgetGoBack();
};

#endif // MAINWINDOW_H
