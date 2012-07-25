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
    //根据布局绘制IO输出控件

   QVBoxLayout * layout_left   = new QVBoxLayout();
   QVBoxLayout * layout_right = new QVBoxLayout();

   QPushButton * pb1 = new QPushButton(tr("按钮1，灯光控制"));
   QPushButton * pb2 = new QPushButton(tr("按钮2，灯光控制"));
   QPushButton * pb3 = new QPushButton(tr("按钮3，灯光控制"));
   QPushButton * pb4 = new QPushButton(tr("按钮4，灯光控制"));

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
