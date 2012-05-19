#include "qpasswordmangerdialog.h"
#include "ui_qpasswordmangerdialog.h"

QPasswordMangerDialog::QPasswordMangerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPasswordMangerDialog)
{
    ui->setupUi(this);
}

QPasswordMangerDialog::~QPasswordMangerDialog()
{
    delete ui;
}
