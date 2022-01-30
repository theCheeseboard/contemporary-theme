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
    contemporary2/animations/animation.cpp \
    contemporary2/animations/animationengine.cpp \
    contemporary2/animations/menuitemanimation.cpp \
    contemporary2/animations/pushbuttonanimation.cpp \
    contemporary2/animations/radiobuttonanimation.cpp \
    contemporary2/animations/viewitemanimation.cpp \
    contemporary2/contemporary.cpp \
    focusDecorations/focusdecoration.cpp \
    focusDecorations/focusdecorationcontroller.cpp \
    plugin.cpp \
    contemporarydefs.cpp \
    contemporarylegacy.cpp \
    widgetHandlers/abstractitemviewhandler.cpp \
    widgetHandlers/abstractscrollareahandler.cpp \
    widgetHandlers/abstractwidgethandler.cpp

HEADERS +=\
    contemporary2/animations/animation.h \
    contemporary2/animations/animationengine.h \
    contemporary2/animations/menuitemanimation.h \
    contemporary2/animations/pushbuttonanimation.h \
    contemporary2/animations/radiobuttonanimation.h \
    contemporary2/animations/viewitemanimation.h \
    contemporary2/contemporary.h \
        contemporary_global.h \
    focusDecorations/focusdecoration.h \
    focusDecorations/focusdecorationcontroller.h \
    plugin.h \
    contemporarylegacy.h \
    widgetHandlers/abstractitemviewhandler.h \
    widgetHandlers/abstractscrollareahandler.h \
    widgetHandlers/abstractwidgethandler.h

win32 {
    # Include the-libs build tools
    include(C:/Program Files/thelibs/pri/varset.pri)

    INCLUDEPATH += "C:/Program Files/thelibs/include"
    LIBS += -L"C:/Program Files/thelibs/lib" -lthe-libs
}

macx {
    # Include the-libs build tools
    include(/usr/local/share/the-libs/pri/varset.pri)

    INCLUDEPATH += "/usr/local/include/the-libs"
    LIBS += -L/usr/local/lib -lthe-libs
}

unix:!macx:!android {
    # Include the-libs build tools
    equals(THELIBS_BUILDTOOLS_PATH, "") {
        THELIBS_BUILDTOOLS_PATH = $$[QT_INSTALL_PREFIX]/share/the-libs/pri
    }
    include($$THELIBS_BUILDTOOLS_PATH/varset.pri)

    QT += thelib
}

target.path = $$THELIBS_INSTALL_QT_PLUGINS/styles/

android {
    INCLUDEPATH += "/opt/thesuite-android/include/the-libs"
    LIBS += -L/opt/thesuite-android/libs/armeabi-v7a -lthe-libs

    target.path = /libs/armeabi-v7a/plugins/styles/
}

INSTALLS += target

DISTFILES += \
    plugin.json
