#include "DebSettingsCommon.h"

#include <QMessageBox>

QDir DebSettingsCommon::workDir = QDir();
QString DebSettingsCommon::maintainer = QString("");

void DebSettingsCommon::setWorkDir(const QDir &value){
    workDir = value;
}

QString DebSettingsCommon::getMaintainer(){
    return maintainer;
}

void DebSettingsCommon::setMaintainer(const QString &value){
    maintainer = value;
}

/**
 * @brief DebSettingsCommon::openFile
 * @param fileName - not absolute path. fileName should be starting from DEBIAN folder
 * @return
 */
QTextStream& DebSettingsCommon::openFileForSave(QString fileName, bool &ok, int access){
    ok = false;
    if ( !workDir.absolutePath().endsWith("DEBIAN") ){
        if ( workDir.cd("DEBIAN") == false ){
            workDir.mkdir("DEBIAN");
            if ( workDir.cd("DEBIAN") == false ){
                QMessageBox::critical(this, tr("Ошибка записи"),
                                      tr("Не смог создать папку.\nПроверьте права доступа к каталогу"));
                return stream;
            }
        }
    }
    file.setFileName(workDir.absolutePath() + "/" + fileName);

    if ( !file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        QMessageBox::critical(this, tr("Ошибка записи"),
                              tr("Не смог создать(перезаписать) файл.\nПроверьте права доступа к каталогу"));
        return stream;
    }
    if ( access != 0 ){
        QProcess::execute("chmod " + QString().number(access) + " " + file.fileName());
    }
    ok = true;
    stream.setDevice(&file);
    return stream;
}

QString DebSettingsCommon::tryToReadDataFromFile(QString fileName){
    QString res;
    if ( !workDir.absolutePath().endsWith("DEBIAN") ){
        if ( workDir.cd("DEBIAN") == false ){
            return res;
        }
    }

    file.setFileName(workDir.absolutePath() + "/" + fileName);

    if ( file.exists()){
        if ( file.open(QIODevice::ReadOnly) ){
            stream.setDevice(&file);
            res = stream.readAll();
            file.close();

            return res;
        }
    }

    return res;

}

void DebSettingsCommon::closeFile(){
    stream.flush();
    file.close();
}

DebSettingsCommon::DebSettingsCommon(QDir _workDir, QWidget *parent) : QWidget(parent){
    workDir = _workDir;

}

void DebSettingsCommon::updateWidgetsData(){

}

void DebSettingsCommon::saveChangesAndGoNext()
{

}

void DebSettingsCommon::exitClicked(){
    int res = QMessageBox::question(this,tr("Выйти?"), tr("Изменения сделанные программой необратимы.\nУверены, что хотите выйти?"),
                          QMessageBox::Ok, QMessageBox::No);
    if ( res == QMessageBox::Ok ){
        emit finished(false);
        this->close();
    }
}
