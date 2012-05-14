#include "QRelayDeviceControl.h"
#include <QDebug>
#include "debug.h"

#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

unsigned int CRC16(unsigned char *Array,unsigned int Len);





QRelayDeviceControl::QRelayDeviceControl(QObject * parent) :
    QObject(parent),pdev_info(new device_info_st)
{
}
void QRelayDeviceControl::InitDeviceAddress(QHostAddress & addr,quint16 port,QSharedPointer<QUdpSocket>  & psocket)
{
    pUdpSocket = psocket;
    deviceaddr  = addr;
    deviceport  = port;
}

void QRelayDeviceControl::SendRxData(QByteArray & data)
{
    device_info_st * pst = (device_info_st *)data.data();
    if(pst->command == CMD_SET_DEVICE_INFO) {
        unsigned int crc = CRC16((unsigned char *)pst,pst->command_len - 2);
        if(pst->command_len != data.size()) {
            debugerror(("command len ERROR!"));
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

void QRelayDeviceControl::SetDeviceInfo(QByteArray & data)
{
    debuginfo((("set deivce info..")));
}

QString QRelayDeviceControl::GetDeviceAddress(void)
{
    return deviceaddr.toString();
}

QString QRelayDeviceControl::GetDeviceName(void)
{
    return deviceaddr.toString();
}
QString QRelayDeviceControl::GetGroup1Name(void)
{
    return deviceaddr.toString();
}
QString QRelayDeviceControl::GetGroup2Name(void)
{
    return deviceaddr.toString();
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

