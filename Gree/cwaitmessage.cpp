#include "cwaitmessage.h"
#include "ui_cwaitmessage.h"
#include "globals.h"

CWaitMessage::CWaitMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CWaitMessage)
{
    ui->setupUi(this);
}
//CWaitMessage::CWaitMessage(QWidget *parent,const QString msg) :
//    QDialog(parent),
//    ui(new Ui::CWaitMessage)
//{

//    m_msg = msg;
//}

void CWaitMessage::open ()
{
    m_EventsState = global::g_pMainWindow->GetEventsState();
    if (m_EventsState == 0)
        return;
    QDialog::open();
    ui->label->repaint();
    QCoreApplication::processEvents(QEventLoop::AllEvents,500);
    m_EventsState = global::g_pMainWindow->GetEventsState();
    global::g_pMainWindow->SetEventsState(0);

    QThread::sleep(global::g_DevTimeout);


}

CWaitMessage::~CWaitMessage()
{
    global::g_pMainWindow->SetEventsState(m_EventsState);
    delete ui;
}
