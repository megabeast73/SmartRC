#ifndef CDISCOVERDEVICES_H
#define CDISCOVERDEVICES_H

#include <QDialog>
#include "cgreeacdevice.h"

namespace Ui {
class CDiscoverDevices;
}

class CDiscoverDevices : public QDialog
{
    Q_OBJECT

public:
    explicit CDiscoverDevices(QWidget *parent = nullptr);
    virtual ~CDiscoverDevices();
    void SetTimeOut(int value);
    void ClearTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_btnAddSelect_clicked();

private:
    Ui::CDiscoverDevices *ui;
    CDeviceList m_dev;
};

#endif // CDISCOVERDEVICES_H
