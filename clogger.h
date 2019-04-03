#ifndef CLOGGER_H
#define CLOGGER_H

#include <QObject>
#include <QString>
#include <qfile.h>

class CLogger : public QObject
{
    Q_OBJECT
public:
    explicit CLogger(QObject *parent = nullptr);
    virtual ~CLogger();
    void SetFile(QString fn);
    void SetMaxLines(int lines) { m_maxlines = lines;}
    void Save();
    int count() const { return  m_Messages.count();}
    const QString lineAt(int i) { return m_Messages.value(i);}
    void Clear() { m_Messages.clear(); }
    void ReloadFile();

    void Message(const QString msg);

    void Lock();
    void Unlock() { m_bLock = false; }
protected:
    QFile *m_pFile;
    QStringList m_Messages;
    int m_maxlines;
    int m_dup;
    bool m_bLock;
};

#endif // CLOGGER_H
