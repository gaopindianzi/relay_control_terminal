#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include "QCheckBoxDelegate.h"
#include "QOnOffPushButton.h"
#include "CDeviceDelegate.h"
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
        manualAddDevice(0);
        manualAddDevice(1);
        manualAddDevice(2);


        //��ʼ��UDP�ӿ�
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
        deviceTable->setItemDelegateForColumn(0,new CDeviceDelegate(this));
        deviceTable->setItemDelegateForColumn(5,new QRelayValueSingalChannalButtonDelegate(this));
        deviceTable->setItemDelegateForColumn(6,new QOnOffPushDelegate(this));
        deviceTable->setItemDelegateForColumn(7,new QOnOffPushDelegate(this));

        QStringList labels;
    //! [22] //! [23]
        labels << tr("S") << tr("DeviceName") << tr("IpAddr")<<tr("Group1") << tr("Group2")<<tr("Control")<<tr("AllOn")<<tr("AllOff")<<tr("Status");

        deviceTable->horizontalHeader()->setDefaultSectionSize(100);
        deviceTable->setColumnCount(labels.size());
        deviceTable->setHorizontalHeaderLabels(labels);
        deviceTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
        deviceTable->horizontalHeader()->resizeSection(0,25);
        deviceTable->horizontalHeader()->resizeSection(5,200);
        deviceTable->horizontalHeader()->resizeSection(6,60);
        deviceTable->horizontalHeader()->resizeSection(7,60);
        deviceTable->horizontalHeader()->resizeSection(8,200);

        deviceTable->verticalHeader()->hide();
        //�Ű�
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(deviceTable);
        deviceGroupBox->setLayout(layout);
}

void MainWindow::manualAddDevice(int index)
{
    QString name;
    name.sprintf("DeviceNme%d",index);

    int row = deviceTable->rowCount();
    deviceTable->setRowCount(row + 1);
    QTableWidgetItem *item0 = new QTableWidgetItem(tr(""));
    QTableWidgetItem *item1 = new QTableWidgetItem(tr(""));
    name.sprintf("192.168.1.%d",100+index);
    QTableWidgetItem *item2 = new QTableWidgetItem(tr(""));
    QTableWidgetItem *item5 = new QTableWidgetItem(tr(""));
    QTableWidgetItem *item6 = new QTableWidgetItem(tr("SetON"));
    QTableWidgetItem *item7 = new QTableWidgetItem(tr("SetOFF"));
    //
    deviceTable->setItem(row, 0, item0);
    deviceTable->setItem(row, 1, item1);
    deviceTable->setItem(row, 2, item2);
    deviceTable->setItem(row, 5, item5);
    deviceTable->setItem(row, 6, item6);
    deviceTable->setItem(row, 7, item7);

    item0->setCheckState(Qt::Checked);
    item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);//���ֻ������

    deviceTable->openPersistentEditor(item5);  //����˫���Ϳ�����ʾ�ؼ�����ʽ
    deviceTable->openPersistentEditor(item6);  //����˫���Ϳ�����ʾ�ؼ�����ʽ
    deviceTable->openPersistentEditor(item7);
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
    //�������ģ��ַ�����
    QString portstr;
    portstr.sprintf("%d",senderport);
    QString str = sender.toString();
    str += ":";
    str += portstr;
    debuginfo(("%s",str.toAscii().data()));
    //����IP��ַ�Ͷ˿ںţ������Ѿ��е����ݣ�������ˣ���������Ϣ�������Լ������Լ�������
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
    }
    //���û���ҵ����򴴽�һ����Ȼ����ת����Ϣ��������
    //�豸������һ�������Ķ��󣬿��Դ����ӵ���Լ�����
}

void MainWindow::UpdateDeviceData(QSharedPointer<QRelayDeviceControl> & pdev)
{
}
