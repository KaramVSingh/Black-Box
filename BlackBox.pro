#-------------------------------------------------
#
# Project created by QtCreator 2018-02-05T13:30:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlackBox
TEMPLATE = app

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
    sideselectionpane.cpp \
    field.cpp \
    topselectionpane.cpp \
    bottomselectionpane.cpp \
    wire.cpp \
    gates/not.cpp \
    gates/and.cpp \
    gates/or.cpp \
    gates/gate.cpp \
    gates/input.cpp \
    gates/output.cpp \
    gates/decoder.cpp \
    gates/encoder.cpp \
    gates/dflipflop.cpp

HEADERS += \
        mainwindow.h \
    sideselectionpane.h \
    field.h \
    topselectionpane.h \
    bottomselectionpane.h \
    detail.h \
    wire.h \
    gates/not.h \
    gates/and.h \
    gates/or.h \
    gates/gate.h \
    gates/input.h \
    gates/output.h \
    gates/decoder.h \
    gates/encoder.h \
    gates/dflipflop.h

FORMS += \
        mainwindow.ui \
    sideselectionpane.ui \
    field.ui \
    topselectionpane.ui \
    bottomselectionpane.ui
