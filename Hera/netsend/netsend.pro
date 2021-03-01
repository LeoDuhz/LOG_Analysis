#-------------------------------------------------
#
# Project created by QtCreator 2019-02-17T09:49:43
#
#-------------------------------------------------

QT       += gui network

TARGET = netsend
TEMPLATE = lib

DEFINES += NETSEND_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        netsend.cpp \
    ../Hera/share/proto/cpp/vision_detection.pb.cc

HEADERS += \
        netsend.h \
        netsend_global.h \ 
    ../Hera/share/staticparams.h \
    ../Hera/share/proto/cpp/vision_detection.pb.h

PROTOBUF_INCLUDE_DIR = $$PWD/../3rdParty/protobuf/include
PROTOBUF_LIB = $$PWD/../3rdParty/protobuf/lib/x64/libprotobuf.lib

LIBS += $$PROTOBUF_LIB \

INCLUDEPATH += $$PROTOBUF_INCLUDE_DIR \

DESTDIR = $$PWD/../bin
