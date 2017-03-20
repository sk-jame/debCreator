#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T14:31:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = debCreator
TEMPLATE = app

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    deb_widgets/ControlSettingsWidget.cpp \
    deb_widgets/DirSettingsWidget.cpp \
    deb_widgets/ScriptSettingsWidget.cpp \
    deb_widgets/DebSettingsCommon.cpp \
    deb_widgets/CopyrightsSettingsWidget.cpp \
    QProgrammSettings.cpp \
    deb_widgets/changelogsettingswidget.cpp

HEADERS  += mainwindow.h \
    deb_widgets/ControlSettingsWidget.h \
    deb_widgets/DirSettingsWidget.h \
    deb_widgets/ScriptSettingsWidget.h \
    deb_widgets/DebSettingsCommon.h \
    deb_widgets/CopyrightsSettingsWidget.h \
    QProgrammSettings.h \
    QSavedProperties.h \
    deb_widgets/changelogsettingswidget.h

INCLUDEPATH += $$PWD/deb_widgets

DEPENDPATH += $$PWD/deb_widgets

FORMS += \
    deb_widgets/changelogsettingswidget.ui
