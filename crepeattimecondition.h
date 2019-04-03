#ifndef CREPEATTIMECONDITION_H
#define CREPEATTIMECONDITION_H

#include <QObject>
#include <qdatetime.h>
#include "ccondition.h"


// RPT_REPEAT_INTERVAL_TYPE: 0-min,1-hour,2-day,3-week
#define RPT_REPEAT_INTERVAL_TYPE "rpt_int_type"

#define RPT_REPEAT_VALUE    "rpt_int_value"

//RPT_FROM_TYPE 0 - first execution, 1 - specific time
#define RPT_FROM_TYPE "rpt_from_type"
#define RPT_TIME "rpt_time"

// RPT_FIRST_TRUE  0 - returns allways TRUE on first check. Then only on the interval
#define RPT_FIRST_TRUE "rpt_first_true"

class CRepeatTimeCondition : public CCondition
{
public:
    CRepeatTimeCondition();

    virtual int GetIntType() const {return m_data[RPT_REPEAT_INTERVAL_TYPE].toInt();}
    virtual void SetIntType(int i) { m_data.insert(RPT_REPEAT_INTERVAL_TYPE,QJsonValue(i));}

    virtual int GetIntValue() const {return m_data[RPT_REPEAT_VALUE].toInt();}
    virtual void SetIntValue(int i) { m_data.insert(RPT_REPEAT_VALUE,QJsonValue(i));}

    virtual bool GetFromType() const {return m_data[RPT_FROM_TYPE].toBool();}
    virtual void SetFromType(bool t) { m_data.insert(RPT_FROM_TYPE,QJsonValue(t));}

    virtual QString GetFromTime() const {return m_data[RPT_TIME].toString();}
    virtual void SetFromTime(const QString &tm) { m_data.insert(RPT_TIME,QJsonValue(tm));}

    virtual bool GetFirstTrue() const {return m_data[RPT_FIRST_TRUE].toInt() == 1 ? true : false;}
    virtual void SetFirstTrue(bool t) { m_data.insert(RPT_FIRST_TRUE,QJsonValue(t ? 1 :0));}

    //pure virtuals
    virtual bool ShowProperties();
    virtual bool IsTrue();
    virtual CCondition * CreateNew();
protected:
    virtual void AddNextTime();

protected:
    bool m_bFirstTime;
    QDateTime m_dtFrom;

};

#endif // CREPEATTIMECONDITION_H
