#-------------------------------------------------
#
# Project created by QtCreator 2018-08-01T12:11:33
#
#-------------------------------------------------

QT += core gui network printsupport svg
QT += concurrent
QT += charts
QT += opengl

CONFIG += c++14 sdk_no_version_check

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sr
TEMPLATE = app
VERSION = 1.0.1
SNAME = Statistics
SCODE = 6000xxxx
SDESCRIPTION = TODO
SCREATIONYEAR = 2018
SCOMPONENTID = it.systemgroup


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    jsonparser.cpp \
    settings.cpp \
    filterdata.cpp \
    visualdata.cpp \
    validatejson.cpp \
    settingsdialog.cpp \
    configurationparams.cpp \
    seeker.cpp \
    seekerparser.cpp \
    seekertokenizer.cpp \
    seekerexchange.cpp

HEADERS += \
        mainwindow.h \
    jsonparser.h \
    settings.h \
    filterdata.h \
    visualdata.h \
    validatejson.h \
    states.h \
    settingsdialog.h \
    configurationparams.h \
    operatorpriority.h \
    seeker.h \
    seekerparser.h \
    seekertokenizer.h \
    seekerexchange.h

FORMS += \
        mainwindow.ui \
    settingsdialog.ui

TRANSLATIONS += sr_it_IT.ts \
               sr_en_US.ts

RESOURCES += \
    i18n.qrc \
