#include "clogview.h"
#include "ui_clogview.h"
#include "globals.h"

CLogView::CLogView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLogView)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

}

CLogView::~CLogView()
{
    delete ui;
}
void CLogView::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    RefreshLog();
}
void CLogView::RefreshLog()
{
    QString log;
    //latest messages at the top
    for (int i = 0; i < global::g_pLogger->count(); i++)
        log.append(global::g_pLogger->lineAt(i));
    ui->txtLog->setPlainText(log);
}

void CLogView::on_btnRefresh_clicked()
{
    global::ExecIPCCommand(global::SaveLog);

    global::g_pLogger->ReloadFile();
    RefreshLog();
}

void CLogView::on_btnClear_clicked()
{
    global::g_pLogger->Clear();
    global::ExecIPCCommand(global::ClearLog);
    RefreshLog();
}
