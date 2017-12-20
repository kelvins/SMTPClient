#-------------------------------------------------
#
# Project created by QtCreator 2017-12-20T21:45:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app

include( $$PWD/../SMTPClient/SMTPClient.pri )

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
