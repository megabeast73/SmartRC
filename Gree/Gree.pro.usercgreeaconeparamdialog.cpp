#include "cgreeaconeparamdialog.h"
#include "ui_cgreeacproperties.h"
#include "cgreeaconeparam.h"
#include <QJsonValue>

CGreeACOneParamDialog::CGreeACOneParamDialog() : CGreeACProperties ()
{
    SetupUI();

}
CGreeACOneParamDialog::CGreeACOneParamDialog(QWidget *parent) : CGreeACProperties(parent)
{
    SetupUI();
}

CGreeACOneParamDialog::~CGreeACOneParamDialog()
{
    delete m_pCmbParam;
}

void CGreeACOneParamDialog::SetUIValues()
{
    CGreeACProperties::SetUIValues();
    m_pCmbParam->setCurrentIndex(reinterpret_cast<CGreeACOneParam *>(m_pDevice)->GetSelIndex());
 //   ui->txtDeviceName->setText(reinterpret_cast<CGreeACOneParam *>(m_pDevice)->GetDevIDName());
    //DisableAllControls();

}

void CGreeACOneParamDialog::GetUIValues()
{
    CGreeACOneParam * pDev = reinterpret_cast<CGreeACOneParam *>(m_pDevice);
    pDev->SetSelIndex(m_pCmbParam->currentIndex());
        CGreeACProperties::GetUIValues();
        switch (m_pCmbParam->currentIndex())
        {
        case 0:
            pDev->SetValue(QJsonValue(m_pPower->value()));
            pDev->SetValueName("Pow");
            break;
        case 1:
            pDev->SetValueName("Mod");
            pDev->SetValue(QJsonValue(ui->cmbMode->currentIndex()));
            break;
        case 2:
            pDev->SetParam("SwUpDn",ui->cmbVDirection->currentIndex());
            break;
        case 3:
            pDev->SetParam("Lig",m_pLights->value());
            break;
        case 4:
            pDev->SetParam("Health",m_pPlasma->value());
            break;
        case 5:
            pDev->SetParam("SvSt",m_pEnergy->value());
            break;
        case 6:
            pDev->SetParam("StHt",m_p8Heat->value());
            break;
        case 7:
            pDev->SetParam("SetTem",ui->sldTemp->value());
            break;
        case 8:
            pDev->SetParam("WdSpd",ui->sldFan->value());
            break;
        case 9:
            pDev->SetParam("Tur",m_pTurbo->value());
            m_pTurbo->setEnabled(true);
            break;
        }

}
void CGreeACOneParamDialog::DisableAllControls()
{
    ui->cmbMode->setEnabled(false);
    ui->cmbVDirection->setEnabled(false);
    ui->sldFan->setEnabled(false);
    ui->sldTemp->setEnabled(false);
    m_pPlasma->setEnabled(false);
    m_pLights->setEnabled(false);
    m_pPower->setEnabled(false);
    m_pTurbo->setEnabled(false);
    m_pEnergy->setEnabled(false);
    m_p8Heat->setEnabled(false);
    switch (m_pCmbParam->currentIndex())
    {
    case 0:
        m_pPower->setEnabled(true);
        break;
    case 1:
        ui->cmbMode->setEnabled(true);
        break;
    case 2:
        ui->cmbVDirection->setEnabled(true);
        break;
    case 3:
        m_pLights->setEnabled(true);
        break;
    case 4:
        m_pPlasma->setEnabled(true);
        break;
    case 5:
        m_pEnergy->setEnabled(true);
        break;
    case 6:
        m_p8Heat->setEnabled(true);
        break;
    case 7:
        ui->sldTemp->setEnabled(true);
        break;
    case 8:
        ui->sldFan->setEnabled(true);
        break;
    case 9:
        m_pTurbo->setEnabled(true);
        break;
    }
}

void CGreeACOneParamDialog::SetupUI()
{
    ui->lblHint->setText("Select wich parameter to be set: ");
    ui->lblHint->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    ui->txtDeviceName->setEnabled(false);
    m_pCmbParam = new QComboBox(this);
    m_pCmbParam->move(ui->btnLoad->x(),ui->lblHint->y());
    m_pCmbParam->setFixedWidth(m_pCmbParam->width()+20);
    m_pCmbParam->addItem("Power");
    m_pCmbParam->addItem("Mode");
    m_pCmbParam->addItem("Vert.Direction");
    m_pCmbParam->addItem("Light");
    m_pCmbParam->addItem("Cold Plasma");
    m_pCmbParam->addItem("Energy saving");
    m_pCmbParam->addItem("8C Heating");
    m_pCmbParam->addItem("Temperature");
    m_pCmbParam->addItem("Fan Speed");
    m_pCmbParam->addItem("Turbo");
    connect(m_pCmbParam , SIGNAL(currentIndexChanged(int)), this, SLOT(ParamIndexChanged(int)));
    DisableAllControls();

}
void CGreeACOneParamDialog::ParamIndexChanged(int index)
{
    DisableAllControls();
}
