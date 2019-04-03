#include "crepeattimecondition.h"

#ifndef NO_GUI
#include "crepeattimeconditiondialog.h"
#endif
CRepeatTimeCondition::CRepeatTimeCondition() : CCondition()
{
    SetName(QString("Time repeated condition"));
    m_bFirstTime = true;
}

bool CRepeatTimeCondition::ShowProperties()
{
#ifndef NO_GUI
    CRepeatTimeConditionDialog dlg;
    dlg.SetCondition(this);
    return dlg.exec() == 1;
#else
    return false;
#endif
}
void CRepeatTimeCondition::AddNextTime()
{
    switch (GetIntType())
    {//0-min,1-hour,2-day,3-week
        case 0:
           m_dtFrom = m_dtFrom.addSecs(GetIntValue() *60);
            break;
        case 1:
            m_dtFrom = m_dtFrom.addSecs(GetIntValue()*3600);
            break;
        case 2:
            m_dtFrom = m_dtFrom.addDays(GetIntValue());
            break;
        case 3:
            m_dtFrom = m_dtFrom.addDays(GetIntValue()*7);
            break;
    }
}
bool CRepeatTimeCondition::IsTrue()
{
    bool bRet = true;
    QDateTime now(QDateTime::currentDateTime());

    if (m_bFirstTime)
        m_dtFrom = now;
//    int d,h,m;

//    d = m_dtFrom.date().day();
//    h= m_dtFrom.time().hour();
//    m = m_dtFrom.time().minute();

    //0 - from first execution or 1 - from specific time
    if (!GetFromType() && m_bFirstTime)
    { //Set the specified time and set the next TRUE
        m_dtFrom.setTime(QTime::fromString(GetFromTime(),"hh:mm"));
        while (m_dtFrom < now)
            AddNextTime();
    } //Otherwise, it is from current time.

//    d = m_dtFrom.date().day();
//    h= m_dtFrom.time().hour();
//    m = m_dtFrom.time().minute();
    bRet = bRet && (m_dtFrom <= now);

    if (bRet)
        AddNextTime(); //True condition is met. Add the next execution time

//    d = m_dtFrom.date().day();
//    h= m_dtFrom.time().hour();
//    m = m_dtFrom.time().minute();

    bRet = bRet ||  (m_bFirstTime && GetFirstTrue()); //return true, if First time TRUE is set.

    m_bFirstTime = false;
    return bRet;
}
CCondition * CRepeatTimeCondition::CreateNew()
{
    return new CRepeatTimeCondition;
}
