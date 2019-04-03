#include "ctimeconditiondialog.h"
#include "ui_ctimeconditiondialog.h"
#include <QMessageBox>

CTimeConditionDialog::CTimeConditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTimeConditionDialog)
{
    ui->setupUi(this);
    ui->chkOnDate->setCheckState(Qt::Unchecked);
    ui->dtExact->setDate(QDate::currentDate().addDays(1));
    m_pCondition = nullptr;
    setFixedSize(width(), height());
}

CTimeConditionDialog::~CTimeConditionDialog()
{
    delete ui;
}

void CTimeConditionDialog::on_chkOnDate_stateChanged(int arg1)
{
    ui->dtExact->setEnabled(ui->chkOnDate->checkState() == Qt::Checked);
    ui->grpWeek->setEnabled((ui->chkOnDate->checkState() == Qt::Unchecked));
}

void CTimeConditionDialog::on_buttonBox_accepted()
{
    if (ui->txtDesc->text().isEmpty())
        QMessageBox::information(this,"Info", "Saving condition without a name ?");

    GetValues();
    done(1);
}

void CTimeConditionDialog::on_buttonBox_rejected()
{
    done(0);
};
void CTimeConditionDialog::SetCondition(CTimeCondition *p)
{
    m_pCondition = p;
    LoadValues();
}


void CTimeConditionDialog::LoadValues()
{
    if (!m_pCondition)
        return;
    ui->txtDesc->setText(m_pCondition->GetDesc());
    ui->timeEdit->setTime(m_pCondition->GetTime());
    WeekList days = m_pCondition->GetWeekDays();
    ui->chkMon->setCheckState(days[0] ? Qt::Checked : Qt::Unchecked);
    ui->chkTue->setCheckState(days[1] ? Qt::Checked : Qt::Unchecked);
    ui->chkWed->setCheckState(days[2] ? Qt::Checked : Qt::Unchecked);
    ui->chkThu->setCheckState(days[3] ? Qt::Checked : Qt::Unchecked);
    ui->chkFri->setCheckState(days[4] ? Qt::Checked : Qt::Unchecked);
    ui->chkSat->setCheckState(days[5] ? Qt::Checked : Qt::Unchecked);
    ui->chkSun->setCheckState(days[6] ? Qt::Checked : Qt::Unchecked);
    ui->chkOnDate->setCheckState(m_pCondition->GetOnDate() ? Qt::Checked : Qt::Unchecked);
    ui->dtExact->setDate(m_pCondition->GetExactDate());
}

void CTimeConditionDialog::GetValues()
{
    if (!m_pCondition)
        return;

    m_pCondition->SetDesc(ui->txtDesc->text());
    m_pCondition->SetTime(ui->timeEdit->time());
    m_pCondition->SetOnDate(ui->chkOnDate->checkState() == Qt::Checked ? 1 : 0);
    m_pCondition->SetExactDate(ui->dtExact->date());
    WeekList days = m_pCondition->GetWeekDays();
    days[0] = ui->chkMon->checkState() == Qt::Checked ? 1 : 0;
    days[1] = ui->chkTue->checkState() == Qt::Checked ? 1 : 0;
    days[2] = ui->chkWed->checkState() == Qt::Checked ? 1 : 0;
    days[3] = ui->chkThu->checkState() == Qt::Checked ? 1 : 0;
    days[4] = ui->chkFri->checkState() == Qt::Checked ? 1 : 0;
    days[5] = ui->chkSat->checkState() == Qt::Checked ? 1 : 0;
    days[6] = ui->chkSun->checkState() == Qt::Checked ? 1 : 0;
    m_pCondition->SetWeekDays(days);
}

