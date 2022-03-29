cmake_minimum_required(VERSION 3.4.0)

project(demo VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Svg Network Multimedia)
find_package(libcontemporary REQUIRED)
find_package(PkgConfig REQUIRED)

set(SOURCES
        mainwindow.cpp
        main.cpp
        mainwindow.ui)

set(HEADERS
        mainwindow.h)

add_executable(contemporary-demo ${SOURCES} ${HEADERS})
cntp_init(contemporary-demo 17)
target_include_directories(contemporary-demo PUBLIC ../theme/)

target_link_libraries(contemporary-demo Qt6::Widgets libcontemporary contemporary-widgets)
