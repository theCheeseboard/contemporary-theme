cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

project(contemporary-theme VERSION 1.0.0 LANGUAGES CXX)

find_package(QT REQUIRED NAMES Qt6)

add_subdirectory(theme)
add_subdirectory(demo)
