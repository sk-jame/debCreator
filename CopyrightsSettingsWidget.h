#ifndef COPYRIGHTSSETTINGSWIDGET_H
#define COPYRIGHTSSETTINGSWIDGET_H

#include <DebSettingsCommon.h>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

class CopyrightsSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT
private:
    QTextEdit* teCopyRights;
    QLabel* label;
    QPushButton* pbNext, *pbBack, *pbExit;
public:
    CopyrightsSettingsWidget(QDir _workDir = QDir(), QWidget* parent = 0);
    virtual ~CopyrightsSettingsWidget();
protected slots:
    virtual void saveChangesAndGoNext();
};

#endif // COPYRIGHTSSETTINGSWIDGET_H
