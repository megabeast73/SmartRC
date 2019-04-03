
#include "corevent.h"

#ifndef NO_GUI
#include "caddevent.h"
#endif

CorEvent::CorEvent() : CEvent ()
{
    m_data.insert(EVENT_TYPE,QJsonValue("OR event - One condition true"));
}

bool CorEvent::TestConditions()
{
    CCondition * pCond;
    bool bRet = false;
    bool bRes;
    int iCondCount =  m_conditions.count();
    QString log = GetName();
    log.prepend("Event ");
    for (int i = 0; (i < iCondCount); i++)
    {
        pCond = m_conditions[i];
        if (pCond->GetActive())
        {
            bRes = pCond->IsTrue();
            if (!pCond->GetNegate())
                bRes = !bRes;
            bRet = bRet || bRes;
        }
    }
    if (iCondCount < 1)
        log.append(" has no conditions! Is this the intent ?");
    else {
        if (bRet)
            log.append(": a condition is met.");
        else
            log.append(": no condition met.");
    }
    LogMessage(log);
    return bRet;

}

bool CorEvent::ShowProperties ()
{
#ifndef NO_GUI
    CAddEvent dlg;
    dlg.SetHint("OR Event - This event will be executed when at least one condition is met");
    dlg.SetEvent(this);
    return  dlg.exec() != 0;
#else
    return false;
#endif
}
