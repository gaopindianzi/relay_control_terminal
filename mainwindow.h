#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QCheckBox>
#include <QGroupBox>
#include <QTableWidget>
#include <QUdpSocket>
#include <QSharedPointer>
#include "QRelayDeviceControl.h"
#include "CDeviceTableWidgetItem.h"
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void CreateDevcieTable(void);
    void manualAddDevice(int index);
    void InsertDevice(QSharedPointer<QRelayDeviceControl> & pdev);
public:
    void InitUdpSocket(void);
    void processTheDeviceData(QByteArray & data,QHostAddress & sender,quint16 senderport);
private slots:
    void UdpreadPendingDatagrams();
    void DeviceInfoChanged(QString hostaddrID);
private:
    QWidget *centralWidget;
    //device table
    QGroupBox *deviceGroupBox;
    QTableWidget *deviceTable;
private:
    QSharedPointer<QUdpSocket>  pUdpSocket;
    QMap<QString,QSharedPointer<QRelayDeviceControl> >   mydevicemap;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
