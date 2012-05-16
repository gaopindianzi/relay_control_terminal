#include "QOnOffPushButton.h"

#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <QColor>
#include <QRect>
#include <QSharedPointer>
#include "QRelayDeviceControl.h"
#include "mainwindow.h"
#include "debug.h"
#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

QOnOffPushButton::QOnOffPushButton(QWidget * parent)
    : QPushButton(parent)
{
}

void QOnOffPushButton::paintEvent ( QPaintEvent * event )
{
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 18));
    painter.drawText(rect(), Qt::AlignCenter, this->text());
}

bool QOnOffPushButton::SetOnOff(void)
{
    if(onoff) {
        onoff = false;
    } else {
        onoff = true;
    }
    this->update();
    return onoff;
}





QSetOnPushDelegate::QSetOnPushDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QSetOnPushDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & /* option */,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QPushButton *pushbutton = new QPushButton(parent);
    connect(pushbutton, SIGNAL(clicked ( bool )), this, SLOT(buttonClicked(bool)));
    return pushbutton;
}

void QSetOnPushDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    pushbutton->setText(tr("SetON"));
}

void QSetOnPushDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    model->setData(index, pushbutton->text());
}

void QSetOnPushDelegate::buttonClicked(bool click)
{
}



QSetOffPushDelegate::QSetOffPushDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QSetOffPushDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & /* option */,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QPushButton *pushbutton = new QPushButton(parent);
    connect(pushbutton, SIGNAL(clicked ( bool )), this, SLOT(buttonClicked(bool)));
    return pushbutton;
}

void QSetOffPushDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    pushbutton->setText(tr("SetOFF"));
}

void QSetOffPushDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    model->setData(index, pushbutton->text());
}

void QSetOffPushDelegate::buttonClicked(bool click)
{
}












QRelayValueSingalChannalButton::QRelayValueSingalChannalButton(RelayDeviceSharePonterType pdev,QWidget * parent)
    : QPushButton(parent)
{
    relay_bitmap = 0;
    pdevice = pdev;
    this->setText("");
}

void QRelayValueSingalChannalButton::paintEvent ( QPaintEvent * event )
{

    QPainter painter(this);

    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 18));
    QString str;
    str.sprintf("0x%02X",this->relay_bitmap);
    painter.drawText(rect(), Qt::AlignCenter, str);
    QRect rct = rect();

    int w = rct.width() / pdevice->GetIoOutNum();
    this->pdevice->relay_bitmask.resize(this->pdevice->GetIoOutNum());
    for(int i=0;i<pdevice->GetIoOutNum();i++) {        
        if(this->pdevice->relay_bitmask[i]) {
            QBrush brush(QColor(0,250,0));
            painter.fillRect(i*w,0,w,rct.height(),brush);
        } else {
            QBrush brush(QColor(200,200,200));
            painter.fillRect(i*w,0,w,rct.height(),brush);
        }
    }
}

int QRelayValueSingalChannalButton::mouseAtButtonPosition(int x)
{
    int star = (x / (this->width() / pdevice->GetIoOutNum()));
    if (star < 0 || star > pdevice->GetIoOutNum())
        return -1;

    return star;
}

void QRelayValueSingalChannalButton::mousePressEvent ( QMouseEvent * event )
{
    int pos = event->x();
    int x = mouseAtButtonPosition(pos);
    if(x >= 0 && x < 32) relay_bitmap ^= (1<<x);
    debuginfo(("mouse at position %d/%d=%drelay=0x%02X",pos,this->width(),x,relay_bitmap));
    //发送指令，翻转这个位
    pdevice->ConvertIoOutOneBitAndSendCmd(x);
    //pdevice->relay_bitmask[x] = !pdevice->relay_bitmask[x];
    this->update();
}


QRelayValueSingalChannalButtonDelegate::QRelayValueSingalChannalButtonDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QRelayValueSingalChannalButtonDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & /* option */,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QRelayValueSingalChannalButton *pushbutton = new QRelayValueSingalChannalButton(pdev,parent);
    connect(pushbutton, SIGNAL(clicked ( bool )), this, SLOT(buttonClicked(bool)));
    return pushbutton;
}

void QRelayValueSingalChannalButtonDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QRelayValueSingalChannalButton *pushbutton = qobject_cast<QRelayValueSingalChannalButton *>(editor);
    pushbutton->relay_bitmap = index.model()->data(index).toUInt();
}

void QRelayValueSingalChannalButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QRelayValueSingalChannalButton *pushbutton = qobject_cast<QRelayValueSingalChannalButton *>(editor);
     index.model()->data(index) = pushbutton->relay_bitmap;
}

void QRelayValueSingalChannalButtonDelegate::buttonClicked(bool click)
{
    //QPushButton *pushbutton = qobject_cast<QPushButton *>(sender());
    debuginfo(("on off button clicked"));
}

