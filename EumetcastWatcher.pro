#-------------------------------------------------
#
# Project created by QtCreator 2011-05-29T17:06:27
#
#-------------------------------------------------

QT       += core gui network
QT       += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EUMETCastWatcher
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    mainwindow.cpp \
    options.cpp \
    dialogfiletemplate.cpp

HEADERS  += dialog.h \
    mainwindow.h \
    options.h \
    dialogfiletemplate.h

FORMS    += dialog.ui \
    mainwindow.ui \
    dialogfiletemplate.ui

RESOURCES += \
    watcher.qrc

OTHER_FILES += \
    README.md

DISTFILES += \
    EumetcastWatcher.ini


















