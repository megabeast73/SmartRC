#ifndef GLOBALS_H
#define GLOBALS_H
#include <qcoreapplication.h>
#include "cdevice.h"
#include "ccondition.h"
#include "cevent.h"
#include "clogger.h"


#include <qthread.h>
#include <qlocalsocket.h>

#ifndef NO_GUI
#include "mainwindow.h"
#include "clogview.h"
#endif

#define LOG_FILENAME "smartrc-LOG.log"

#define SET_FILENAME "smartrc.settings"
#define SET_EVENTS_ACTIVE "ev_act"
#define SET_DEVICE_POL "dev_pol"
#define SET_DEVICE_RQ   "dev_rq"
#define SET_LOG_MAX "log_max"
#define SET_AUTOSAVE "auto_save"
#define SET_EVENT_FILTER_TYPE "event_filter_type"
#define SET_EVENT_FILTER_NAME "event_filter_name"
#define SET_DEVICE_FILTER  "device_filter"

#define IPC_NAME "SmartRC_IPC"


namespace global {

enum IPCCommands
{
    Ping,
    Pong,
    SaveLog,
    ClearLog,
    SetLoopState,
    GetLoopState,
    Reload,
    ExecEvent,
    WriteToLog,
    Quit,
};
extern QLocalSocket g_IPC;
extern CDeviceList *g_pKnownDevices;
extern QList<CDeviceLocator *> g_Locators;
extern int g_DevTimeout;
extern ConditionsList g_RegisteredConditions;
extern CEventList g_Events;
extern CLogger * g_pLogger;

#ifndef NO_GUI
extern CLogView * g_pLogWindow;
extern MainWindow * g_pMainWindow;
#endif

extern CEventList g_RegisteredEvents;


extern QJsonObject gSettings;
extern QString g_WorkDir;

extern void RegisterLocators();
extern void RegisterEvents();
extern void RegisterConditions();
extern void LoadDevicesFromDisk();
extern void LoadEventsFromDisk();
extern void ClearExit();
extern void SaveSettings();
extern void LoadSettings();
extern QString GetActiveMac();
extern QString GetActiveMac();
extern void ClearIPC();
extern void ExecIPCCommand(IPCCommands c,QByteArray *data = nullptr);

}



#endif // GLOBALS_H
