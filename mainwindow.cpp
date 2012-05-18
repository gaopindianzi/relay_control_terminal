#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QModelIndex>
#include "QCheckBoxDelegate.h"
#include "QOnOffPushButton.h"
#include "CDeviceDelegate.h"
#include "QIpAddrDelegate.h"
#include "QDeviceMainGroupDelegate.h"
#include "QDeviceStatusDelegate.h"
#include "editparamdialog.h"
#include "qeditipconfigdialog.h"

#include "debug.h"

#define THISINFO             1
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
    CreateAction();
    CreateMenu();

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

void MainWindow::CreateAction(void)
{
    this->quitact = new QAction(tr("&Quit"), this);
    //addImagesAct->setShortcut(tr("Ctrl+A"));
    connect(quitact, SIGNAL(triggered()), this, SLOT(Quit()));
    //
    this->edit_device_param_act = new QAction(tr("&EditDevice..."), this);
    connect(edit_device_param_act, SIGNAL(triggered()), this, SLOT(EditDeviceParam()));

    this->cleardevicetable = new QAction(tr("&Clear"), this);
    connect(cleardevicetable, SIGNAL(triggered()), this, SLOT(ClearDeviceTable()));

    this->edit_device_ipconfig = new QAction(tr("&Ipconfig..."), this);
    connect(edit_device_ipconfig, SIGNAL(triggered()), this, SLOT(EditDeviceIpconfig()));
}

void MainWindow::CreateMenu(void)
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitact);


    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(edit_device_param_act);
    toolsMenu->addAction(edit_device_ipconfig);
    toolsMenu->addSeparator();
    toolsMenu->addAction(cleardevicetable);




}
void MainWindow::Quit(void)
{
    close();
}

void MainWindow::EditDeviceParam(void)
{    
    EditParamDialog dialog;

    this->update();

    int row = deviceTable->rowCount();
    for(int i=0;i<row;i++) {
        QTableWidgetItem * item = deviceTable->item(i,0);
        QVariant var = item->data(0);
        QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(var);
        if(pdev->is_checked) {
            dialog.InsertDevice(pdev);
        }
    }
    dialog.exec();
}
void MainWindow::EditDeviceIpconfig(void)
{
    QEditIpConfigDialog dialog;
    this->update();

    int row = deviceTable->rowCount();
    for(int i=0;i<row;i++) {
        QTableWidgetItem * item = deviceTable->item(i,0);
        QVariant var = item->data(0);
        QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(var);
        if(pdev->is_checked) {
            dialog.InsertDevice(pdev);
        }
    }
    dialog.exec();
}

void MainWindow::ClearDeviceTable(void)
{
    int rowcount = deviceTable->rowCount();
    for(int i=0;i<rowcount;i++) {
        deviceTable->removeRow(0);
    }
    mydevicemap.clear();
}

void MainWindow::CreateDevcieTable(void)
{
    unsigned int index = 0;
        deviceGroupBox = new QGroupBox(tr("DeviceTable"));
        deviceTable = new QTableWidget;
        deviceTable->setSelectionMode(QAbstractItemView::NoSelection);
        deviceTable->setItemDelegateForColumn(index++,new QCheckBoxDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QDeviceNameDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QIpAddressDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QDeviceMainGroupDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QDeviceSlaveGroupDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QRelayValueSingalChannalButtonDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QSetOnPushDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QSetOffPushDelegate(this));
        deviceTable->setItemDelegateForColumn(index++,new QDeviceStatusDelegate(this));
#if APP_DISPLAY_TIME
        deviceTable->setItemDelegateForColumn(index++,new QDeviceTimeDelegate(this));
#endif

        QStringList labels;
    //! [22] //! [23]
        labels << tr("S") << tr("DeviceName") << tr("IpAddr")<<tr("Group1") << tr("Group2")<<tr("Control")<<tr("AllOn")<<tr("AllOff")<<tr("Status");
#if APP_DISPLAY_TIME
        labels << tr("DeviceTime");
#endif

        deviceTable->horizontalHeader()->setDefaultSectionSize(100);
        deviceTable->setColumnCount(labels.size());
        deviceTable->setHorizontalHeaderLabels(labels);
        deviceTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
        index = 0;
        deviceTable->horizontalHeader()->resizeSection(index++,25);
        deviceTable->horizontalHeader()->resizeSection(index++,120);
        deviceTable->horizontalHeader()->resizeSection(index++,100);
        deviceTable->horizontalHeader()->resizeSection(index++,80);
        deviceTable->horizontalHeader()->resizeSection(index++,80);
        deviceTable->horizontalHeader()->resizeSection(index++,250);
        deviceTable->horizontalHeader()->resizeSection(index++,60);
        deviceTable->horizontalHeader()->resizeSection(index++,60);
        deviceTable->horizontalHeader()->resizeSection(index++,50);
#if APP_DISPLAY_TIME
        deviceTable->horizontalHeader()->resizeSection(index++,120);
#endif
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
#if APP_DISPLAY_TIME
     QTableWidgetItem *item9 = new CDeviceTableWidgetItem(pdev);
#endif
     //
     int index = 0;
     deviceTable->setItem(row, index++, item0);
     deviceTable->setItem(row, index++, item1);
     deviceTable->setItem(row, index++, item2);
     deviceTable->setItem(row, index++, item3);
     deviceTable->setItem(row, index++, item4);
     deviceTable->setItem(row, index++, item5);
     deviceTable->setItem(row, index++, item6);
     deviceTable->setItem(row, index++, item7);
     deviceTable->setItem(row, index++, item8);
#if APP_DISPLAY_TIME
     deviceTable->setItem(row, index++, item9);
#endif

    // item0->setCheckState(Qt::Checked);
    // item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);//表格只读属性

     deviceTable->openPersistentEditor(item0);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item2);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item5);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item6);  //不用双击就可以显示控件的形式
     deviceTable->openPersistentEditor(item7);
     deviceTable->openPersistentEditor(item8);
#if APP_DISPLAY_TIME
     //deviceTable->openPersistentEditor(item9);
#endif
     //
     //添加信号
     //updateGeometries
     //modelReset
     connect(pdev.data(),SIGNAL(DeviceInfoChanged(QString)),this,SLOT(DeviceInfoChanged(QString))); //,Qt::QueuedConnection);
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

             pUdpSocket->readDatagram(datagram.data(), 1024,&sender, &senderPort);

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
    debuginfo(("precess udp rx data len(%d)",data.size()));
    //根据IP地址和端口号，查找已经有的数据，如果有了，给它发消息，让他自己处理自己的事情
    QMap<QString,QSharedPointer<QRelayDeviceControl> >::const_iterator i = mydevicemap.find(str);
    if(i != mydevicemap.end()) {
        //debuginfo(("found device,send rx data"));
        (*i)->SendRxData(data);
        //DeviceInfoChanged(str);
    } else {
        debuginfo(("Not found device,insert it. and send msg"));
        QSharedPointer<QRelayDeviceControl> pdev(new QRelayDeviceControl(this));
        pdev->InitDeviceAddress(sender,senderport,pUdpSocket);
        pdev->SendRxData(data);
        if(pdev->devcie_info_is_useful()) {
            mydevicemap.insert(str,pdev);
            InsertDevice(pdev);
        }
    }
    //如果没有找到，则创建一个，然后再转发消息给他处理
    //设备对象是一个完整的对象，可以处理和拥有自己数据
}

void MainWindow::DeviceInfoChanged(QString  hostaddrID)
{
   // debuginfo(("host:%s",hostaddrID.toAscii().data()));
    QMap<QString,QSharedPointer<QRelayDeviceControl> >::const_iterator it = mydevicemap.find(hostaddrID);
    if(it != mydevicemap.end()) {
        int row = deviceTable->rowCount();
        for(int i=0;i<row;i++) {
            QTableWidgetItem * item = deviceTable->item(i,0);
            QVariant var = item->data(0);
            QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(var);
            if((*it) == pdev) {
                QAbstractItemModel * model = deviceTable->model ();
                int index_max = 9;
#if APP_DISPLAY_TIME
                index_max++;
#endif
                for(int j=0;j<index_max;j++) {
                    QModelIndex index = model->index(i,j);
                    deviceTable->update(index);
                }
                emit DeviceUpdata(pdev);
                break;
            }
        }
    }
}
