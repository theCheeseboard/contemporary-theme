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
    focusDecorations/focusdecoration.cpp \
    focusDecorations/focusdecorationcontroller.cpp \
    plugin.cpp \
    contemporarydefs.cpp \
    contemporarylegacy.cpp \
    widgetHandlers/abstractitemviewhandler.cpp \
    widgetHandlers/abstractscrollareahandler.cpp \
    widgetHandlers/abstractwidgethandler.cpp

HEADERS +=\
        contemporary_global.h \
    focusDecorations/focusdecoration.h \
    focusDecorations/focusdecorationcontroller.h \
    plugin.h \
    contemporarylegacy.h \
    widgetHandlers/abstractitemviewhandler.h \
    widgetHandlers/abstractscrollareahandler.h \
    widgetHandlers/abstractwidgethandler.h

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
