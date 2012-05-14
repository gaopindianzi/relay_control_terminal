#ifndef QRELAYDEVICECONTROL_H
#define QRELAYDEVICECONTROL_H

#include <QObject>
#include <QSharedPointer>
#include <QUdpSocket>
 #include <QBitArray>
#include <QTableWidgetItem>
#include "multimgr_device_dev.h"
#include "CDeviceDelegate.h"





class QRelayDeviceControl : public QObject
{
    Q_OBJECT
public:
    QRelayDeviceControl(QObject * parent);
public slots:
    void InitDeviceAddress(QHostAddress & addr,quint16 port,QSharedPointer<QUdpSocket>  & psocket);
    void SendRxData(QByteArray & data);
public:
    void SetDeviceInfo(QByteArray & data);
    int   GetDeviceRelayCount(void) { return relay_bitmask.size(); }

private:
    QHostAddress   deviceaddr;
    quint16              deviceport;
    QSharedPointer<QUdpSocket>      pUdpSocket;
    QSharedPointer<device_info_st>   pdev_info;
    QBitArray                                       relay_bitmask;
};


#endif // QRELAYDEVICECONTROL_H
