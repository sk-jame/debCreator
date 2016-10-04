#ifndef DEBSETTINGSCOMMON_H
#define DEBSETTINGSCOMMON_H

#include <QWidget>
#include <QTextStream>
#include <QDir>
#include <QProcess>

class DebSettingsCommon : public QWidget
{
    Q_OBJECT

private:
    QFile file;
    QTextStream stream;
protected:
    static QDir workDir;
    QTextStream& openFileForSave(QString fileName, bool &ok, int access = 0);
    QString tryToReadDataFromFile( QString fileName );
    void closeFile();
public:
    explicit DebSettingsCommon(QDir _workDir = QDir(),QWidget *parent = 0);

    typedef enum{
        eControlWidget,
        eDirWidget,
        eChangeLogWidget,
        eScriptWidget,
        eCopyrightWidget,
        eEndOfWidgets
    }ESettingsWidgets;

    virtual void updateWidgetsData();
    static void setWorkDir(const QDir &value);

signals:
    void tryGoBack();
    void finished(bool);
protected slots:
    void saveChangesAndGoNext();
    void exitClicked();
};

#endif // DEBSETTINGSCOMMON_H
