#ifndef CEVENTSELECTOR_H
#define CEVENTSELECTOR_H

#include <QDialog>
class CEvent;
namespace Ui {
class CEventSelector;
}

class CEventSelector : public QDialog
{
    Q_OBJECT

public:
    explicit CEventSelector(QWidget *parent = nullptr);
    ~CEventSelector();
    CEvent * GetSelectedEvent();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CEventSelector *ui;
};

#endif // CEVENTSELECTOR_H
