#ifndef MAIN_H
#define MAIN_H



#include <QCoreApplication>
#include <qobject.h>
#include <qlocalserver.h>
#include <qthread.h>
#include <QTimer>

class QLocalSocket;
class CEvent;

class App : public QCoreApplication
{
    Q_OBJECT
public:
    App(int &argc, char **argv) : QCoreApplication(argc,argv), m_pSocket(nullptr) {}
    virtual ~App();

    void Reload();
    void ReloadDevicesAndEvents();
    void ClearDevicesAndEvents();
    void StartEvents();
    void StopEvents();


    int exec()
    {
        QObject::connect(this,&QCoreApplication::aboutToQuit,this,&App::Exitting);

        t.setSingleShot(true);
        t.setInterval(0);
        QObject::connect(&t,&QTimer::timeout,this,&App::timer);
        t.start();
        return QCoreApplication::exec();
    }

public slots:

    void CheckCommand();
    void timer ();
    void newconnection();

   void Exitting();

protected:

    QTimer t;
    bool m_bActive;
    bool m_bExitRequested;
    QLocalSocket *m_pSocket;
    QLocalServer m_Server;

};

class CEventStarter : public QThread
{
         Q_OBJECT
public:
    CEventStarter (CEvent *p, QObject *pPar);
    virtual ~CEventStarter();
    void quit();
    bool IsActive() const;
    int GetWaitTime();
protected:
    void run() override;



protected:
    CEvent * m_pEvent;
    QTimer * m_pTimer;
};

#endif // MAIN_H
