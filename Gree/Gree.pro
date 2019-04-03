#-------------------------------------------------
#
# Project created by QtCreator 2018-11-25T17:28:08
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gree
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    caddevent.cpp \
    switch.cpp \
    qclicablelabel.cpp \
    cevent.cpp \
    cdevice.cpp \
    crypto.cpp \
    cgreeacdevice.cpp \
    cdiscoverdevices.cpp \
    globals.cpp \
    cgreeacproperties.cpp \
    ccondition.cpp \
    ctimecondition.cpp \
    ctimeconditiondialog.cpp \
    cconditionselector.cpp \
    cpausecondition.cpp \
    cpauseconditiondialog.cpp \
    clogger.cpp \
    clogview.cpp \
    cbeforetimecondition.cpp \
    cbeforetimeconditiondialog.cpp \
    ceventselector.cpp \
    corevent.cpp \
    cgreeaconeparam.cpp \
    cgreeaconeparamdialog.cpp \
    cwaitmessage.cpp \
    cbroadlinkrm.cpp \
    crepeattimecondition.cpp \
    crepeattimeconditiondialog.cpp

HEADERS += \
        mainwindow.h \
    caddevent.h \
    switch.h \
    qclicablelabel.h \
    cevent.h \
    cdevice.h \
    crypto.h \
    cgreeacdevice.h \
    cdiscoverdevices.h \
    globals.h \
    cgreeacproperties.h \
    ccondition.h \
    ctimecondition.h \
    ctimeconditiondialog.h \
    cconditionselector.h \
    cpausecondition.h \
    cpauseconditiondialog.h \
    clogger.h \
    clogview.h \
    cbeforetimecondition.h \
    cbeforetimeconditiondialog.h \
    ceventselector.h \
    corevent.h \
    cgreeaconeparam.h \
    cgreeaconeparamdialog.h \
    cwaitmessage.h \
    cbroadlinkrm.h \
    crepeattimecondition.h \
    crepeattimeconditiondialog.h

FORMS += \
        mainwindow.ui \
    caddevent.ui \
    cdiscoverdevices.ui \
    cgreeacproperties.ui \
    ctimeconditiondialog.ui \
    cconditionselector.ui \
    cpauseconditiondialog.ui \
    clogview.ui \
    ceventselector.ui \
    cwaitmessage.ui \
    crepeattimeconditiondialog.ui
#QMAKE_CXXFLAGS_DEBUG += /MT #
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/release/ -lcryptlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/debug/ -lcryptlib
else:unix: LIBS += -L$$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/ -lcryptlib

INCLUDEPATH += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp
DEPENDPATH += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/release/libcryptlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/debug/libcryptlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/release/cryptlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/debug/cryptlib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../Users/vasko/Source/Repos/gree-remote/3rdparty/cryptopp/x64/Output/libcryptlib.a
