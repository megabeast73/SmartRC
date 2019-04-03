#include "cpausecondition.h"

#ifndef NO_GUI
#include "cpauseconditiondialog.h"
#endif

#include <QThread>
#include "globals.h"

CPauseCondition::CPauseCondition() : CCondition ()
{
    CCondition::SetName(QString("Pause condition"));
    SetPause(100);
}

bool CPauseCondition::ShowProperties()
{
#ifndef NO_GUI
    CPauseConditionDialog dlg;
    dlg.SetTime(GetPause());
    dlg.SetDesc(GetDesc());
    if (dlg.exec() == 0)
        return false;
    SetPause(dlg.GetTime());
    SetDesc(dlg.GetDesc());
#endif
    return true;
}

bool CPauseCondition::IsTrue()
{
    QString log (GetDesc());
    log.append(" is pausing the event for ");
    log.append(QString::number(GetPause()));
    log.append(" msecs");
    global::g_pLogger->Message(log);
    QThread::msleep(GetPause());
    return true;
}
CPauseCondition * CPauseCondition::CreateNew()
{
    return new CPauseCondition;
}
