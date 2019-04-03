#include "main.h"
#include "globals.h"



QList<CEventStarter *> g_StartedEvents;

CEventStarter::CEventStarter (CEvent *p, QObject *pPar) :QThread (pPar)
{
    m_pEvent = p;
    m_pTimer = nullptr;
}
CEventStarter::~CEventStarter()
{
    quit();
}
void CEventStarter::quit()
{
    m_pEvent->Terminate();
    delete  m_pTimer;
    m_pTimer = nullptr;
    QThread::quit();
}

bool CEventStarter::IsActive() const
{
    return m_pEvent->GetActive();
}
int CEventStarter::GetWaitTime()
{
    return m_pEvent->GetWaitTime() > global::g_DevTimeout ?
                                  m_pEvent->GetWaitTime() :
                                  global::g_DevTimeout;
}
void CEventStarter::run()
{
    m_pTimer = new QTimer;
    m_pTimer->setInterval(0);
    m_pTimer->setSingleShot(true);
    connect(m_pTimer,&QTimer::timeout,m_pEvent,&CEvent::RunEvent);
    m_pTimer->start();
    exec();
}

void App::Reload()
{
        global::LoadSettings();
        global::g_pLogger->SetMaxLines(global::gSettings[SET_LOG_MAX].toInt());
        m_bActive = global::gSettings[SET_EVENTS_ACTIVE].toBool();
        global::g_DevTimeout = global::gSettings[SET_DEVICE_RQ].toInt();

        //Must not reload devices and events.
}
void App::ClearDevicesAndEvents()
{
    CEventStarter *p;
    while (global::g_pKnownDevices->count() > 0)
        delete global::g_pKnownDevices->takeAt(0);

    while (g_StartedEvents.count() > 0)
    {
        p = g_StartedEvents.at(0);
        if (p->isRunning())
            p->quit();
        if (!p->wait(p->GetWaitTime() * 1000))
            p->terminate(); // Kill ~9
        delete g_StartedEvents.takeAt(0);
    }

    while (global::g_Events.count() > 0)
        delete global::g_Events.takeAt(0);
}

void App::ReloadDevicesAndEvents()
{


    global::LoadDevicesFromDisk();
    global::LoadEventsFromDisk();

    for (int i = global::g_Events.count() -1; i>=0; i--)
        g_StartedEvents.append(new CEventStarter(global::g_Events.at(i),this));


    QString msg ("Event loop: Loaded Devices: ");
    msg.append(QString::number(global::g_pKnownDevices->count()));
    msg.append(" Loaded Events: ");
    msg.append(QString::number(global::g_Events.count()));
    global::g_pLogger->Message(msg);
}
void App::StartEvents()
{
    for (int i = g_StartedEvents.count() -1; i >=0; i--)
        if (g_StartedEvents.at(i)->IsActive())
            g_StartedEvents.at(i)->start();

    m_bActive = true;
}

void App::StopEvents()
{

    for (int i = g_StartedEvents.count() -1; i >=0; i--)
        if (g_StartedEvents.at(i)->IsActive())
            g_StartedEvents.at(i)->quit();


    m_bActive = false;
}

void App::timer ()
{


    global::g_pLogger = new CLogger();

    Reload();

    QString expath(global::g_WorkDir);
    expath.append("/");
    expath.append(LOG_FILENAME);
    global::g_pLogger->SetFile(expath);

    global::g_pLogger->Message("Event loop: initialized");

    global::RegisterEvents();
    global::RegisterConditions();
    global::RegisterLocators();

    ReloadDevicesAndEvents();

    global::g_pLogger->Message(QString("Event loop: Event execution is currently ").append(m_bActive ? "enabled" : "disabled"));
    global::g_pLogger->Save();

    QObject::connect(&m_Server,&QLocalServer::newConnection,this,&App::newconnection);
    m_Server.listen(IPC_NAME);

    if (m_bActive)
        StartEvents();
}
void App::newconnection()
{
    if (m_pSocket)
    {
        disconnect(m_pSocket,&QLocalSocket::readyRead,this,&App::CheckCommand);
        delete m_pSocket;
    }

    m_pSocket = m_Server.nextPendingConnection();
    connect(m_pSocket,&QLocalSocket::readyRead,this,&App::CheckCommand);
}
void App::CheckCommand()
{
    if (!m_pSocket)
        return;

    QByteArray cmd =m_pSocket->readAll();
    QByteArray ret;
    QString msg;
    bool bPrev;

    switch (cmd[0])
    {
        case global::Ping :
            ret.append((char) global::Pong);
            m_pSocket->write(ret);
            return;

        case global::SaveLog :
            global::g_pLogger->Save();
            return;

        case global::SetLoopState :
            if (cmd.length() < 2)
                return;

            if (cmd.at(1) == 1)
            {
                if (!m_bActive)
                    StartEvents();
            }
            else
                StopEvents();

            global::g_pLogger->Message(QString("Event loop: SmartRC GUI set Event execution to ").append(m_bActive ? "enabled" : "disabled"));
            return ;

        case global::GetLoopState :
            ret.append((char) m_bActive ? 1 : 0);
            m_pSocket->write(ret);
            return;

        case global::Reload :
            global::g_pLogger->Message(QString("Event loop: SmartRC GUI requested reload"));
            bPrev = m_bActive; //Save preiuos state
            StopEvents();
            ClearDevicesAndEvents();
            Reload();
            ReloadDevicesAndEvents();
            if (bPrev)
                StartEvents();
            return ;

        case global::ClearLog :
            global::g_pLogger->Clear();
            global::g_pLogger->Save();
            return;
        case global::WriteToLog :
            cmd.remove(0,1);
            msg = QString::fromLocal8Bit(cmd);
            msg.prepend("GUI: ");
            global::g_pLogger->Message(msg);
            return;
        case global::ExecEvent :
            return;

        case global::Quit :
            StopEvents();
            quit();
            global::g_pLogger->Message("Event loop: GUI requested exit");
            return;

    }
}

void App::Exitting()
{
    m_bExitRequested = true;
    global::g_pLogger->Message("Event loop: exitting by user request...");
}

App::~App()
{
    delete m_pSocket;
    m_pSocket = nullptr;
    global::ClearExit();

}

int main(int argc, char *argv[])
{
    App a(argc, argv);

    int ret = a.exec();

    global::g_pLogger->Save();

    return ret;
}
