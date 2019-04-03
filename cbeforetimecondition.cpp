#include "cbeforetimecondition.h"

#ifndef NO_GUI
#include "cbeforetimeconditiondialog.h"
#endif

CBeforeTimeCondition::CBeforeTimeCondition() : CTimeCondition ()
{
    SetName(QString("Before Time Condition"));
}

bool CBeforeTimeCondition::ShowProperties()
{
#ifndef NO_GUI
    CBeforeTimeConditionDialog dlg;
    dlg.SetCondition(this);
    return dlg.exec() == 1;
#else
    return false;
#endif

}

bool CBeforeTimeCondition::IsTrue()
{
    QDateTime current = QDateTime::currentDateTime();
    QDateTime conddate;
    conddate.setDate(GetOnDate() ? GetExactDate() : current.date());
    conddate.setTime(GetTime());

    return conddate > current;
}

CCondition * CBeforeTimeCondition::CreateNew()
{
    return new CBeforeTimeCondition;
}
