#ifndef CONTROLSETTINGSWIDGET_H
#define CONTROLSETTINGSWIDGET_H
#include "DebSettingsCommon.h"

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QDir>
#include <QComboBox>

class ControlSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT
private:
    QLineEdit* packName;
    QLineEdit* version;
    QLineEdit* depends;
    QLineEdit* maintName;
    QLineEdit* maintMail;
    QLineEdit* descriptionShort;
    QTextEdit* descriptionLong;
    QComboBox* section;
    QComboBox* arch;

    QLabel* lName;
    QLabel *lVersion;
    QLabel *lSection;
    QLabel *lArch;
    QLabel *lDepends;
    QLabel *lMainName;
    QLabel *lMainMail;
    QLabel *lDescrShort;
    QLabel *lDescrLong;


    QPushButton* pbNext;
    QPushButton* pbBack;
    QPushButton *pbExit;

    void parseControlFile(QStringList &data, const QString key, QWidget* leForSave );
public:
    explicit ControlSettingsWidget(QDir workDir = QDir(), QWidget *parent = 0);
    virtual ~ControlSettingsWidget();
    virtual void updateWidgetsData();
signals:
    void someData(QString name, QString version, QString arch);
public slots:
private slots:
    virtual void saveChangesAndGoNext();
};

#endif // CONTROLSETTINGSWIDGET_H
