#include "changelogsettingswidget.h"
#include "ui_changelogsettingswidget.h"
#include <QDateTime>
#include <QMessageBox>

ChangelogSettingsWidget::ChangelogSettingsWidget(QDir workDir, QWidget *parent)
    : DebSettingsCommon(workDir, parent),
        ui(new Ui::changelogsettingswidget){

    ui->setupUi(this);
    ui->widget->setLayout(ui->change_mainlay);
}

ChangelogSettingsWidget::~ChangelogSettingsWidget(){
    delete ui;
}

void ChangelogSettingsWidget::updateWidgetsData(){
    QString res = this->tryToReadDataFromFile("changelog");
    if ( res.isEmpty() == false ){
       return;
    }

    QString temp = res.split("\n").first();
    QStringList items;
    items << "low" << "medium" << "high" << "emergency" << "critical";
    foreach (QString str, items) {
        if (temp.contains(str)){
            ui->cbUrgency->setCurrentIndex(items.indexOf(str));
            break;
        }
    }
    items.clear();
    items << "stable" << "unstable" << "experimental";
    foreach(QString str, items){
        if (temp.contains(str)){
            ui->cbDestrib->setCurrentIndex(items.indexOf(str));
            break;
        }
    }
}

void ChangelogSettingsWidget::saveChangesAndGoNext(){

    QString res = this->tryToReadDataFromFile("changelog");
    if (res.isEmpty()){
        return;
    }

    bool ok;
    QTextStream& stream = openFileForSave("changelog", ok, 755);
    if ( ok == false ) return;


    stream << getChangeLogFromText() << res;

    closeFile();

    QDir root = workDir;
    root.cdUp();

    if (!root.cd("usr")){
        root.mkdir("usr");
        root.cd("usr");
    }

    if (!root.cd("share")){
        root.mkdir("share");
        root.cd("share");
    }

    if (!root.cd("doc")){
        root.mkdir("doc");
        root.cd("doc");
    }

    if (!root.cd(this->getPackageName())){
        root.mkdir(this->getPackageName());
        root.cd(this->getPackageName());
    }

    if (!QProcess::execute("cp " + workDir.absoluteFilePath("changelog") +
                           " " + root.absolutePath()+ "/changelog.Debian")){

        QProcess::execute("gzip --best " + root.absolutePath()+ "/changelog.Debian");
    }

    emit finished(true);
}

void ChangelogSettingsWidget::on_teNewChange_textChanged(){
    getChangeLogFromText();
}

QString ChangelogSettingsWidget::getChangeLogFromText(){
    QString result = QString("%1 (%2) %3; urgency=%4\n\n")
            .arg(this->getPackageName())
            .arg(this->getVersion())
            .arg(ui->cbDestrib->currentText())
            .arg(ui->cbUrgency->currentText());

    QStringList list = ui->teNewChange->toPlainText().split("\n");
    foreach(QString str, list){
        if (!str.startsWith("+")){
            str.prepend("  * ");
        }
        else{
            str.prepend("    ");
        }
        result+=str+"\n";
    }

    result.append("\n");

    result.append(" -- ");

    QDateTime dt = QDateTime::currentDateTime();
    QLocale locale = QLocale(QLocale::English, QLocale::UnitedStates); // set the locale you want here

    result.append( this->getMaintainer() );
    result.append( locale.toString(dt.date(), " ddd, dd MMM yyyy "));
    result.append( locale.toString(dt.time(), "hh:mm:ss +0300")); //TODO timezone
    result.append("\n\n");
    ui->tePrevision->setPlainText(result);
    return result;
}

void ChangelogSettingsWidget::on_cbUrgency_currentTextChanged(){
    getChangeLogFromText();
}

void ChangelogSettingsWidget::on_cbDestrib_currentTextChanged(){
    getChangeLogFromText();
}

void ChangelogSettingsWidget::on_pbBack_clicked(){
    emit tryGoBack();
}
#include <QDebug>
void ChangelogSettingsWidget::on_pbNext_clicked(){
    saveChangesAndGoNext();



}

class OldChangeLogDialog : public QDialog{
    QTextEdit* te;
    QVBoxLayout* lay;
public:
    OldChangeLogDialog(QString title, QString text, QWidget* parent = Q_NULLPTR)
        : QDialog(parent){

        this->setWindowTitle(title);
        te = new QTextEdit(this);
        te->setText(text);
        te->setReadOnly(true);
        lay = new QVBoxLayout(this);
        lay->addWidget(te);
        this->setGeometry(parent->geometry());
    }

    ~OldChangeLogDialog(){
        delete lay;
        delete te;
    }
};

void ChangelogSettingsWidget::on_pbOldView_clicked(){
    QString res = this->tryToReadDataFromFile("changelog");
    if ( res.isEmpty() == false ){
        OldChangeLogDialog dialog("Старый список изменений", res, this);
        dialog.exec();
        return;
    }
    OldChangeLogDialog dialog("Старый список изменений", "Список изменений пуст", this);
    dialog.exec();
}

