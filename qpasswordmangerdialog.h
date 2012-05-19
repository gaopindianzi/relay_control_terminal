#ifndef QPASSWORDMANGERDIALOG_H
#define QPASSWORDMANGERDIALOG_H

#include <QDialog>

namespace Ui {
class QPasswordMangerDialog;
}

class QPasswordMangerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QPasswordMangerDialog(QWidget *parent = 0);
    ~QPasswordMangerDialog();
    
private:
    Ui::QPasswordMangerDialog *ui;
};

#endif // QPASSWORDMANGERDIALOG_H
