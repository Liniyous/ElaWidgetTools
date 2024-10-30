# ElaAppBar 组件文档

## 概述
ElaAppBar 是一个现代化的窗口标题栏组件,提供了完整的窗口控制功能。它支持窗口拖动、调整大小、最大/最小化、主题切换等功能,并可以自定义标题栏内容。

## 特性
- 无边框窗口支持
- 窗口拖拽移动
- 窗口大小调整
- 系统按钮(最小化/最大化/关闭)
- 主题切换按钮
- 窗口置顶功能
- 自定义区域内容
- 路由返回按钮
- 导航菜单按钮
- 支持 Windows Aero Snap

## 属性

### IsStayTop
窗口是否置顶。

**类型:** `bool`  
**默认值:** `false`

### IsFixedSize
窗口是否固定大小。

**类型:** `bool`  
**默认值:** `false`

### IsDefaultClosed
是否使用默认关闭行为。

**类型:** `bool`  
**默认值:** `true`

### AppBarHeight
标题栏高度。

**类型:** `int`  
**默认值:** `45`

### CustomWidgetMaximumWidth
自定义区域最大宽度。

**类型:** `int`  
**默认值:** `550`

## 按钮标志

### ButtonType
```cpp
enum ButtonType {
    RouteBackButtonHint    = 0x00000001,
    NavigationButtonHint   = 0x00000002,
    ThemeChangeButtonHint  = 0x00000004,
    StayTopButtonHint      = 0x00000008,
    MinimizeButtonHint     = 0x00000010,
    MaximizeButtonHint     = 0x00000020,
    CloseButtonHint        = 0x00000040
};
```

## 自定义区域

### CustomArea
```cpp
enum CustomArea {
    LeftArea,   // 左侧区域
    MiddleArea, // 中间区域
    RightArea   // 右侧区域
};
```

## 公共方法

### 设置自定义组件
```cpp
void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget);
QWidget* getCustomWidget() const;
```

### 按钮控制
```cpp
void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
ElaAppBarType::ButtonFlags getWindowButtonFlags() const;
```

### 路由返回按钮
```cpp
void setRouteBackButtonEnable(bool isEnable);
```

## 信号
- `routeBackButtonClicked()`: 路由返回按钮点击
- `navigationButtonClicked()`: 导航按钮点击
- `themeChangeButtonClicked()`: 主题切换按钮点击
- `closeButtonClicked()`: 关闭按钮点击
- `customWidgetChanged()`: 自定义组件变更

## 使用示例

### 基础用法

#### 1. 创建标题栏
```cpp
ElaAppBar* appBar = new ElaAppBar(this);
appBar->setWindowTitle("My Application");
```

#### 2. 自定义按钮
```cpp
// 禁用最大化按钮
appBar->setWindowButtonFlag(ElaAppBarType::MaximizeButtonHint, false);

// 设置多个按钮
appBar->setWindowButtonFlags(
    ElaAppBarType::MinimizeButtonHint | 
    ElaAppBarType::CloseButtonHint
);
```

### 高级用法

#### 1. 自定义区域
```cpp
// 添加菜单栏到中间区域
ElaMenuBar* menuBar = new ElaMenuBar(this);
menuBar->setFixedHeight(30);
appBar->setCustomWidget(ElaAppBarType::MiddleArea, menuBar);
appBar->setCustomWidgetMaximumWidth(500);
```

#### 2. 自定义关闭行为
```cpp
// 禁用默认关闭
appBar->setIsDefaultClosed(false);

// 监听关闭按钮
connect(appBar, &ElaAppBar::closeButtonClicked, this, [=](){
    // 显示确认对话框
    if (showConfirmDialog()) {
        window()->close();
    }
});
```

## 注意事项
1. 组件需要父窗口为无边框窗口
2. Windows 平台支持系统窗口特性(Aero Snap)
3. 自定义区域宽度不应超过最大宽度限制
4. 路由返回按钮默认禁用状态
5. 窗口置顶状态会影响其他窗口
6. 主题切换会自动更新标题栏样式

## 相关组件
- ElaWindow: 主窗口组件
- ElaWidget: 模态窗口组件
- ElaTheme: 主题管理器
- ElaNavigationBar: 导航栏组件
