#ifndef CBEFORETIMECONDITION_H
#define CBEFORETIMECONDITION_H
#include "ctimecondition.h"

class CBeforeTimeCondition : public CTimeCondition
{
    Q_OBJECT
public:
    CBeforeTimeCondition();
    virtual bool ShowProperties();

    virtual bool IsTrue();
    virtual CCondition * CreateNew();
};

#endif // CBEFORETIMECONDITION_H
