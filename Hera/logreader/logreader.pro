#-------------------------------------------------
#
# Project created by QtCreator 2019-02-17T10:19:28
#
#-------------------------------------------------

QT       += quick

QT       -= gui

TARGET = logreader
TEMPLATE = lib

DEFINES += LOGREADER_LIBRARY

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
        log_slider.cpp \
    file_format_legacy.cpp \
    file_format_timestamp_type_size_raw_message.cpp \
    log_file.cpp \
    player.cpp \
    qtiocompressor.cpp \


HEADERS += \
        log_slider.h \
        logreader_global.h \ 
    file_format.h \
    file_format_legacy.h \
    file_format_timestamp_type_size_raw_message.h \
    log_file.h \
    message_type.h \
    player.h \
    qtiocompressor.h \


DESTDIR = $$PWD/../bin

# Third party library dir
win32 {
    THIRD_PARTY_DIR = $$PWD/../3rdParty
}
unix:!macx{
    THIRD_PARTY_DIR = /usr/local
}
macx {
    THIRD_PARTY_DIR = /usr/local/Cellar
}

win32 {
    PROTOBUF_INCLUDE_DIR = $${THIRD_PARTY_DIR}/protobuf/include
    ZLIB_INCLUDE_DIR = $${THIRD_PARTY_DIR}/zlib/include
    EIGEN_INCLUDE_DIR = $${THIRD_PARTY_DIR}/Eigen

    CONFIG(release,debug|release){
        PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/libprotobuf.lib
        ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/zlib.lib
    }
    CONFIG(debug,debug|release){
        PROTOBUF_LIB = $${THIRD_PARTY_DIR}/protobuf/lib/libprotobufD.lib
        ZLIB_LIB = $${THIRD_PARTY_DIR}/zlib/lib/zlibD.lib
    }
}
unix:!macx{
    PROTOBUF_INCLUDE_DIR = $${THIRD_PARTY_DIR}/include
    PROTOBUF_LIB = $${THIRD_PARTY_DIR}/lib/libprotobuf.so
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

LIBS += \
    $$ZLIB_LIB

INCLUDEPATH += \
    $$ZLIB_INCLUDE_DIR \


