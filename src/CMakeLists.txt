cmake_minimum_required(VERSION 3.5)

project(ELAWIDGETTOOLS VERSION 2.0.0 LANGUAGES CXX)

set(EXPORT_NAME "ElaWidgetTools")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "Installation path" FORCE)
add_definitions(-DELAWIDGETTOOLS_LIBRARY)
option(ELAWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

FILE(GLOB ORIGIN *.h *.cpp)
FILE(GLOB INCLUDE include/*.h)
FILE(GLOB PRIVATE private/*.h private/*.cpp)
FILE(GLOB DEVELOPER DeveloperComponents/*.h DeveloperComponents/*.cpp)

source_group(include FILES ${INCLUDE})
source_group(private FILES ${PRIVATE})
source_group(DeveloperComponents FILES ${DEVELOPER})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${INCLUDE}
    ${PRIVATE}
    ${DEVELOPER}
)

if (ELAWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED"
        DeveloperComponents/ElaKeyBinderContainer.cpp
        DeveloperComponents/ElaKeyBinderContainer.h)
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${EXPORT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
else ()
    add_library(${EXPORT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
endif ()

FILE(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.qrc
)
target_include_directories(${EXPORT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DeveloperComponents>
    $<INSTALL_INTERFACE:include>
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js *.ttc)
    foreach (filepath ${RES_PATHS})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        list(APPEND resource_files ${filename})
    endforeach (filepath)
    qt_add_resources(${EXPORT_NAME} "ElaWidgetTools"
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )
endif ()

if (MINGW)
    set_target_properties(${EXPORT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    set_target_properties(${EXPORT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif ()

if (WIN32)
    target_link_libraries(${EXPORT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Svg
        D3D11
        DXGI
    )
else ()
    target_link_libraries(${EXPORT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Svg
    )
endif ()


install(
    TARGETS ${EXPORT_NAME}
    EXPORT ${EXPORT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${EXPORT_HEADERS} DESTINATION include)
install(TARGETS ${EXPORT_NAME}
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)

set(INCLUDE_DIRS include)
set(LIBRARIES ${EXPORT_NAME})
set(LIB_DIR lib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${EXPORT_NAME}ConfigVersion.cmake
    VERSION 2.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/${EXPORT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${EXPORT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake
    PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

install(
    FILES ${PROJECT_BINARY_DIR}/${EXPORT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${EXPORT_NAME}ConfigVersion.cmake DESTINATION lib/cmake
)
