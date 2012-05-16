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
    void update(void);
signals:
    void DeviceInfoChanged(QString hostaddrID);
public:
    void SetDeviceInfo(QByteArray & data);
    int   GetDeviceRelayCount(void) { return relay_bitmask.size(); }
    bool devcie_info_is_useful(void) { return bdevcie_info_is_useful; }
    QString GetDeviceAddress(void);
    QString GetDeviceName(void);
    void      SetDeviceName(QString newDeviceName);
    QString GetGroup1Name(void);
    void      SetGroup1Name(QString name);
    QString GetGroup2Name(void);
    void      SetGroup2Name(QString name);
    QString GetStatus(void);
    int        GetIoOutNum(void);
    void      ReadIoOut(void);
    void      ReadIoOutAck(QByteArray & data);
    void      ConvertIoOutOneBitAndSendCmd(int bit);
    void      ConvertIoOutOneBitAndSendCmdAck(QByteArray & data);
    void      MultiIoOutSet(unsigned int start_index,QBitArray bit_mask);
    void      MultiIoOutSetAck(QByteArray & data);
    QBitArray   relay_bitmask;

private:
    void      SendCommandData(const char * buffer,int len);
    void      DeviceUpdate(void);
private:
    QString devicename;
    QString devicegroup1;
    QString devicegroup2;
    QString devicestatus;
private:
    QHostAddress   deviceaddr;
    quint16              deviceport;
    QSharedPointer<QUdpSocket>      pUdpSocket;
    QSharedPointer<device_info_st>   pdev_info;
    bool       bdevcie_info_is_useful;
    bool       relay_bitmask_inited;
};

typedef QSharedPointer<QRelayDeviceControl>   RelayDeviceSharePonterType;

Q_DECLARE_METATYPE(RelayDeviceSharePonterType)


#endif // QRELAYDEVICECONTROL_H
