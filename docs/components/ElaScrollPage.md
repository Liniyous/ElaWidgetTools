# ElaScrollPage 组件文档

## 概述
ElaScrollPage 是一个现代化的滚动页面组件,提供了页面导航、面包屑、滚动区域等特性。它适用于需要多页面切换和导航的场景。

## 特性
- 页面导航
- 面包屑导航
- 滚动支持
- 动画切换
- 手势支持
- 路由记录
- 自定义标题
- 自适应布局

## 属性

### 基础属性
```cpp
// 自定义部件
Q_PROPERTY(QWidget* customWidget 
    READ getCustomWidget 
    WRITE setCustomWidget)
```

## 公共方法

### 页面操作
```cpp
// 添加中心部件
void addCentralWidget(
    QWidget* centralWidget,
    bool isWidgetResizeable = true,
    bool isVerticalGrabGesture = true,
    qreal mousePressEventDelay = 0.5
);

// 页面导航
void navigation(
    int widgetIndex, 
    bool isLogRoute = true
);

// 设置标题间距
void setPageTitleSpacing(int spacing);
int getPageTitleSpacing() const;

// 设置标题可见性
void setTitleVisible(bool isVisible);
```

## 使用示例

### 基础用法

#### 1. 创建页面
```cpp
// 创建滚动页面
ElaScrollPage* page = 
    new ElaScrollPage(this);

// 创建内容页面
QWidget* content1 = new QWidget();
content1->setWindowTitle("页面1");
QWidget* content2 = new QWidget();
content2->setWindowTitle("页面2");

// 添加页面
page->addCentralWidget(content1);
page->addCentralWidget(content2);
```

#### 2. 页面导航
```cpp
// 导航到指定页面
page->navigation(1); // 导航到第二个页面

// 导航且记录路由
page->navigation(1, true);

// 设置标题间距
page->setPageTitleSpacing(20);

// 隐藏标题
page->setTitleVisible(false);
```

#### 3. 自定义部件
```cpp
// 添加自定义部件
QWidget* custom = new QWidget();
page->setCustomWidget(custom);

// 获取自定义部件
QWidget* widget = 
    page->getCustomWidget();
```

### 高级用法

#### 1. 滚动设置
```cpp
// 添加可滚动页面
page->addCentralWidget(
    content,     // 内容部件
    true,        // 允许调整大小
    true,        // 启用垂直手势
    0.5          // 手
