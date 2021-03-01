QT += gui network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    converter/chipsolver.cpp \
    converter/collisiondetect.cpp \
    converter/dealball.cpp \
    converter/dealrobot.cpp \
    converter/kalmanfilter.cpp \
    converter/kalmanfilterdir.cpp \
    converter/maintain.cpp \
    converter/montage.cpp \
    converter/visionmodule.cpp \
    share/geometry.cpp \
    share/globalsettings.cpp \
    share/matrix2d.cpp \
    share/parammanager.cpp \
    share/singleparams.cpp \
    share/treeitem.cpp \
    share/proto/cpp/game_event.pb.cc \
    share/proto/cpp/grSim_Commands.pb.cc \
    share/proto/cpp/grSim_Packet.pb.cc \
    share/proto/cpp/grSim_Replacement.pb.cc \
    share/proto/cpp/grSimMessage.pb.cc \
    share/proto/cpp/messages_robocup_ssl_detection.pb.cc \
    share/proto/cpp/messages_robocup_ssl_geometry.pb.cc \
    share/proto/cpp/messages_robocup_ssl_geometry_legacy.pb.cc \
    share/proto/cpp/messages_robocup_ssl_refbox_log.pb.cc \
    share/proto/cpp/messages_robocup_ssl_wrapper.pb.cc \
    share/proto/cpp/messages_robocup_ssl_wrapper_legacy.pb.cc \
    share/proto/cpp/referee.pb.cc \
    share/proto/cpp/vision_detection.pb.cc \
    share/proto/cpp/zss_cmd.pb.cc \
    share/proto/cpp/zss_debug.pb.cc \
    share/proto/cpp/zss_train.pb.cc

INCLUDEPATH += \
    $$PWD/../logreader \
    $$PWD/../logwriter \
    $$PWD/../netreceive \
    $$PWD/../netsend \
    $$PWD/converter \
    $$PWD/share \
    $$PWD/share/proto/cpp

LOGREADER_LIB = $$PWD/../bin/logreader.lib
LOGWRITER_LIB = $$PWD/../bin/logwriter.lib
NETSEND_LIB = $$PWD/../bin/netsend.lib
NETRECEIVE_LIB = $$PWD/../bin/netreceive.lib

LIBS += $$LOGREADER_LIB \
        $$LOGWRITER_LIB \
        $$NETSEND_LIB \
        $$NETRECEIVE_LIB

DESTDIR = $$PWD/../bin/

HEADERS += \
    converter/ballrecords.h \
    converter/chipsolver.h \
    converter/collisiondetect.h \
    converter/dealball.h \
    converter/dealrobot.h \
    converter/kalmanfilter.h \
    converter/kalmanfilterdir.h \
    converter/maintain.h \
    converter/messageformat.h \
    converter/montage.h \
    converter/visionmodule.h \
    share/dataqueue.hpp \
    share/geometry.h \
    share/globalsettings.h \
    share/json.hpp \
    share/matrix2d.h \
    share/parammanager.h \
    share/singleparams.h \
    share/singleton.hpp \
    share/staticparams.h \
    share/treeitem.h \
    share/proto/cpp/game_event.pb.h \
    share/proto/cpp/grSim_Commands.pb.h \
    share/proto/cpp/grSim_Packet.pb.h \
    share/proto/cpp/grSim_Replacement.pb.h \
    share/proto/cpp/grSimMessage.pb.h \
    share/proto/cpp/messages_robocup_ssl_detection.pb.h \
    share/proto/cpp/messages_robocup_ssl_geometry.pb.h \
    share/proto/cpp/messages_robocup_ssl_geometry_legacy.pb.h \
    share/proto/cpp/messages_robocup_ssl_refbox_log.pb.h \
    share/proto/cpp/messages_robocup_ssl_wrapper.pb.h \
    share/proto/cpp/messages_robocup_ssl_wrapper_legacy.pb.h \
    share/proto/cpp/referee.pb.h \
    share/proto/cpp/vision_detection.pb.h \
    share/proto/cpp/zss_cmd.pb.h \
    share/proto/cpp/zss_debug.pb.h \
    share/proto/cpp/zss_train.pb.h

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

LIBS += $$PROTOBUF_LIB \
        $$ZLIB_LIB

INCLUDEPATH += $$PROTOBUF_INCLUDE_DIR \
               $$ZLIB_INCLUDE_DIR \
               $$EIGEN_INCLUDE_DIR
