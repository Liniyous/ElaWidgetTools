cmake_minimum_required(VERSION 3.5)

project(ElaWidgetTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(example)
add_subdirectory(src)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ElaWidgetTools)
endif()

if(MSVC)
    # Release 模式下的特殊处理
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")  # 使用动态运行时库
    
    # 确保导出宏定义在 Release 模式下也正确设置
    target_compile_definitions(elawidgettools
        PRIVATE 
        $<$<CONFIG:Release>:ELAWIDGETTOOLS_LIBRARY>
        $<$<CONFIG:Release>:ELAWIDGETTOOLS_SHARED>
    )
endif()
