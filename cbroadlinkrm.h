#ifndef CBROADLINKRM_H
#define CBROADLINKRM_H

#include <QObject>
#include <qstring.h>
#include <qtimer.h>
#include <qudpsocket>
#include "cdevice.h"
#include <qbytearray.h>


#define BDEV_TYPE "type"

class CBroadLinkRM : public CDevice
{
    Q_OBJECT
public:
    CBroadLinkRM();
    virtual ~CBroadLinkRM();


    virtual bool PollDevice(int timeout_sec);
    virtual bool GetDeviceData(int timeout_sec);
    virtual bool SetDeviceData(int timeout_sec);

    virtual ulong GetDeviceType() { return m_data[BDEV_TYPE].toInt();}
    virtual void SetDeviceType(int tp) { m_data.insert(BDEV_TYPE,QJsonValue(tp));}

    virtual void SetVendor(const QString v) { return; } //Empty. Vendor can not be set
    virtual const QString GetVendor ();

    virtual void SetMac (const QString mac);

    virtual CDevice * CreateNew();
    virtual const QString GetDevIDName();

    virtual bool SaveToFile(const QString &dir);
    virtual const QString GetFileExt() const;

    virtual bool ShowDevProp();

    virtual const QString GetDeviceKey();
    static int CalcChecksum(QByteArray &packet);

    virtual void CleanUp() {}
protected:
//    bool m_Busy;
//    QTimer m_Timer;
    QUdpSocket m_socket;
    int m_SendCnt;
    unsigned int m_devkey;
    unsigned char m_encr_key[16];


//    virtual void TimeOut() { m_Busy = false; }
    virtual void PreparePacket(QByteArray &ret, int cmd);
    virtual void HandleAUTH();

};

class CBroadLinkRMLocator : public CDeviceLocator
{
    Q_OBJECT

public:
    CBroadLinkRMLocator();
    virtual bool ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList = nullptr );
    virtual const QString GetName();
    virtual bool LoadFromDir(QString const &dir, CDeviceList * toList);
    virtual const QString GetWorkingDeviceIDName() const;

protected:
    virtual void HandleResponse();
    virtual void TimeOut();

    QTimer m_timer;
    QUdpSocket m_socket;

};

#endif // CBROADLINKRM_H
