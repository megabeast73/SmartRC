#include "qmessagebox.h"
#include "crepeattimeconditiondialog.h"
#include "ui_crepeattimeconditiondialog.h"
#include "crepeattimecondition.h"

CRepeatTimeConditionDialog::CRepeatTimeConditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRepeatTimeConditionDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    m_pCondition = nullptr;

}
void CRepeatTimeConditionDialog::SetCondition(CRepeatTimeCondition *pCond)
{
    m_pCondition = pCond;
    SetUi();

}
void CRepeatTimeConditionDialog::SetUi()
{
    if (!m_pCondition)
        return;
    ui->txtDesc->setText(m_pCondition->GetDesc());
    ui->tmEdit->setTime(QTime::fromString(m_pCondition->GetFromTime(),"hh:mm"));
    ui->chkFirstTrue->setCheckState(m_pCondition->GetFirstTrue() ? Qt::Checked : Qt::Unchecked);
    ui->chkOnTime->setCheckState(m_pCondition->GetFromType() ? Qt::Checked : Qt::Unchecked);
    ui->spnValue->setValue(m_pCondition->GetIntValue());
    ui->cmbIntType->setCurrentIndex(m_pCondition->GetIntType());

}
void CRepeatTimeConditionDialog::GetUi()
{
    if (!m_pCondition)
        return;

    m_pCondition->SetDesc(ui->txtDesc->text());
    m_pCondition->SetFromTime(ui->tmEdit->time().toString("hh:mm"));
    m_pCondition->SetFirstTrue(ui->chkFirstTrue->checkState()== Qt::Checked);
    m_pCondition->SetFromType(ui->chkOnTime->checkState() == Qt::Checked);
    m_pCondition->SetIntValue(ui->spnValue->value());
    m_pCondition->SetIntType(ui->cmbIntType->currentIndex());
}

CRepeatTimeConditionDialog::~CRepeatTimeConditionDialog()
{
    delete ui;
}

void CRepeatTimeConditionDialog::on_chkOnTime_stateChanged(int arg1)
{
    ui->tmEdit->setEnabled(ui->chkOnTime->checkState() != Qt::Checked);
}

void CRepeatTimeConditionDialog::on_buttonBox_accepted()
{
    if (ui->txtDesc->text().isEmpty())
        QMessageBox::information(this,"Info", "Saving condition without a name ?");
    GetUi();
    done(1);

}

void CRepeatTimeConditionDialog::on_buttonBox_rejected()
{
    done(0);
}
