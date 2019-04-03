#include "cbroadlinkrm.h"
#include <QCoreApplication>
#include <qbytearray.h>
#include <qdatetime.h>
#include <qtimezone.h>
#include <qfile.h>

#ifndef NO_GUI
#include <qmessagebox.h>
#endif

#include <qnetworkdatagram.h>
#include <qdir.h>
#include "crypto.h"
#include "globals.h"

unsigned char generic_key[16] = { 0x09, 0x76, 0x28, 0x34, 0x3f, 0xe9, 0x9e, 0x23, 0x76, 0x5c, 0x15, 0x13, 0xac, 0xcf, 0x8b, 0x02};
unsigned char vector[16] = {0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28, 0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};

struct BroadLinkDevices {
    ulong id;
    QString name;
};
#define NUM_DEVICES 36

#define BROADLINK_NAME QString("BroadLink RM device")

BroadLinkDevices Devices[NUM_DEVICES] =
{
    {0,""},
    {0x2719,"Honeywell SP2"},
    {0x7919, "Honeywell SP2"},
    {0x271a, "Honeywell SP2"},
    {0x791a, "Honeywell SP2"},
    {0x2720, "SPMini"},
    {0x753e, "SP3"},
    {0x7D00, "OEM branded SP3"},
    {0x947a, "SP3S"},
    {0x9479, "SP3S"},
    {0x2728, "SPMini2"},
    {0x2733, "OEM branded SPMini"},
    {0x273e, "OEM branded SPMini"},
    {0x7530, "OEM branded SPMini2"},
    {0x7918, "OEM branded SPMini2"},
    {0x2736, "SPMiniPlus"},
    {0x2712, "RM2"},
    {0x2737, "RM Mini"},
    {0x273d, "RM Pro Phicomm"},
    {0x2783, "RM2 Home Plus"},
    {0x277c, "RM2 Home Plus GDT"},
    {0x272a, "RM2 Pro Plus"},
    {0x2787, "RM2 Pro Plus2"},
    {0x279d, "RM2 Pro Plus3"},
    {0x27a9, "RM2 Pro Plus_300"},
    {0x278b, "RM2 Pro Plus BL"},
    {0x2797, "RM2 Pro Plus HYC"},
    {0x27a1, "RM2 Pro Plus R1"},
    {0x27a6, "RM2 Pro PP"},
    {0x278f, "RM Mini Shat"},
    {0x2714, "A1"},
    {0x4EB5, "MP1"},
    {0x4EF7, "Honyar oem mp1"},
    {0x4EAD, "Hysen controller"},
    {0x2722, "S1 (SmartOne Alarm Kit)"},
    {0x4E4D, "Dooya DT360E (DOOYA_CURTAIN_V2)"}
};

/*
unsigned char ret_bytes[] = {
  0xd1, 0x91, 0xec, 0x94, 0xa8, 0xef, 0xd2, 0xce,
  0xc9, 0xca, 0x6c, 0xa8, 0xdc, 0xba, 0x6f, 0x93,
  0x82, 0x84, 0x97, 0xbd, 0x72, 0x48, 0x27, 0x3b,
  0x3e, 0x05, 0xaa, 0x72, 0x40, 0xdf, 0xa4, 0x16
};
*/

#define BDEV_FILE_EXT ".brlnk"
#define CMD_AUTH 0x65

#pragma warning( push )
#pragma warning( disable : 4309)

CBroadLinkRM::CBroadLinkRM() : CDevice()
{
//    m_Busy = false;
//    connect(&m_Timer, &QTimer::timeout, this, &CBroadLinkRM::TimeOut);
//    m_Timer.setSingleShot(true);
    m_socket.open(QIODevice::ReadWrite);
    m_SendCnt = 0;
    m_devkey = 0;
    memcpy(&m_encr_key,&generic_key,sizeof(m_encr_key));
}

CBroadLinkRM::~CBroadLinkRM()
{
 //   disconnect(&m_Timer, &QTimer::timeout, this, &CBroadLinkRM::TimeOut);
}
void CBroadLinkRM::PreparePacket(QByteArray &ret, int cmd)
{
    QStringList mac = global::GetActiveMac().split(":");

    m_SendCnt = (m_SendCnt + 1) &  0xffff;
    ret[0x00] = 0x5a;
    ret[0x01] = 0xa5;
    ret[0x02] = 0xaa;
    ret[0x03] = 0x55;
    ret[0x04] = 0x5a;
    ret[0x05] = 0xa5;
    ret[0x06] = 0xaa;
    ret[0x07] = 0x55;
    unsigned int dev = GetDeviceType();
    ret[0x24] = dev & 0xff;
    ret[0x25] = dev >> 8;

    ret[0x26] = cmd;
    ret[0x2a] = mac[0].toInt(nullptr,16);
    ret[0x2b] = mac[1].toInt(nullptr,16);
    ret[0x2c] = mac[2].toInt(nullptr,16);
    ret[0x2d] = mac[3].toInt(nullptr,16);
    ret[0x2e] = mac[4].toInt(nullptr,16);
    ret[0x2f] = mac[5].toInt(nullptr,16);
    ret[0x28] = m_SendCnt & 0xff00;
    ret[0x29] = m_SendCnt >> 8;

}
bool CBroadLinkRM::PollDevice(int timeout_sec)
{
//    while (m_Busy)
//        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
//    m_Busy = true;
     m_devkey = 0;
    //Fill the packet
    QByteArray header(0x38,0);
    PreparePacket(header,CMD_AUTH);
    QByteArray payload(0x50,0);
    payload[0x04] = 0x31;
    payload[0x05] = 0x31;
    payload[0x06] = 0x31;
    payload[0x07] = 0x31;
    payload[0x08] = 0x31;
    payload[0x09] = 0x31;
    payload[0x0a] = 0x31;
    payload[0x0b] = 0x31;
    payload[0x0c] = 0x31;
    payload[0x0d] = 0x31;
    payload[0x0e] = 0x31;
    payload[0x0f] = 0x31;
    payload[0x10] = 0x31;
    payload[0x11] = 0x31;
    payload[0x12] = 0x31;

    payload[0x1e] = 0x01;

    payload[0x2d] = 0x01;

    payload[0x30] = 'S';
    payload[0x31] = 'm';
    payload[0x32] = 'a';
    payload[0x33] = 'r';
    payload[0x34] = 't';
    payload[0x35] = 'R';
    payload[0x36] = 'C';

    //Pad the paylod for encryption
    int numpad=(payload.length() /16+1)*16;
    payload = payload.leftJustified(numpad,0);

    //checksum of the payload
    unsigned long  checksum = CalcChecksum(payload);
    header[0x34] = checksum & 0xff;
    header[0x35] = checksum >> 8;

    //Encrypt the payload with AES_CBC, but with the default key. AUTH rq must be with the default key
    payload = Crypto::Encrypt_CBC(payload,generic_key[0],vector[0]);
    header += payload;

    //checksum of whole packet
    checksum = CalcChecksum(header);
    header[0x20] = checksum & 0xff;
    header[0x21] = checksum >> 8;

//    connect(&m_socket, &QUdpSocket::readyRead, this, &CBroadLinkRM::HandleAUTH);
//    m_Timer.start(timeout_sec);
    QString ad = GetAddr();
    int p = GetPort();
   m_socket.writeDatagram(header, QHostAddress(GetAddr()),GetPort());
   if(m_socket.waitForReadyRead(timeout_sec*100))
       HandleAUTH();

//    while (m_Busy)
//        QCoreApplication::processEvents(QEventLoop::AllEvents,100);

//    disconnect(&m_socket, &QUdpSocket::readyRead, this, &CBroadLinkRM::HandleAUTH);

//    m_Busy = false;
    return m_devkey != 0;
}
void CBroadLinkRM::HandleAUTH()
{
    //unsigned char datagram[1024] {0};
    QNetworkDatagram rec;
    QHostAddress remoteAddress;
    int remotePort;

//    if (!m_Busy)
//        return;


    rec = m_socket.receiveDatagram(1024);
    QByteArray datagram(rec.data());

    remoteAddress = rec.senderAddress();
    remotePort = rec.senderPort();

    //Get the result and strip the header
    unsigned int ret = (unsigned char) datagram[38] | ((unsigned char) datagram[39] << 8);
    unsigned int cs = (unsigned char) datagram[0x20] | ((unsigned char) datagram[0x21] << 8);

    datagram[0x20] = 0;
    datagram[0x21] =0;

    if (CalcChecksum(datagram) != cs)
        return; // invalid packet

    if ( ret != 0x3e9)
        return; // Not expected return command. Leave and wait for timeout

    //checksum of unencrypted payload
    cs = (unsigned char) datagram[0x34] | ((unsigned char) datagram[0x35] << 8);

    //Strip the header, and decrypt the payload.
    datagram.remove(0,56);
    if(datagram.length() < 1 || (datagram.length() % 16) != 0)
        return;

    //decrypt the payload - decryption of AUTH must be with default key
    datagram = Crypto::Decrypt_CBC(datagram,generic_key[0],vector[0]);

    if( cs != CalcChecksum(datagram))
        return ; //Invalid payload
    memcpy(&m_devkey,datagram.constData(),sizeof(m_devkey));
    datagram.remove(0,4);
    memcpy(&m_encr_key,datagram.constData(),sizeof(m_encr_key));

//    m_Busy = false;
}

bool CBroadLinkRM::GetDeviceData(int timeout_sec)
{
    return false;
}

bool CBroadLinkRM::SetDeviceData(int timeout_sec)
{
    return false;
}


CDevice * CBroadLinkRM::CreateNew()
{
    return new CBroadLinkRM;
}

const QString CBroadLinkRM::GetDevIDName()
{
    return BROADLINK_NAME;
}



bool CBroadLinkRM::ShowDevProp()
{
    return false;
}

 const QString CBroadLinkRM::GetVendor ()
{
    ulong devid = GetDeviceType();
    for (int i = 0; i <= NUM_DEVICES; i++)
        if (Devices[i].id == devid)
         return Devices[i].name;
    return QString();
}
const QString CBroadLinkRM::GetDeviceKey()
{
    if(m_devkey)
        return QString::number(m_devkey);
    return QString();
}

void CBroadLinkRM::SetMac (const QString mac)
{
    CDevice::SetMac(mac);
    QString id(mac);
    int iPos = 0;
    while ((iPos = id.indexOf(":")) && iPos >= 0)
        id.remove(iPos,1);
    SetID(id);
}



const QString CBroadLinkRM::GetFileExt() const
{
    return QString(BDEV_FILE_EXT);
}
bool CBroadLinkRM::SaveToFile(const QString &dir)
{
    QString file_name (dir);
    file_name.append(QString("/"));
    file_name.append(GetID());
    file_name.append(GetFileExt());


    return CDevice::SaveToFile(file_name);
}

int CBroadLinkRM::CalcChecksum(QByteArray &packet)
{

    ulong checksum = 0xbeaf;

    for (int i = 0; i < packet.length(); i++)
        checksum = checksum + (unsigned char) packet.at(i);

    checksum = checksum & 0xffff;
    return checksum;

}

CBroadLinkRMLocator::CBroadLinkRMLocator() : CDeviceLocator()
{
    m_Busy = false;

}


void CBroadLinkRMLocator::HandleResponse()
{
    char datagram[1024] = { 0 };
    QHostAddress remoteAddress;
    uint16_t remotePort = 0;
    QString mac;
    unsigned char iTmp;
    CBroadLinkRM *pDev = nullptr;

    if (!m_Busy)
        return;
    int length = m_socket.readDatagram(reinterpret_cast<char*>(&datagram), sizeof(datagram), &remoteAddress, &remotePort);
    if (length < 1)
        return;
    //Fill the values
    for (int i = 0x3f; i>0x3a; i--)
    {
        iTmp = (unsigned char) datagram[i];
        mac.append(QString::number(iTmp,16));
        mac.append(":");

    }
    iTmp = (unsigned char) datagram[0x3a];
    mac.append(QString::number(iTmp,16));
    QString name(&datagram[0x40]);


    unsigned int  devtype = (unsigned char) datagram[0x34] | (unsigned char)datagram[0x35] << 8;

    pDev = new CBroadLinkRM;
    pDev->SetAddress(remoteAddress,remotePort);
    pDev->SetDeviceType(devtype);
    pDev->SetDeviceName(name);
    pDev->SetMac(mac);
    m_pFoundDevices->append(pDev);

}

void CBroadLinkRMLocator::TimeOut()
{
    m_Busy = false;
}


bool CBroadLinkRMLocator::ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList)
{
    if (!DeviceList)
        return false;
    if (m_Busy)
        return false;

    m_pFoundDevices = DeviceList;
    m_Busy = true;
    port = 80;

     if (!m_socket.isOpen())
         m_socket.open(QIODevice::ReadWrite);

    connect(&m_socket, &QUdpSocket::readyRead, this, &CBroadLinkRMLocator::HandleResponse);
    connect(&m_timer, &QTimer::timeout, this, &CBroadLinkRMLocator::TimeOut);

    //Prepare discover packet
    QByteArray packet(0x30,0);

    long int year = QDate::currentDate().year();
    long  int tz = QTimeZone::systemTimeZone().daylightTimeOffset(QDateTime::currentDateTime()) / -3600;
    if (tz < 0)
    {
       tz = 0xff + tz - 1;
       packet[0x09] = 0xff;
       packet[0x0a] = 0xff;
       packet[0x0b] = 0xff;
    }
    packet[0x08] = tz;

    packet[0x0c] = year & 0xff;
    packet[0x0d] = year >> 8;
    packet[0x0e] = QTime::currentTime().minute();
    packet[0x0f] = QTime::currentTime().hour();

    packet[0x10] = char(year - 2000);
    packet[0x11] = QDate::currentDate().dayOfWeek();
    packet[0x12] = QDate::currentDate().day();
    packet[0x13] = QDate::currentDate().month();
    packet[0x18] = 0; //ip octets
    packet[0x19] = 0;
    packet[0x1a] = 0;
    packet[0x1b] = 0;
    packet[0x1c] = 0; //port & 0xff;
    packet[0x1d] = 0; //port >> 8;
    packet[0x26] = 6; //Send HELLO

    int checksum = CBroadLinkRM::CalcChecksum(packet);
    packet[0x20] = checksum & 0xff;
    packet[0x21] = checksum >> 8;

    m_timer.setSingleShot(true);
    m_timer.start(timeout);
    m_socket.writeDatagram(packet, addr,port);

    while (isBusy())
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);

    disconnect(&m_socket, &QUdpSocket::readyRead, this, &CBroadLinkRMLocator::HandleResponse);
    disconnect(&m_timer, &QTimer::timeout, this, &CBroadLinkRMLocator::TimeOut);
    return true;
}
#pragma warning( pop )
const QString CBroadLinkRMLocator::GetName()
{
    return QString("Locating BroadLink RM 2/3/Pro/Mini devices");
}

bool CBroadLinkRMLocator::LoadFromDir(QString const &dir, CDeviceList * toList)
{
    QDir loaddir(dir);
    loaddir.setFilter(QDir::Files);
    QFileInfoList files = loaddir.entryInfoList();
    CBroadLinkRM *p;

    if (!toList)
        return false;

    for (int i = files.count() -1; i>=0; i--)
        if (files[i].fileName().contains(BDEV_FILE_EXT,Qt::CaseInsensitive))
        {
            p = new CBroadLinkRM;

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
 const QString CBroadLinkRMLocator::GetWorkingDeviceIDName() const
 {
    return BROADLINK_NAME;
 }
