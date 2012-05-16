#ifndef EDITPARAMDIALOG_H
#define EDITPARAMDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

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
private:
    Ui::EditParamDialog *ui;
};

#endif // EDITPARAMDIALOG_H
