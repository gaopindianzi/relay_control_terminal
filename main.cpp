#include <QtGui/QApplication>
#include <QTextCodec>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    MainWindow w;

    //w.setWindowFlags(w.windowFlags() & ~Qt::WindowMinimizeButtonHint );

    w.show();

    return a.exec();
}
