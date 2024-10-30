# ElaWindow 组件文档

## 概述
ElaWindow 是一个现代化的主窗口组件,继承自 QMainWindow。它集成了标题栏、导航栏、内容区域等完整的窗口布局,并提供了丰富的自定义选项。支持无边框设计、主题切换、导航路由等特性。

## 特性
- 现代化无边框设计
- 集成标题栏(ElaAppBar)
- 集成导航栏(ElaNavigationBar)
- 支持 Windows Aero Snap
- 自适应导航栏显示模式
- 主题切换动画
- 窗口阴影效果
- 路由导航系统
- 支持 Dock 窗口
- 支持工具栏
- 支持状态栏
- 支持自定义标题栏内容

## 属性

### IsStayTop
窗口是否置顶。

**类型:** `bool`  
**默认值:** `false`

### IsFixedSize
窗口是否固定大小。

**类型:** `bool`  
**默认值:** `false`

### AppBarHeight
标题栏高度。

**类型:** `int`  
**默认值:** `45`

### NavigationBarDisplayMode
导航栏显示模式。

**类型:** `ElaNavigationType::NavigationDisplayMode`  
**默认值:** `Auto`  
**可选值:**
- `Minimal`: 最小化模式
- `Compact`: 紧凑模式
- `Maximal`: 最大化模式
- `Auto`: 自动适应模式

### ThemeChangeTime
主题切换动画时间。

**类型:** `int`  
**默认值:** `700`

## 公共方法

### 窗口控制
```cpp
void moveToCenter();
void closeWindow();
```

### 导航栏操作
```cpp
// 添加页面节点
ElaNavigationType::NodeOperateReturnType addPageNode(
    QString pageTitle,
    QWidget* page,
    ElaIconType::IconName awesome = ElaIconType::None
) const;

// 添加展开节点
ElaNavigationType::NodeOperateReturnType addExpanderNode(
    QString expanderTitle,
    QString& expanderKey,
    ElaIconType::IconName awesome = ElaIconType::None
) const;

// 添加页脚节点
ElaNavigationType::NodeOperateReturnType addFooterNode(
    QString footerTitle,
    QWidget* page,
    QString& footerKey,
    int keyPoints = 0,
    ElaIconType::IconName awesome = ElaIconType::None
) const;

// 导航到指定页面
void navigation(QString pageKey);
```

### 自定义区域
```cpp
void setCustomWidget(
    ElaAppBarType::CustomArea customArea,
    QWidget* customWidget
);
```

## 信号
- `userInfoCardClicked()`: 用户信息卡片被点击
- `closeButtonClicked()`: 关闭按钮被点击
- `navigationNodeClicked(NodeType, QString)`: 导航节点被点击
- `customWidgetChanged()`: 自定义组件变更

## 使用示例

### 基础用法

#### 1. 创建主窗口
```cpp
class MainWindow : public ElaWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : ElaWindow(parent) {
        resize(1200, 740);
        setWindowTitle("My Application");
        moveToCenter();
    }
};
```

#### 2. 添加导航页面
```cpp
// 添加主页
HomePage* homePage = new HomePage(this);
addPageNode("HOME", homePage, ElaIconType::House);

// 添加分组页面
QString viewKey;
addExpanderNode("Views", viewKey, ElaIconType::CameraViewfinder);
addPageNode("ListView", listView, viewKey, ElaIconType::List);
addPageNode("TableView", tableView, viewKey, ElaIconType::Table);
```

### 高级用法

#### 1. 自定义标题栏
```cpp
// 添加菜单栏到标题栏
ElaMenuBar* menuBar = new ElaMenuBar(this);
menuBar->setFixedHeight(30);
setCustomWidget(ElaAppBarType::MiddleArea, menuBar);
setCustomWidgetMaximumWidth(500);
```

#### 2. 自定义关闭行为
```cpp
// 禁用默认关闭
setIsDefaultClosed(false);

// 添加关闭确认
connect(this, &ElaWindow::closeButtonClicked, this, [=](){
    if (showConfirmDialog()) {
        closeWindow();
    }
});
```

#### 3. 导航栏显示模式
```cpp
// 设置固定显示模式
setNavigationBarDisplayMode(ElaNavigationType::Compact);

// 自动适应模式(默认)
setNavigationBarDisplayMode(ElaNavigationType::Auto);
```

## 注意事项
1. 默认窗口大小为 1020x680
2. 自动适应模式下导航栏会根据窗口宽度自动切换显示模式
3. Windows 平台支持 Aero Snap 特性
4. 主题切换会触发动画效果
5. 导航节点的 key 必须唯一
6. 自定义区域宽度不应超过最大限制

## 相关组件
- ElaAppBar: 标题栏组件
- ElaNavigationBar: 导航栏组件
- ElaCentralStackedWidget: 内容区域组件
- ElaTheme: 主题管理器
- ElaWidget: 模态窗口组件
