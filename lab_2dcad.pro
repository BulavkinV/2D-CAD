#-------------------------------------------------
#
# Project created by QtCreator 2017-12-27T18:01:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab_2dcad
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
    ENABLE_DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    logger.cpp \
    segment2p.cpp \
    point2d.cpp \
    constraint.cpp \
    geometryobject.cpp \
    geometryinterface.cpp \
    gui_buttonarea.cpp \
    gui_statusbar.cpp \
    gui_centralwidget.cpp \
    gui_scene.cpp \
    gui_mainwindow.cpp \
    notimplementedexception.cpp \
    mainwindowwrongstate.cpp \
    solver.cpp \
    drawingredifinedexception.cpp

HEADERS += \
    logger.h \
    segment2p.h \
    point2d.h \
    constraint.h \
    geometryobject.h \
    geometryinterface.h \
    gui_buttonarea.h \
    gui_statusbar.h \
    gui_centralwidget.h \
    gui_scene.h \
    gui_mainwindow.h \
    notimplementedexception.h \
    mainwindowwrongstate.h \
    solver.h \
    drawingredifinedexception.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    Arch.md

RESOURCES += \
    buttonareaicons.qrc
