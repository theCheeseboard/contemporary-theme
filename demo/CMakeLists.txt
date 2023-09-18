cmake_minimum_required(VERSION 3.4.0)

project(demo VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg Network Multimedia)
find_package(libcontemporary REQUIRED)

set(SOURCES
        mainwindow.cpp
        main.cpp
        mainwindow.ui)

set(HEADERS
        mainwindow.h)

add_executable(contemporary-demo ${SOURCES} ${HEADERS})
cntp_init(contemporary-demo 20)
target_include_directories(contemporary-demo PUBLIC ../theme/)

target_link_libraries(contemporary-demo Qt6::Widgets libcontemporary contemporary-widgets)
