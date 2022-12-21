cmake_minimum_required(VERSION 3.20.0)

project(theme VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Svg Network Multimedia)
find_package(libcontemporary REQUIRED)

cntp_define_build_dirs()

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
cntp_init(contemporary-widgets 17)
set_target_properties(contemporary-widgets PROPERTIES
        PREFIX ""
        OUTPUT_NAME "contemporary")

target_compile_definitions(contemporary-widgets PRIVATE CONTEMPORARY_LIBRARY)

target_link_libraries(contemporary-widgets Qt6::Widgets Qt6::DBus Qt6::Svg libcontemporary)

message("${CNTP_INSTALL_PLUGINS}")
install(TARGETS contemporary-widgets
        LIBRARY DESTINATION ${CNTP_INSTALL_PLUGINS}/styles/
        RUNTIME DESTINATION ${CNTP_INSTALL_PLUGINS}/styles/)