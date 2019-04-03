#ifndef CPAUSECONDITIONDIALOG_H
#define CPAUSECONDITIONDIALOG_H

#include <QDialog>

namespace Ui {
class CPauseConditionDialog;
}

class CPauseConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPauseConditionDialog(QWidget *parent = nullptr);
    ~CPauseConditionDialog();
    void SetTime(ulong time);
    ulong GetTime();

    const QString GetDesc() const;
    void SetDesc(const QString desc);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CPauseConditionDialog *ui;
};

#endif // CPAUSECONDITIONDIALOG_H
