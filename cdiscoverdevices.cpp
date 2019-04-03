#include <QTableView>
#include "cdiscoverdevices.h"
#include "ui_cdiscoverdevices.h"
#include "switch.h"
#include "globals.h"


CDiscoverDevices::CDiscoverDevices(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDiscoverDevices)
{


    ui->setupUi(this);
    setFixedSize(width(), height());

    //Load locators
    CDeviceLocator * p;
    QVariant v;
    for (int i =0; i < global::g_Locators.count(); i++ )
    {
        p = global::g_Locators.value(i);
        v.setValue(static_cast <void *> (p));
         ui->cmbDeviceType->addItem((static_cast<CGreeACLocator *>(p))->GetName(),v);
    }



}
void CDiscoverDevices::ClearTable()
{
    QObject *s;

    while (ui->tbvDiscovered->rowCount() >0)
    {
        for (int c = 0; c < ui->tbvDiscovered->columnCount(); c++)
        {
            s =  ui->tbvDiscovered->cellWidget(0,c);
            if (s)
              delete s;
         }
      ui->tbvDiscovered->removeRow(0);

    }
}

void CDiscoverDevices::SetTimeOut(int value)
{
    ui->spnTimeOut->setValue(value);
}
CDiscoverDevices::~CDiscoverDevices()
{

    ClearTable();
    delete ui;
}

void CDiscoverDevices::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton->setText(QString("scanning..."));

    CDeviceLocator * p = static_cast<CDeviceLocator *>( ui->cmbDeviceType->currentData().value<void *>());
    QHostAddress host;
    long port = 7000;
    if (ui->grpRemote->isChecked())
    {
        host.setAddress(ui->txtAddress->text());
        port = ui->txtPort->text().toLong();
    }
    else host.setAddress(QString("255.255.255.255"));//192.168.90.255")); //TO DO: Use the interface IP

   // m_dev.clear();
    ClearTable();

    p->ScanDevices(ui->spnTimeOut->value() * 1000,host,port,&m_dev);
    while (p->isBusy())
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);

    QTableWidget *t = ui->tbvDiscovered;
    long row = qMax(0, t->rowCount() - 1);
    int nDev = m_dev.count();

    for (int i = 0; i < nDev; i++) {
        t->insertRow(row);
        t->setCellWidget(row,0, new SwitchButton(t,SwitchButton::YESNO));
        t->setItem(row, 1, new QTableWidgetItem(m_dev.value(i)->GetID()));
        t->setItem(row, 2, new QTableWidgetItem(m_dev.value(i)->GetAddr()));
        t->setItem(row, 3, new QTableWidgetItem(m_dev.value(i)->GetVendor()));
        t->setItem(row, 4, new QTableWidgetItem(m_dev.value(i)->GetDeviceName()));
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText(QString("Scan"));

}

void CDiscoverDevices::on_pushButton_3_clicked()
{
    done(0);

}

void CDiscoverDevices::on_btnAddSelect_clicked()
{
    QString devid;
    long index;
//    CDevice *pDev;

    for (int i = 0; i < ui->tbvDiscovered->rowCount(); i ++)
        if (static_cast<SwitchButton *>(ui->tbvDiscovered->cellWidget(i,0))->value())
        {
            devid = ui->tbvDiscovered->item(i,1)->text();
            index = m_dev.FindByID(&devid);
            if (index > -1)
                 global::g_pKnownDevices->AddIfNotExists(m_dev.takeAt(index));

        }
    qInfo() << "Known devices: " <<  global::g_pKnownDevices->count() << endl;

}
