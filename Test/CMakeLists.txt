cmake_minimum_required(VERSION 3.16)

project(Tests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Quick Qml Positioning QuickWidgets Test #[[Core]])
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Quick Qml Positioning QuickWidgets Test #[[Core]])

set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(CMAKE_AUTOUIC_SEARCH_PATHS ../)  #path to .ui file

set(TEST_SOURCES
    tst_tests.cpp
    ../mainwindow.cpp
    ../mainwindow.h
    ../mainwindow.ui
)

add_subdirectory($ENV{GOOGLE_TEST} ${CMAKE_BINARY_DIR}/googletest_build)

add_test(NAME Tests COMMAND Tests)

add_executable(Tests ${TEST_SOURCES}

    ../Maps.qrc
    ../mainwindow.ui
    mocks/MockGeolocationProvider.h
    mocks/MockDataStorage.h)

target_link_libraries(Tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt6::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Test
    gtest
    gmock
    ws2_32)
