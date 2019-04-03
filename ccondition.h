#ifndef CCONDITION_H
#define CCONDITION_H

#include <QObject>
#include <QString>
#include <qjsonobject.h>

#define COND_ID QString("id")
#define COND_DESC QString("desc")
// COND_NEG true means condition is negative and must be evaluated as NOT
#define COND_NEG QString("neg")
#define COND_ACTIVE QString("act")
#define COND_NAME QString("name")
#define  COND_ORDER QString("order")



class CCondition : public QObject
{
    Q_OBJECT

public:
    CCondition();
    virtual ~CCondition();

    virtual const QString GetName() const;

    virtual const QString GetDesc() const;
    virtual void SetDesc(const QString desc);

    virtual const QJsonObject GetData()  const { return m_data; }
    virtual void SetData(const QJsonObject data) { m_data = data; }

    virtual const QJsonValue GetCondValue(const QString key) const { return m_data[key]; }
    virtual void SetCondValue(const QString key,const QJsonValue value) {m_data.insert(key,value); }

    virtual const QString GetID() const;

    virtual void SetNegate(bool negate);
    virtual bool GetNegate() const;

    virtual void SetActive(bool active);
    virtual bool GetActive() const;

    virtual int GetOrder() const;
    virtual void SetOrder(int o);


    //pure virtuals
    virtual bool ShowProperties() = 0;
    virtual bool IsTrue() = 0;
    virtual CCondition * CreateNew() = 0;

protected:

    virtual void SetName(const QString name);

protected:

    QJsonObject m_data;
    QString m_name;

};

class ConditionsList : public QList<CCondition *>
{
public:
    ConditionsList () : QList<CCondition *>() { }
    virtual CCondition * GetById (const QString id);
    virtual CCondition * FirsByName(const QString name);
    virtual int GetIndexById(const QString id);
    virtual void Order();
    virtual void ItemUp(int index);
    virtual void ItemDown(int index);
    virtual ~ConditionsList ();

};


#endif // CCONDITION_H
