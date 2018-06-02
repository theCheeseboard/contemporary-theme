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

SOURCES += \
    plugin.cpp \
    contemporarydefs.cpp \
    contemporarylegacy.cpp \
    contemporary2/contemporary.cpp

HEADERS +=\
        contemporary_global.h \
    plugin.h \
    contemporarylegacy.h \
    contemporary2/contemporary.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    plugin.json
