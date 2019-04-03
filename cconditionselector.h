#ifndef CCONDITIONSELECTOR_H
#define CCONDITIONSELECTOR_H

#include <QDialog>
#include "ccondition.h"

namespace Ui {
class CConditionSelector;
}

class CConditionSelector : public QDialog
{
    Q_OBJECT

public:
    explicit CConditionSelector(QWidget *parent = nullptr);
    ~CConditionSelector();
    CCondition * GetSelectedCondition();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_cmbCond_currentIndexChanged(int index);

private:
    Ui::CConditionSelector *ui;
    //QString m_SelectedID;
    CCondition *m_pSelected;
};

#endif // CCONDITIONSELECTOR_H
