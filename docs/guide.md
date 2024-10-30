# ElaWidgetTools 安装指南

[ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) 是一个基于 QT-Widget 开发的 FluentUI 风格组件库。本指南将帮助您在项目中集成 ElaWidgetTools。

## 获取源码

首先从 GitHub 克隆项目：

```bash
git clone https://github.com/Liniyous/ElaWidgetTools.git
```

## 编译项目

使用 CMake 编译项目：

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

运行示例程序验证编译是否成功：

```bash
cd example
./example  # Windows 下为 example.exe
```

## 集成到自己的项目

### 1. 拷贝必要文件

1. 将 `src/include` 目录拷贝到您的项目中
2. 创建 `lib` 目录，拷贝构建生成的库文件

### 2. 添加库文件

不同平台下的库文件格式：

::: code-group

```bash [Windows]
lib/elawidgettools.dll
lib/elawidgettools.lib
```

```bash [MacOS]
lib/libelawidgettools.dylib
```

```bash [Linux]
lib/libelawidgettools.so
```

:::

### 3. 配置 CMakeLists.txt

将头文件和库文件添加到项目的 CMakeLists.txt 中：

::: code-group

```cmake [Windows]
include_directories(
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.lib
)
```

```cmake [MacOS]
include_directories(
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libelawidgettools.dylib
)
```

```cmake [Linux]
include_directories(
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libelawidgettools.so
)
```

:::

### 4. 添加资源文件

在 CMakeLists.txt 中添加资源文件：

```cmake
qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    include/ElaWidgetTools.qrc
)
```

## 验证安装

完成上述步骤后，您就可以在项目中使用 ElaWidgetTools 的组件了。创建一个简单的测试程序验证安装是否成功：

```cpp
#include <ElaApplication.h>
#include <ElaWindow.h>

int main(int argc, char *argv[]) {
    ElaApplication a(argc, argv);
    ElaWindow w;
    w.show();
    return a.exec();
}
```

## 注意事项

- 确保 Qt 版本兼容性：Main 分支支持 Qt 5.12 以上所有版本(Linux 为 Qt 5.15 以上)
- 推荐使用 Qt 6.4.3 和 Qt 6.6.2 版本
- 如遇到问题可以加入 QQ 群：850243692 获取支持
