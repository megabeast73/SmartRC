#include "cgreeacproperties.h"
#include "ui_cgreeacproperties.h"
#include "cgreeacdevice.h"
#include "globals.h"


CGreeACProperties::CGreeACProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGreeACProperties)
{
    ui->setupUi(this);

    m_pPlasma = new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->lyPlasma->addWidget(m_pPlasma);
//    m_pPlasma->mousePressEvent(nullptr);

    m_pLights = new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->lyLights->addWidget(m_pLights);
//    m_pLights->mousePressEvent(nullptr);

    m_pPower = new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->lyPower->addWidget(m_pPower);
//    m_pPower->mousePressEvent(nullptr);

    m_pTurbo =  new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->lyQuiet->addWidget(m_pTurbo);
    connect(m_pTurbo , &SwitchButton::Switched, this, &CGreeACProperties::onTurboChanged );
//    m_pQuiet->mousePressEvent(nullptr);

    m_pEnergy =  new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->lyEnergy->addWidget(m_pEnergy);

    m_p8Heat =  new SwitchButton(nullptr,SwitchButton::ONOFF);
    ui->ly8Heat->addWidget(m_p8Heat);

    setFixedSize(width(), height());

    m_FanModes.push_back(QString("Auto"));
    m_FanModes.push_back(QString("Low"));
    m_FanModes.push_back(QString("Med-low"));
    m_FanModes.push_back(QString("Med"));
    m_FanModes.push_back(QString("Med-High"));
    m_FanModes.push_back(QString("High"));

     m_Modes.push_back(QString("Auto"));
    m_Modes.push_back(QString("Cool"));
    m_Modes.push_back(QString("Dry"));
    m_Modes.push_back(QString("Fan"));
    m_Modes.push_back(QString("Heat"));
    ui->cmbMode->addItems(m_Modes);
    ui->cmbMode->setItemData(1, QBrush(Qt::blue), Qt::TextColorRole);
    ui->cmbMode->setItemData(2, QBrush(QColor(255,140,0)), Qt::TextColorRole);
    ui->cmbMode->setItemData(4, QBrush(Qt::red), Qt::TextColorRole);
                                                        //Swing Code
    m_BlowDirection.push_back(QString("Default(mid)")); //0
    m_BlowDirection.push_back(QString("Full Swing")); // 1
    m_BlowDirection.push_back(QString("Up")); //2
    m_BlowDirection.push_back(QString("Middle-Up")); //3
    m_BlowDirection.push_back(QString("Middle")); //4
    m_BlowDirection.push_back(QString("Mid-Down")); //5
    m_BlowDirection.push_back(QString("Down")); //6
    m_BlowDirection.push_back(QString("Swing-Down")); //7
    m_BlowDirection.push_back(QString("Swing-Mid-Down")); //8 Not supported by all devices
    m_BlowDirection.push_back(QString("Swing-Mid")); //9
    m_BlowDirection.push_back(QString("Swing-Mid-Up")); //10 Not supported by all devices
    m_BlowDirection.push_back(QString("Swing-Up")); //11

    ui->cmbVDirection->addItems(m_BlowDirection);

    QPalette p;
    p.setColor(QPalette::WindowText,Qt::red);
    ui->lcdTemp->setPalette(p);

    ui->lcdTemp->display(16);
}
 void CGreeACProperties::SetDevice(CGreeACDevice * pDevice)
 {
    m_pDevice = pDevice;
    if (m_pDevice->GetDeviceValue(QString("Pow")).toInt(-1) <0) // No data from device
            if (m_pDevice->PollDevice(global::g_DevTimeout))
                m_pDevice->GetDeviceData(global::g_DevTimeout); // Try to load it
    SetUIValues();
}
void CGreeACProperties::GetUIValues()
{
    m_pDevice->SetDeviceValue(QString("Pow"),m_pPower->value());
    m_pDevice->SetDeviceValue(QString("Mod"),ui->cmbMode->currentIndex());
    m_pDevice->SetDeviceValue(QString("SetTem"),ui->sldTemp->value());
    m_pDevice->SetDeviceValue(QString("WdSpd"),ui->sldFan->value());
    m_pDevice->SetDeviceValue(QString("Lig"),m_pLights->value());
    m_pDevice->SetDeviceValue(QString("Health"),m_pPlasma->value());
    m_pDevice->SetDeviceValue(QString("SwUpDn"),ui->cmbVDirection->currentIndex());
    m_pDevice->SetDeviceValue(QString("Tur"),m_pTurbo->value());
    m_pDevice->SetDeviceValue(QString("SvSt"),m_pEnergy->value());
    m_pDevice->SetDeviceValue(QString("StHt"),m_p8Heat->value());
    m_pDevice->SetDeviceName(ui->txtDeviceName->text());

}
void CGreeACProperties::SetUIValues()
{
    ui->txtDeviceName->setText(m_pDevice->GetDeviceName());
    ui->lblDevId->setText(m_pDevice->GetID());
    if (m_pDevice->GetDeviceKey().isEmpty())
    {
        ui->lblStatus->setText(QString("NO CONNECTION"));
        ui->lblStatus->setStyleSheet("QLabel {color : black; }");
    }
    else {
        ui->lblStatus->setText(QString("ON-LINE"));
        ui->lblStatus->setStyleSheet("QLabel {color : green; }");
    }
    m_pPower->setValue(m_pDevice->GetDeviceValue(QString("Pow")).toInt(0));
    ui->cmbMode->setCurrentIndex(m_pDevice->GetDeviceValue(QString("Mod")).toInt(0));
    on_cmbMode_activated(ui->cmbMode->currentIndex());
    ui->sldTemp->setValue(m_pDevice->GetDeviceValue(QString("SetTem")).toInt(16) );
    ui->sldFan->setValue(m_pDevice->GetDeviceValue(QString("WdSpd")).toInt(0));
    m_pLights->setValue(m_pDevice->GetDeviceValue(QString("Lig")).toInt(0));
    m_pPlasma->setValue(m_pDevice->GetDeviceValue(QString("Health")).toInt(0));
    ui->cmbVDirection->setCurrentIndex(m_pDevice->GetDeviceValue(QString("SwUpDn")).toInt(0));
    m_pTurbo->setValue(m_pDevice->GetDeviceValue(QString("Tur")).toInt(0));
    m_pEnergy->setValue(m_pDevice->GetDeviceValue(QString("SvSt")).toInt(0));
    m_p8Heat->setValue(m_pDevice->GetDeviceValue(QString("StHt")).toInt(0));


//    m_pPower->mousePressEvent(nullptr);
//    m_pPower->mousePressEvent(nullptr);

//    m_pLights->mousePressEvent(nullptr);
//    m_pLights->mousePressEvent(nullptr);

//    m_pQuiet->mousePressEvent(nullptr);
//    m_pQuiet->mousePressEvent(nullptr);

}
CGreeACProperties::~CGreeACProperties()
{
    disconnect(m_pTurbo , &SwitchButton::Switched, this, &CGreeACProperties::onTurboChanged );
    delete m_pLights;
    delete m_pPlasma;
    delete m_pPower;
    delete m_pTurbo;
    delete m_pEnergy;
    delete m_p8Heat;

    delete ui;
}

void CGreeACProperties::on_sldTemp_valueChanged(int value)
{
    ui->lcdTemp->display(value);
}

void CGreeACProperties::on_sldFan_valueChanged(int value)
{
    ui->lblFan->setText(m_FanModes[value]);
}

void CGreeACProperties::on_btnClose_clicked()
{
    close();
}

void CGreeACProperties::on_btnLoad_clicked()
{
    if (!m_pDevice)
        return;
    if (m_pDevice->GetDeviceKey().isEmpty()) //Device not bound
    {
        ui->lblStatus->setText(QString("Polling..."));
        m_pDevice->PollDevice(global::g_DevTimeout);
    }
    if (m_pDevice->GetDeviceKey().isEmpty()) //fail to connect
    {
        ui->lblStatus->setText(QString("OFF-LINE"));
        ui->lblStatus->setStyleSheet("QLabel {color : red; }");
        return;
    }
    ui->lblStatus->setText(QString("CONNECTED"));
    ui->lblStatus->setStyleSheet("QLabel {color : green; }");

    if (!m_pDevice->GetDeviceData(global::g_DevTimeout))
    {
        ui->lblStatus->setText(QString("CONNECTION FAILED"));
        ui->lblStatus->setStyleSheet("QLabel {color : red; }");
        return;
    }
    SetUIValues();
}

void CGreeACProperties::on_btnSet_clicked()
{
    GetUIValues();
    if (m_pDevice->SetDeviceData(global::g_DevTimeout))
    {
        ui->lblStatus->setText(QString("DATA SET"));
        ui->lblStatus->setStyleSheet("QLabel {color : green; }");
    }
    else {
        ui->lblStatus->setText(QString("CONNECTION FAILED"));
        ui->lblStatus->setStyleSheet("QLabel {color : red; }");
    }
}
void CGreeACProperties::onTurboChanged()
{
    ui->sldFan->setEnabled(!m_pTurbo->value());
}

void CGreeACProperties::on_cmbMode_activated(int index)
{
    QPalette p;
/*
 * 0: auto
1: cool
2: dry
3: fan
4: heat
*/
   // m_pTurbo->setEnabled(!(index == 2 || index == 3));
    ui->sldTemp->setEnabled(index != 3);
    switch (index) {
        case 1:
            p.setColor(QPalette::WindowText,Qt::blue);
            break;
        case 2:
            p.setColor(QPalette::WindowText,QColor(255,140,0)); //orange
            break;
        case 3:
            p.setColor(QPalette::WindowText,Qt::black);
            break;
        case 4:
            p.setColor(QPalette::WindowText,Qt::red);
            break;
    default: //0
             p.setColor(QPalette::WindowText,Qt::yellow);
}

    ui->sldTemp->setEnabled(index != 3);
    ui->lcdTemp->setPalette(p);
    m_pEnergy->setEnabled(index <3);
}

void CGreeACProperties::on_btnSave_clicked()
{
    GetUIValues();
}
