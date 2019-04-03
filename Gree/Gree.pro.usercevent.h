#ifndef CEVENT_H
#define CEVENT_H
#include "cdevice.h"
#include "ccondition.h"
#include <qthread.h>
#include <qdatetime>


#define CONDLIST ConditionsList
#define EVENT_NAME          QString("name")
#define EVENT_ID            QString("id")
#define EVENT_DEV_ID        QString("devid")
#define EVENT_OVERRIDE      QString("over")
#define EVENT_HOST          QString("host")
#define EVENT_PORT          QString("port")
#define EVENT_SYNC          QString("sync")
#define EVENT_MAX_TIME      QString("maxtime")
#define EVENT_COND          QString("cond")
#define EVENT_ACT           QString("act")
#define EVENT_DEVICE_DATA   QString("devdata")
#define EVENT_LOGGING       QString("logging")
#define EVENT_TYPE          QString("ev_type")

#define EVENT_FILE_EXT      QString(".smartrceve")

#define EVENT_MAX_TERM      3

class CDevice;
// To add a new event type, you must subclass CEvent
// it is important to override CreateNew() method to
// ensure that the main program will create an instance of your class
class QTimer;

class CEvent : public QObject
{
    Q_OBJECT

public:
    CEvent();
    virtual ~CEvent();


    virtual const QString GetName() const {return m_data[EVENT_NAME].toString();}
    virtual void SetName (const QString name) { m_data.insert(EVENT_NAME,QJsonValue(name));}
    virtual const QString GetEventType() const {return  m_data[EVENT_TYPE].toString(); }

    virtual const QString GetID() const { return  m_data[EVENT_ID].toString(); }

    virtual const QString GetDeviceID() const;
    virtual void SetDeviceID(const QString id);
    virtual CDevice * GetWorkingDevice() { return m_pDevice; }

    virtual int GetSync() const {return m_data[EVENT_SYNC].toInt();}
    virtual void SetSync(int sync) { m_data.insert(EVENT_SYNC,QJsonValue(sync));}

    virtual int GetWaitTime() const {return m_data[EVENT_MAX_TIME].toInt();}
    virtual void SetWaitTime (int sec) {m_data.insert(EVENT_MAX_TIME,QJsonValue(sec));}

    virtual int GetOverridePort() const { return m_data[EVENT_PORT].toInt();}
    virtual void SetOverridePort(int port) { m_data.insert(EVENT_PORT,QJsonValue(port));}

    virtual int GetOverride() const { return m_data[EVENT_OVERRIDE].toInt();}
    virtual void SetOverride(int o) { m_data.insert(EVENT_OVERRIDE,QJsonValue(o));}

    virtual int GetActive() const { return m_data[EVENT_ACT].toInt();}
    virtual void SetActive(int a) { m_data.insert(EVENT_ACT,QJsonValue(a));}

    virtual const QString GetHost() const { return m_data[EVENT_HOST].toString(); }
    virtual void SetHost(QString host) {m_data.insert(EVENT_HOST,QJsonValue(host));}

    virtual int GetLogging() const { return m_data[EVENT_LOGGING].toInt();}
    virtual void SetLogging(int a) { m_data.insert(EVENT_LOGGING,QJsonValue(a));}

    virtual void LogMessage(const QString msg);

    virtual const QString GetErrorMessage(int code) const;

    virtual bool SaveToFile(QString dir);
    virtual bool LoadFromFile(QString file);
    virtual bool Remove(const QString dir); //delete physically

    virtual void SetOverrideConnection(const QString host,qint16 port);
    virtual void ClearOverrideConnection();

    virtual void AddCondition(CCondition * cond) { m_conditions.append(cond); }
    virtual void DelCondition(const QString id);

    virtual bool TestConditions();

    virtual  CONDLIST * GetConditions()  { return & m_conditions;}

    virtual CEvent * CreateNew() { return new CEvent;}
    virtual bool ShowProperties ();

    //Execution related stuff
    virtual int Execute(); //Executes the event SYNCHRONOUS
    virtual void RunEvent(); //Creates timers, attaches Execute to the timout signal

    virtual void Terminate(); //stops the m_pTimer amd wait until m_bRunning gets false


protected:
    QJsonObject m_data;
    CDevice * m_pDevice;
    bool m_bRunning;
    QTimer * m_pTimer;

    CONDLIST m_conditions;
};

class CEventList : public QList<CEvent *>
{
public:
    CEventList () : QList<CEvent *>() { }
    virtual CEvent * GetById (const QString id);
    virtual CEvent * GetByType(const QString tp) const;
    virtual void LoadFromDir(const QString dir);
    virtual void SaveToDir(const QString dir);

    virtual ~CEventList ();

};

#endif // CEVENT_H
