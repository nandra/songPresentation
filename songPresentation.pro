#-------------------------------------------------
#
# Project created by QtCreator 2012-08-14T22:56:16
#
#-------------------------------------------------

QT       += core gui network

TARGET = songPresentation
TEMPLATE = app
TRANSLATIONS = songPresentation_sk.ts

SOURCES += main.cpp\
		userwindow.cpp \
        displayform.cpp \
    dynamicfontsizelabel.cpp

HEADERS  += userwindow.h \
	displayform.h \
    dynamicfontsizelabel.h


FORMS    += userwindow.ui \
	displayform.ui

