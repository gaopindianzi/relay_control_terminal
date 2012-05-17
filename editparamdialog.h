#ifndef EDITPARAMDIALOG_H
#define EDITPARAMDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QListWidget>
#include "QRelayDeviceControl.h"

namespace Ui {
class EditParamDialog;
}

class EditParamDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditParamDialog(QWidget *parent = 0);
    ~EditParamDialog();
public:
    void CreateDeviceList(void);
    void InsertDevice(RelayDeviceSharePonterType & pdev);
protected slots:
    void	itemClicked ( QListWidgetItem * item );
private:
    Ui::EditParamDialog *ui;
    QListWidget * list_devices;
};

#endif // EDITPARAMDIALOG_H
