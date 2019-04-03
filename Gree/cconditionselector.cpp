#include "cconditionselector.h"
#include "ui_cconditionselector.h"
#include "globals.h"
#include <QVariant>

CConditionSelector::CConditionSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CConditionSelector)
{


    ui->setupUi(this);
    setFixedSize(width(), height());
    m_pSelected = nullptr;
    QVariant id;
    for (int i = global::g_RegisteredConditions.count() -1; i>=0; i--)
    {
        id = QVariant(global::g_RegisteredConditions[i]->GetID());
        ui->cmbCond->addItem(global::g_RegisteredConditions[i]->GetName(),id);
    }
    setFixedSize(width(), height());


}

CConditionSelector::~CConditionSelector()
{
    delete ui;
}

void CConditionSelector::on_buttonBox_accepted()
{

    done(1);
}

void CConditionSelector::on_buttonBox_rejected()
{
    if (m_pSelected)
        delete m_pSelected;
    m_pSelected = nullptr;
    done (0);
}

CCondition * CConditionSelector::GetSelectedCondition()
{
    if ( m_pSelected)
        return m_pSelected;
    //locate condition id in the list of avialable conditions
    m_pSelected = global::g_RegisteredConditions.GetById(ui->cmbCond->currentData().toString());

    if (m_pSelected) //located, create new one of same type
        m_pSelected = m_pSelected->CreateNew();
    return m_pSelected;
}

void CConditionSelector::on_cmbCond_currentIndexChanged(int index)
{
    index++; //avoid some warnings
    if (m_pSelected)
        delete m_pSelected;
    m_pSelected = nullptr;
}

