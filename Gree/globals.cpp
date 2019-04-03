#include "globals.h"

#include <qnetworkinterface.h>
#include <QDir>
#include "cgreeacdevice.h"
#include "cgreeaconeparam.h"
#include "ccondition.h"
#include "ctimecondition.h"
#include "cpausecondition.h"
#include "cevent.h"
#include "corevent.h"
#include "cbroadlinkrm.h"
#include "cbeforetimecondition.h"
#include "crepeattimecondition.h"

#ifndef NO_GUI
#include "mainwindow.h"
#endif

// Issues:
/*
 * Renaming event doesnt rename the file. Next load, duplicates the event
 * Event name must not contain special chars, like ":"
 * Need to implement device functionality to override default connection
 * Execute event from Event Properties: Executing is inside the GUI.
 * Eventloop does not handle Execute Event command.
 *
*/
namespace global
{
CDeviceList * g_pKnownDevices = nullptr;
CEventList g_RegisteredEvents;
ConditionsList  g_RegisteredConditions;

QList<CDeviceLocator *> g_Locators;

int g_DevTimeout = 5;

CEventList g_Events;
CLogger * g_pLogger  = nullptr;



QLocalSocket g_IPC;

#ifndef NO_GUI
MainWindow * g_pMainWindow = nullptr;
CLogView * g_pLogWindow = nullptr;
#endif

QJsonObject gSettings;

#ifdef NO_GUI
QString g_WorkDir = QDir::currentPath();
#else
QString g_WorkDir;
#endif

void RegisterLocators()
{
    //Register Locators.
    global::g_Locators.append(new CGreeACLocator);
    global::g_Locators.append(new CGreeACOneParamLocator);
    global::g_Locators.append(new CBroadLinkRMLocator);


}
void RegisterEvents()
{
    //Register events
    global::g_RegisteredEvents.append(new CEvent);
    global::g_RegisteredEvents.append(new CorEvent);
}

void RegisterConditions()
{
    // Register conditions
    global::g_RegisteredConditions.append(new CTimeCondition);
    global::g_RegisteredConditions.append(new CPauseCondition);
    global::g_RegisteredConditions.append(new CBeforeTimeCondition);
    global::g_RegisteredConditions.append(new CRepeatTimeCondition);
}

void LoadDevicesFromDisk()
{
    QString expath(global::g_WorkDir);
    global::g_pKnownDevices = new CDeviceList;

    for (int i = global::g_Locators.count() -1; i >= 0; i--)
        global::g_Locators.value(i)->LoadFromDir(expath,global::g_pKnownDevices);

}
void LoadEventsFromDisk()
{
    QString expath(global::g_WorkDir);
    global::g_Events.LoadFromDir(expath);
}

void ClearExit()
{
    delete  global::g_pKnownDevices;

    while(global::g_Locators.count())
        delete global::g_Locators.takeAt(0);

    while (global::g_RegisteredConditions.count())
        delete global::g_RegisteredConditions.takeAt(0);

    while (global::g_RegisteredEvents.count())
        delete global::g_RegisteredEvents.takeAt(0);
    delete global::g_pLogger;
    global::g_pLogger = nullptr;
}
void SaveSettings()
{
    QString expath(global::g_WorkDir);
    expath.append("/");
    expath.append(SET_FILENAME);
    QJsonDocument savedoc(gSettings);
    QFile savefile(expath);

    if (!savefile.open(QIODevice::WriteOnly))
        qWarning("Couldn't open file to save settings.");
    else
        savefile.write(savedoc.toBinaryData());
}
void LoadSettings()
{
    QString expath(global::g_WorkDir);
    expath.append("/");
    expath.append(SET_FILENAME);
    QFile loadFile(expath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open settings file.");
        return;
    }
    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromBinaryData(saveData));
    gSettings = loadDoc.object();
}
QString GetActiveMac()
{
    QNetworkInterface intf;
    QNetworkAddressEntry addr;
    QString ret("00:00:00:00:00:00");

    for (int i = QNetworkInterface::allInterfaces().count() -1; i >=0; i--)
    {
        intf = QNetworkInterface::allInterfaces().at(i);
        if (intf.flags().testFlag(QNetworkInterface::IsUp) && !intf.flags().testFlag(QNetworkInterface::IsLoopBack))
            if (intf.hardwareAddress() != "00:00:00:00:00:00" && !intf.humanReadableName().contains("VM"))
                for (int j = intf.addressEntries().count() -1; j >=0; j--)
                {
                    addr = intf.addressEntries().at(j);
                    if ( addr.ip().toString().contains(".") )
                        return intf.hardwareAddress();
                }

    }
    return ret;
}

void ClearIPC()
{
    QByteArray b(g_IPC.readAll());
}
void ExecIPCCommand(IPCCommands c,QByteArray *data)
{
    QByteArray cmd;
    cmd.append((char) c);
    if (data)
        cmd.append(*data);
    g_IPC.write(cmd.constData(),cmd.length());

}
};
