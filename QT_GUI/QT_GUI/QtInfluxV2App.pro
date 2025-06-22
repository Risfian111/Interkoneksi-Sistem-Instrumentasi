QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSensorApp
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sensorreader.cpp

HEADERS += \
    mainwindow.h \
    sensorreader.h

FORMS += \
    mainwindow.ui
