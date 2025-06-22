cmake_minimum_required(VERSION 3.5)

project(QtInfluxV2App LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

add_executable(QtInfluxV2App
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    SensorReader.cpp
    SensorReader.h
)

target_link_libraries(QtInfluxV2App Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
