# ElaApplication 组件文档

## 概述
ElaApplication 是一个专门用于实现 Windows 11 Mica 效果的 Qt 组件。该组件提供了一套完整的解决方案，用于在 Qt 应用程序中实现类似 Windows 11 的磨砂玻璃效果背景。它采用单例模式设计，继承自 QObject。

## 特性
- Windows Mica 效果的完整实现
- 支持明暗主题动态切换
- 多显示器环境自适应
- 高性能实时窗口同步
- 自定义背景图像支持
- 线程安全的效果处理
- 自动事件过滤和资源管理

## 属性

### IsEnableMica
- 类型: `bool`
- 默认值: `false`
- 说明: 控制 Mica 效果的全局启用状态
- 访问方法:
```cpp
bool getIsEnableMica() const;
void setIsEnableMica(bool isEnable);
```

### MicaImagePath
- 类型: `QString`
- 默认值: `":/include/Image/MicaBase.png"`
- 说明: Mica 效果的基础图片路径
- 访问方法:
```cpp
QString getMicaImagePath() const;
void setMicaImagePath(QString micaImagePath);
```

## 公共方法

### init()
初始化应用程序的基础配置。

**功能:**
- 配置 Qt 应用程序属性
- 加载必要的字体资源
- 设置默认字体样式

```cpp
void init();
```

### syncMica()
为指定窗口同步 Mica 效果。

**参数:**
- `widget`: 目标窗口对象
- `isSync`: 是否启用同步（默认为 true）

```cpp
void syncMica(QWidget* widget, bool isSync = true);
```

### containsCursorToItem()
检测鼠标指针是否位于指定窗口部件区域内。

**参数:**
- `item`: 待检测的窗口部件

**返回值:**
- `true`: 鼠标在部件区域内
- `false`: 鼠标在部件区域外

```cpp
static bool containsCursorToItem(QWidget* item);
```

## 事件处理
组件自动处理以下 Qt 事件：
- `QEvent::Show`: 窗口显示时更新效果
- `QEvent::Move`: 窗口移动时更新效果
- `QEvent::Resize`: 窗口大小改变时更新效果
- `QEvent::Destroy`: 窗口销毁时清理资源

## 信号
- `pIsEnableMicaChanged()`: Mica 效果启用状态变更信号
- `pMicaImagePathChanged()`: Mica 背景图片路径变更信号

## 使用示例

### 基础用法
```cpp
// 获取单例实例并初始化
eApp->init();

// 全局启用 Mica 效果
eApp->setIsEnableMica(true);

// 为窗口添加效果
QWidget* mainWindow = new QWidget();
eApp->syncMica(mainWindow, true);
```

### 自定义背景图片
```cpp
// 设置自定义的背景图片
eApp->setMicaImagePath(":/assets/custom_background.png");
```

### 动态控制效果
```cpp
class MainWindow : public QWidget {
public:
    MainWindow() {
        // 初始化时添加效果
        eApp->syncMica(this, true);
    }
    
    void toggleMicaEffect(bool enable) {
        // 动态控制效果
        eApp->syncMica(this, enable);
    }
};
```

### 多窗口管理
```cpp
class Application {
private:
    QList<QWidget*> windows;
    
public:
    void addWindow(QWidget* window) {
        windows.append(window);
        if (eApp->getIsEnableMica()) {
            eApp->syncMica(window, true);
        }
    }
    
    void removeWindow(QWidget* window) {
        windows.removeOne(window);
        eApp->syncMica(window, false);
    }
};
```

## 性能优化建议

### 批量处理
```cpp
// 批量处理多个窗口时，建议先禁用效果
eApp->setIsEnableMica(false);
// 进行窗口操作
// ...
// 完成后重新启用效果
eApp->setIsEnableMica(true);
```

### 主题切换处理
```cpp
connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode mode) {
    // 主题切换时的自定义处理
    updateCustomUI();
});
```

### 资源清理
```cpp
class CustomWidget : public QWidget {
public:
    ~CustomWidget() {
        // 窗口销毁前手动清理效果
        eApp->syncMica(this, false);
    }
};
```

## 注意事项
1. 使用前必须调用 `init()` 进行初始化
2. Mica 效果的视觉表现依赖于适当的背景图片
3. 在多显示器环境下，组件会自动适配当前显示器的分辨率
4. 主题切换时会自动更新所有启用 Mica 效果的窗口
5. 窗口销毁时会自动清理相关资源
6. 效果的初始化过程在独立线程中进行，确保不阻塞主线程

## 相关组件
- ElaTheme: 主题管理器，控制明暗主题切换
- ElaWindow: 带导航栏的无边框窗口，可直接使用 Mica 效果
- ElaWidget: 无边框模态窗口，可配合使用 Mica 效果