#include "cdevice.h"
#include <qjsondocument.h>
#include <qfile.h>


CDevice::CDevice() : QObject ()
{

}
/*
CDevice::CDevice (CDevice &dev) : QObject (), m_DeviceAddress(dev.GetAddr())
{
    m_DeviceID = dev.GetID();
    m_Port = dev.GetPort();
    m_mac = dev.GetMac();
    m_DeviceName = dev.GetDeviceName();
    m_Vendor = dev.GetVendor();

}
*/
bool CDevice::SaveToFile(const QString &file_name)
{
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

bool CDevice::LoadFromFile(const QString &file_name )
{
    QFile loadFile(file_name);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }

        QByteArray saveData = loadFile.readAll();
        m_data =QJsonDocument::fromBinaryData(saveData).object();
        return true;
}

bool CDevice::Remove(const QString &file_name)
{
    QFile f (file_name);
    return f.remove();
}



void CDeviceList::clear()
{
    while (!isEmpty())
        delete takeFirst();
}
bool CDeviceList::SaveToDir(const QString dir)
{
    bool bret = true;

    for (int i = count() -1; i >-1; i--)
        bret = bret && value(i)->SaveToFile(dir);
    return bret;
}

CDevice* CDeviceList::GetByID (QString const * id)
{
    long index = FindByID(id);
    if (index > -1)
            return value(index);
    return nullptr;
}

long CDeviceList::FindByID(QString const * id)
{
    for (int i =0; i < count(); i++)
        if (id->compare(value(i)->GetID()) == 0)
            return  i;
    return -1;
}


CDeviceList::~CDeviceList()
{
    clear();
}
CDevice * CDeviceList::FirsByIDName(const QString name)
{
    for (int i = count() -1; i >= 0; i--)
        if (name.compare(at(i)->GetDevIDName()) == 0)
            return at(i);
    return nullptr;
}

long CDeviceList::FindByID(CDevice * pDevice)
{
    for (int i =0; i < count(); i++)
        if( at(i)->GetID().compare(pDevice->GetID()) == 0)
            return i;
    return -1;
}
long CDeviceList::AddIfNotExists(CDevice * pDevice)
{
    for (int i =0; i < count(); i++)
        if( at(i)->Compare(pDevice) == CDevice::EQ)
            return i;
    append(pDevice);
   return -1;
}
void CDeviceList::RemoveById(CDevice * pDevice)
{
    long i = FindByID(pDevice);
    if (i > -1)
        removeAt(i);
}



void CDevice::SetAddress( const QHostAddress &addr, quint16 port)
{
    m_data.insert(DEV_ADDR,QJsonValue(addr.toString()));
    SetPort(port);
}



 CDevice::CompareResult CDevice::Compare (CDevice *p)
{

 if ((GetID().compare(p->GetID()) != 0) || (GetDevIDName().compare(p->GetDevIDName()) !=0))
        return CompareResult::NOT_EQ;
// if (!GetAddr().compare(p->GetAddr()) || GetPort() != p->GetPort())
//     return  CompareResult::EQ_DIF_HOST;


 return CompareResult::EQ;

}
