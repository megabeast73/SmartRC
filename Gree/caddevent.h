#ifndef CADDEVENT_H
#define CADDEVENT_H

#include <QDialog>
#include <QAbstractButton>
#include "cevent.h"

namespace Ui {
class CAddEvent;
}

class CAddEvent : public QDialog
{
    Q_OBJECT

public:
    explicit CAddEvent(QWidget *parent = nullptr);
    ~CAddEvent();

    CEvent * GetEvent() {return m_pEvent;}
    void SetEvent(CEvent * pEvent);
    void ClearCondList();
    void ReloadCondList();
    void LoadUI();
    void SaveUI();
    void SetHint(const QString hint);

protected:
    CEvent * m_pEvent;

    QString GetSelectedCondID();

private slots:
    void on_grpRemote_clicked();

    void on_btnSetDevice_clicked();

    void on_btnAddCond_clicked();

    void on_btnCondProp_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_btnDelCond_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnExec_clicked();

    void on_btnTest_clicked();

private:
    Ui::CAddEvent *ui;
};

#endif // CADDEVENT_H
