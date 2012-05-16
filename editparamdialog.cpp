#include "editparamdialog.h"
#include "ui_editparamdialog.h"


EditParamDialog::EditParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditParamDialog)
{
    ui->setupUi(this);

    CreateDeviceList();
}

EditParamDialog::~EditParamDialog()
{
    delete ui;
}


void EditParamDialog::CreateDeviceList(void)
{
}
