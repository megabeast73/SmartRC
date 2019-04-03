#ifndef CLOGVIEW_H
#define CLOGVIEW_H

#include <QDialog>

namespace Ui {
class CLogView;
}

class CLogView : public QDialog
{
    Q_OBJECT

public:
    explicit CLogView(QWidget *parent = nullptr);
    ~CLogView();

    void RefreshLog();

private slots:
    void on_btnRefresh_clicked();

    void on_btnClear_clicked();

protected:
    virtual void showEvent(QShowEvent *event);

private:
    Ui::CLogView *ui;

};

#endif // CLOGVIEW_H
