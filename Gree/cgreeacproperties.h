#ifndef CGREEACPROPERTIES_H
#define CGREEACPROPERTIES_H

#include <QDialog>
#include <qlist.h>
#include "switch.h"


namespace Ui {
class CGreeACProperties;
}
class CGreeACDevice;

class CGreeACProperties : public QDialog
{
    Q_OBJECT

public:
    explicit CGreeACProperties(QWidget *parent = nullptr);
    virtual void SetDevice(CGreeACDevice * pDevice);
    ~CGreeACProperties();
protected:

    SwitchButton *m_pPlasma;
    SwitchButton *m_pLights;
    SwitchButton *m_pPower;
    SwitchButton *m_pTurbo;
    SwitchButton *m_pEnergy;
    SwitchButton *m_p8Heat;

    CGreeACDevice *m_pDevice;
    QStringList m_FanModes;
    QStringList m_Modes;
    QStringList m_BlowDirection;

    Ui::CGreeACProperties *ui;

    virtual void SetUIValues();
    virtual void GetUIValues();

private slots:
    void on_sldTemp_valueChanged(int value);
    void on_sldFan_valueChanged(int value);
    void on_btnClose_clicked();
    void on_btnLoad_clicked();
    void on_btnSet_clicked();
    void on_cmbMode_activated(int index);
    void onTurboChanged();

    void on_btnSave_clicked();

private:


};

#endif // CGREEACPROPERTIES_H
