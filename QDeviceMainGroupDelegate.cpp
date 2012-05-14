#include "QDeviceMainGroupDelegate.h"
#include <QLineEdit>


QDeviceNameDelegate::QDeviceNameDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QDeviceNameDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & option,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QLineEdit * edit = new QLineEdit(parent);
    edit->setText(pdev->GetDeviceName());
    return edit;
}

void QDeviceNameDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    edit->setText(pdev->GetDeviceName());
}

void QDeviceNameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    model->setData(index, qVariantFromValue(pdev));
}

void QDeviceNameDelegate::buttonClicked(bool click)
{
}










QDeviceMainGroupDelegate::QDeviceMainGroupDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QDeviceMainGroupDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & option,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::createEditor(parent,option,index);
}

void QDeviceMainGroupDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::setEditorData(editor,index);
}

void QDeviceMainGroupDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::setModelData(editor,model,index);
}

void QDeviceMainGroupDelegate::buttonClicked(bool click)
{
}














QDeviceSlaveGroupDelegate::QDeviceSlaveGroupDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QDeviceSlaveGroupDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & option,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::createEditor(parent,option,index);
}

void QDeviceSlaveGroupDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::setEditorData(editor,index);
}

void QDeviceSlaveGroupDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    return CDeviceDelegate::setModelData(editor,model,index);
}

void QDeviceSlaveGroupDelegate::buttonClicked(bool click)
{
}
