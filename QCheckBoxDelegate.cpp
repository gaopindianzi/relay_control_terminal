#include "QCheckBoxDelegate.h"


QCheckBoxDelegate::QCheckBoxDelegate(QObject *parent)
    : CDeviceDelegate(parent)
{
}

QWidget *QCheckBoxDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &  option ,
                                     const QModelIndex &index) const
{
    return QItemDelegate::createEditor(parent,option,index);
}

void QCheckBoxDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    this->setEditorData(editor,index);
}

void QCheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    this->setModelData(editor,model,index);
}



void QCheckBoxDelegate::emitCommitData()
{
    emit commitData(qobject_cast<QWidget *>(sender()));
}



void QCheckBoxDelegate::buttonClicked(bool click)
{
}


