#include <QtGui/QApplication>
#include <QMainWindow>
#include <QTextCodec>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QString>
#include <QWidget>
#include <QObject>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSystemSemaphore sema("JAMKey",1,QSystemSemaphore::Open);
    sema.acquire();   // 在临界区操作共享内存   SharedMemory
    QSharedMemory mem("SystemObject");// 全局对象名
    if (!mem.create(1))// 如果全局对象以存在则退出
    {
        QMessageBox box(QMessageBox::Warning,
                        "Warning","The program has already been running!");
        box.exec();
        sema.release();// 如果是 Unix 系统，会自动释放。
        return 0;
    }

    sema.release();// 临界区


    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    MainWindow w;

    w.show();
    return a.exec();
}
