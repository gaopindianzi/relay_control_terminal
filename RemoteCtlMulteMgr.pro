#-------------------------------------------------
#
# Project created by QtCreator 2012-05-09T12:10:05
#
#-------------------------------------------------

QT       += core gui network

TARGET = RemoteCtlMulteMgr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QOnOffPushButton.cpp \
    QRelayDeviceControl.cpp \
    QCheckBoxDelegate.cpp \
    CDeviceDelegate.cpp

HEADERS  += mainwindow.h \
    debug.h \
    QOnOffPushButton.h \
    QRelayDeviceControl.h \
    multimgr_device_dev.h \
    QCheckBoxDelegate.h \
    CDeviceDelegate.h

FORMS    += mainwindow.ui

RC_FILE = myapp.rc
