#-------------------------------------------------
#
# Project created by QtCreator 2015-08-13T03:46:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kotab
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    hadith.cpp \
    addhadith.cpp

HEADERS  += widget.h \
    hadith.h \
    addhadith.h

FORMS    += widget.ui \
    addhadith.ui

RESOURCES +=
