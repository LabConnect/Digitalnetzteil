#-------------------------------------------------
#
# Project created by QtCreator 2014-11-10T14:51:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Interface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hiddata.c

HEADERS  += mainwindow.h \
    ../firmware/usbconfig.h \
    hiddata.h

FORMS    += mainwindow.ui


LIBS += -L/usr/lib -lusb
