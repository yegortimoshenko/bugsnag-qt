#-------------------------------------------------
#
# Project created by QtCreator 2014-07-14T14:29:19
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = bugsnag-qt
TEMPLATE = lib

DEFINES += BUGSNAGQT_LIBRARY

SOURCES += bugsnag.cpp

HEADERS += bugsnag.h\
        bugsnag-qt_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
