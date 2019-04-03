#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include "QClicableLabel.h"
#include "cdiscoverdevices.h"
#include "globals.h"

#include "ctimecondition.h"
#include "cpausecondition.h"
#include "cbeforetimecondition.h"
#include "ceventselector.h"
#include "corevent.h" //COrEvent
#include "cgreeaconeparam.h"
#include "cwaitmessage.h"
#include "cbroadlinkrm.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //intialize globals

    global::g_pMainWindow = this;
    global::g_WorkDir =  QCoreApplication::applicationDirPath();

    global::RegisterLocators();
    global::RegisterConditions();
    global::RegisterEvents();

    global::g_IPC.connectToServer(IPC_NAME);
    global::g_IPC.open();

    lbl = new ClickableLabel();
    lbl->setText("Events active ");
    ui->lyBottom->addWidget(lbl);
    connect(lbl,&ClickableLabel::LeftClicked,this,&MainWindow::on_LabelActiveClicked);

    m_sbtnEventsState = new SwitchButton(ui->tabWidget,SwitchButton::ONOFF); // Default style is Style::ONOFF
    ui->lyBottom->addWidget(m_sbtnEventsState);

    connect(m_sbtnEventsState , &SwitchButton::Switched, this, &MainWindow::on_sbtnvalueChanged );

    lblSaveOnExit = new ClickableLabel;
    lblSaveOnExit->setText("Autosave on exit ");
    ui->lySaveOnExit->addWidget(lblSaveOnExit);

    m_sbtnSave = new SwitchButton(nullptr,SwitchButton::YESNO);
    ui->lySaveOnExit->addWidget(m_sbtnSave);

    ui->tblEvents->horizontalHeader()->resizeSection(3,200);

    ui->spnTimeOut->setValue(global::g_DevTimeout);
    setFixedSize(width(), height());



    global::LoadDevicesFromDisk();
    global::LoadEventsFromDisk();

//    ReloadEventList();
//    ReloadDevicesList();

    SetupDeviceFilter();
    SetupEventFilter();
    //SetSettings ill reload the Device and Event lists
    SetSettings();

    global::g_pLogger = new CLogger;
    global::g_pLogger->SetMaxLines(ui->spnMaxLog->value());

    QString expath(global::g_WorkDir);
    expath.append("/");
    expath.append(LOG_FILENAME);
    global::g_pLogger->SetFile(expath);
    global::g_pLogWindow = new CLogView;
//    global::g_pLogger->Message("SmartRC - Application started...");

    m_timer.setInterval(ui->spnPollTime->value()*1000*60);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::PollDevices);
    m_timer.start();

    m_IPCTimer.setInterval(10000);
    m_IPCTimer.setSingleShot(false);
    connect(&m_IPCTimer, &QTimer::timeout, this, &MainWindow::on_btnPing_clicked);
    m_IPCTimer.start();

}

void MainWindow::SetupDeviceFilter()
{
    ui->cmbDeviceFilter->clear();
    ui->cmbDeviceFilter->addItem("All devices");
    for (int i = global::g_Locators.count()-1;i >= 0; i--)
        ui->cmbDeviceFilter->addItem(global::g_Locators.at(i)->GetWorkingDeviceIDName());
}

void MainWindow::SetupEventFilter()
{
    ui->cmbEventFilter->clear();
    ui->cmbEventFilter->addItem("All events");
    for (int i = global::g_RegisteredEvents.count()-1;i >= 0; i--)
        ui->cmbEventFilter->addItem(global::g_RegisteredEvents.at(i)->GetEventType());
}
void MainWindow::PollDevices()
{
    QTableWidgetItem *pItem;
    QString devId;
    for (int i = ui->tblKnownDevices->rowCount()-1; i>=0; i--)
    {
        pItem = ui->tblKnownDevices->item(i,1);
        pItem->setText(QString("Polling..."));
        pItem->setBackgroundColor(QColor("yellow"));
        devId = ui->tblKnownDevices->item(i,2)->text();
        global::g_pKnownDevices->GetByID(&devId)->PollDevice(global::g_DevTimeout);
    }
    ReloadDevicesList();
    m_timer.setInterval(ui->spnPollTime->value()*1000*60);
    m_timer.start();
}

MainWindow::~MainWindow()
{
    //Save Settings
    GetSettings();


    if (m_sbtnSave->value())
    {
        QString dir(global::g_WorkDir);
        global::g_Events.SaveToDir(dir);
        global::g_pKnownDevices->SaveToDir(dir);

    }

    ClearDeviceList();
    ClearEventList();

    global::ClearExit();

    delete lbl;
    delete lblSaveOnExit;
    delete m_sbtnSave;

    delete global::g_pLogger;
    global::g_pLogger = nullptr;

    delete ui;
}

void MainWindow::GetSettings()
{
    global::gSettings.insert(SET_EVENTS_ACTIVE,QJsonValue(m_sbtnEventsState->value() == 1 ? true : false));
    global::gSettings.insert(SET_DEVICE_POL,QJsonValue(ui->spnPollTime->value()));
    global::gSettings.insert(SET_DEVICE_RQ,QJsonValue(ui->spnTimeOut->value()));
    global::gSettings.insert(SET_LOG_MAX,QJsonValue(ui->spnMaxLog->value()));
    global::gSettings.insert(SET_AUTOSAVE,QJsonValue(m_sbtnSave->value()));

    global::gSettings.insert(SET_DEVICE_FILTER,QJsonValue(ui->cmbDeviceFilter->currentText()));
    global::gSettings.insert(SET_EVENT_FILTER_TYPE,QJsonValue(ui->cmbEventFilter->currentText()));
    global::gSettings.insert(SET_EVENT_FILTER_NAME,QJsonValue(ui->txtEventFilter->text()));

    global::SaveSettings();

}
void MainWindow::SetSettings()
{

    global::LoadSettings();

    m_sbtnEventsState->setValue(global::gSettings[SET_EVENTS_ACTIVE].toBool() ? 1 :0);
    ui->spnPollTime->setValue(global::gSettings[SET_DEVICE_POL].toInt());
    ui->spnTimeOut->setValue(global::gSettings[SET_DEVICE_RQ].toInt());
    ui->spnMaxLog->setValue(global::gSettings[SET_LOG_MAX].toInt());
    m_sbtnSave->setValue(global::gSettings[SET_AUTOSAVE].toBool() ? 1 :0);
    //Will reload the Device and Event lists
    ui->cmbDeviceFilter->setCurrentText(global::gSettings[SET_DEVICE_FILTER].toString());
    ui->cmbEventFilter->setCurrentText(global::gSettings[SET_EVENT_FILTER_TYPE].toString());
    ui->txtEventFilter->setText(global::gSettings[SET_EVENT_FILTER_NAME].toString());

}

void MainWindow::hideEvent(QHideEvent *event)
{
    /*
    global::g_pLogger->Message("SmartRC - Application terminated");
    global::g_pLogWindow->close();
    if (global::g_pLogWindow)
        delete global::g_pLogWindow; //TODO GPF on minimize
    global::g_pLogWindow = nullptr;
    */
    QWidget::hideEvent(event);
}
void MainWindow::on_sbtnvalueChanged(bool newvalue)
{
    char cmd[2];
    cmd[0] = global::SetLoopState;
    cmd[1] = newvalue;
    global::ClearIPC();
    global::g_IPC.write(&cmd[0],2);

}
void MainWindow::on_LabelActiveClicked()
{
    m_sbtnEventsState->setValue(!m_sbtnEventsState->value());
    this->on_sbtnvalueChanged(m_sbtnEventsState->value());
}
void MainWindow::on_btnAddEvent_clicked()
{

    CAddEvent frm;
    CEventSelector pSel;
    CEvent *pEvent;

    if(pSel.exec() ==0)
        return;

    pEvent = pSel.GetSelectedEvent();

    if (pEvent->ShowProperties())
        global::g_Events.append(pEvent);
    else
        delete pEvent;
    ReloadEventList();

}

void  MainWindow::ReloadEventList()
{
    QTableWidget *t = ui->tblEvents;
    QString str;
    CEvent *pEvent;
    CDevice *pDevice = nullptr;
    //clear devices list
    ClearEventList();

    //add list

    long row = qMax(0, t->rowCount() - 1);
    for (int i = 0; i < global::g_Events.count(); i++)
    {
        pEvent = global::g_Events.at(i);
        //Apply filters
        if (ui->cmbEventFilter->currentIndex() != 0)
            if (pEvent->GetEventType().compare(ui->cmbEventFilter->currentText()) !=0)
                continue;
        if (!ui->txtEventFilter->text().isEmpty())
            if ( pEvent->GetName().contains(ui->txtEventFilter->text()) == 0
                 &&
                 ( pEvent->GetWorkingDevice() &&
                   (pEvent->GetWorkingDevice()->GetDeviceName().contains(ui->txtEventFilter->text()) == 0)
                 )
               )
                continue;

        t->insertRow(row);
        t->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        t->item(row,0)->setData(Qt::UserRole,pEvent->GetID());
        t->setItem(row, 1, new QTableWidgetItem(pEvent->GetActive() == 0 ? QString("No") : QString("Yes")));
        t->setItem(row, 2, new QTableWidgetItem(pEvent->GetName()));
        str = pEvent->GetDeviceID();
        t->setItem(row, 4, new QTableWidgetItem(str));
        pDevice =  global::g_pKnownDevices->GetByID(&str);
        if (pDevice)
            str = pDevice->GetDeviceName();
        else
            str = "DEVICE UNKNOW";
        t->setItem(row, 3, new QTableWidgetItem(str));
    }
}

void  MainWindow::ClearEventList()
{
    QObject *s;
    QTableWidget *t = ui->tblEvents;

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

void MainWindow::ClearDeviceList()
{
    QObject *s;
    QTableWidget *t = ui->tblKnownDevices;

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
void MainWindow::ReloadDevicesList()
{

    QTableWidget *t = ui->tblKnownDevices;
    QTableWidgetItem * pItem = nullptr;
    QString devKey;
    //clear devices list
    ClearDeviceList();

    //add list

    long row = qMax(0, t->rowCount() - 1);
    int nDev = global::g_pKnownDevices->count();

    for (int i = 0; i < nDev; i++) {
        if (ui->cmbDeviceFilter->currentIndex() != 0)
            if (global::g_pKnownDevices->at(i)->GetDevIDName().compare(ui->cmbDeviceFilter->currentText()) != 0)
                continue;
        t->insertRow(row);
        t->setItem(row, 0, new QTableWidgetItem(global::g_pKnownDevices->value(i)->GetDeviceName()));
        pItem = new QTableWidgetItem;
        devKey = global::g_pKnownDevices->value(i)->GetDeviceKey();
        if (devKey.isEmpty())
        {
            pItem->setText(QString("OFF-LINE"));
            pItem->setBackgroundColor(QColor("red"));
        }
        else {
            pItem->setText(QString("ON-LINE"));
            pItem->setBackgroundColor(QColor("green"));
        }
        t->setItem(row,1,pItem);
        t->setItem(row, 2, new QTableWidgetItem(global::g_pKnownDevices->value(i)->GetID()));
        t->setItem(row, 3, new QTableWidgetItem(global::g_pKnownDevices->value(i)->GetAddr()));
    }
}

void MainWindow::on_btnDiscover_clicked()
{
    CDiscoverDevices discover;
    discover.SetTimeOut(ui->spnTimeOut->value());
    discover.exec();
    ReloadDevicesList();

}

void MainWindow::on_btnSave_clicked()
{

    for (int i = global::g_pKnownDevices->count() -1;i >= 0; i--)
        global::g_pKnownDevices->value(i)->SaveToFile(global::g_WorkDir);
}

void MainWindow::on_cmdUpdateNow_clicked()
{
    m_timer.stop();
    PollDevices();
    m_timer.start();
}


CDevice* MainWindow::GetSelectedDev()
{
    QList<QTableWidgetItem *> sel = ui->tblKnownDevices->selectedItems();
    if (sel.count()< 1)
        return nullptr;
    QString devid = sel[2]->text();
    return global::g_pKnownDevices->GetByID(&devid);

}

CEvent* MainWindow::GetSelectedEvent()
{
    QList<QTableWidgetItem *> sel = ui->tblEvents->selectedItems();
    if (sel.count()< 1)
        return nullptr;
    QString devid = sel[0]->data(Qt::UserRole).toString();
    return global::g_Events.GetById(devid);

}

void MainWindow::on_btnDelDevice_clicked()
{
    CDevice * pDev = GetSelectedDev();

    if (!pDev)
    {
        QMessageBox::information(this,"Information","Please,select a device");
        return;
    }
    if (QMessageBox::question(this,"Question",
                                    QString("Are you sure to delete device ").append(pDev->GetDeviceName()).append(" ?"),
                                    QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::No)
        return;


    Retry:
    if (!pDev->Remove(global::g_WorkDir))
        if ( QMessageBox::warning(this,"Error",QString("Unable to delete file: ").append(pDev->GetID()),
             QMessageBox::Retry | QMessageBox::Ok) == QMessageBox::Retry)
            goto Retry;
    global::g_pKnownDevices->RemoveById(pDev);
    delete pDev;
    ReloadDevicesList();

}

void MainWindow::on_btnDeviceProp_clicked()
{
    CDevice * pDev = GetSelectedDev();
    if (!pDev)
    {
        QMessageBox::information(this,"Information","Please,select a device");
        return;
    }
    pDev->ShowDevProp();
    ReloadDevicesList();
}

void MainWindow::on_spnTimeOut_valueChanged(int arg1)
{

    global::g_DevTimeout = arg1;

}

void MainWindow::on_btnSaveEvents_clicked()
{
    QString expath(QCoreApplication::applicationDirPath());
    global::g_Events.SaveToDir(expath);
}

void MainWindow::on_btnEdit_clicked()
{

    CEvent * pEvent = GetSelectedEvent();
    if (!pEvent)
        return;
    pEvent->ShowProperties();
    ReloadEventList();
}

void MainWindow::on_btnDelEvent_clicked()
{

    CEvent * pEvent = GetSelectedEvent();
    if (!pEvent)
        return;
    QString eventname = pEvent->GetName();
    if (QMessageBox::question(this,"Confirm",eventname.prepend("Delete condition ").append(" ?"))
         != QMessageBox::Yes)
        return;

    pEvent->Remove(QCoreApplication::applicationDirPath());

    //suspend event execution

    CWaitMessage w;
    w.open();
    //Remove from event list
    for (int i = global::g_Events.count()-1; i >= 0; i--)
        if(global::g_Events[i]->GetID().compare(pEvent->GetID()) == 0)
        {
            global::g_Events.takeAt(i);
            i = -1;
        }
    delete pEvent;

    ReloadEventList();

}

void MainWindow::on_btnLog_clicked()
{

    global::g_pLogWindow->show();

}

void MainWindow::on_spnPollTime_valueChanged(const QString &arg1)
{
    global::g_DevTimeout = ui->spnTimeOut->value();
}

void MainWindow::on_btnPing_clicked()
{
    if (!global::g_IPC.isValid())
    {
        global::g_IPC.close();
        global::g_IPC.connectToServer(IPC_NAME);
        if (!global::g_IPC.waitForConnected(1000))
            qDebug() << global::g_IPC.errorString();
        else
            global::g_IPC.open();

    }
    global::ClearIPC();
    char cmd = global::Ping;
    global::g_IPC.write(&cmd,1);
    if (global::g_IPC.waitForReadyRead(1000))
        global::g_IPC.read(&cmd,1);
    if (cmd == global::Pong)
    {
        ui->lblELState->setText(QString("Running, PONG recieved "));
        ui->lblELState->setStyleSheet("QLabel {color : green; }");
    }
    else {
        ui->lblELState->setText(QString("Not running - no response"));
        ui->lblELState->setStyleSheet("QLabel {color : red; }");
    }
}

void MainWindow::on_btnReload_clicked()
{
    global::ClearIPC();
    char cmd = global::Reload;
    global::g_IPC.write(&cmd,1);
}

void MainWindow::on_btnQuit_clicked()
{
    if (QMessageBox::question(this,"Confirm","This will stop event execution. Quit the event loop ?") == QMessageBox::No)
        return;
    global::ClearIPC();
    char cmd = global::Quit;
    global::g_IPC.write(&cmd,1);
}

void MainWindow::on_cmbDeviceFilter_currentIndexChanged(const QString &arg1)
{
    ReloadDevicesList();
}

void MainWindow::on_cmbEventFilter_currentIndexChanged(int index)
{
    ReloadEventList();
}

void MainWindow::on_txtEventFilter_textChanged(const QString &arg1)
{
    ReloadEventList();
}
