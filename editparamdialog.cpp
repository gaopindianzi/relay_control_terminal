#include "editparamdialog.h"
#include "ui_editparamdialog.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include "QDeviceList.h"

#include "debug.h"
#define THISINFO                1
#define THISERROR            1
#define THISASSERT          1

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
    list_devices = new QListWidget(this);
    list_devices->setSelectionMode(QAbstractItemView::ContiguousSelection);
    connect(list_devices,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(list_devices);
    ui->groupBoxDeviceList->setLayout(layout);
}
void EditParamDialog::InsertDevice(RelayDeviceSharePonterType & pdev)
{
    int row = list_devices->count();
    QDeviceList * item = new QDeviceList(pdev);
    item->setText(pdev->GetHostAddressString());
    this->list_devices->insertItem(row,item);
}
void	EditParamDialog::itemClicked ( QListWidgetItem * item )
{
    QDeviceList * it = (QDeviceList *)item;
    debuginfo(("item clicked:%s",it->pdevice->GetDeviceName().toAscii().data()));
}

