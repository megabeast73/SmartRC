#ifndef CGREEACONEPARAM_H
#define CGREEACONEPARAM_H

/*
 *CGreeACOneParam is a fake device which will override SetParam method
 * As an ID it uses different guid. This will allow the user to have many
 * different instances
 */

#include <QObject>
#include "cgreeacdevice.h"

#define GREE_ONE_PARAM_NAME QString ("Gree Air Conditioner - Sets only one parameter")

class CGreeACOneParam : public CGreeACDevice
{
    Q_OBJECT

public:
    CGreeACOneParam();

    virtual CDevice * CreateNew() { return new CGreeACOneParam; }

    virtual void SetValue(const QJsonValue val);
    virtual const QJsonValue GetValue() const;

    virtual void SetValueName (const QString name);
    virtual const QString GetValueName() const;

    virtual void SetParam(const QString name, int val) { SetValueName(name); SetValue(QJsonValue(val));}

    //ui related value. To skip some checks on UI load
    virtual void SetSelIndex(int i) { m_data.insert("sel_index",QJsonValue(i));}
    virtual int GetSelIndex() const { return  m_data["sel_index"].toInt();}


    virtual const QString GetID();
    virtual void SetID(const QString id);

    virtual const QString GetDeviceName();
    virtual void SetDeviceName(const QString name);

    virtual const QString GetDevIDName();

    virtual bool SetDeviceData(int timeout_sec);
    virtual bool GetDeviceData(int timeout_sec);

    virtual bool ShowDevProp();

    virtual ~CGreeACOneParam();

protected:
    virtual const QString GetFileExt() const;

};

class CGreeACOneParamLocator : public CGreeACLocator
{

public:
    CGreeACOneParamLocator();
    virtual bool ScanDevices( quint16 timeout,const QHostAddress addr, quint16 port,CDeviceList *DeviceList );
    bool LoadFromDir(QString const &dir, CDeviceList * toList);
    virtual const QString GetWorkingDeviceIDName() const;

};

#endif // CGREEACONEPARAM_H
