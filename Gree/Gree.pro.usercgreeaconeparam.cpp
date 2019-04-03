#include "cgreeaconeparam.h"
#include "globals.h"
#include <qfile.h>
#include <qdir.h>
#include <QUuid>

#ifndef NO_GUI
#include <qmessagebox.h>
#include "cgreeaconeparamdialog.h"
#endif



#define GREE_ONE_PREFIX QString("ONE_P_")
#define GREE_ONE_FILE_EXT QString(".onep_grac")

//CGreeACOneParam will have a unique ID. This will allow the user
//to have many instances of this class
CGreeACOneParam::CGreeACOneParam() : CGreeACDevice ()
{
    m_data.insert(GREE_ONE_PREFIX.append("_id"),QJsonValue(QUuid::createUuid().toString()));
}

CGreeACOneParam::~CGreeACOneParam()
{

}

bool CGreeACOneParam::SetDeviceData(int timeout_sec)
{
    if (!GetDeviceData(timeout_sec))
        return false;
    QString propname = GetValueName();
    QJsonValue propvalue = GetValue();
    m_devicestatus.insert(propname,propvalue);
    return CGreeACDevice::SetDeviceData(timeout_sec);

}
bool CGreeACOneParam::GetDeviceData(int timeout_sec)
{
    QString str = GetDeviceName();
    bool bret = CGreeACDevice::GetDeviceData(timeout_sec);
    SetDeviceName(str);
    return bret;
}

const QString CGreeACOneParam::GetDevIDName()
{
    return GREE_ONE_PARAM_NAME;
}

void CGreeACOneParam::SetValue(const QJsonValue val)
{
    m_data.insert(GREE_ONE_PREFIX.append("_VAL"),val);
}
const QJsonValue CGreeACOneParam::GetValue() const
{
    return m_data[GREE_ONE_PREFIX.append("_VAL")];
}


void CGreeACOneParam::SetValueName (const QString name)
{
    m_data.insert(GREE_ONE_PREFIX.append("_NAME"),QJsonValue(name));
}

const QString CGreeACOneParam::GetValueName() const
{
    return m_data[GREE_ONE_PREFIX.append("_NAME")].toString();
}

bool CGreeACOneParam::ShowDevProp()
{
#ifndef NO_GUI
    CGreeACOneParamDialog dlg(nullptr);
    dlg.SetDevice(this);
    return dlg.exec() != 0;
#else
    return false;
#endif
}

const QString CGreeACOneParam::GetFileExt() const
{
    return GREE_ONE_FILE_EXT;
}
void CGreeACOneParam::SetDeviceName(const QString name)
{
    m_data.insert(GREE_ONE_PREFIX.append("name"),QJsonValue(name));

}
const QString CGreeACOneParam::GetDeviceName()
{
    return m_data[GREE_ONE_PREFIX.append("name")].toString();

}

const QString CGreeACOneParam::GetID()
{
    return m_data[GREE_ONE_PREFIX.append("_id")].toString();

}
void CGreeACOneParam::SetID(const QString id)
{
    m_data.insert(GREE_ONE_PREFIX.append("_id"),QJsonValue(id));

}

CGreeACOneParamLocator::CGreeACOneParamLocator()
{
    m_name = "Gree ACs(one param) - works on already discovered devices";
}

//Will cycle all gree known devices and create new object using the device data
bool CGreeACOneParamLocator::ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList )
{
    CDevice *pDev;
    CGreeACOneParam *pNewDev = nullptr;
    QString name;
    for (int i = global::g_pKnownDevices->count()-1;i >= 0; i--)
    {
        pDev = global::g_pKnownDevices->at(i);
        if(pDev->GetDevIDName().compare(GREE_AC_NAME) == 0)
        {
            pNewDev = new CGreeACOneParam;
            name = pNewDev->GetID(); //Save the ID since SetDeviceObject will clear it
            pNewDev->SetDeviceObject(pDev->GetDeviceObject());
            pNewDev->SetID(name); //restore the ID
            name = pDev->GetDeviceName();
            pNewDev->SetDeviceName(name.prepend("(1P) "));
            if (DeviceList->AddIfNotExists(pNewDev) > -1)
                delete pNewDev; //Already added to list
        }
    }
    return pNewDev == nullptr;
}

bool CGreeACOneParamLocator::LoadFromDir(QString const &dir, CDeviceList * toList)
{

    QDir loaddir(dir);
    loaddir.setFilter(QDir::Files);
    QFileInfoList files = loaddir.entryInfoList();
    CGreeACDevice *p;

    if (!toList)
        return false;

    for (int i = files.count() -1; i>=0; i--)
        if (files[i].fileName().contains(GREE_ONE_FILE_EXT,Qt::CaseInsensitive))
        {
            p = new CGreeACOneParam;

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
#endif
                else delete p;
        }
    return  true;
}

const QString CGreeACOneParamLocator::GetWorkingDeviceIDName() const
{
    return GREE_ONE_PARAM_NAME;
}
