#ifndef SCRIPTSETTINGSWIDGET_H
#define SCRIPTSETTINGSWIDGET_H

#include <QWidget>
#include <QDir>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include "DebSettingsCommon.h"

class ScriptSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT
private:
    QPushButton *pbNext;
    QPushButton *pbBack;
    QPushButton *pbExit;

    typedef struct{
        QPushButton *pbOpen;
        QGroupBox *grBox;
        QLineEdit *lePath;
    }ChoosePath;

    enum{
        preInst = 0,
        postInst,
        preRm,
        postRm,
        endOfScripts
    };

    QList<ChoosePath*> widgetsList;
public:
    explicit ScriptSettingsWidget(QDir workDir = QDir(), QWidget *parent = 0);
    virtual ~ScriptSettingsWidget();
private slots:
    void pbOpenClicked();
    virtual void saveChangesAndGoNext();
};

#endif // SCRIPTSETTINGSWIDGET_H
