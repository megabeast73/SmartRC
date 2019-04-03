#ifndef CTIMECONDITION_H
#define CTIMECONDITION_H
#include <qobject.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qlist.h>
#include "ccondition.h"

#define COND_TIME_TIME QString("time")
#define COND_TIME_EXACT_DATE QString("date")
#define COND_TIME_WEEKDAYS QString("weekdays")
#define COND_TIME_ON_DATE QString("ondate")

#define WeekList QList<int>

class CTimeCondition : public CCondition
{
    Q_OBJECT
public:
    CTimeCondition();
    virtual ~CTimeCondition();

    virtual bool ShowProperties();

    virtual bool IsTrue();
    virtual CCondition * CreateNew();

    virtual void SetTime(const QTime time);
    virtual const QTime GetTime() const;

    virtual void SetExactDate(const QDate dt);
    virtual const QDate GetExactDate() const;

    virtual void SetWeekDays(WeekList days);
    virtual const WeekList GetWeekDays() const;

    virtual void SetOnDate(int i);

    virtual int GetOnDate() const;

};

#endif // CTIMECONDITION_H
