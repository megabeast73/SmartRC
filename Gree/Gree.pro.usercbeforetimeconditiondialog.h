#ifndef CBEFORETIMECONDITIONDIALOG_H
#define CBEFORETIMECONDITIONDIALOG_H

#include <QObject>
#include "ctimeconditiondialog.h"

class CBeforeTimeConditionDialog : public CTimeConditionDialog
{
    Q_OBJECT
public:
    CBeforeTimeConditionDialog();
    virtual ~CBeforeTimeConditionDialog() { }
};

#endif // CBEFORETIMECONDITIONDIALOG_H
