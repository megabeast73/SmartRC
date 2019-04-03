#ifndef COREVENT_H
#define COREVENT_H

#include <QObject>
#include "cevent.h"

class CorEvent : public CEvent
{
    Q_OBJECT
public:
    CorEvent();
    virtual bool TestConditions();
    virtual CEvent * CreateNew() { return new CorEvent;}
    virtual bool ShowProperties ();

};

#endif // COREVENT_H
