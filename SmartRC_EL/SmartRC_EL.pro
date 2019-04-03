QT -= gui
QT += network

INCLUDEPATH += ..\gree\gree

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += NO_GUI
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    ../gree/Gree/cbeforetimecondition.cpp \
    ../gree/Gree/cbroadlinkrm.cpp \
    ../gree/Gree/ccondition.cpp \
    ../gree/Gree/cdevice.cpp \
    ../gree/Gree/cevent.cpp \
    ../gree/Gree/cgreeacdevice.cpp \
    ../gree/Gree/cgreeaconeparam.cpp \
    ../gree/Gree/clogger.cpp \
    ../gree/Gree/corevent.cpp \
    ../gree/Gree/cpausecondition.cpp \
    ../gree/Gree/crypto.cpp \
    ../gree/Gree/ctimecondition.cpp \
    ../gree/Gree/globals.cpp \
    ../gree/Gree/crepeattimecondition.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    main.h \
    ../gree/Gree/cbeforetimecondition.h \
    ../gree/Gree/cbroadlinkrm.h \
    ../gree/Gree/ccondition.h \
    ../gree/Gree/cdevice.h \
    ../gree/Gree/cevent.h \
    ../gree/Gree/cgreeacdevice.h \
    ../gree/Gree/cgreeaconeparam.h \
    ../gree/Gree/clogger.h \
    ../gree/Gree/corevent.h \
    ../gree/Gree/cpausecondition.h \
    ../gree/Gree/crypto.h \
    ../gree/Gree/ctimecondition.h \
    ../gree/Gree/globals.h \
    ../gree/Gree/crepeattimecondition.h


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
