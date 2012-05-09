#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "debug.h"

#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setItemDelegate(new TrackDelegate(ui->tableWidget));
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(3);
    for(int row=0;row<3;row++)
    {
        QString str;
        str.sprintf("ÐÐ:%d",row);
        QTableWidgetItem * item = new QTableWidgetItem(str);
        ui->tableWidget->setItem(row,0,item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



TrackDelegate::TrackDelegate(QWidget * pwidget,QObject *parent)
    //: checkbox(new QCheckBox(pwidget))
{
    debuginfo(("TrackDelegate´´½¨"));
}

void TrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //debuginfo(("delegate paint"));
    if(0) { //index.column() == 1) {
        int secs = index.model()->data(index,Qt::DisplayRole).toInt();
        QString text = QString("%1:%2").arg(secs/60,2,10,QChar('0')).arg(secs%60,2,10,QChar('0'));
        QStyleOptionViewItem myOption = option;
    }
    QItemDelegate::paint(painter,option,index);
}

QWidget * TrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    debuginfo(("delegate createEditor"));
    return QItemDelegate::createEditor(parent,option,index);
   //return  new QCheckBox(parent);
}

void TrackDelegate::setEditorData(QWidget * editor,const QModelIndex & index) const
{
    debuginfo(("delegate setEditorData"));
    //QItemDelegate::setEditorData(editor,index);
}
void TrackDelegate::setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex & index) const
{
    debuginfo(("delegate setModelData"));
    QItemDelegate::setModelData(editor,model,index);
}

void TrackDelegate::commitAndCloseEditor()
{
    debuginfo(("commitAndCloseEditor"));
}
