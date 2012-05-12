#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QCheckBox>
#include <QGroupBox>
#include <QTableWidget>


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
private:
    QWidget *centralWidget;
    //device table
    QGroupBox *deviceGroupBox;
    QTableWidget *deviceTable;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
