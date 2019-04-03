#include "cgreeacdevice.h"
#include <qcoreapplication.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <qfile.h>
#include <qdir.h>
#include <qthread.h>

#ifndef NO_GUI
#include <qmessagebox.h>
#include "cgreeacproperties.h"
#endif

#include <qnetworkdatagram.h>
#include <QLoggingCategory>
#include "crypto.h"


#define GR_DEV_ADDR "deviceaddr"
#define GR_MAC_ADDR "macaddr"
#define GR_DEV_NAME "devicename"
#define GR_VENDOR "vendor"
#define GR_PORT "port"
#define GR_DEV_ID "deviceid"
#define GR_DEV_STAT "device_status"
#define GR_DEV_OBJ  "parrent"
#define GR_AC_FILE_EXT ".grac"


Q_DECLARE_LOGGING_CATEGORY(DeviceLog)
Q_LOGGING_CATEGORY(DeviceLog, "Device")



CGreeACDevice::CGreeACDevice() : CDevice()//,m_waitngdata(false)
{

    m_pSocket = nullptr;

}
CGreeACDevice::CGreeACDevice( const QString& DeviceId) : CDevice(DeviceId)//,m_waitngdata(false)
{
//   m_pTimer = nullptr;
    m_pSocket = nullptr;
}
CGreeACDevice::CGreeACDevice (const QJsonObject pack) : CDevice()
{
    SetID(pack["cid"].toString());
    SetMac(pack["mac"].toString());
    SetDeviceName(pack["name"].toString());
    SetVendor(pack["brand"].toString());

    m_devicekey = pack["device_key"].toString();

    m_pSocket = nullptr;

}
CGreeACDevice::~CGreeACDevice()
{
    CleanUp();
}

void CGreeACDevice::CleanUp()
{

    if (m_pSocket)
        delete m_pSocket;
    m_pSocket = nullptr;

}
CDevice * CGreeACDevice::CreateNew()
{
    CGreeACDevice *pNew = new CGreeACDevice;
    pNew->SetDeviceObject(GetDeviceObject());
    return pNew;
}
bool CGreeACDevice::ShowDevProp()
{

#ifndef NO_GUI
    CGreeACProperties dialog;
    dialog.SetDevice(this);
    dialog.exec();
#endif
    return true;
}
bool CGreeACDevice::SaveToFile(const QString &dir)
{

    QString file_name (dir);
    file_name.append(QString("/"));
    file_name.append(GetID());
    file_name.append(GetFileExt());
    m_data.insert(GR_DEV_STAT,QJsonValue(m_devicestatus));

    return CDevice::SaveToFile(file_name);
}

const QString CGreeACDevice::GetFileExt() const
{
    return GR_AC_FILE_EXT;
}

void CGreeACDevice::SetDeviceObject(const QJsonObject dev)
{
    m_devicestatus = dev[GR_DEV_STAT].toObject();
    CDevice::SetDeviceObject(dev[GR_DEV_OBJ].toObject());

}

const QJsonObject CGreeACDevice::GetDeviceObject () const
{
    QJsonObject ret;
    ret.insert(GR_DEV_STAT,QJsonValue(m_devicestatus));
    ret.insert(GR_DEV_OBJ,QJsonValue(CDevice::GetDeviceObject()));
    return ret;

}

const QString CGreeACDevice::GetDevIDName()
{
    return GREE_AC_NAME;
}

bool CGreeACDevice::LoadFromFile(const QString &file_name )
{
    bool bret = CDevice::LoadFromFile(file_name);
    if (bret)
            m_devicestatus = m_data[GR_DEV_STAT].toObject();
    return bret;
}

const QString CGreeACDevice::GetDeviceKey()
{
    return  m_devicekey;
}

void CGreeACDevice::GetDataReady()
{
    QJsonObject pack;
    QJsonParseError parseError;

    QHostAddress remoteAddress;

    qCDebug(DeviceLog) << "GetDataReady() - incomming data " << GetDeviceName();

    QNetworkDatagram datagram;
    Q_ASSERT(m_pSocket);
    datagram = m_pSocket->receiveDatagram(4096);
    qCDebug(DeviceLog) << "GetDataReady() - data read. Parsing... " << GetDeviceName();

    QString encryptedPack, mac;
    QByteArray decryptedPack;
    QJsonValue keys, values;
    QJsonArray keyArray;
    QJsonArray valueArray;

    QJsonDocument responseJsonDocument = QJsonDocument::fromJson(datagram.data(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return;

    if ( datagram.senderAddress().toString().compare(GetAddr()))
        return;

    encryptedPack = responseJsonDocument.object()["pack"].toString();

    if (encryptedPack.isEmpty())
           return; // incorrect response. no PACK field

    decryptedPack = Crypto::decryptPack(encryptedPack.toUtf8(),m_devicekey);

    pack = QJsonDocument::fromJson(decryptedPack, &parseError).object();

    if (parseError.error != QJsonParseError::NoError)
        return; // Error parsing document
    mac = GetMac();
    if (!mac.isEmpty()  && mac.compare(pack["mac"].toString()))
            return; // Device is with different MAC.

    //Get data from response
    if (pack["t"] != "dat")
        return;
    keys = pack["cols"];
    if (!keys.isArray())
        return;
   keyArray = keys.toArray();
    if (keyArray.isEmpty())
        return;

    values = pack["dat"];
    if (!values.isArray())
         return;

    valueArray = values.toArray();
    if (valueArray.isEmpty())
          return;

    if (keyArray.size() != valueArray.size())
        return;

    for (int i = 0; i < keyArray.size(); i++)
        m_devicestatus.insert(keyArray[i].toString(),valueArray[i]);

    if (!m_devicestatus["name"].toString("").isEmpty())
        SetDeviceName(m_devicestatus["name"].toString());


}

bool CGreeACDevice::GetDeviceData(int timeout_sec)
{

    if (m_devicekey.isEmpty()) // not  polled
        if (!PollDevice(timeout_sec))
            return false; //poll failed - no encryption key
    m_pSocket = new QUdpSocket;

    if (!m_pSocket->isOpen())
        m_pSocket->open(QIODevice::ReadWrite);
;
//    //Create rq packet
    QJsonObject json
    {
        { "cols", QJsonArray {
                "Pow",
                "Mod",
                "SetTem",
                "WdSpd",
                "Air",
                "Blo",
                "Health",
                "SwhSlp",
                "Lig",
                "SwingLfRig",
                "SwUpDn",
                "Quiet",
                "Tur",
                "StHt",
                "TemUn",
                "HeatCoolType",
                "TemRec",
                "SvSt",
                "NoiseSet",
                "name"
            }
        },
        { "mac", GetMac() },
        { "t", "status" }
    };
    QJsonDocument jdoc {json};
    QByteArray packet = jdoc.toJson(QJsonDocument::Compact);

    QByteArray enc_pack = Crypto::encryptPack(packet,m_devicekey);

    //Create device rq
    json = QJsonObject
        {
            { "cid", "app" },
            { "i", 0 },
            { "t", "pack" },
            { "uid", 0 },
            { "pack", QString::fromUtf8(enc_pack) }
        };

    enc_pack = QJsonDocument{ json }.toJson(QJsonDocument::Compact);
    qCDebug(DeviceLog) << "GetDeviceData() - sending request " << GetDeviceName();
    m_pSocket->writeDatagram(enc_pack, QHostAddress(GetAddr()), GetPort());
    if (m_pSocket->waitForReadyRead(timeout_sec * 1000))
            GetDataReady();
    CleanUp();

    return true;
}
bool CGreeACDevice::SetDeviceData(int timeout_sec)
{
    m_response = QJsonObject();

    if (m_devicekey.isEmpty()) // not  polled
        if (!PollDevice(timeout_sec))
            return false; //poll failed - no encryption key
    m_pSocket = new QUdpSocket;

    // fill values pack
    QVariantList values;
    QStringList keys = m_devicestatus.keys();

    for (int i =0; i < keys.count(); i++)
        if(!m_devicestatus[keys[i]].isString())
            values.append(QVariant(m_devicestatus[keys[i]].toInt(0)));
        else
            values.append(QVariant(m_devicestatus[keys[i]].toString("")));
    QJsonObject json
    {
        { "opt", QJsonArray::fromStringList(keys) },
        { "p", QJsonArray::fromVariantList(values) },
        { "t", "cmd" }
    };
    //encrypt packet
    QJsonDocument jdoc {json};
    QByteArray packet = jdoc.toJson(QJsonDocument::Compact);
    QByteArray enc_pack = Crypto::encryptPack(packet,m_devicekey);

    //Create device rq
    json = QJsonObject
        {
            { "cid", "app" },
            { "i", 0 },
            { "pack", QString::fromUtf8(enc_pack)},
            { "t", "pack" },
            { "tcid", GetMac() },
            { "uid", 0 }
        };
    packet = QJsonDocument{json}.toJson(QJsonDocument::Compact);

    // Send to the device
    if (!m_pSocket->isOpen())
        m_pSocket->open(QIODevice::ReadWrite);

    m_pSocket->writeDatagram(packet, QHostAddress(GetAddr()), GetPort());
    if (m_pSocket->waitForReadyRead(timeout_sec *1000))
        SendDataReady();

    CleanUp();
    return m_response.count() > 0;
}

bool CGreeACDevice::PollDevice(int timeout_sec)
{

    m_devicekey = ""; //Set to empty. Means, BIND is failed

    m_pSocket = new QUdpSocket;

    if (!m_pSocket->isOpen())
        m_pSocket->open(QIODevice::ReadWrite);

    //Create binding rq

    QJsonObject json
    {
        { "mac", GetMac() },
        { "t", "bind" },
        { "uid", 0 }
    };
    QJsonDocument jdoc {json};
    QByteArray packet = jdoc.toJson(QJsonDocument::Compact);


    QByteArray enc_packet =   Crypto::encryptPack(packet, Crypto::GenericAESKey);

    json = QJsonObject
    {
        { "cid", "app" },
        { "i", 1 },
        { "t", "pack" },
        { "uid", 0 },
        { "pack", QString::fromUtf8(enc_packet) }
    };

    jdoc.setObject(json);
    enc_packet = jdoc.toJson(QJsonDocument::Compact);
    m_pSocket->writeDatagram(enc_packet, QHostAddress(GetAddr()), GetPort());
    if (m_pSocket->waitForReadyRead(timeout_sec*1000))
        BindReady();
    CleanUp();
  return m_devicekey.length() >0;
}
void CGreeACDevice::SendDataReady()
{
    QJsonObject pack;
    QJsonParseError parseError;
    QHostAddress remoteAddress;
    QNetworkDatagram datagram;

    datagram = m_pSocket->receiveDatagram(4096);

     m_response = QJsonDocument::fromJson(datagram.data(), &parseError).object();

}

void CGreeACDevice::BindReady()
{

    QJsonObject pack;
    QJsonParseError parseError;
    QNetworkDatagram netdatagram = m_pSocket->receiveDatagram(4096);
    QHostAddress remoteAddress =netdatagram.senderAddress();


    QJsonDocument responseJsonDocument = QJsonDocument::fromJson(netdatagram.data(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return;
    if (!remoteAddress.isEqual(QHostAddress(GetAddr())))
        return;



    QString encryptedPack = responseJsonDocument.object()["pack"].toString();

    if (encryptedPack.isEmpty())
           return; // incorrect response. no PACK field

    QByteArray decryptedPack = Crypto::decryptPack(encryptedPack.toUtf8(), Crypto::GenericAESKey);

    pack = QJsonDocument::fromJson(decryptedPack, &parseError).object();

    if (parseError.error != QJsonParseError::NoError)
        return; // Error parsing document
    QString mac = GetMac();
    if (!mac.isEmpty()  && mac.compare(pack["mac"].toString()))
            return; // Device is with different MAC.

    m_devicekey = pack["key"].toString();
    SetMac(mac);
}
void CGreeACDevice::timerTimeout()
{
    qCDebug(DeviceLog) << "timerTimeout()  " << GetDeviceName();
}

bool CGreeACDevice::Remove(const QString &dir)
{
    QString fname(dir);
    return CDevice::Remove(fname.append("/").append(GetID()).append(GetFileExt()));

}

bool CGreeACDevice::readPackFromResponse(const QByteArray& response,
                                         const QString& decryptionKey,
                                         QJsonObject& pack)
{

    QJsonParseError parseError;
    auto&& responseJsonDocument = QJsonDocument::fromJson(response, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {

        return false;
    }

    auto&& responseJson = responseJsonDocument.object();

    auto&& encryptedPack = responseJson["pack"].toString();
    if (encryptedPack.isEmpty())
    {
        return false;
    }

    auto&& decryptedPack = Crypto::decryptPack(encryptedPack.toUtf8(), decryptionKey);
    auto&& packJsonDocument = QJsonDocument::fromJson(decryptedPack, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        return false;
    }

    pack = packJsonDocument.object();

    return true;
}
void CGreeACDevice::SetDeviceName(const QString name)
{
    CDevice::SetDeviceName(name);
    m_devicestatus.insert(QString("name"),name);
}


CGreeACLocator::CGreeACLocator() : m_name("Gree Air Conditioners")
{
    m_Busy = false;
    connect(&m_socket, &QUdpSocket::readyRead, this, &CGreeACLocator::socketReadyRead);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &CGreeACLocator::timerTimeout);

};
CGreeACLocator::~CGreeACLocator()
{
    disconnect(&m_socket, &QUdpSocket::readyRead, this, &CGreeACLocator::socketReadyRead);
    disconnect(&m_timer, &QTimer::timeout, this, &CGreeACLocator::timerTimeout);
    if (m_socket.isOpen())
        m_socket.close();

};

bool CGreeACLocator::LoadFromDir(QString const &dir, CDeviceList * toList)
{
    QDir loaddir(dir);
    loaddir.setFilter(QDir::Files);
    QFileInfoList files = loaddir.entryInfoList();
    CGreeACDevice *p;

    if (!toList)
        return false;

    for (int i = files.count() -1; i>=0; i--)
        if (files[i].fileName().contains(GR_AC_FILE_EXT,Qt::CaseInsensitive))
        {
            p = new CGreeACDevice;

            Retry:
            if(p->LoadFromFile(files[i].absoluteFilePath()))
                toList->append(p);
#ifndef NO_GUI
            else
                if (QMessageBox::warning(nullptr,"Error",QString( "Unable to load file: ").append(files[i].fileName()),
                                         QMessageBox::Retry | QMessageBox::Cancel,
                                         QMessageBox::Retry)
                        == QMessageBox::Retry)
                    goto Retry;
                else delete p;
#endif
        }
    return  true;
}
bool CGreeACLocator::ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList)
{
    if (m_Busy)
        return false;
    if (!DeviceList)
        return false;

    m_pFoundDevices = DeviceList;

    m_Busy = true;

     if (!m_socket.isOpen())
    {
         m_socket.open(QIODevice::ReadWrite);
    }

    qint64 written = m_socket.writeDatagram(R"({"t":"scan"})", addr,port);
    if (written < 1)
    {
        m_Busy = false;
        return false;
    }
    m_timer.start(timeout);
  return true;
};

void CGreeACLocator::socketReadyRead()
{

    if (!m_Busy)
        return;

    QNetworkDatagram datagram;

    datagram = m_socket.receiveDatagram(4096);



    QJsonObject pack;
    if (!CGreeACDevice::readPackFromResponse(datagram.data(), Crypto::GenericAESKey, pack))
    {
        return;
    }
    CGreeACDevice *pDevice = new CGreeACDevice(pack);
    pDevice->SetAddress(datagram.senderAddress(),datagram.senderPort());

    if (m_pFoundDevices->AddIfNotExists(pDevice) > -1)
        delete pDevice; //already exists
};

void CGreeACLocator::timerTimeout()
{
    m_Busy = false;

};
const QString CGreeACLocator::GetWorkingDeviceIDName() const
{
    return GREE_AC_NAME;
}
