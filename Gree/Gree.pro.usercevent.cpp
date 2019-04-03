#include "cevent.h"
#include <quuid.h>
#include <QJsonArray>
#include <qjsondocument.h>
#include <qfile.h>
#include <qdir.h>
#include <QLoggingCategory>
#include <QTimer>

#ifndef NO_GUI
#include <qmessagebox.h>
#include "caddevent.h"
#endif

#include "globals.h"
#include "cdevice.h"


Q_DECLARE_LOGGING_CATEGORY(EventLog)
Q_LOGGING_CATEGORY(EventLog, "Event")

CEvent::CEvent() : QObject ()
{
    SetOverridePort( 0);
    m_data.insert(EVENT_ID,QUuid::createUuid().toString());
    m_pDevice = nullptr;
    m_data.insert(EVENT_TYPE,QJsonValue("Generic event - All conditions true"));
    m_bRunning = false;
    m_pTimer = nullptr;
}


CEvent::~CEvent()
{
    Terminate();
    if (m_pDevice)
        delete m_pDevice;
}
void CEvent::RunEvent()
{

    if (m_pTimer)
        return;
    m_pTimer = new QTimer;
    m_pTimer->setInterval(1000);
    m_pTimer->setSingleShot(true);
    connect(m_pTimer,&QTimer::timeout,this,&CEvent::Execute);
    m_pTimer->start();
}

void CEvent::Terminate()
{

    if (m_pTimer)
    {
        m_pTimer->stop();
//        while (m_bRunning)
//            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        disconnect(m_pTimer,&QTimer::timeout,this,&CEvent::Execute);
        delete m_pTimer;
        m_pTimer = nullptr;
    }
}

bool CEvent::ShowProperties ()
{
#ifndef NO_GUI

    CAddEvent frm;

    frm.SetEvent(this);
    return frm.exec();
#else
    return false;
#endif
}

const QString CEvent::GetDeviceID() const {
    return  m_pDevice ? m_pDevice->GetID() : QString("");
}

void CEvent::LogMessage(const QString msg)
{
    if (GetLogging())
        global::g_pLogger->Message(msg);
}
int CEvent::Execute()
{
    m_bRunning = true;
    bool bRet = false;
    QString log(GetName());
    bool bTestConditions = TestConditions();
    if (bTestConditions)
    {
        global::g_pLogger->Message(log.prepend("Executing event: "));
        bRet = m_pDevice->SetDeviceData(global::g_DevTimeout);
    }
    while (bTestConditions && TestConditions())
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);//Cool down
        QThread::sleep(1);
    }

    if (m_pTimer)
        m_pTimer->start(); //Restart Timer
    m_bRunning = false;
    return bRet;

}
const QString CEvent::GetErrorMessage(int code) const
{

    return QString(code);
}

bool CEvent::TestConditions()
{
    CCondition * pCond;
    bool bRes;
    int iCondCount =  m_conditions.count();
    QString log = GetName();
    log.prepend("Event ");
    for (int i = 0; i < iCondCount; i++)
    {
        pCond = m_conditions.at(i);
        if (pCond->GetActive())
        {
            bRes = pCond->IsTrue();
            if (!pCond->GetNegate())
                bRes = !bRes;
            if (!bRes)
            {
                log.append(" will not be executed, since condition \" ");
                log.append(pCond->GetDesc());
                log.append("\" is not met");
                LogMessage(log);
                return false;
            }
        }
    }
    if (iCondCount > 0)
        log.append(": all conditions met.");
    else
        log.append(" has no conditions! Is this the intent ?");
    LogMessage(log);
    return true;
}
bool CEvent::Remove(const QString dir)
{
    QString file_name = dir;
    file_name.append("/");
    file_name.append(GetName());
    file_name.append(EVENT_FILE_EXT);
    QFile f (file_name);
    return f.remove();
}


void CEvent::SetDeviceID(const QString id)
{
    if (m_pDevice && id.compare(m_pDevice->GetID()) == 0)
        return;
    if (m_pDevice)
        delete m_pDevice;
    m_pDevice = nullptr;

    long ix = global::g_pKnownDevices->FindByID(&id);
    if (ix < 0)
        return;
     m_pDevice = global::g_pKnownDevices->at(ix)->CreateNew();
    m_pDevice->SetDeviceObject(global::g_pKnownDevices->at(ix)->GetDeviceObject());
    m_data.insert(EVENT_DEV_ID,QJsonValue((id)));
}
bool CEvent::SaveToFile(QString dir)
{
    QJsonArray cond;

    for (int i = 0; i < m_conditions.count(); i++)
        cond.append(QJsonValue(m_conditions.at(i)->GetData()));
    m_data.insert(EVENT_COND,QJsonValue(cond));
    QJsonObject dev = m_pDevice->GetDeviceObject();

    m_data.insert(EVENT_DEVICE_DATA,QJsonValue(dev));

    QString file_name (dir);
    file_name.append(QString("/"));
    file_name.append(GetName());
    file_name.append(EVENT_FILE_EXT);

    QJsonDocument savedoc(m_data);
    QFile savefile(file_name);

    if (!savefile.open(QIODevice::WriteOnly))
    {
              qWarning("Couldn't open save file.");
              return false;
    }
    savefile.write(savedoc.toBinaryData());

    return true;
}
bool CEvent::LoadFromFile(QString file)
{
    QFile loadFile(file);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }

        QByteArray saveData = loadFile.readAll();
        //Filea read. The event has data
        QJsonDocument loadDoc(QJsonDocument::fromBinaryData(saveData));
        m_data = loadDoc.object();
        //Load Conditions
        QJsonArray cond = m_data[EVENT_COND].toArray();
        QString condname;
        CCondition * pCond = nullptr;

        for (int i = 0; i < cond.count(); i++)
        {
            condname = cond[i].toObject().value(COND_NAME).toString();
            pCond = global::g_RegisteredConditions.FirsByName(condname);
            if (pCond)
            {
                pCond = pCond->CreateNew();
                pCond->SetData(cond[i].toObject());
                m_conditions.append(pCond);
            }
            else {
                global::g_pLogger->Message(condname.prepend("Error loading condition: ").append(" , while loading event: ").append(GetName()));
                SetActive(false);
            }



        }
        m_conditions.Order();
        // Dont forget the device setup
        QJsonObject devobj = m_data[EVENT_DEVICE_DATA].toObject();
        SetDeviceID(m_data[EVENT_DEV_ID].toString());

        if (!m_pDevice)
        {
            global::g_pLogger->Message(
                        QString("Error loading data for device with ID: ").append(GetDeviceID()).append(" Device is not within known devices."));
            SetActive(false);
        }
        else
            m_pDevice->SetDeviceObject(devobj);

        return true;

}

void CEvent::DelCondition(const QString id)
{
    for (int i = m_conditions.count()-1; i>=0; i--)
        if (id.compare(m_conditions.at(i)->GetID()) == 0)
        {
            delete m_conditions.takeAt(i);
            m_conditions.Order();
        }
}
void CEvent::SetOverrideConnection(const QString host,qint16 port)
{
    m_data.insert(EVENT_HOST,host);
    SetOverridePort(port);
}
void CEvent::ClearOverrideConnection()
{
    SetOverride(0);
}



CEvent * CEventList::GetById (const QString id)
{
    for (int i = count()-1; i>=0; i--)
        if (id.compare(at(i)->GetID()) == 0)
            return at(i);
    return nullptr;
}

void CEventList::LoadFromDir(const QString dir)
{
    QDir loaddir(dir);
    loaddir.setFilter(QDir::Files);
    QFileInfoList files = loaddir.entryInfoList();
    CEvent *p = nullptr;
    QString evtype;

    for (int i = files.count() -1; i>=0; i--)
        if (files[i].fileName().contains(EVENT_FILE_EXT,Qt::CaseInsensitive))
        {
            //try to load generic event
            p = new CEvent;
            Retry:
            if(!p->LoadFromFile(files[i].absoluteFilePath()))
#ifndef NO_GUI
            {
                if (QMessageBox::warning(nullptr,"Error",QString( "Unable to load file: ").append(files[i].fileName()),
                                         QMessageBox::Retry | QMessageBox::Cancel,
                                         QMessageBox::Retry)
                        == QMessageBox::Retry)
                    goto Retry;
                else {
                        delete p;
                        p = nullptr;
                     }
            }
#else
            {
                delete p;
                p = nullptr;
             }
#endif


            if (p)
            {
                //Loaded generic event. Get the event type, and load the correct instance
                evtype = p->GetEventType();
                delete p;
                p = global::g_RegisteredEvents.GetByType(evtype)->CreateNew();
                p->LoadFromFile(files[i].absoluteFilePath());
                append(p);
            }

        }

}
void CEventList::SaveToDir(const QString dir)
{

    for (int i = count() -1;i >= 0; i--)
        value(i)->SaveToFile(dir);
}
CEvent * CEventList::GetByType(const QString tp) const
{
    for (int i = count()-1; i>=0; i--)
        if (tp.compare(at(i)->GetEventType()) == 0)
            return at(i);
    return nullptr;
}

CEventList::~CEventList ()
{
    while (count() > 0)
        delete takeAt(0);
}
