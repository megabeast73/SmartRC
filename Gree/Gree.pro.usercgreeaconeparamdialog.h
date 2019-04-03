#ifndef CGREEACONEPARAMDIALOG_H
#define CGREEACONEPARAMDIALOG_H

#include <QObject>
#include <QWidget>
#include <qcombobox.h>
#include "cgreeacproperties.h"

class CGreeACOneParamDialog : public CGreeACProperties
{
    Q_OBJECT
public:
    CGreeACOneParamDialog();
    explicit CGreeACOneParamDialog(QWidget *parent = nullptr);
    virtual ~CGreeACOneParamDialog();

protected:
    virtual void SetupUI();
    virtual void SetUIValues();
    virtual void GetUIValues();
    virtual void DisableAllControls();

    QComboBox * m_pCmbParam;

    public slots:
    void ParamIndexChanged(int index);
};

#endif // CGREEACONEPARAMDIALOG_H
