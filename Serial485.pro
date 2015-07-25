#-------------------------------------------------
#
# Project created by QtCreator 2015-07-23T20:23:41
#
#-------------------------------------------------

QT       += core gui
QT +=serialport
QT +=core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Serial485
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
