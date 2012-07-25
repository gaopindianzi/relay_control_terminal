#include "qioexpendsettingdialog.h"
#include "ui_qioexpendsettingdialog.h"

QIoExpendSettingDialog::QIoExpendSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIoExpendSettingDialog)
{
    ui->setupUi(this);
    //根据布局绘制IO输出控件
}

QIoExpendSettingDialog::~QIoExpendSettingDialog()
{
    delete ui;
}
