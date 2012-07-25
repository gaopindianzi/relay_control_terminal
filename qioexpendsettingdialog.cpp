#include "qioexpendsettingdialog.h"
#include "ui_qioexpendsettingdialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

QIoExpendSettingDialog::QIoExpendSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIoExpendSettingDialog)
{
    ui->setupUi(this);    
    //���ݲ��ֻ���IO����ؼ�

   QVBoxLayout * layout_left   = new QVBoxLayout();
   QVBoxLayout * layout_right = new QVBoxLayout();

   QPushButton * pb1 = new QPushButton(tr("��ť1���ƹ����"));
   QPushButton * pb2 = new QPushButton(tr("��ť2���ƹ����"));
   QPushButton * pb3 = new QPushButton(tr("��ť3���ƹ����"));
   QPushButton * pb4 = new QPushButton(tr("��ť4���ƹ����"));

   layout_left->addWidget(pb1);
   layout_left->addWidget(pb2);
   layout_right->addWidget(pb3);
   layout_right->addWidget(pb4);


   ui->gridLayout->addLayout(layout_left,0,0);
   ui->gridLayout->addLayout(layout_right,0,1);



}

QIoExpendSettingDialog::~QIoExpendSettingDialog()
{
    delete ui;
}
