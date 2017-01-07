#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T16:16:08
#
#-------------------------------------------------

QT       += widgets thelib
CONFIG   += c++11

TARGET = Contemporary
TEMPLATE = lib

DEFINES += CONTEMPORARY_LIBRARY

SOURCES += contemporary.cpp \
    plugin.cpp \
    contemporarydefs.cpp

HEADERS += contemporary.h\
        contemporary_global.h \
    plugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    plugin.json
