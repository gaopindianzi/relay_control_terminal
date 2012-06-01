#ifndef QIOEXPENDSETTINGDIALOG_H
#define QIOEXPENDSETTINGDIALOG_H

#include <QDialog>
#include "QRelayDeviceControl.h"

namespace Ui {
class QIoExpendSettingDialog;
}

class QIoExpendSettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QIoExpendSettingDialog(QWidget *parent = 0);
    ~QIoExpendSettingDialog();
    
private:
    Ui::QIoExpendSettingDialog *ui;
};

#endif // QIOEXPENDSETTINGDIALOG_H
