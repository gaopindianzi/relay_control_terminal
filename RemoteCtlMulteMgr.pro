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
    CDeviceDelegate.cpp \
    CDeviceTableWidgetItem.cpp \
    QDeviceMainGroupDelegate.cpp \
    QIpAddrDelegate.cpp \
    QDeviceStatusDelegate.cpp \
    editparamdialog.cpp \
    QDeviceList.cpp

HEADERS  += mainwindow.h \
    debug.h \
    QOnOffPushButton.h \
    QRelayDeviceControl.h \
    multimgr_device_dev.h \
    QCheckBoxDelegate.h \
    CDeviceDelegate.h \
    CDeviceTableWidgetItem.h \
    QDeviceMainGroupDelegate.h \
    QIpAddrDelegate.h \
    QDeviceStatusDelegate.h \
    modbus_interface.h \
    editparamdialog.h \
    QDeviceList.h

FORMS    += mainwindow.ui \
    editparamdialog.ui

RC_FILE = myapp.rc

OTHER_FILES +=
