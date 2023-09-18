cmake_minimum_required(VERSION 3.20.0)

project(theme VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Svg Network Multimedia OPTIONAL_COMPONENTS DBus)
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
        contemporarythemeplugin.cpp
        contemporarydefs.cpp
        contemporarylegacy.cpp
        widgetHandlers/abstractitemviewhandler.cpp
        widgetHandlers/abstractscrollareahandler.cpp
        widgetHandlers/abstractwidgethandler.cpp
        widgetHandlers/scrollbarhandler.cpp
        contemporary2/widgets/polishengine.cpp
    )

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
        contemporarythemeplugin.h
        contemporarylegacy.h
        widgetHandlers/abstractitemviewhandler.h
        widgetHandlers/abstractscrollareahandler.h
        widgetHandlers/abstractwidgethandler.h
        widgetHandlers/scrollbarhandler.h
        contemporary2/widgets/polishengine.h
    )

add_library(contemporary-widgets SHARED ${SOURCES} ${HEADERS})
cntp_init(contemporary-widgets 20)
set_target_properties(contemporary-widgets PROPERTIES
        PREFIX ""
        OUTPUT_NAME "contemporary")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Android")
set_target_properties(contemporary-widgets PROPERTIES
        PREFIX "libplugins_styles_"
        SUFFIX "_${CMAKE_ANDROID_ARCH_ABI}")
ENDIF()

target_compile_definitions(contemporary-widgets PRIVATE CONTEMPORARY_LIBRARY)

target_link_libraries(contemporary-widgets Qt::Widgets Qt::Svg libcontemporary)
if(TARGET Qt::DBus)
    target_link_libraries(contemporary-widgets Qt::DBus)
endif()

message("${CNTP_INSTALL_PLUGINS}")
install(TARGETS contemporary-widgets
        LIBRARY DESTINATION ${CNTP_INSTALL_PLUGINS}/styles/
        RUNTIME DESTINATION ${CNTP_INSTALL_PLUGINS}/styles/)
