QT += network
QT += quick qml
QT += gui
CONFIG += c++17 console
CONFIG -= app_bundle
#CONFIG(debug, debug | release) : DESTDIR = $$PWD/../newVisionModule/qmake/debug
#else : DESTDIR = $$PWD/../newVisionModule/qmake/release
#TARGET = $$PWD/../newVisionModule/bin

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Qt中屏蔽警告和调试信息
DEFINES += QT_NO_WARNING_OUTPUT\
                        QT_NO_DEBUG_OUTPUT

THIRD_PARTY_DIR = /usr/local
PROTOBUF_DIR = $${THIRD_PARTY_DIR}
EIGEN_INCLUDE_DIR = /usr/include/eigen3

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Model/ballstate.cpp \
        communication/actionmodule.cpp \
        communication/communicator.cpp \
        communication/radiopacket.cpp \
        main.cpp \
        interaction.cpp \
        networkinterfaces.cpp \
        share/geometry.cpp \
        share/parammanager.cpp \
        share/proto/cpp/grSimMessage.pb.cc \
        share/proto/cpp/grSim_Commands.pb.cc \
        share/proto/cpp/grSim_Packet.pb.cc \
        share/proto/cpp/grSim_Replacement.pb.cc \
        share/proto/cpp/log_labeler_data.pb.cc \
        share/proto/cpp/log_labels.pb.cc \
        share/proto/cpp/messages_robocup_ssl_detection.pb.cc \
        share/proto/cpp/messages_robocup_ssl_geometry.pb.cc \
        share/proto/cpp/messages_robocup_ssl_geometry_legacy.pb.cc \
        share/proto/cpp/messages_robocup_ssl_refbox_log.pb.cc \
        share/proto/cpp/messages_robocup_ssl_wrapper.pb.cc \
        share/proto/cpp/messages_robocup_ssl_wrapper_legacy.pb.cc \
        share/proto/cpp/ssl_game_controller_auto_ref.pb.cc \
        share/proto/cpp/ssl_game_controller_common.pb.cc \
        share/proto/cpp/ssl_game_controller_team.pb.cc \
        share/proto/cpp/ssl_game_event.pb.cc \
        share/proto/cpp/ssl_game_event_2019.pb.cc \
        share/proto/cpp/ssl_referee.pb.cc \
        share/proto/cpp/vision_detection.pb.cc \
        share/proto/cpp/zss_cmd.pb.cc \
        share/proto/cpp/zss_debug.pb.cc \
        share/proto/cpp/zss_rec.pb.cc \
        vision/cvision.cpp \
        vision/field.cpp \
        vision/kalmanfilter.cpp \
        vision/messageformat.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Model/ballstate.h \
    communication/actionmodule.h \
    communication/communicator.h \
    communication/radiopacket.h \
    demo.h \
    interaction.h \
    networkinterfaces.h \
    share/dataqueue.hpp \
    share/geometry.h \
    share/parammanager.h \
    share/proto/cpp/grSimMessage.pb.h \
    share/proto/cpp/grSim_Commands.pb.h \
    share/proto/cpp/grSim_Packet.pb.h \
    share/proto/cpp/grSim_Replacement.pb.h \
    share/proto/cpp/log_labeler_data.pb.h \
    share/proto/cpp/log_labels.pb.h \
    share/proto/cpp/messages_robocup_ssl_detection.pb.h \
    share/proto/cpp/messages_robocup_ssl_geometry.pb.h \
    share/proto/cpp/messages_robocup_ssl_geometry_legacy.pb.h \
    share/proto/cpp/messages_robocup_ssl_refbox_log.pb.h \
    share/proto/cpp/messages_robocup_ssl_wrapper.pb.h \
    share/proto/cpp/messages_robocup_ssl_wrapper_legacy.pb.h \
    share/proto/cpp/ssl_game_controller_auto_ref.pb.h \
    share/proto/cpp/ssl_game_controller_common.pb.h \
    share/proto/cpp/ssl_game_controller_team.pb.h \
    share/proto/cpp/ssl_game_event.pb.h \
    share/proto/cpp/ssl_game_event_2019.pb.h \
    share/proto/cpp/ssl_referee.pb.h \
    share/proto/cpp/vision_detection.pb.h \
    share/proto/cpp/zss_cmd.pb.h \
    share/proto/cpp/zss_debug.pb.h \
    share/proto/cpp/zss_rec.pb.h \
    share/singleton.hpp \
    share/staticparams.h \
    vision/cvision.h \
    vision/field.h \
    vision/kalmanfilter.h \
    vision/messageformat.h \
    zsplugin.hpp

INCLUDEPATH += \
    ./vision \
    ./communication \
    ./share \
    ./share/proto/cpp \
    $${PROTOBUF_DIR}/include \
    $$EIGEN_INCLUDE_DIR


LIBS += $${PROTOBUF_DIR}/lib/libprotobuf.so

FORMS +=

DISTFILES += \
    main.qml \
    share/proto/auto.sh \
    share/proto/grSimMessage.proto \
    share/proto/grSim_Commands.proto \
    share/proto/grSim_Packet.proto \
    share/proto/grSim_Replacement.proto \
    share/proto/log_labeler_data.proto \
    share/proto/log_labels.proto \
    share/proto/messages_robocup_ssl_detection.proto \
    share/proto/messages_robocup_ssl_geometry.proto \
    share/proto/messages_robocup_ssl_geometry_legacy.proto \
    share/proto/messages_robocup_ssl_refbox_log.proto \
    share/proto/messages_robocup_ssl_wrapper.proto \
    share/proto/messages_robocup_ssl_wrapper_legacy.proto \
    share/proto/ssl_game_controller_auto_ref.proto \
    share/proto/ssl_game_controller_common.proto \
    share/proto/ssl_game_controller_team.proto \
    share/proto/ssl_game_event.proto \
    share/proto/ssl_game_event_2019.proto \
    share/proto/ssl_referee.proto \
    share/proto/vision_detection.proto \
    share/proto/zss_cmd.proto \
    share/proto/zss_debug.proto \
    share/proto/zss_rec.proto
