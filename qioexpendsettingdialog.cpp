#include "qioexpendsettingdialog.h"
#include "ui_qioexpendsettingdialog.h"

QIoExpendSettingDialog::QIoExpendSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIoExpendSettingDialog)
{
    ui->setupUi(this);
    //���ݲ��ֻ���IO����ؼ�
}

QIoExpendSettingDialog::~QIoExpendSettingDialog()
{
    delete ui;
}
