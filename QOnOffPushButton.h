#ifndef QONOFFPUSHBUTTON_H
#define QONOFFPUSHBUTTON_H
#include <QPushButton>
#include <QItemDelegate>
#include <QWidget>
#include <QSharedPointer>
#include "multimgr_device_dev.h"
#include "QRelayDeviceControl.h"
#include "CDeviceDelegate.h"

class QOnOffPushButton : public QPushButton
{
    Q_OBJECT
public:
    QOnOffPushButton(QWidget * parent = 0 );
    bool SetOnOff(void);
private:
    void paintEvent ( QPaintEvent * event );
public:
    bool onoff;
    QSharedPointer<QRelayDeviceControl>  pdevice;
};


class QSetOnPushDelegate : public CDeviceDelegate
{
    Q_OBJECT

public:
    QSetOnPushDelegate(QObject *parent = 0);
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

class QSetOffPushDelegate : public CDeviceDelegate
{
    Q_OBJECT

public:
    QSetOffPushDelegate(QObject *parent = 0);
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


class QRelayValueSingalChannalButton : public QPushButton
{
    Q_OBJECT
public:
    QRelayValueSingalChannalButton(QWidget * parent = 0 );
private:
    void paintEvent ( QPaintEvent * event );
protected:
    void mousePressEvent ( QMouseEvent * event );
    int   mouseAtButtonPosition(int x);
public:
    unsigned int relay_bitmap;
};





class QRelayValueSingalChannalButtonDelegate : public CDeviceDelegate
{
    Q_OBJECT

public:
    QRelayValueSingalChannalButtonDelegate(QObject *parent = 0);
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




#endif // QONOFFPUSHBUTTON_H
