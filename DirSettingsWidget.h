#ifndef DIRSETTINGSWIDGET_H
#define DIRSETTINGSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDir>
#include "DebSettingsCommon.h"

class DirSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT
private:
    QDir workDir;
    QTextEdit* teDirs;
    QLabel* label;
    QPushButton *pbNext;
    QPushButton *pbExit;
public:
    explicit DirSettingsWidget(QDir workDir = QDir(), QWidget *parent = 0);
    virtual ~DirSettingsWidget();
private slots:
    virtual void saveChangesAndGoNext();
};

#endif // DIRSETTINGSWIDGET_H
