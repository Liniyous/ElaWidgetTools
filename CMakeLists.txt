cmake_minimum_required(VERSION 3.14)

project(ElaFramework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(QT_SDK_DIR "D:/QtFramework/Environment/Qt/5.15.2/msvc2019_64" CACHE PATH "Qt SDK directory")
if (QT_SDK_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${QT_SDK_DIR}")
endif ()
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (QT_VERSION VERSION_GREATER 6.7.0 OR QT_VERSION VERSION_LESS 5.12.0)
    message(FATAL_ERROR "当前 Qt 版本不在维护范围内 推荐 Qt 5.15.2 或 Qt 6.6.2 / 6.6.3")
endif ()
if (QT_VERSION VERSION_GREATER_EQUAL 6.5.3 AND QT_VERSION VERSION_LESS_EQUAL 6.6.1)
    message(FATAL_ERROR "当前 Qt 版本存在已知兼容问题 推荐 Qt 5.15.2 或 Qt 6.6.2 / 6.6.3")
endif ()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Install/${QT_VERSION}" CACHE PATH "Installation path" FORCE)

if (UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../ElaWidgetTools/lib")
endif ()

option(ELAWIDGETTOOLS_BUILD_EXAMPLE "Build the widget gallery" ON)

add_subdirectory(ElaWidgetTools)
if (ELAWIDGETTOOLS_BUILD_EXAMPLE)
    add_subdirectory(ElaWidgetToolsExample)
endif ()
