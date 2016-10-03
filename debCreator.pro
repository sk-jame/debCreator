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
    ControlSettingsWidget.cpp \
    DirSettingsWidget.cpp \
    ChangelogSettingsWidget.cpp \
    ScriptSettingsWidget.cpp \
    DebSettingsCommon.cpp \
    CopyrightsSettingsWidget.cpp

HEADERS  += mainwindow.h \
    ControlSettingsWidget.h \
    DirSettingsWidget.h \
    ChangelogSettingsWidget.h \
    ScriptSettingsWidget.h \
    DebSettingsCommon.h \
    CopyrightsSettingsWidget.h
