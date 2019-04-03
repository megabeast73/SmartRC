#include "cpauseconditiondialog.h"
#include "ui_cpauseconditiondialog.h"
#include <QMessageBox>

CPauseConditionDialog::CPauseConditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPauseConditionDialog)
{
    ui->setupUi(this);
    ui->spnTime->setValue(100);
    setFixedSize(width(), height());
}

CPauseConditionDialog::~CPauseConditionDialog()
{
    delete ui;
}
void CPauseConditionDialog::SetTime(ulong time)
{
    ui->spnTime->setValue(time);
}

ulong CPauseConditionDialog::GetTime()
{
    return ui->spnTime->value();
}
const QString CPauseConditionDialog::GetDesc() const
{

    return ui->txtDesc->text();
}
void CPauseConditionDialog::SetDesc(const QString desc)
{

    ui->txtDesc->setText(desc);
}
void CPauseConditionDialog::on_buttonBox_accepted()
{
    if (ui->txtDesc->text().isEmpty())
        QMessageBox::information(this,"Info", "Saving condition without a name ?");
    done(1);

}

void CPauseConditionDialog::on_buttonBox_rejected()
{
    done(0);
}
