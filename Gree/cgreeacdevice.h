#ifndef CGREEACDEVICE_H
#define CGREEACDEVICE_H
#include <qtimer.h>
#include "cdevice.h"
#include <qtcpsocket.h>
#include <QJsonDocument>

#define GREE_AC_NAME QString ("Gree Air Conditioner")

class CGreeACDevice : public CDevice
{
public:
    CGreeACDevice();
    CGreeACDevice( const QString& DeviceId);
    CGreeACDevice (const QJsonObject pack);
    virtual ~CGreeACDevice();

    static bool readPackFromResponse(const QByteArray& response,
                                     const QString& decryptionKey,
                                            QJsonObject& pack);

    virtual bool SaveToFile(const QString &dir);
    virtual bool LoadFromFile(const QString &file_name );

    virtual bool PollDevice(int timeout_sec);
    virtual bool Remove(const QString &dir);
    virtual bool ShowDevProp();
    virtual CDevice * CreateNew();

    virtual const QString GetDeviceKey();

    virtual bool GetDeviceData(int timeout_sec);
    virtual bool SetDeviceData(int timeout_sec);
    virtual QJsonValue GetDeviceValue(const QString key) {
        return m_devicestatus[key];
    }
    virtual void SetDeviceValue(const QString key,int value) {
        m_devicestatus.insert(key,QJsonValue(value));
    }
    virtual void SetDeviceName(const QString name);
    virtual const QString GetDevIDName();

    virtual void SetDeviceObject(const QJsonObject dev);
    virtual const QJsonObject GetDeviceObject () const;

    virtual void CleanUp();

protected:
    QUdpSocket *m_pSocket;

    QString m_devicekey;
    QJsonObject m_devicestatus;
    QJsonObject m_response;

    virtual const QString GetFileExt() const;

private:
    void BindReady();
    void timerTimeout();
    void GetDataReady();
    void SendDataReady();


};

class CGreeACLocator : public CDeviceLocator
{
public:
    CGreeACLocator();
    ~CGreeACLocator();

    virtual bool ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList );
    virtual const QString GetName() { return m_name; }
    virtual bool LoadFromDir(QString const &dir, CDeviceList * toList);
    virtual const QString GetWorkingDeviceIDName() const;

protected:
    QUdpSocket m_socket;
    //QTcpSocket m_socket;
    QTimer m_timer;
    QString m_name;

private:
    void socketReadyRead();
    void timerTimeout();
    void processScanResponse(const QByteArray response, const QHostAddress& remoteAddress, uint16_t remotePort);

};

#endif // CGREEACDEVICE_H
