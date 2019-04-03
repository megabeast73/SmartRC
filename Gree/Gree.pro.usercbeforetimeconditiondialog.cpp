#include "cbeforetimeconditiondialog.h"
#include "ui_ctimeconditiondialog.h"

CBeforeTimeConditionDialog::CBeforeTimeConditionDialog() : CTimeConditionDialog()
{
    ui->grpWeek->setVisible(false);
    ui->lblHint->setText("Condition will be met, if the current time is before selected");
    ui->chkOnDate->move(ui->chkOnDate->x(),ui->grpWeek->y());
    ui->dtExact->move(ui->dtExact->x(),ui->grpWeek->y());
}
