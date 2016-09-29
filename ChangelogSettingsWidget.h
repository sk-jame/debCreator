#ifndef CHANGELOGSETTINGSWIDGET_H
#define CHANGELOGSETTINGSWIDGET_H

#include <QWidget>

#include <QDir>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

#include "DebSettingsCommon.h"

class ChangelogSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT
private:
    QTextEdit* teChangeLog;
    QLabel* label;
    QPushButton* pbNext;
    QPushButton* pbBack;
    QPushButton *pbExit;
public:
    explicit ChangelogSettingsWidget(QDir workDir = QDir(), QWidget *parent = 0);
    ~ChangelogSettingsWidget();
private slots:
    virtual void saveChangesAndGoNext();
};

#endif // CHANGELOGSETTINGSWIDGET_H
