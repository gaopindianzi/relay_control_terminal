#ifndef QDEVCESTATUSDELEGATE_H
#define QDEVCESTATUSDELEGATE_H
#include <QItemDelegate>
#include <QWidget>
#include <QSharedPointer>
#include "multimgr_device_dev.h"
#include "QRelayDeviceControl.h"
#include "CDeviceDelegate.h"

class QDeviceStatusDelegate : public CDeviceDelegate
{
    Q_OBJECT

public:
    QDeviceStatusDelegate(QObject *parent = 0);
//! [0]

//! [1]
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

//! [1] //! [2]
private slots:
    void buttonClicked ( bool );
};


#endif // QDEVCESTATUSDELEGATE_H
