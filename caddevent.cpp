#include "caddevent.h"
#include "ui_caddevent.h"
#include "globals.h"
#include "cconditionselector.h"
#include "switch.h"

CAddEvent::CAddEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddEvent)
{
    QVariant v;
    long n = global::g_pKnownDevices->count();
    m_pEvent = nullptr;

    ui->setupUi(this);

    ui->btnSetDevice->setEnabled(n>0);
    if (!n) //no devices
        return;
    ui->cmbDeviceName->clear();
    ui->cmbDeviceName->setEnabled(true);
    for (int i =0; i < n; i++)
    {
        v.setValue(global::g_pKnownDevices->value(i)->GetID());
        ui->cmbDeviceName->addItem(global::g_pKnownDevices->value(i)->GetDeviceName(),v);
    }
    ui->tableWidget->horizontalHeader()->resizeSection(2,150);
    setFixedSize(width(), height());
    ui->spnMaxWait->setMinimum(global::g_DevTimeout);
}

CAddEvent::~CAddEvent()
{
    ClearCondList();
    delete ui;
}

void CAddEvent::SetHint(const QString hint)
{
    ui->lblHint->setText(hint);
}

void CAddEvent::LoadUI()
{
    ui->txtName->setText(m_pEvent->GetName());
    ui->grpRemote->setChecked(m_pEvent->GetOverride() == 0 ? Qt::Unchecked : Qt::Checked);
    ui->txtIP->setText(m_pEvent->GetHost());
    ui->txtPort->setText(QString(m_pEvent->GetOverridePort()));
    ui->spnMaxWait->setValue(m_pEvent->GetWaitTime());
    ui->chkActive->setChecked(m_pEvent->GetActive()  == 0 ? Qt::Unchecked : Qt::Checked);
    ui->chkSync->setCheckState(m_pEvent->GetSync()  == 0 ? Qt::Unchecked : Qt::Checked);
    ui->chkLog->setCheckState(m_pEvent->GetLogging()  == 0 ? Qt::Unchecked : Qt::Checked);

    QString devid = m_pEvent->GetDeviceID();
    if (!devid.isEmpty())
    {
        if (global::g_pKnownDevices->FindByID(&devid) < 0)
        {
            QMessageBox::warning(this,"Error",QString("Device with that ID is no longer available: ").append(devid),QMessageBox::Ok);
        }
        else
            ui->cmbDeviceName->setCurrentText(global::g_pKnownDevices->GetByID(&devid)->GetDeviceName());
    }
    else // Adding event
        if(global::g_pKnownDevices->count() >0) //add the first device in the list
            ui->cmbDeviceName->setCurrentText(global::g_pKnownDevices->at(0)->GetDeviceName());
        else {// No devices
                ui->btnSetDevice->setEnabled(false);
                ui->btnExec->setEnabled(false);
                ui->buttonBox->setEnabled(false);
              }


    ReloadCondList();
}

void CAddEvent::SaveUI()
{
    m_pEvent->SetName(ui->txtName->text());
    m_pEvent->SetOverride(ui->grpRemote->isChecked() ? 1 : 0);
    m_pEvent->SetHost(ui->txtIP->text());
    m_pEvent->SetOverridePort(ui->txtPort->text().toInt());
    m_pEvent->SetWaitTime(ui->spnMaxWait->value());
    m_pEvent->SetActive(ui->chkActive->checkState() == Qt::Unchecked ? 0 : 1);
    m_pEvent->SetSync(ui->chkSync->checkState() == Qt::Unchecked ? 0 : 1);
    m_pEvent->SetLogging(ui->chkLog->checkState() == Qt::Unchecked ? 0 : 1);

    QString devkey = ui->cmbDeviceName->currentData().toString();
    m_pEvent->SetDeviceID(devkey);
    //SaveConditions active & negate
    SwitchButton *pS;
    CCondition *pC;
    QString id ;
    QTableWidget *t = ui->tableWidget;

    for (int i = t->rowCount()-1; i >= 0; i--)
    {
        //Find the condition
        id = t->item(i,3)->data(Qt::UserRole).toString();
        pC = m_pEvent->GetConditions()->GetById(id);
        //Set active & negate
        pS = reinterpret_cast<SwitchButton *>(t->cellWidget(i,0));
        pC->SetActive(pS->value());
        pS = reinterpret_cast<SwitchButton *>(t->cellWidget(i,1));
        pC->SetNegate(pS->value());
    }

}

void CAddEvent::SetEvent(CEvent * pEvent)
{
    m_pEvent = pEvent;
    LoadUI();
}
void CAddEvent::ClearCondList()
{
    QObject *s;
    QTableWidget *t = ui->tableWidget;

    while (t->rowCount() >0)
    {
        for (int c = 0; c < t->columnCount(); c++)
        {
            s =  t->cellWidget(0,c);
            if (s)
              delete s;
         }
     t->removeRow(0);
    }
}

void CAddEvent::ReloadCondList()
{
    QTableWidget *t = ui->tableWidget;
    SwitchButton * pb;
    CONDLIST *cond = m_pEvent->GetConditions();


    ClearCondList();

    //add list

 //   long row = qMax(0, t->rowCount() - 1);
    int nDev = cond->count();

    for (int i = 0; i < nDev; i++)
    {
        t->insertRow(i);
        pb =  new SwitchButton(t,SwitchButton::YESNO);
        pb->setValue(cond->at(i)->GetActive());
        t->setCellWidget(i,0,pb);
        pb =  new SwitchButton(t,SwitchButton::BOOL);
        pb->setValue(cond->at(i)->GetNegate());
        t->setCellWidget(i,1,pb);
        t->setItem(i, 2,new QTableWidgetItem(cond->at(i)->GetName()));
        t->setItem(i, 3,new QTableWidgetItem(cond->at(i)->GetDesc()));
        t->item(i,3)->setData(Qt::UserRole,cond->at(i)->GetID());
    }
}

QString CAddEvent::GetSelectedCondID()
{
    QList<QTableWidgetItem *> sel = ui->tableWidget->selectedItems();
    if (sel.count()< 1)
        return QString();
//Selection contains only QTableWidgetItems (SwitchButtons are not in the selection)
//sel[1] actually is the third row, containing the name of the condition
    return sel[1]->data(Qt::UserRole).toString();
}

void CAddEvent::on_grpRemote_clicked()
{
    ui->txtIP->setEnabled(ui->grpRemote->isChecked());
    ui->txtPort->setEnabled(ui->grpRemote->isChecked());
    ui->btnCheck->setEnabled(ui->grpRemote->isChecked());
}

void CAddEvent::on_btnSetDevice_clicked()
{
    setEnabled(false);
    QString id = ui->cmbDeviceName->currentData().toString();
    m_pEvent->SetDeviceID(id);
    CDevice *pSelected = m_pEvent->GetWorkingDevice();//global::g_pKnownDevices->GetByID(&id);
    if (pSelected)
        pSelected->ShowDevProp();
    else
        Q_ASSERT(true);
    setEnabled(true);
}

void CAddEvent::on_btnAddCond_clicked()
{
    CConditionSelector dlg(this);
    CCondition * cond =nullptr;
    SaveUI();
    if (dlg.exec() != 0)
        cond = dlg.GetSelectedCondition();
    if (cond)
    {
        if (!cond->ShowProperties())
            delete cond;
        else
            m_pEvent->AddCondition(cond);
    }
    ReloadCondList();

}



void CAddEvent::on_btnCondProp_clicked()
{
    SaveUI();

    CCondition * pCond;
    QString id = GetSelectedCondID();
    pCond = m_pEvent->GetConditions()->GetById(id);
    if(!pCond)
        return;
    pCond->ShowProperties();
    ReloadCondList();
}

void CAddEvent::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->txtName->text().isEmpty())
    {
        QMessageBox::critical(this,"Error","The event must have a name.");
        return;
    }
    SaveUI();
    done(1);
}

void CAddEvent::on_btnDelCond_clicked()
{
    SaveUI();

    CCondition * pCond;
    QString id = GetSelectedCondID();

    pCond = m_pEvent->GetConditions()->GetById(id);
    if(!pCond)
        return;
    QString name = pCond->GetDesc();
   if (QMessageBox::question(this,"Confirm",name.prepend("Delete condition ")) == QMessageBox::No)
       return;
    m_pEvent->DelCondition(id);
    ReloadCondList();

}

void CAddEvent::on_btnUp_clicked()
{
    SaveUI();

     int index = m_pEvent->GetConditions()->GetIndexById(GetSelectedCondID());
     if (index < 0)
         return;
    m_pEvent->GetConditions()->ItemUp(index);
    ReloadCondList();
}

void CAddEvent::on_btnDown_clicked()
{
    SaveUI();

    int index = m_pEvent->GetConditions()->GetIndexById(GetSelectedCondID());
    if (index < 0)
        return;
    m_pEvent->GetConditions()->ItemDown(index);
    ReloadCondList();
}

void CAddEvent::on_btnExec_clicked()
{
    if (QMessageBox::question(this,"Execute event","Execute this event synchronous ?") == QMessageBox::No)
        return;
    SaveUI();
    //Enable sync, adctive and log of the event for test purposes
    m_pEvent->SetSync(1);
    m_pEvent->SetActive(1);
    m_pEvent->SetLogging(1);

    this->setEnabled(false);
    m_pEvent->Execute();
    QMessageBox::information(this,"Execute event","Execution completed. Check the Log for details");
    this->setEnabled(true);
    //Return event prop
    m_pEvent->SetSync(ui->chkSync->checkState() == Qt::Checked ? 1 : 0);
    m_pEvent->SetActive(ui->chkActive->checkState() == Qt::Checked ? 1 : 0);
    m_pEvent->SetLogging(ui->chkLog->checkState() == Qt::Checked ? 1 : 0);
}

void CAddEvent::on_btnTest_clicked()
{
    SaveUI();

    if (m_pEvent->TestConditions())
        QMessageBox::information(this,"Conditions test","The event will be executed and will set the data to the device!");
    else
        QMessageBox::information(this,"Conditions test","The event will NOT be executed! No data will be sent to the device.");

}
