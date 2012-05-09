#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QCheckBox>

namespace Ui {
    class MainWindow;
}



class TrackDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TrackDelegate(QWidget * pwidget,QObject * parent = 0);
    void paint(QPainter * painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    QWidget * createEditor(QWidget * parent,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget * editor,const QModelIndex & index) const;
    void setModelData(QWidget * editor,QAbstractItemModel * model,const QModelIndex & index) const;
private slots:
    void commitAndCloseEditor();
private:
    QCheckBox  *  checkbox;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
