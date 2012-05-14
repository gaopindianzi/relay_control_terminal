#include "QDeviceStatusDelegate.h"
#include <QLabel>

#include "debug.h"

#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

QDeviceStatusDelegate::QDeviceStatusDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QDeviceStatusDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem & option,
                                     const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QLabel * edit = new QLabel(parent);
    edit->setText(pdev->GetStatus());
    return edit;
}

void QDeviceStatusDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    QLabel *edit = qobject_cast<QLabel *>(editor);
    edit->setText(pdev->GetStatus());
}

void QDeviceStatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QSharedPointer<QRelayDeviceControl> pdev = qVariantValue<RelayDeviceSharePonterType>(index.data());
    model->setData(index, qVariantFromValue(pdev));
}

void QDeviceStatusDelegate::buttonClicked(bool click)
{
}



