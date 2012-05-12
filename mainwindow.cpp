#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include "QCheckBoxDelegate.h"
#include "QOnOffPushButton.h"
#include "debug.h"

#define THISINFO                1
#define THISERROR            1
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
        //排版
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
    QTableWidgetItem *item1 = new QTableWidgetItem(name);
    name.sprintf("192.168.1.%d",100+index);
    QTableWidgetItem *item2 = new QTableWidgetItem(name);
    QTableWidgetItem *item5 = new QTableWidgetItem(tr("SingalChannelButton"));
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
    item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);//表格只读属性

    deviceTable->openPersistentEditor(item5);  //不用双击就可以显示控件的形式
    deviceTable->openPersistentEditor(item6);  //不用双击就可以显示控件的形式
    deviceTable->openPersistentEditor(item7);
}


