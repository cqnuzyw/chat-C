TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    tcpsocket.c \
    udpsocket.c \
    chat.c

HEADERS += \
    tcpsocket.h \
    udpsocket.h \
    chat.h
