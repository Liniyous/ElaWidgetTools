cmake_minimum_required(VERSION 3.5)

project(ElaWidgetTools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DELAWIDGETTOOLS_BUILD_STATIC_LIB)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE CPP_FILES src/*.cpp src/*.h example/*.cpp example/*.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        ${sources_files}
)

include_directories(
    src
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ElaWidgetTools
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ElaWidgetTools APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ElaWidgetTools SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ElaWidgetTools
            ${PROJECT_SOURCES}
        )
    endif()
endif()
if (ELAWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else()
    set(LIB_TYPE "SHARED")
endif()
# qt_add_library(${PROJECT_NAME} ${LIB_TYPE}
#     ${sources_files}
# )

qt_add_resources(ElaWidgetTools
    RESOURCES PREFIX "/"
    FILES
    src/Font/ElaAwesome.ttf
    src/Image/Moon.jpg src/Image/Home_Background.png src/Image/github.png src/Image/noise.png src/Image/Cirno.jpg
    src/Image/control/Acrylic.png src/Image/control/AnimatedIcon.png src/Image/control/AnimatedVisualPlayer.png src/Image/control/AnimationInterop.png src/Image/control/AppBarButton.png src/Image/control/AppBarSeparator.png src/Image/control/AppBarToggleButton.png src/Image/control/AutomationProperties.png src/Image/control/AutoSuggestBox.png src/Image/control/Border.png src/Image/control/BreadcrumbBar.png src/Image/control/Button.png src/Image/control/CalendarDatePicker.png src/Image/control/CalendarView.png src/Image/control/Canvas.png src/Image/control/Checkbox.png src/Image/control/Clipboard.png src/Image/control/ColorPaletteResources.png src/Image/control/ColorPicker.png src/Image/control/ComboBox.png src/Image/control/CommandBar.png src/Image/control/CommandBarFlyout.png src/Image/control/CompactSizing.png src/Image/control/ConnectedAnimation.png src/Image/control/ContentDialog.png src/Image/control/CreateMultipleWindows.png src/Image/control/DataGrid.png src/Image/control/DatePicker.png src/Image/control/DropDownButton.png src/Image/control/EasingFunction.png src/Image/control/Expander.png src/Image/control/FilePicker.png src/Image/control/FlipView.png src/Image/control/Flyout.png src/Image/control/Grid.png src/Image/control/GridView.png src/Image/control/HyperlinkButton.png src/Image/control/IconElement.png src/Image/control/Image.png src/Image/control/ImplicitTransition.png src/Image/control/InfoBadge.png src/Image/control/InfoBar.png src/Image/control/InkCanvas.png src/Image/control/InkToolbar.png src/Image/control/InputValidation.png src/Image/control/ItemsRepeater.png src/Image/control/Line.png src/Image/control/ListBox.png src/Image/control/ListView.png src/Image/control/MediaPlayerElement.png src/Image/control/MenuBar.png src/Image/control/MenuFlyout.png src/Image/control/NavigationView.png src/Image/control/NumberBox.png src/Image/control/PageTransition.png src/Image/control/ParallaxView.png src/Image/control/PasswordBox.png src/Image/control/PersonPicture.png src/Image/control/PipsPager.png src/Image/control/Pivot.png src/Image/control/ProgressBar.png src/Image/control/ProgressRing.png src/Image/control/PullToRefresh.png src/Image/control/RadialGradientBrush.png src/Image/control/RadioButton.png src/Image/control/RadioButtons.png src/Image/control/RatingControl.png src/Image/control/RelativePanel.png src/Image/control/RepeatButton.png src/Image/control/RevealFocus.png src/Image/control/RichEditBox.png src/Image/control/RichTextBlock.png src/Image/control/ScrollViewer.png src/Image/control/SemanticZoom.png src/Image/control/Shape.png src/Image/control/Slider.png src/Image/control/Sound.png src/Image/control/SplitButton.png src/Image/control/SplitView.png src/Image/control/StackPanel.png src/Image/control/StandardUICommand.png src/Image/control/SwipeControl.png src/Image/control/TabView.png src/Image/control/TeachingTip.png src/Image/control/TextBlock.png src/Image/control/TextBox.png src/Image/control/ThemeTransition.png src/Image/control/TimePicker.png src/Image/control/TitleBar.png src/Image/control/ToggleButton.png src/Image/control/ToggleSplitButton.png src/Image/control/ToggleSwitch.png src/Image/control/ToolTip.png src/Image/control/TreeView.png src/Image/control/VariableSizedWrapGrid.png src/Image/control/Viewbox.png src/Image/control/WebView.png src/Image/control/XamlUICommand.png
)

target_link_libraries(ElaWidgetTools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ElaWidgetTools)
endif()
set_target_properties(ElaWidgetTools PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ElaWidgetTools
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ElaWidgetTools)
endif()
