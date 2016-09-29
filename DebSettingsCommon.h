#ifndef DEBSETTINGSCOMMON_H
#define DEBSETTINGSCOMMON_H

#include <QWidget>
#include <QTextStream>
#include <QDir>

class DebSettingsCommon : public QWidget
{
    Q_OBJECT

private:
    QFile file;
    QTextStream stream;
protected:
    static QDir workDir;
    QTextStream& openFile(QString fileName, bool &ok);
    void closeFile();
public:
    explicit DebSettingsCommon(QDir _workDir = QDir(),QWidget *parent = 0);

    typedef enum{
        eControlWidget,
        eDirWidget,
        eChangeLogWidget,
        eScriptWidget,
        eEndOfWidgets
    }ESettingsWidgets;

    static void setWorkDir(const QDir &value);

signals:
    void tryGoBack();
    void finished(bool);
protected slots:
    void saveChangesAndGoNext();
    void exitClicked();
};

#endif // DEBSETTINGSCOMMON_H
