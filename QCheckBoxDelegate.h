#ifndef QCHECKBOXDELEGATE_H
#define QCHECKBOXDELEGATE_H

#include <QItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QComboBox>
#include "CDeviceDelegate.h"

class QCheckBoxDelegate : public CDeviceDelegate
{
    Q_OBJECT

public:
    QCheckBoxDelegate(QObject *parent = 0);
//! [0]

//! [1]
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

//! [1] //! [2]
private slots:
    void emitCommitData();
    void buttonClicked ( bool );
};

#endif // QCHECKBOXDELEGATE_H
