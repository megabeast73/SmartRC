#include "ctimecondition.h"

#ifndef NO_GUI
#include "ctimeconditiondialog.h"
#endif

CTimeCondition::CTimeCondition() : CCondition ()

{
    SetName(QString("Exact Time Condition"));
}

CTimeCondition::~CTimeCondition()
{
}

CCondition * CTimeCondition::CreateNew()

{
    return new CTimeCondition;
}

bool CTimeCondition::ShowProperties()
{
#ifndef NO_GUI
    CTimeConditionDialog dlg;
    dlg.SetCondition(this);
    return dlg.exec() !=0;
#else
    return false;
#endif
}

bool CTimeCondition::IsTrue()
{
    bool bRet = true;
    QDateTime current = QDateTime::currentDateTime();
    QDate conddate = GetExactDate();
    QTime condtime = GetTime();

    if (GetOnDate())
        bRet = current.date() == conddate;
    if (bRet)
        bRet = GetWeekDays().at(current.date().dayOfWeek()-1) == 1;
    if (bRet)
        bRet = current.time().hour() == condtime.hour() && current.time().minute() == condtime.minute();
    return bRet;
}

void CTimeCondition::SetTime(const QTime time)
{
    m_data.insert(COND_TIME_TIME,QJsonValue(time.toString(Qt::TextDate)));

}

const QTime CTimeCondition::GetTime() const
{
    return QTime().fromString(m_data[COND_TIME_TIME].toString(),Qt::TextDate);
}


void CTimeCondition::SetExactDate(const QDate dt)
{
    m_data.insert(COND_TIME_EXACT_DATE,QJsonValue(dt.toString(Qt::TextDate)));
}

const QDate CTimeCondition::GetExactDate() const
{
    return QDate().fromString(m_data[COND_TIME_EXACT_DATE].toString(),Qt::TextDate);
}



void CTimeCondition::SetWeekDays(WeekList days)
{
    QString value;
    for (int i =0; i < days.count(); i++)
    {
        if ( i > 0)
             value.append(",");
        value.append(QString::number(days.at(i),10));
    }

    m_data.insert(COND_TIME_WEEKDAYS,QJsonValue(value));
    return ;
}
void CTimeCondition::SetOnDate(int i)
{
     m_data.insert(COND_TIME_ON_DATE,QJsonValue(i));
}

int CTimeCondition::GetOnDate() const
{
    return m_data[COND_TIME_ON_DATE].toInt();
}

const WeekList CTimeCondition::GetWeekDays() const
{
    QString v = m_data[COND_TIME_WEEKDAYS].toString();
    WeekList ret;

    while (v.indexOf(",") > 0)
    {
        ret.append(v.left(1).toInt());
        v.remove(0,2);
    }
    ret.append(v.toInt());
    while (ret.count() < 7)
        ret.append(0);
    return ret;
}
