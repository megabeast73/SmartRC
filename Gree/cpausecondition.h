#ifndef CPAUSECONDITION_H
#define CPAUSECONDITION_H
#include "ccondition.h"

#define COND_PAUSE_TIME QString("pause")

class CPauseCondition : public CCondition
{
public:
    CPauseCondition();

    virtual bool ShowProperties();
    virtual bool IsTrue();
    virtual CPauseCondition * CreateNew();

    virtual void SetPause(ulong msec) { m_data.insert(COND_PAUSE_TIME, QJsonValue((qint64)msec));}
    virtual ulong GetPause() { return m_data[COND_PAUSE_TIME].toInt();}
};

#endif // CPAUSECONDITION_H
