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
    language_selector.cpp

HEADERS  += userwindow.h \
	displayform.h \
    language_selector.h


FORMS    += userwindow.ui \
	displayform.ui \
    language_dialog.ui

RESOURCES += \
    images.qrc

