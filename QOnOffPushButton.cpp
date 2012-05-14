#include "QOnOffPushButton.h"

#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <QColor>
#include <QRect>

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
    setText("");
    if(onoff) {
        onoff = false;
    } else {
        onoff = true;
    }
    this->update();
    return onoff;
}








QOnOffPushDelegate::QOnOffPushDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QOnOffPushDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & /* option */,
                                     const QModelIndex &index) const
{
    QPushButton *pushbutton = new QPushButton(parent);
    connect(pushbutton, SIGNAL(clicked ( bool )), this, SLOT(buttonClicked(bool)));
    return pushbutton;
}

void QOnOffPushDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{

    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    pushbutton->setText(index.model()->data(index).toString());
}

void QOnOffPushDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QPushButton *pushbutton = qobject_cast<QPushButton *>(editor);
    model->setData(index, pushbutton->text());
}

void QOnOffPushDelegate::emitCommitData()
{
    emit commitData(qobject_cast<QWidget *>(sender()));
}

void QOnOffPushDelegate::buttonClicked(bool click)
{
    //QPushButton *pushbutton = qobject_cast<QPushButton *>(sender());
    //debuginfo(("on off button clicked"));
}












QRelayValueSingalChannalButton::QRelayValueSingalChannalButton(QWidget * parent)
    : QPushButton(parent)
{
    relay_bitmap = 0;
    this->setText("");
}

void QRelayValueSingalChannalButton::paintEvent ( QPaintEvent * event )
{

    QPainter painter(this);
#if 1
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 18));
    QString str;
    str.sprintf("0x%02X",this->relay_bitmap);
    painter.drawText(rect(), Qt::AlignCenter, str);
#endif
    QRect rct = rect();
    int w = rct.width() / 5;
    for(int i=0;i<5;i++) {
        if(this->relay_bitmap&(1<<i)) {
            QBrush brush(QColor(0,250,0));
            painter.fillRect(i*w,0,w,rct.height(),brush);
        } else {
            QBrush brush(QColor(200,200,200));
            painter.fillRect(i*w,0,w,rct.height(),brush);
        }
    }
    //painter.drawLine(0,0,100,20);
}

int QRelayValueSingalChannalButton::mouseAtButtonPosition(int x)
{
    int star = (x / (this->width() / 5));
    if (star < 0 || star > 5)
        return -1;

    return star;
}

void QRelayValueSingalChannalButton::mousePressEvent ( QMouseEvent * event )
{
    int pos = event->x();
    int x = mouseAtButtonPosition(pos);
    if(x >= 0 && x < 32) relay_bitmap ^= (1<<x);
    debuginfo(("mouse at position %d/%d=%drelay=0x%02X",pos,this->width(),x,relay_bitmap));
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
    QRelayValueSingalChannalButton *pushbutton = new QRelayValueSingalChannalButton(parent);
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
    //model->setData(index, pushbutton->relay_bitmap);
     index.model()->data(index) = pushbutton->relay_bitmap;
}

void QRelayValueSingalChannalButtonDelegate::emitCommitData()
{
   // emit commitData(qobject_cast<QWidget *>(sender()));
}

void QRelayValueSingalChannalButtonDelegate::buttonClicked(bool click)
{
    //QPushButton *pushbutton = qobject_cast<QPushButton *>(sender());
    debuginfo(("on off button clicked"));
}



