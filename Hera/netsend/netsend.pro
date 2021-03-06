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

win32 {
    PROTOBUF_INCLUDE_DIR = $${THIRD_PARTY_DIR}/protobuf/include
    ZLIB_INCLUDE_DIR = $${THIRD_PARTY_DIR}/zlib/include
    EIGEN_INCLUDE_DIR = $${THIRD_PARTY_DIR}/Eigen

    contains(QMAKE_TARGET.arch, x86_64){
        message("64-bit")
        CONFIG(release,debug|release){
            PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/x64/libprotobuf.lib
            ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/x64/zlib.lib
        }
        CONFIG(debug,debug|release){
            PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/x64/libprotobufd.lib
            ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/x64/zlibD.lib
        }
    } else {
        message("32-bit")
        CONFIG(release,debug|release){
            PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/x86/libprotobuf.lib
            ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/x86/zlib.lib
        }
        CONFIG(debug,debug|release){
            PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/x86/libprotobufd.lib
            ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/x86/zlib.lib
        }
    }
}

unix:!macx{
    PROTOBUF_INCLUDE_DIR = $${PROTOBUF2_DIR}/include
    PROTOBUF_LIB = -L$${PROTOBUF2_DIR}/lib \
                -lprotobuf
    ZLIB_INCLUDE_DIR = $${THIRD_PARTY_DIR}/zlib/include
    ZLIB_LIB = -lz
    EIGEN_INCLUDE_DIR = /usr/include/eigen3
}

macx {
    PROTOBUF_INCLUDE_DIR = $${THIRD_PARTY_DIR}/protobuf/2.6.1/include
    PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/2.6.1/lib/libprotobuf.a
    ZLIB_INCLUDE_DIR = $${THIRD_PARTY_DIR}/zlib/include
    ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/zlib.a
    EIGEN_INCLUDE_DIR = $${THIRD_PARTY_DIR}/Eigen
}

LIBS += $$PROTOBUF_LIB \

INCLUDEPATH += $$PROTOBUF_INCLUDE_DIR \

DESTDIR = $$PWD/../bin
