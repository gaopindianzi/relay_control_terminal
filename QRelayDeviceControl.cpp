#include "QRelayDeviceControl.h"
#include <QDebug>
#include <QString>
#include <QTimer>
#include "modbus_interface.h"

#include "debug.h"
#define THISINFO                0
#define THISERROR            1
#define THISASSERT          1

uint16_t CRC16(unsigned char *Array,unsigned int Len);


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


QRelayDeviceControl::QRelayDeviceControl(QObject * parent) :
    QObject(parent),
    bdevcie_info_is_useful(false),
    relay_bitmask_inited(false)
{
    pdev_info = QSharedPointer<device_info_st>(new device_info_st);
    QTimer *timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(TimeoutUpdataInfo()));
         timer->start(1000);
    is_checked = true;
    online_timeout = 0;
    is_online = false;
}

void QRelayDeviceControl::TimeoutUpdataInfo(void)
{
    //debuginfo(("timeout updata..."));
    GetDevcieInfoFormDevcie();
    ReadIoOut();
    if(online_timeout > 0) {
        online_timeout--;
        devicestatus  = tr("On Line");
        is_online = true;
        DeviceUpdate();
    } else {
        debuginfo(("OffLine ..."));
        devicestatus = tr("Off Line");
        is_online = false;
        DeviceUpdate();
    }
}


void QRelayDeviceControl::InitDeviceAddress(QHostAddress & addr,quint16 port,QSharedPointer<QUdpSocket>  & psocket)
{
    pUdpSocket = psocket;
    deviceaddr  = addr;
    deviceport  = port;
}

QString QRelayDeviceControl::GetHostAddressString(void)
{
    QString str;
    str.sprintf(":%d",deviceport);
    return deviceaddr.toString() + str;
}
QString QRelayDeviceControl::GetBroadcastTime(void)
{
    QString str;
    str.sprintf("%d",this->pdev_info->broadcast_time);
    return str;
}
QString QRelayDeviceControl::GetListionPort(void)
{
    QString str;
    unsigned int port = this->pdev_info->work_port[1];
    port <<= 8;
    port += this->pdev_info->work_port[0];
    str.sprintf("%d", port);
    return str;
}
QString QRelayDeviceControl::GetRemoteHostAddress(void)
{
    QString str(QString::fromAscii(&pdev_info->remote_host_addr[0]));
    return str;
}

QString QRelayDeviceControl::GetRemoteHostPort(void)
{
    QString str;
    unsigned int port = this->pdev_info->remote_host_port[1];
    port <<= 8;
    port += this->pdev_info->remote_host_port[0];
    str.sprintf("%d", port);
    return str;
}

void QRelayDeviceControl::SetDeviceName(QString name)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info->command,sizeof(device_info_st));
    name.resize(sizeof(newinfo.host_name));
    size_t strlen = name.size();
    memcpy(newinfo.host_name,name.toAscii().data(),strlen-1);
    newinfo.host_name[strlen-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    newinfo.to_host = 0;
    newinfo.broadcast_time = 3;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}
void QRelayDeviceControl::SetGroup1Name(QString name)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info->command,sizeof(device_info_st));
    name.resize(sizeof(newinfo.group_name1));
    size_t strlen = name.size();
    memcpy(newinfo.group_name1,name.toAscii().data(),strlen-1);
    newinfo.group_name1[strlen-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    newinfo.to_host = 0;
    newinfo.broadcast_time = 3;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}
void QRelayDeviceControl::SetGroup2Name(QString name)
{
    device_info_st  newinfo;
    memcpy(&newinfo,&pdev_info->command,sizeof(device_info_st));
    name.resize(sizeof(newinfo.group_name2));
    size_t strlen = name.size();
    memcpy(newinfo.group_name2,name.toAscii().data(),strlen-1);
    newinfo.group_name2[strlen-1] = 0;
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.change_password = 0;
    newinfo.to_host = 0;
    newinfo.broadcast_time = 3;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(newinfo));
}
void QRelayDeviceControl::WriteNewDeviceInfoToDevice(device_info_st * pst)
{
    device_info_st  newinfo;
    memcpy(&newinfo,pst,sizeof(device_info_st));
    newinfo.command = CMD_SET_DEVICE_INFO;
    newinfo.command_len = sizeof(device_info_st);
    newinfo.to_host = 0;
    unsigned int crc = CRC16((unsigned char *)&newinfo,sizeof(device_info_st) - 2);
    newinfo.crc[0] = crc & 0xFF;
    newinfo.crc[1] = crc >> 8;
    //发送新的信息
    SendCommandData((const char *)&newinfo,sizeof(device_info_st));
}

void QRelayDeviceControl::ResetDevice(void)
{
    reset_device_st rst;
    rst.command = CMD_RESET_DEVICE;
    rst.command_len = sizeof(reset_device_st);
    for(unsigned int i=0;i<sizeof(rst.pad);i++) {
        rst.pad[i] = (unsigned char)rand();
    }
    unsigned int crc = CRC16((unsigned char *)&rst,sizeof(reset_device_st) - 2);
    rst.crc[0] = (unsigned char)(crc & 0xFF);
    rst.crc[1] = (unsigned char)(crc >> 8);
    //发送新的信息
    dumpthisdata((const char*)&rst,sizeof(reset_device_st));
    SendCommandData((const char *)&rst,sizeof(reset_device_st));
}

void QRelayDeviceControl::GetDevcieInfoFormDevcie(void)
{
    unsigned char buffer[32];
    buffer[0] = CMD_GET_DEVICE_INFO;
    SendCommandData((const char *)buffer,sizeof(buffer));
}

void QRelayDeviceControl::ConvertIoOutOneBitAndSendCmd(int bit)
{
    QByteArray sb;
    unsigned int buflen = sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + sizeof(modbus_type_fc5_cmd) - 2;
    sb.resize(buflen);
    modbus_command_st  * mst = (modbus_command_st *)sb.data();
    mst->command = CMD_MODBUSPACK_SEND;
    mst->command_len = (unsigned char)sb.size();
    modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(mst);
    mh->idh = 0;
    mh->idl = 0;
    mh->protocolh = 0;
    mh->protocoll = 0;
    mh->lengthh = (unsigned char)(sizeof(modbus_type_fc5_cmd) >> 8);
    mh->lengthl = (unsigned char)(sizeof(modbus_type_fc5_cmd) & 0xFF);
    modbus_type_fc5_cmd * pfc  = (modbus_type_fc5_cmd *)GET_MODBUS_DATA(mh);
    pfc->slave_addr = 0;
    pfc->function_code = 0x05;
    pfc->ref_number_h = bit >> 8;
    pfc->ref_number_l = bit & 0xFF;
    int max = relay_bitmask.size();
    max = ((bit+1)>max)?(bit+1):max;
    relay_bitmask.resize(max);
    if(relay_bitmask[bit]) {
        pfc->onoff = 0x00;
    } else {
        pfc->onoff = 0xFF;
    }
    pfc->pad = 0x00;
    unsigned int crc = CRC16(&mst->command_len,sb.size() - 3);
    //debuginfo(("send ConvertIoOutOneBitAndSendCmd CRC(0x%X)",crc));
    //dumpthisdata((const char *)mst,sb.size());
    mst->crc[0] = (unsigned char)(crc  & 0xFF);
    mst->crc[1] = (unsigned char)(crc >> 8);
    //dumpthisdata((const char *)mst,sb.size());
    SendCommandData((const char *)mst,sb.size());
}
void QRelayDeviceControl::ConvertIoOutOneBitAndSendCmdAck(QByteArray & data)
{
    int buflen = sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + sizeof(modbus_type_fc5_cmd) - 2;
    if(data.size() != buflen) {
        debugerror(("convert io out ack data len error!"));
        return ;
    }
    //debuginfo(("convert io ack."));
    modbus_command_st  * mst = (modbus_command_st *)data.data();
    unsigned int crc = CRC16(&mst->command_len,buflen-3);
    if(mst->crc[0] != (unsigned char)(crc&0xFF) || mst->crc[1] != (unsigned char)(crc>>8)) {
        debugerror(("convert io out data CRC(0x%X) ERROR!",crc));
        return ;
    }
    modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(mst);
    modbus_type_fc5_cmd * pfc  = (modbus_type_fc5_cmd *)GET_MODBUS_DATA(mh);
    unsigned int bit = pfc->ref_number_h;
    bit <<= 8;
    bit |= pfc->ref_number_l;
    debuginfo(("convert io ack bit = %d",bit));
    unsigned int max = relay_bitmask.size();
    max = ((bit+1)>max)?(bit+1):max;
    relay_bitmask.resize(max);
    if(pfc->onoff) {
        relay_bitmask[bit] = true;
    } else {
        relay_bitmask[bit] = false;
    }
}

void QRelayDeviceControl::ReadIoOut(void)
{
    int buflen = sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + sizeof(modbus_type_fc1_cmd) - 2;
    QByteArray sb;
    sb.resize(buflen);
    modbus_command_st * mst = (modbus_command_st *)sb.data();
    modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(mst);
    modbus_type_fc1_cmd * fc = (modbus_type_fc1_cmd *)GET_MODBUS_DATA(mh);
    mh->idh = 0;
    mh->idl = 0;
    mh->protocolh = 0;
    mh->protocoll = 0;
    mh->lengthh = (unsigned char)(sizeof(modbus_type_fc1_cmd) >> 8);
    mh->lengthl = (unsigned char)(sizeof(modbus_type_fc1_cmd) & 0xFF);
    fc->slave_addr = 0;
    fc->function_code = 0x01;
    fc->ref_number_h = 0;
    fc->ref_number_l = 0;
    fc->bit_count_h = (unsigned char)(this->GetIoOutNum() >> 8);
    fc->bit_count_l = (unsigned char)(this->GetIoOutNum() & 0xFF);
    mst->command = CMD_MODBUSPACK_SEND;
    mst->command_len = sb.size();
    unsigned int crc = CRC16(&mst->command_len,sb.size() - 3);
   // dumpthisdata((const char *)mst,sb.size());
    mst->crc[0] = (unsigned char)(crc  & 0xFF);
    mst->crc[1] = (unsigned char)(crc >> 8);
   // dumpthisdata((const char *)mst,sb.size());
    SendCommandData((const char *)mst,sb.size());
}
void QRelayDeviceControl::ReadIoOutAck(QByteArray & data)
{
    unsigned int rx_len  = data.size();
    if(rx_len < sizeof(modbus_command_st)) {
        debugerror(("read io out ack data < modbus_command_st size.ERROR!"));
        return ;
    }
    modbus_command_st * mst = (modbus_command_st *)data.data();
    //dumpthisdata((const char *)mst,data.size());
    unsigned int crc = CRC16(&mst->command_len,rx_len - 3);
    if(mst->crc[0] != (unsigned char)(crc&0xFF) || mst->crc[1] != (unsigned char)(crc>>8)) {
        debugerror(("read io out ack data CRC(0x%X) ERROR!",crc));
        return ;
    }
    modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(mst);
    modbus_type_fc1_ack * fc = (modbus_type_fc1_ack *)GET_MODBUS_DATA(mh);
    if(rx_len < (sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + 4 - 2)) {
        debugerror(("read io out ack data < (sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + 2.ERROR"));
        return ;
    }
    if(rx_len < (sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + fc->byte_count + 3 - 2)) {
        debugerror(("read io out ack data <  (sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + fc->byte_count + 3 - 2.ERROR"));
        return ;
    }
    //都正确了
   // debuginfo(("read io out,ALL OK,byte_count=%d",fc->byte_count));
    int max = relay_bitmask.size();
    max = ((fc->byte_count*8) > max)?(fc->byte_count*8):max;
    relay_bitmask.resize(max);
    for(int i=0;i<fc->byte_count*8;i++) {
        relay_bitmask[i] = (fc->bit_valus[i/8]&(1<<(i%8)))?true:false;
    }
    relay_bitmask_inited = true;
}


void QRelayDeviceControl::MultiIoOutSet(unsigned int start_index,QBitArray bit_mask)
{
    int bitbytes = ((bit_mask.size()+7)/8);
    int len = sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + 7 + bitbytes;
    QByteArray sb;
    sb.resize(len);
    modbus_command_st * mst = (modbus_command_st *)sb.data();
    modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(mst);
    modbus_type_fc15_cmd * fc = (modbus_type_fc15_cmd *)GET_MODBUS_DATA(mh);
    mst->command = CMD_MODBUSPACK_SEND;
    mst->command_len = sb.size();
    mh->function_code = 0x0F;
    mh->slave_addr = 0;
    mh->idh = 0;
    mh->idl = 0;
    mh->protocolh = 0;
    mh->protocoll = 0;
    mh->lengthh = (unsigned char)((7 + bitbytes) >> 8);
    mh->lengthl = (unsigned char)((7 + bitbytes) & 0xFF);
    fc->ref_number_h = (unsigned char)(start_index>>8);
    fc->ref_number_l = (unsigned char)(start_index&0xFF);
    fc->bit_count_h = (unsigned char)(bit_mask.size() >> 8);
    fc->bit_count_l = (unsigned char)(bit_mask.size() & 0xFF);
    fc->byte_count = (unsigned char)bitbytes;
    unsigned char * pbit_val = (unsigned char *)&fc->byte_count;
    ++pbit_val;
    memset(pbit_val,0,bitbytes);
    for(int i=0;i<bit_mask.size();i++) {
        pbit_val[i/8] |= (bit_mask[i])?(1<<(i%8)):0;
    }
    //可以计算CRC了
    unsigned int crc = CRC16(&mst->command_len,sb.size() - 3);
    mst->crc[0] = (unsigned char)(crc  & 0xFF);
    mst->crc[1] = (unsigned char)(crc >> 8);
    //dumpthisdata((const char *)mst,sb.size());
    SendCommandData((const char *)mst,sb.size());
}

void QRelayDeviceControl::MultiIoOutSetAck(QByteArray & data)
{
    int len = sizeof(modbus_command_st) + sizeof(modbus_tcp_head) + 4;
    if(data.size() != len) {
        debugerror(("multi io set ack data error!"));
    } else {
        modbus_command_st * mst = (modbus_command_st *)data.data();
        unsigned int crc = CRC16(&mst->command_len,data.size() - 3);
        if(mst->crc[0] != (unsigned char)(crc&0xFF) || mst->crc[1] != (unsigned char)(crc>>8)) {
            debugerror(("MultiIoOutSetAck data CRC(0x%X) ERROR!",crc));
        } else {
            //debuginfo(("MultiIoOutSetAck ack ok."));
        }
    }
}

int  QRelayDeviceControl::GetIoOutNum(void)
{
    //根据板子的型号，确定输入输出的数量
    switch(pdev_info->device_model) {
    case EXT_BOARD_IS_2CHIN_2CHOUT_BOX:return 2;
    case EXT_BOARD_IS_4CHIN_4CHOUT: return 4;
    case EXT_BOARD_IS_8CHIN_8CHOUT_V2: return 8;
    case EXT_BOARD_IS_16CHOUT:return 16;
    default:
        return 2;
    }
}
QString QRelayDeviceControl::GetDeviceModelName(void)
{
    //根据板子的型号，输出名字
    switch(pdev_info->device_model) {
    default:
    case EXT_BOARD_IS_2CHIN_2CHOUT_BOX:return tr("2 ch dig input,2 ch relay output.");
    case EXT_BOARD_IS_4CHIN_4CHOUT: return tr("4 ch dig input,4 ch relay output.");
    case EXT_BOARD_IS_8CHIN_8CHOUT_V2: return tr("8 ch dig input,8 ch relay output.");
    case EXT_BOARD_IS_16CHOUT:return tr("16 ch relay output.");
    }
}

void QRelayDeviceControl::SendCommandData(const char * buffer,int len)
{
    //debuginfo(("send command data to %s:%d",this->deviceaddr.toString().toAscii().data(),this->deviceport));
    this->pUdpSocket->writeDatagram(buffer,len,this->deviceaddr,this->deviceport);
}

void QRelayDeviceControl::SendRxData(QByteArray & data)
{
    device_info_st * pst = (device_info_st *)data.constData();
    if(pst->command == CMD_SET_DEVICE_INFO) {
        //debuginfo(("cmd set device info ack."));
        unsigned int crc = CRC16((unsigned char *)pst,sizeof(device_info_st) - 2);
        if(pst->command_len != data.size() || data.size() != sizeof(device_info_st)) {
            debugerror(("command len ERROR!pst->command_len=%d, data.size()=%d,sizeof(device_info_st)=%d,",
                        pst->command_len,data.size(),sizeof(device_info_st)));
            return ;
        } else {
           // debugerror(("command len ok"));
        }
        //debuginfo(("Get:crc[0] = 0x%X,crc[1] = 0x%X\r\n",pst->crc[0],pst->crc[1]));
        if((unsigned char)(crc>>8) == pst->crc[1] && (unsigned char)(crc&0xFF) == pst->crc[0]) {
            //debuginfo(("crc ok."));
            SetDeviceInfo(data);
        } else {
            debugerror(("crc ERROR(0x%X)",crc));
           // dumpthisdata((const char *)pst,data.size());
            return ;
        }
    } else if(CMD_MODBUSPACK_SEND == pst->command) {
        //dumpthisdata((const char *)pst,data.size());
        int minlen = sizeof(modbus_command_st) + sizeof(modbus_tcp_head);
        if(pst->command_len < minlen) {
            debugerror(("modbus command data size too small...ERROR!"));
            return ;
        }
        modbus_tcp_head * mh = (modbus_tcp_head *)GET_MODBUS_COMMAND_DATA(pst);
        switch(mh->function_code)
        {
        case 0x05: //单促发指令
        {
            ConvertIoOutOneBitAndSendCmdAck(data);
            ReadIoOut();
        }
            break;
        case 0x01:
        {
            ReadIoOutAck(data);
            if(!relay_bitmask_inited) {
                ReadIoOut();
            } else {
                online_timeout = 10;
            }
        }
            break;
        case 0x02:
        {
        }
            break;
        case 0x0F:
        {
            MultiIoOutSetAck(data);
            ReadIoOut();
        }
            break;
        default:
            break;
        }
    } else if(CMD_RESET_DEVICE == pst->command) {
        devicestatus  = tr("Reseting...");
        online_timeout = 0;
    }
    //debuginfo(("%d:%s:%d:  send rx data",count++,deviceaddr.toString().toAscii().data(),deviceport));
    DeviceUpdate();
}


void QRelayDeviceControl::DeviceUpdate(void)
{
    QString hostaddrid;
    hostaddrid.sprintf("%d",this->deviceport);
    hostaddrid = ":" + hostaddrid;
    hostaddrid = this->deviceaddr.toString() + hostaddrid;
    //debuginfo(("set device info:%s",hostaddrid.toAscii().data()));
    bdevcie_info_is_useful = true;
    emit DeviceInfoChanged(hostaddrid);
}

void QRelayDeviceControl::SetDeviceInfo(QByteArray & data)
{
    debuginfo(("-----------updata device info-------------"));
    memcpy(&pdev_info->command,data.constData(),sizeof(device_info_st));
    pdev_info->host_name[63] = 0;
    pdev_info->group_name1[31] = 0;
    pdev_info->group_name2[31] = 0;
    devicename   = QString::fromAscii(&pdev_info->host_name[0]);
    devicegroup1 = QString::fromAscii(&pdev_info->group_name1[0]);
    devicegroup2 = QString::fromAscii(&pdev_info->group_name2[0]);

    QString str;
    const unsigned char * pch = pdev_info->local_ip;
    str.sprintf("ip:%d-%d-%d-%d ",pch[3],pch[2],pch[1],pch[0]);
    pch = pdev_info->net_mask;
    QString netmask;
    netmask.sprintf("netmask:%d-%d-%d-%d ",pch[3],pch[2],pch[1],pch[0]);
    pch = pdev_info->gateway;
    QString gateway;
    gateway.sprintf("gateway:%d-%d-%d-%d ",pch[3],pch[2],pch[1],pch[0]);
    pch = pdev_info->dns;
    QString dns;
    dns.sprintf("dns:%d-%d-%d-%d ",pch[3],pch[2],pch[1],pch[0]);

    QString timeout;
    timeout.sprintf("timeout=%d ",pdev_info->broadcast_time);

    QString mac;
    pch = pdev_info->mac;
    mac.sprintf("%X-%X-%X-%X-%X-%X ",pch[0],pch[1],pch[2],pch[3],pch[4],pch[5]);

    str += netmask + gateway + mac + timeout;

    debuginfo(("%s",str.toAscii().data()));
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

QString QRelayDeviceControl::GetDeviceTime(void)
{
    QString str;
    unsigned char * pbuf = this->pdev_info->device_time;
    str.sprintf("%d-%d-%d %d-%d-%d",1900+pbuf[5],1+pbuf[4],pbuf[3],pbuf[2],pbuf[1],pbuf[0]);
    debuginfo(("device time:%s",str.toAscii().data()));
    return str;
}

uint16_t CRC16(unsigned char *Array,unsigned int Len)
{
    uint16_t IX,IY,CRC;
    CRC=0xFFFF;//set all 1
    if (Len<=0) {
        CRC = 0;
    } else {
        Len--;
        for (IX=0;IX<=Len;IX++)
        {
            CRC=CRC^(uint16_t)(Array[IX]);
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

