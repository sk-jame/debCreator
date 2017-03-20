#ifndef CHANGELOGSETTINGSWIDGET_H
#define CHANGELOGSETTINGSWIDGET_H

#include <DebSettingsCommon.h>

namespace Ui {
class changelogsettingswidget;
}

class ChangelogSettingsWidget : public DebSettingsCommon
{
    Q_OBJECT

public:
    explicit ChangelogSettingsWidget(QDir workDir, QWidget *parent);
    ~ChangelogSettingsWidget();
    virtual void updateWidgetsData();
private slots:
    virtual void saveChangesAndGoNext();
    void on_teNewChange_textChanged();
    void on_cbUrgency_currentTextChanged();
    void on_cbDestrib_currentTextChanged();
    void on_pbOldView_clicked();

    void on_pbBack_clicked();

    void on_pbNext_clicked();

private:
    Ui::changelogsettingswidget *ui;

    QString getChangeLogFromText();
};

#endif // CHANGELOGSETTINGSWIDGET_H
