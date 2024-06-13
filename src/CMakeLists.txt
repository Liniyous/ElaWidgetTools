cmake_minimum_required(VERSION 3.5)

project(elawidgettools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DELAWIDGETTOOLS_LIBRARY)
option(ELAWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." OFF)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/private
    ${PROJECT_SOURCE_DIR}/include/DeveloperComponents
    ${PROJECT_SOURCE_DIR}/include/DeveloperComponents/private
)

set(PROJECT_SOURCES
    ${sources_files}
)

if (ELAWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else()
    set(LIB_TYPE "SHARED")
endif()
qt_add_library(${PROJECT_NAME} ${LIB_TYPE}
    ${sources_files}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_resources(${PROJECT_NAME}
        RESOURCES PREFIX "/"
        FILES
        Font/ElaAwesome.ttf
        Image/Moon.jpg Image/Home_Background.png Image/github.png Image/noise.png Image/Cirno.jpg
        Image/control/Acrylic.png Image/control/AnimatedIcon.png Image/control/AnimatedVisualPlayer.png Image/control/AnimationInterop.png Image/control/AppBarButton.png Image/control/AppBarSeparator.png Image/control/AppBarToggleButton.png Image/control/AutomationProperties.png Image/control/AutoSuggestBox.png Image/control/Border.png Image/control/BreadcrumbBar.png Image/control/Button.png Image/control/CalendarDatePicker.png Image/control/CalendarView.png Image/control/Canvas.png Image/control/Checkbox.png Image/control/Clipboard.png Image/control/ColorPaletteResources.png Image/control/ColorPicker.png Image/control/ComboBox.png Image/control/CommandBar.png Image/control/CommandBarFlyout.png Image/control/CompactSizing.png Image/control/ConnectedAnimation.png Image/control/ContentDialog.png Image/control/CreateMultipleWindows.png Image/control/DataGrid.png Image/control/DatePicker.png Image/control/DropDownButton.png Image/control/EasingFunction.png Image/control/Expander.png Image/control/FilePicker.png Image/control/FlipView.png Image/control/Flyout.png Image/control/Grid.png Image/control/GridView.png Image/control/HyperlinkButton.png Image/control/IconElement.png Image/control/Image.png Image/control/ImplicitTransition.png Image/control/InfoBadge.png Image/control/InfoBar.png Image/control/InkCanvas.png Image/control/InkToolbar.png Image/control/InputValidation.png Image/control/ItemsRepeater.png Image/control/Line.png Image/control/ListBox.png Image/control/ListView.png Image/control/MediaPlayerElement.png Image/control/MenuBar.png Image/control/MenuFlyout.png Image/control/NavigationView.png Image/control/NumberBox.png Image/control/PageTransition.png Image/control/ParallaxView.png Image/control/PasswordBox.png Image/control/PersonPicture.png Image/control/PipsPager.png Image/control/Pivot.png Image/control/ProgressBar.png Image/control/ProgressRing.png Image/control/PullToRefresh.png Image/control/RadialGradientBrush.png Image/control/RadioButton.png Image/control/RadioButtons.png Image/control/RatingControl.png Image/control/RelativePanel.png Image/control/RepeatButton.png Image/control/RevealFocus.png Image/control/RichEditBox.png Image/control/RichTextBlock.png Image/control/ScrollViewer.png Image/control/SemanticZoom.png Image/control/Shape.png Image/control/Slider.png Image/control/Sound.png Image/control/SplitButton.png Image/control/SplitView.png Image/control/StackPanel.png Image/control/StandardUICommand.png Image/control/SwipeControl.png Image/control/TabView.png Image/control/TeachingTip.png Image/control/TextBlock.png Image/control/TextBox.png Image/control/ThemeTransition.png Image/control/TimePicker.png Image/control/TitleBar.png Image/control/ToggleButton.png Image/control/ToggleSplitButton.png Image/control/ToggleSwitch.png Image/control/ToolTip.png Image/control/TreeView.png Image/control/VariableSizedWrapGrid.png Image/control/Viewbox.png Image/control/WebView.png Image/control/XamlUICommand.png
    )
else()
    qt_add_resources(PROJECT_SOURCES
        ElaWidgetTools.qrc
    )
endif()

if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()


target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    D3D11
    DXGI
)
