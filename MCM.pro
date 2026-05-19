#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T13:32:22
# Initiated for SDN-P15B-2nd System on 2018-05-02T14:52:42
#-------------------------------------------------

QT       += core xml
QT       += network

QT       -= gui

TARGET = MCM
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lssh


SOURCES += main.cpp \
    threadudpsock.cpp \
    mcm_receiver.cpp \
    data_controller.cpp \
    mcm_data.cpp \
    mdm_data_contrl.cpp \
    xml_read_write.cpp \
    mcast_sender.cpp \
    dadm_sender.cpp \
    file_sender.cpp \
    health_sender.cpp \
    log_file.cpp \
    print_msg.cpp \
    mcm_controller.cpp

HEADERS += \
    threadudpsock.h \
    mcm_receiver.h \
    mcm_struct.h \
    mcm_message_id.h \
    mcm_constant.h \
    data_controller.h \
    mcm_data.h \
    mdm_data_contrl.h \
    xml_read_write.h \
    mcast_sender.h \
    dadm_sender.h \
    file_sender.h \
    health_sender.h \
    log_file.h \
    print_msg.h \
    mcm_controller.h
