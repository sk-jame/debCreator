#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QProgrammSettings::initializeSettings();
    w.show();

    return a.exec();
}
