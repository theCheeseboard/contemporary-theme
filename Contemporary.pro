#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T16:16:08
#
#-------------------------------------------------

QT       += widgets thelib
CONFIG   += c++14 plugin

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

win32 {
    QT += thelib
    INCLUDEPATH += "C:/Program Files/thelibs/include"
    LIBS += -L"C:/Program Files/thelibs/lib" -lthe-libs
}

macx {
    INCLUDEPATH += "/usr/local/include/the-libs"
    LIBS += -L/usr/local/lib -lthe-libs
}

target.path = $$[QT_INSTALL_PLUGINS]/styles/
INSTALLS += target

DISTFILES += \
    plugin.json
