#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T15:58:49
#
#-------------------------------------------------

QT       -= gui

TARGET = QtOpc
TEMPLATE = lib

#INCLUDEPATH = c:\WinDDK\7600.16385.1\inc\atl71 c:\WinDDK\7600.16385.1\inc\crt c:\WinDDK\7600.16385.1\inc\api
INCLUDEPATH += "C:\Program Files\Windows Kits\10\Include\10.0.10150.0\ucrt" "C:\Program Files\Microsoft SDKs\Windows\v7.1A\Include"
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1A\Lib" -L"C:\Program Files\Windows Kits\10\Lib\10.0.10150.0\ucrt\x86"
DEFINES += QTOPC_LIBRARY
DEFINES += NOMINMAX WIN32_LEAN_AND_MEAN _USING_V110_SDK71_

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS",5.01"

SOURCES += \
    opcvalue.cpp \
    opcreader.cpp \
    opcdatareceiver.cpp \
    opccontroller.cpp \
    opc/Transaction.cpp \
    opc/stdafx.cpp \
    opc/OPCServer.cpp \
    opc/OPCProperties.cpp \
    opc/OPCItemData.cpp \
    opc/OPCItem.cpp \
    opc/OPCHost.cpp \
    opc/OPCGroup.cpp \
    opc/OpcEnum_i.c \
    opc/opcda_i.c \
    opc/opccomn_i.c \
    opc/OPCClient.cpp \
    opcdaservermodel.cpp \
    opctagmodel.cpp

HEADERS +=\
        qtopc_global.h \
    opcreader.h \
    opcdatareceiver.h \
    opccontroller.h \
    opcvalue.h \
    opc/Transaction.h \
    opc/stdafx.h \
    opc/OPCServer.h \
    opc/OPCProperties.h \
    opc/OPCItemData.h \
    opc/OPCItem.h \
    opc/OPCHost.h \
    opc/OPCGroup.h \
    opc/OpcEnum.h \
    opc/opcda.h \
    opc/opccomn.h \
    opc/OPCClientToolKitDLL.h \
    opc/OPCClient.h \
    opcdaservermodel.h \
    opctagmodel.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
