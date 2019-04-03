#ifndef CDEVICE_H
#define CDEVICE_H
#include <QList>
#include <qjsonobject.h>
#include <QUdpSocket>

#define DEV_ID          QString("device_id")
#define DEV_ADDR        QString("device_address")
#define DEV_MAC         QString("device_mac")
#define DEV_NAME        QString("device_name")
#define DEV_VENDOR      QString("device_vendor")
#define DEV_PORT        QString("device_port")
#define DEV_IDNAME      QString("deviceid_name")

class CDevice : public QObject
{
    Q_OBJECT

public:

    enum CompareResult
    {
      EQ,
      NOT_EQ,
      EQ_DIF_HOST
    };

    CDevice();
    CDevice( const QString& DeviceId) { m_data.insert(DEV_ID,QJsonValue(DeviceId));}
//    CDevice (CDevice &dev);
    virtual ~CDevice() {}


    virtual const QString GetID() { return m_data[DEV_ID].toString(); }
    virtual void SetID(const QString id) { m_data.insert(DEV_ID,QJsonValue(id));}


    virtual const QString GetAddr() {return m_data[DEV_ADDR].toString();}
    virtual void SetAddress(const QHostAddress &addr, quint16 port);

    virtual void SetMac (const QString mac) {m_data.insert(DEV_MAC,QJsonValue(mac));}
    virtual const QString GetMac() { return m_data[DEV_MAC].toString(); }

    virtual void SetDeviceName( const QString name) { m_data.insert(DEV_NAME,QJsonValue(name));}
    virtual const QString GetDeviceName() { return m_data[DEV_NAME].toString(); }

    virtual void SetVendor (const QString vendor) {m_data.insert(DEV_VENDOR,QJsonValue(vendor));}
    virtual const QString GetVendor () { return m_data[DEV_VENDOR].toString();}

    virtual void SetPort (quint16 port) {m_data.insert(DEV_PORT,QJsonValue(port));}
    virtual quint16 GetPort () { return m_data[DEV_PORT].toInt();}

    virtual CompareResult Compare (CDevice *p);

    virtual void SetDeviceObject(const QJsonObject dev) { m_data = dev;}
    virtual const QJsonObject GetDeviceObject () const { return m_data; }

    virtual bool SaveToFile(const QString &file_name);
    virtual bool LoadFromFile(const QString &file_name );
    virtual bool Remove(const QString &file_name);

    //Pure Virtuals

    virtual bool PollDevice(int timeout_sec) =0;
    virtual bool GetDeviceData(int timeout_sec) = 0;
    virtual bool SetDeviceData(int timeout_sec) = 0;

    virtual CDevice * CreateNew() = 0;
    virtual const QString GetDevIDName() = 0;


    virtual bool ShowDevProp() = 0; // shows the properties dialog

    virtual const QString GetDeviceKey() =0;

    virtual void CleanUp() = 0; // Will be called after killing the thread where the event is executed

protected:
    QJsonObject m_data;

};

class CDeviceList : public QList<CDevice *>
{

public:
    CDeviceList () : QList<CDevice *>()
    {}
    virtual ~CDeviceList() ;

    //CDeviceList( const CDeviceList & );
    virtual void clear();
    virtual long AddIfNotExists(CDevice * pDevice);
    virtual CDevice* GetByID (QString const * id);
    virtual void RemoveById(CDevice * pDevice);
    virtual long FindByID(CDevice * pDevice);
    virtual long FindByID(QString const * id);
    CDevice * FirsByIDName(const QString name);
    virtual bool SaveToDir(const QString dir);
};

class CDeviceLocator : public QObject
{
    Q_OBJECT

public:
    CDeviceLocator() {}
    virtual ~CDeviceLocator () {}
    virtual const CDeviceList* GetDevices() { return m_pFoundDevices; }
    virtual bool isBusy() { return m_Busy;}
    //Pure virtual
    virtual bool ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList = nullptr ) =0;
    virtual const QString GetName() = 0;
    virtual bool LoadFromDir(QString const &dir, CDeviceList * toList) = 0;
    virtual const QString GetWorkingDeviceIDName() const = 0;
protected:
    CDeviceList  *m_pFoundDevices;
    bool m_Busy;
};

#endif // CDEVICE_H
