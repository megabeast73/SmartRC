#include "clogger.h"
#include <qcoreapplication.h>
#include <qdatetime.h>
#include "globals.h"

CLogger::CLogger(QObject *parent) : QObject(parent)
{
    m_pFile = nullptr;
    m_maxlines = 100;
    m_dup =0;
    m_bLock = false;
}

CLogger::~CLogger()
{
    if (m_pFile)
        delete m_pFile;
}

void CLogger::Lock()
{
    while (m_bLock)
        QThread::usleep(100);
    m_bLock = true;
}
void CLogger::ReloadFile()
{
    QString fname(m_pFile->fileName());
    Clear();
    delete m_pFile;
    m_pFile = nullptr;
    SetFile(fname);

}
void CLogger::SetFile(QString fn)
{
    int cnt =0;

    char buff[1024] = {0};
    QString line;
    long szr =0;
    QString fname(fn);

    if (m_pFile)
    {
        if (m_pFile->fileName().compare(fname,Qt::CaseInsensitive) == 0)
            return; // same file passed
        Save(); //New filename. Save current work
        delete m_pFile;
    }
    m_pFile = new QFile(fname);
    if(m_pFile->exists())
    {
        m_pFile->open(QIODevice::ReadOnly);
        Lock();
        while((szr = m_pFile->readLine(reinterpret_cast<char *>(&buff),1024)) && szr >0)
        {
            line.clear();
            line = QString::fromUtf8(buff,szr);
//            line.append('\n');
            if (cnt  > m_maxlines)
                m_Messages.removeFirst();
            else
                cnt++;
            m_Messages.append(line);
        }
        Unlock();
    }
}

void CLogger::Save()
{
    if (!m_pFile)
        return;
//   char endl = '\n';
    m_pFile->close();
    m_pFile->open(QIODevice::WriteOnly);
    Lock();
    for (int i =0; i < m_Messages.count(); i++)
    {
        m_pFile->write(m_Messages.value(i).toUtf8());
    }
    Unlock();
    m_pFile->flush();
    m_pFile->close();

}

void CLogger::Message(const QString msg)
{

#ifdef NO_GUI

    Lock();
    QString message (msg);
    QString lastmsg;
    QDateTime dt = QDateTime::currentDateTime();
    QString dtstr = dt.toString(QStringView(QString("dddd,dd/MM/yy hh:mm:ss")));
    message.prepend(" -> ");
    message.prepend(dtstr);

    if (m_Messages.count() > 0)
    {
        lastmsg = m_Messages[0];
        if (lastmsg.indexOf(msg) > -1)
        { //Last message repeated
            m_dup++;
            m_Messages.removeAt(0);
            message.append(" (");
            message.append(QString::number(m_dup));
            message.append(")");
        }
        else
        {
            m_dup = 0;
            if (m_Messages.count () > m_maxlines)
                m_Messages.removeLast();
        }
    }
    message.append('\n');
    m_Messages.insert(0,message);
    Unlock();
#else
    QByteArray data(msg.toLocal8Bit());
    data.prepend((char) global::WriteToLog);
    global::g_IPC.write(data);
#endif
}

