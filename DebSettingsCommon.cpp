#include "DebSettingsCommon.h"

#include <QMessageBox>

QDir DebSettingsCommon::workDir = QDir();

void DebSettingsCommon::setWorkDir(const QDir &value){
    workDir = value;
}


/**
 * @brief DebSettingsCommon::openFile
 * @param fileName - not absolute path. fileName should be starting from DEBIAN folder
 * @return
 */
QTextStream& DebSettingsCommon::openFile( QString fileName, bool &ok){
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
    ok = true;
    stream.setDevice(&file);
    return stream;
}

void DebSettingsCommon::closeFile(){
    stream.flush();
    file.close();
}

DebSettingsCommon::DebSettingsCommon(QDir _workDir, QWidget *parent) : QWidget(parent){
    workDir = _workDir;

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
