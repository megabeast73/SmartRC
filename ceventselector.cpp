#include "ceventselector.h"
#include "ui_ceventselector.h"
#include "cevent.h"
#include "globals.h"

CEventSelector::CEventSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEventSelector)
{

    ui->setupUi(this);
    setFixedSize(width(), height());
    for (int i = global::g_RegisteredEvents.count()-1; i>= 0; i--)
        ui->cmbEvents->addItem(global::g_RegisteredEvents.at(i)->GetEventType());
}

CEventSelector::~CEventSelector()
{
    delete ui;
}
CEvent * CEventSelector::GetSelectedEvent()
{

    return global::g_RegisteredEvents.GetByType(ui->cmbEvents->currentText())->CreateNew();
}
void CEventSelector::on_comboBox_currentIndexChanged(int index)
{

}

void CEventSelector::on_buttonBox_accepted()
{
    done (1);
}

void CEventSelector::on_buttonBox_rejected()
{
    done (0);
}
