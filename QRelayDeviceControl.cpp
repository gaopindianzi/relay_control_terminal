#include "QRelayDeviceControl.h"
#include <QDebug>
#include <QString>
#include <QTimer>
#include "debug.h"

#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

unsigned int CRC16(unsigned char *Array,unsigned int Len);





QRelayDeviceControl::QRelayDeviceControl(QObject * parent) :
    QObject(parent),pdev_info(new device_info_st)
{
    QTimer *timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(update()));
         //timer->start(1000);
}
void QRelayDeviceControl::InitDeviceAddress(QHostAddress & addr,quint16 port,QSharedPointer<QUdpSocket>  & psocket)
{
    pUdpSocket = psocket;
    deviceaddr  = addr;
    deviceport  = port;
}

void QRelayDeviceControl::update(void)
{
    debuginfo(("updata..."));
}
void QRelayDeviceControl::SetDeviceName(QString newDeviceName)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info,sizeof(device_info_st));
    memcpy(newinfo.host_name,newDeviceName.toAscii().data(),sizeof(newinfo.host_name)-1);
    newinfo.host_name[sizeof(newinfo.host_name)-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}
void QRelayDeviceControl::SetGroup1Name(QString name)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info,sizeof(device_info_st));
    memcpy(newinfo.group_name1,name.toAscii().data(),sizeof(newinfo.group_name1)-1);
    newinfo.host_name[sizeof(newinfo.group_name1)-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}
void QRelayDeviceControl::SetGroup2Name(QString name)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info,sizeof(device_info_st));
    memcpy(newinfo.group_name2,name.toAscii().data(),sizeof(newinfo.group_name2)-1);
    newinfo.host_name[sizeof(newinfo.group_name2)-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}

void QRelayDeviceControl::SendCommandData(const char * buffer,int len)
{
    debuginfo(("send command data to %s:%d",this->deviceaddr.toString().toAscii().data(),this->deviceport));
    this->pUdpSocket->writeDatagram(buffer,len,this->deviceaddr,this->deviceport);
}

void QRelayDeviceControl::SendRxData(QByteArray & data)
{
    device_info_st * pst = (device_info_st *)data.data();
    if(pst->command == CMD_SET_DEVICE_INFO) {
        unsigned int crc = CRC16((unsigned char *)pst,pst->command_len - 2);
        if(pst->command_len != data.size() || data.size() != sizeof(device_info_st)) {
            debugerror(("command len ERROR!pst->command_len=%d, data.size()=%d,sizeof(device_info_st)=%d,",
                        pst->command_len,data.size(),sizeof(device_info_st)));
            return ;
        } else {
            //debugerror(("command len ok"));
        }
      //  debuginfo(("Get:crc[0] = 0x%X,crc[1] = 0x%X\r\n",pst->crc[0],pst->crc[1]));
        if((unsigned char)(crc>>8) == pst->crc[1] && (unsigned char)(crc&0xFF) == pst->crc[0]) {
           // debuginfo(("crc ok."));
            SetDeviceInfo(data);
        } else {
            debugerror(("crc ERROR."));
            return ;
        }
    } else if(pst->command == CMD_GET_DEVICE_INFO) {
    } else if(CMD_MODBUSPACK_SEND == pst->command) {
    } else if(CMD_RESET_DEVICE == pst->command) {
    }
    //debuginfo(("%d:%s:%d:  send rx data",count++,deviceaddr.toString().toAscii().data(),deviceport));
}

void dumpthisdata(const char * buffer,int len)
{
    QString str;
    for(int i=0;i<len;i++) {
        QString tmp;
        tmp.sprintf("%2X,",(unsigned char)buffer[i]);
        str += tmp;
    }
    debuginfo(("%s",str.toAscii().data()));
}

void QRelayDeviceControl::SetDeviceInfo(QByteArray & data)
{
    memcpy(&pdev_info->command,data.constData(),sizeof(device_info_st));
    pdev_info->host_name[63] = 0;
    pdev_info->group_name1[31] = 0;
    pdev_info->group_name2[31] = 0;
    devicename   = QString::fromAscii(&pdev_info->host_name[0]);
    devicegroup1 = QString::fromAscii(&pdev_info->group_name1[0]);
    devicegroup2 = QString::fromAscii(&pdev_info->group_name2[0]);
    devicestatus  = tr("online.");
    QString hostaddrid;
    hostaddrid.sprintf("%d",this->deviceport);
    hostaddrid = ":" + hostaddrid;
    hostaddrid = this->deviceaddr.toString() + hostaddrid;
    debuginfo(("set device info."));
    emit DeviceInfoChanged(hostaddrid);
}

QString QRelayDeviceControl::GetDeviceAddress(void)
{
    return deviceaddr.toString();
}

QString QRelayDeviceControl::GetDeviceName(void)
{
    return devicename;
}
QString QRelayDeviceControl::GetGroup1Name(void)
{
    return devicegroup1;
}
QString QRelayDeviceControl::GetGroup2Name(void)
{
    return devicegroup2;
}
QString QRelayDeviceControl::GetStatus(void)
{
    return devicestatus;
}

unsigned int CRC16(unsigned char *Array,unsigned int Len)
{
    unsigned int  IX,IY,CRC;
    CRC=0xFFFF;//set all 1
    if (Len<=0) {
        CRC = 0;
    } else {
        Len--;
        for (IX=0;IX<=Len;IX++)
        {
            CRC=CRC^(unsigned int)(Array[IX]);
            for(IY=0;IY<=7;IY++) {
                if ((CRC&1)!=0) {
                    CRC=(CRC>>1)^0xA001;
                } else {
                    CRC=CRC>>1;
                }
            }
        }
    }
    return CRC;
}

