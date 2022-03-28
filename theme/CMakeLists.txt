cmake_minimum_required(VERSION 3.4.0)

project(theme VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Svg Network Multimedia)
find_package(libcontemporary REQUIRED)
find_package(PkgConfig REQUIRED)

#find_package(ECM REQUIRED NO_MODULE)
#set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
#include(KDEInstallDirs)

add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(SOURCES
        contemporary2/animations/animation.cpp
        contemporary2/animations/animationengine.cpp
        contemporary2/animations/checkboxanimation.cpp
        contemporary2/animations/menuitemanimation.cpp
        contemporary2/animations/pushbuttonanimation.cpp
        contemporary2/animations/radiobuttonanimation.cpp
        contemporary2/animations/scrollbaranimation.cpp
        contemporary2/animations/viewitemanimation.cpp
        contemporary2/contemporary.cpp
        focusDecorations/focusdecoration.cpp
        focusDecorations/focusdecorationcontroller.cpp
        plugin.cpp
        contemporarydefs.cpp
        contemporarylegacy.cpp
        widgetHandlers/abstractitemviewhandler.cpp
        widgetHandlers/abstractscrollareahandler.cpp
        widgetHandlers/abstractwidgethandler.cpp
        widgetHandlers/scrollbarhandler.cpp)

set(HEADERS
        contemporary2/animations/animation.h
        contemporary2/animations/animationengine.h
        contemporary2/animations/checkboxanimation.h
        contemporary2/animations/menuitemanimation.h
        contemporary2/animations/pushbuttonanimation.h
        contemporary2/animations/radiobuttonanimation.h
        contemporary2/animations/scrollbaranimation.h
        contemporary2/animations/viewitemanimation.h
        contemporary2/contemporary.h
        contemporary_global.h
        focusDecorations/focusdecoration.h
        focusDecorations/focusdecorationcontroller.h
        plugin.h
        contemporarylegacy.h
        widgetHandlers/abstractitemviewhandler.h
        widgetHandlers/abstractscrollareahandler.h
        widgetHandlers/abstractwidgethandler.h
        widgetHandlers/scrollbarhandler.h)

add_library(contemporary-widgets SHARED ${SOURCES} ${HEADERS})

target_link_libraries(contemporary-widgets Qt6::Widgets Qt6::DBus Qt6::Svg libcontemporary)
