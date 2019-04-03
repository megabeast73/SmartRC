#ifndef CREPEATTIMECONDITIONDIALOG_H
#define CREPEATTIMECONDITIONDIALOG_H

#include <QDialog>

class CRepeatTimeCondition;

namespace Ui {
class CRepeatTimeConditionDialog;
}

class CRepeatTimeConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRepeatTimeConditionDialog(QWidget *parent = nullptr);
    ~CRepeatTimeConditionDialog();
    void SetCondition(CRepeatTimeCondition *pCond);

    void SetUi();
    void GetUi();

protected:
    CRepeatTimeCondition *m_pCondition;

private slots:
    void on_chkOnTime_stateChanged(int arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CRepeatTimeConditionDialog *ui;
};

#endif // CREPEATTIMECONDITIONDIALOG_H
