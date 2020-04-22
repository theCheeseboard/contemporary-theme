#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T16:16:08
#
#-------------------------------------------------

QT       += widgets
CONFIG   += c++14 plugin

TARGET = Contemporary
TEMPLATE = lib

DEFINES += CONTEMPORARY_LIBRARY

SOURCES += \
    plugin.cpp \
    contemporarydefs.cpp \
    contemporarylegacy.cpp

HEADERS +=\
        contemporary_global.h \
    plugin.h \
    contemporarylegacy.h

target.path = $$[QT_INSTALL_PLUGINS]/styles/

win32 {
    INCLUDEPATH += "C:/Program Files/thelibs/include"
    LIBS += -L"C:/Program Files/thelibs/lib" -lthe-libs
}

macx {
    INCLUDEPATH += "/usr/local/include/the-libs"
    LIBS += -L/usr/local/lib -lthe-libs
}

unix:!macx:!android {
    QT += thelib
}

android {
    INCLUDEPATH += "/opt/thesuite-android/include/the-libs"
    LIBS += -L/opt/thesuite-android/libs/armeabi-v7a -lthe-libs

    target.path = /libs/armeabi-v7a/plugins/styles/
}

INSTALLS += target

DISTFILES += \
    plugin.json
