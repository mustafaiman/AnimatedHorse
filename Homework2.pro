#-------------------------------------------------
#
# Project created by QtCreator 2014-11-03T17:00:38
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Homework2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    horse.cpp

HEADERS  += mainwindow.h \
    horse.h \
    Angel.h \
    CheckError.h \
    vec.h \
    mat.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
