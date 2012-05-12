#include "QOnOffPushButton.h"

#include <QPainter>
#include <QMouseEvent>

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
    : QItemDelegate(parent)
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
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 18));
    QString str;
    str.sprintf("0x%02X",this->relay_bitmap);
    painter.drawText(rect(), Qt::AlignCenter, str);
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
     this->setText("");
    this->update();
}


QRelayValueSingalChannalButtonDelegate::QRelayValueSingalChannalButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
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
    model->setData(index, pushbutton->relay_bitmap);
}

void QRelayValueSingalChannalButtonDelegate::emitCommitData()
{
    emit commitData(qobject_cast<QWidget *>(sender()));
}

void QRelayValueSingalChannalButtonDelegate::buttonClicked(bool click)
{
    //QPushButton *pushbutton = qobject_cast<QPushButton *>(sender());
    debuginfo(("on off button clicked"));
}



