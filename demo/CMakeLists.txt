cmake_minimum_required(VERSION 3.4.0)

project(demo VERSION 1.0.0 LANGUAGES CXX)

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

add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(SOURCES
        mainwindow.cpp
        main.cpp
        mainwindow.ui)

set(HEADERS
        mainwindow.h)

add_executable(contemporary-demo ${SOURCES} ${HEADERS})
target_include_directories(contemporary-demo PUBLIC ../theme/)

target_link_libraries(contemporary-demo Qt6::Widgets libcontemporary contemporary-widgets)
