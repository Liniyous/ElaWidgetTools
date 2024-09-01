cmake_minimum_required(VERSION 3.5)

project(elawidgettools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DELAWIDGETTOOLS_LIBRARY)
option(ELAWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)

file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/DeveloperComponents
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME} 
        ${ELAWIDGETTOOLS_BUILD_STATIC_LIB} STATIC
        ${sources_files}
    )
else()
    add_library(${PROJECT_NAME} 
        ${ELAWIDGETTOOLS_BUILD_STATIC_LIB} STATIC
        ${sources_files}
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
    foreach(filepath ${RES_PATHS})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        list(APPEND resource_files ${filename})
    endforeach(filepath)
    qt_add_resources(${PROJECT_NAME} "ElaWidgetTools"
        PREFIX "/"
        FILES
        ${resource_files}
    )
endif()

if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        D3D11
        DXGI
    )
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
    )
endif()

message(${CMAKE_INSTALL_PREFIX})
