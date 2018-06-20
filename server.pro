#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T08:39:13
#
#-------------------------------------------------

QT       += core gui
QT +=sql
QT +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpserverlisten.cpp \
    dbinterface.cpp \
    tcpclient.cpp \
    tcpserver.cpp \
    dbhandle.cpp

HEADERS  += mainwindow.h \
    datastruct.h \
    tcpserverlisten.h \
    dbinterface.h \
    tcpclient.h \
    tcpserver.h \
    dbhandle.h

FORMS    += mainwindow.ui

RC_FILE += \
    server.rc
