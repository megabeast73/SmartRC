#include "ccondition.h"
#include <quuid.h>


CCondition::CCondition() : QObject()
{
    m_data.insert(COND_ID,QJsonValue(QUuid::createUuid().toString()));
    m_data.insert(COND_NEG,QJsonValue(false));
    m_data.insert(COND_ACTIVE,QJsonValue(false));
}

CCondition::~CCondition()
{

}
void CCondition::SetName(const QString name)
{
    m_name = name;
    m_data.insert(COND_NAME,QJsonValue(m_name));
}
void CCondition::SetDesc(const QString desc)
{
    m_data.insert(COND_DESC,QJsonValue(desc));
}

const QString CCondition::GetName() const
{
    return m_name;
}

const QString CCondition::GetDesc()  const
{
    return m_data[COND_DESC].toString();
}


const QString CCondition::GetID() const
{
    return m_data[COND_ID].toString();
}

void CCondition::SetNegate(bool negate)
{
    m_data.insert(COND_NEG,QJsonValue(negate));
}
bool CCondition::GetNegate() const
{
    return m_data[COND_NEG].toBool();
}

void CCondition::SetActive(bool active)
{
    m_data.insert(COND_ACTIVE,QJsonValue(active));
}
bool CCondition::GetActive() const
{
    return m_data[COND_ACTIVE].toBool();
}

int CCondition::GetOrder() const
{
    return m_data[COND_ORDER].toInt();
}

void CCondition::SetOrder(int o)
{
    m_data.insert(COND_ORDER,QJsonValue(o));
}

CCondition * ConditionsList::GetById (const QString id)
{
    for (int i = count() -1; i >= 0; i--)
        if (id.compare(at(i)->GetID()) == 0)
            return at(i);
    return nullptr;
}
int ConditionsList::GetIndexById(const QString id)
{
    for (int i = count() -1; i >= 0; i--)
        if (id.compare(at(i)->GetID()) == 0)
            return i;
    return -1;
}
CCondition *ConditionsList::FirsByName(const QString name)
{
    for (int i = count() -1; i >= 0; i--)
        if (name.compare(at(i)->GetName()) == 0)
            return at(i);
    return nullptr;
}

void ConditionsList::Order()
{
    bool bexit = false;

    // the old bubble, remember ?
    while (!bexit)
    {
        bexit = true;
        for (int i = count()-1; i > 0; i--)
            if (at(i)->GetOrder() < at(i-1)->GetOrder())
            {
                bexit = false;
                swap(i,i-1);
            }
    }
}

void ConditionsList::ItemUp(int index)
{
    if (index >= count()  || index == 0)
        return;
    swap(index,index-1);
    for (int i = count()-1; i>=0; i--)
        at(i)->SetOrder(i);

}

void ConditionsList::ItemDown(int index)
{
    ItemUp(index+1);
}

ConditionsList::~ConditionsList ()
{
    while (count() >0)
        delete takeAt(0);
}
