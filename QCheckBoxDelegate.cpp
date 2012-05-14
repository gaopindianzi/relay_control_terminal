#include "QCheckBoxDelegate.h"
#include <QCheckBox>

#include "debug.h"

#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1


QDeviceCheckBox::QDeviceCheckBox ( QWidget * parent)
    : QCheckBox(parent)
{
}

QCheckBoxDelegate::QCheckBoxDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

void QCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    //Ïû³ýÏÔÊ¾
}

QWidget *QCheckBoxDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &  option ,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QDeviceCheckBox * edit = new QDeviceCheckBox(parent);
    edit->move(5,0);
    edit->setText("");
    return edit;
}

void QCheckBoxDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QDeviceCheckBox *edit = qobject_cast<QDeviceCheckBox *>(editor);
    edit->move(5,0);
    edit->setText("");
    debuginfo(("check box set edit data"));
    edit->setCheckState(Qt::Unchecked);
}

void QCheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    model->setData(index, qVariantFromValue(pdev));
}

void QCheckBoxDelegate::buttonClicked(bool click)
{
}



