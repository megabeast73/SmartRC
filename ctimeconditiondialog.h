#ifndef CTIMECONDITIONDIALOG_H
#define CTIMECONDITIONDIALOG_H
#include <QDialog>
#include "ctimecondition.h"

namespace Ui {
class CTimeConditionDialog;
}

class CTimeConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTimeConditionDialog(QWidget *parent = nullptr);
    ~CTimeConditionDialog();
    void SetCondition(CTimeCondition *p);

protected:
    Ui::CTimeConditionDialog *ui;
    CTimeCondition * m_pCondition;

    void LoadValues();
    void GetValues();

private slots:
    void on_chkOnDate_stateChanged(int arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


};

#endif // CTIMECONDITIONDIALOG_H
