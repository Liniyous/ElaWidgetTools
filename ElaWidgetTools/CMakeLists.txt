cmake_minimum_required(VERSION 3.12)

project(ElaWidgetTools VERSION 2.0.0 LANGUAGES CXX)

add_definitions(-DELAWIDGETTOOLS_LIBRARY)
option(ELAWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." OFF)

FILE(GLOB Origin CONFIGURE_DEPENDS *.h *.cpp)
FILE(GLOB private CONFIGURE_DEPENDS private/*.h private/*.cpp)
FILE(GLOB Development CONFIGURE_DEPENDS Development/*.h Development/*.cpp)
FILE(GLOB Development_Command CONFIGURE_DEPENDS Development/Command/*.h Development/Command/*.cpp)

source_group(private FILES ${private})
source_group(Development FILES ${Development})
source_group(Development/Command FILES ${Development_Command})

set(PROJECT_SOURCES
    ${Origin}
    ${private}
    ${Development}
    ${Development_Command}
    ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools.qrc
)

if (ELAWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED")
endif ()

add_library(${PROJECT_NAME} ${LIB_TYPE}
    ${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME WidgetTools)
add_library(Ela::WidgetTools ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Development>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Development/Command>
    $<INSTALL_INTERFACE:${PROJECT_NAME}/include>
)

if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WidgetsPrivate
)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        D3D11
        DXGI
    )
endif ()


install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
    LIBRARY DESTINATION ${PROJECT_NAME}/lib
    RUNTIME DESTINATION ${PROJECT_NAME}/bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${PROJECT_NAME}/include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "Font" EXCLUDE
    PATTERN "Image" EXCLUDE
    PATTERN "private" EXCLUDE
    PATTERN "Development" EXCLUDE
)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Ela::
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${PROJECT_NAME}/lib/cmake
)

include(CMakePackageConfigHelpers)
install(FILES Image/Fluent/LICENSE.txt Image/Fluent/SOURCE.txt DESTINATION ${PROJECT_NAME}/licenses/FluentSystemIcons)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 2.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
)

install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION ${PROJECT_NAME}/lib/cmake
)
