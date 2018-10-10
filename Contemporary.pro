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
    contemporary2/contemporary.cpp \
    contemporary2/animations/animationengine.cpp \
    contemporary2/animations/animation.cpp \
    contemporary2/animations/pushbuttonanimation.cpp

HEADERS +=\
        contemporary_global.h \
    plugin.h \
    contemporarylegacy.h \
    contemporary2/contemporary.h \
    contemporary2/animations/animationengine.h \
    contemporary2/animations/animation.h \
    contemporary2/animations/pushbuttonanimation.h

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/styles/
    INSTALLS += target
}

DISTFILES += \
    plugin.json
