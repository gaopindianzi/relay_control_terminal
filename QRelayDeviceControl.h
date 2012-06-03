#ifndef QRELAYDEVICECONTROL_H
#define QRELAYDEVICECONTROL_H

#include <QObject>
#include <QSharedPointer>
#include <QUdpSocket>
 #include <QBitArray>
#include <QTimer>
#include <QTableWidgetItem>
#include "multimgr_device_dev.h"
#include "CDeviceDelegate.h"
#include "rc4.h"
#include "PasswordItemDef.h"




class QRelayDeviceControl : public QObject
{
    Q_OBJECT
public:
    QRelayDeviceControl(QObject * parent);
signals:
    void DeviceInfoChanged(QString hostaddrID);
    void DeviceAckStatus(QString status);
public slots:
    void InitDeviceAddress(QHostAddress & addr,quint16 port,QSharedPointer<QUdpSocket>  & psocket);
    int   SendRxData(QByteArray & data,QList<password_item> & pwdlist);
    void Updata(void);
    void DeviceUpdate(void);
private slots:
    void TimeoutUpdataInfo(void);
public:
    void GetDevcieInfoFormDevcie(void);
    void SetDeviceInfo(QByteArray & data);
    int   GetDeviceRelayCount(void) { return relay_bitmask.size(); }
    bool devcie_info_is_useful(void) { return bdevcie_info_is_useful; }
    QString GetDeviceAddress(void);
    QString GetDeviceName(void);
    QString GetHostAddressString(void);
    QString GetBroadcastTime(void);
    QString GetListionPort(void);
    QString GetRemoteHostAddress(void);
    QString GetRemoteHostPort(void);
    QString GetDeviceTime(void);
    void      WriteNewDeviceInfoToDevice(device_info_st * pst);
    void      SetDeviceName(QString newDeviceName);
    QString GetGroup1Name(void);
    void      SetGroup1Name(QString name);
    QString GetGroup2Name(void);
    void      SetGroup2Name(QString name);
    QString GetStatus(void);
    QString GetAckStatus(void);
    int        GetIoOutNum(void);
    QString GetDeviceModelName(void);
    void      ReadIoOut(void);
    int        ReadIoOutAck(QByteArray & data);
    void      ConvertIoOutOneBitAndSendCmd(int bit);
    int        ConvertIoOutOneBitAndSendCmdAck(QByteArray & data);
    void      MultiIoOutSet(unsigned int start_index,QBitArray bit_mask);
    int        MultiIoOutSetAck(QByteArray & data);
    void      ResetDevice(void);
    QBitArray   relay_bitmask;
    QSharedPointer<device_info_st> & GetDeviceInfo(void) { return pdev_info; }
private:
    void      SendCommandData(const char * buffer,int len);
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
    int         online_timeout;
    bool       need_encryption;
    QString  password;
    //QEncryptRc4    rc4;
    QTimer * timer;
    QString   ack_status;
public:
    bool       is_checked;
    bool       is_online;
    int          index;
};

typedef QSharedPointer<QRelayDeviceControl>   RelayDeviceSharePonterType;

Q_DECLARE_METATYPE(RelayDeviceSharePonterType)


#endif // QRELAYDEVICECONTROL_H
