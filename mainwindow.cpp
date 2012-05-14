#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include "QCheckBoxDelegate.h"
#include "QOnOffPushButton.h"
#include "CDeviceDelegate.h"
#include "QIpAddrDelegate.h"
#include "QDeviceMainGroupDelegate.h"
#include "QDeviceStatusDelegate.h"
#include "debug.h"

#define THISINFO              0
#define THISERROR           1
#define THISASSERT          1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    CreateDevcieTable();

    QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(deviceGroupBox, 0, 0, 1, 2);
        centralWidget->setLayout(mainLayout);
        this->resize(1000,300);
        setWindowTitle(tr("RemoteMultiDeviceManger"));

        //
        ///manualAddDevice(0);
       // manualAddDevice(1);
       // manualAddDevice(2);


        //初始化UDP接口
        InitUdpSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateDevcieTable(void)
{
        deviceGroupBox = new QGroupBox(tr("DeviceTable"));
        deviceTable = new QTableWidget;
        deviceTable->setSelectionMode(QAbstractItemView::NoSelection);
        deviceTable->setItemDelegateForColumn(0,new QCheckBoxDelegate(this));
        deviceTable->setItemDelegateForColumn(1,new QDeviceNameDelegate(this));
        deviceTable->setItemDelegateForColumn(2,new QIpAddressDelegate(this));
        deviceTable->setItemDelegateForColumn(3,new QDeviceMainGroupDelegate(this));
        deviceTable->setItemDelegateForColumn(4,new QDeviceSlaveGroupDelegate(this));
        deviceTable->setItemDelegateForColumn(5,new QRelayValueSingalChannalButtonDelegate(this));
        deviceTable->setItemDelegateForColumn(6,new QSetOnPushDelegate(this));
        deviceTable->setItemDelegateForColumn(7,new QSetOffPushDelegate(this));
        deviceTable->setItemDelegateForColumn(8,new QDeviceStatusDelegate(this));

        QStringList labels;
    //! [22] //! [23]
        labels << tr("S") << tr("DeviceName") << tr("IpAddr")<<tr("Group1") << tr("Group2")<<tr("Control")<<tr("AllOn")<<tr("AllOff")<<tr("Status");

        deviceTable->horizontalHeader()->setDefaultSectionSize(100);
        deviceTable->setColumnCount(labels.size());
        deviceTable->setHorizontalHeaderLabels(labels);
        deviceTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
        deviceTable->horizontalHeader()->resizeSection(0,20);
        deviceTable->horizontalHeader()->resizeSection(5,200);
        deviceTable->horizontalHeader()->resizeSection(6,60);
        deviceTable->horizontalHeader()->resizeSection(7,60);
        deviceTable->horizontalHeader()->resizeSection(8,200);

        deviceTable->verticalHeader()->hide();
        //排版
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(deviceTable);
        deviceGroupBox->setLayout(layout);
}

void MainWindow::manualAddDevice(int index)
{
}
 void MainWindow::InsertDevice(QSharedPointer<QRelayDeviceControl> & pdev)
 {
     int row = deviceTable->rowCount();
     deviceTable->setRowCount(row + 1);
     QTableWidgetItem *item0 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item1 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item2 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item3 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item4 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item5 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item6 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item7 = new CDeviceTableWidgetItem(pdev);
     QTableWidgetItem *item8 = new CDeviceTableWidgetItem(pdev);
     //
     deviceTable->setItem(row, 0, item0);
     deviceTable->setItem(row, 1, item1);
     deviceTable->setItem(row, 2, item2);
     deviceTable->setItem(row, 3, item3);
     deviceTable->setItem(row, 4, item4);
     deviceTable->setItem(row, 5, item5);
     deviceTable->setItem(row, 6, item6);
     deviceTable->setItem(row, 7, item7);
     deviceTable->setItem(row, 8, item8);

    // item0->setCheckState(Qt::Checked);
    // item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);//表格只读属性

     deviceTable->openPersistentEditor(item0);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item2);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item5);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item6);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item7);
     deviceTable->openPersistentEditor(item8);
 }

void MainWindow::InitUdpSocket(void)
{
    QSharedPointer<QUdpSocket>  pudp(new QUdpSocket);
    pUdpSocket = pudp;
    pUdpSocket->bind(505);
    connect(pUdpSocket.data(), SIGNAL(readyRead()),
                 this, SLOT(UdpreadPendingDatagrams()));
}

void MainWindow::UdpreadPendingDatagrams()
{
    while (pUdpSocket->hasPendingDatagrams()) {
             QByteArray datagram;
             datagram.resize(pUdpSocket->pendingDatagramSize());
             QHostAddress sender;
             quint16 senderPort;

             pUdpSocket->readDatagram(datagram.data(), datagram.size(),
                                     &sender, &senderPort);

             processTheDeviceData(datagram,sender,senderPort);
         }
}

void MainWindow::processTheDeviceData(QByteArray & data,
                                      QHostAddress & sender,quint16 senderport)
{
    //解析报文，分发命令
    QString portstr;
    portstr.sprintf("%d",senderport);
    QString str = sender.toString();
    str += ":";
    str += portstr;
    debuginfo(("%s",str.toAscii().data()));
    //根据IP地址和端口号，查找已经有的数据，如果有了，给它发消息，让他自己处理自己的事情
    QMap<QString,QSharedPointer<QRelayDeviceControl> >::const_iterator i = mydevicemap.find(str);
    if(i != mydevicemap.end()) {
        debuginfo(("found device,send msg"));
        (*i)->SendRxData(data);
    } else {
        debuginfo(("Not found device,insert it. and send msg"));
        QSharedPointer<QRelayDeviceControl> pdev(new QRelayDeviceControl(this));
        pdev->InitDeviceAddress(sender,senderport,pUdpSocket);
        pdev->SendRxData(data);
        mydevicemap.insert(str,pdev);
        InsertDevice(pdev);
    }
    //如果没有找到，则创建一个，然后再转发消息给他处理
    //设备对象是一个完整的对象，可以处理和拥有自己数据
}

void MainWindow::UpdateDeviceData(QSharedPointer<QRelayDeviceControl> & pdev)
{
}
