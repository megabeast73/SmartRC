#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "caddevent.h"
#include "switch.h"
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class CEvent;

class CDevice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetEventsState(int value) {m_sbtnEventsState->setValue(value);}
    int GetEventsState() { return m_sbtnEventsState->value();}

protected:
    void ReloadDevicesList();
    void ClearDeviceList();
    CDevice* GetSelectedDev();
    void ReloadEventList();
    void ClearEventList();
    CEvent* GetSelectedEvent();
    void hideEvent(QHideEvent *event);

    void GetSettings();
    void SetSettings();

    void SetupDeviceFilter();
    void SetupEventFilter();


private slots:
    void on_btnAddEvent_clicked();
    void on_sbtnvalueChanged(bool newvalue);
    void on_LabelActiveClicked();

    void on_btnDiscover_clicked();
    void on_btnSave_clicked();

    void PollDevices();

    void on_cmdUpdateNow_clicked();


    void on_btnDelDevice_clicked();

    void on_btnDeviceProp_clicked();

    void on_spnTimeOut_valueChanged(int arg1);

    void on_btnSaveEvents_clicked();

    void on_btnEdit_clicked();

    void on_btnDelEvent_clicked();

    void on_btnLog_clicked();

    void on_spnPollTime_valueChanged(const QString &arg1);

    void on_btnPing_clicked();

    void on_btnReload_clicked();

    void on_btnQuit_clicked();

    void on_cmbDeviceFilter_currentIndexChanged(const QString &arg1);

    void on_cmbEventFilter_currentIndexChanged(int index);

    void on_txtEventFilter_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    ClickableLabel *lbl;
    ClickableLabel *lblSaveOnExit;
    SwitchButton *m_sbtnEventsState;
    SwitchButton *m_sbtnSave;
    QTimer m_timer;
    QTimer m_IPCTimer;
};

#endif // MAINWINDOW_H
