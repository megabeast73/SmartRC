#ifndef CWAITMESSAGE_H
#define CWAITMESSAGE_H

#include <QDialog>
#include <qstring.h>

namespace Ui {
class CWaitMessage;
}

class CWaitMessage : public QDialog
{
    Q_OBJECT

public:
    explicit CWaitMessage(QWidget *parent = nullptr);
//    explicit CWaitMessage(QWidget *parent = nullptr,const QString msg);
    virtual void open ();
    ~CWaitMessage();


private:
    Ui::CWaitMessage *ui;
//    QString m_msg;
    int m_EventsState;
};

#endif // CWAITMESSAGE_H
