# ElaWidget 组件文档

## 概述
ElaWidget 是一个现代化的窗口组件,继承自 QWidget。它提供了标题栏、主题支持、Mica 效果等特性,适用于创建统一风格的应用窗口。

## 特性
- 自定义标题栏
- 主题支持
- Mica 效果
- 窗口按钮
- 居中显示
- 置顶控制
- 大小控制
- 事件处理

## 属性

### 基础属性
```cpp
// 窗口置顶
Q_PROPERTY(bool isStayTop 
    READ getIsStayTop 
    WRITE setIsStayTop)

// 固定大小
Q_PROPERTY(bool isFixedSize 
    READ getIsFixedSize 
    WRITE setIsFixedSize)

// 默认关闭
Q_PROPERTY(bool isDefaultClosed 
    READ getIsDefaultClosed 
    WRITE setIsDefaultClosed)
```

## 公共方法

### 窗口控制
```cpp
// 移动到屏幕中心
void moveToCenter();

// 设置窗口按钮
void setWindowButtonFlag(
    ElaAppBarType::ButtonType buttonFlag, 
    bool isEnable = true
);

// 设置窗口按钮组
void setWindowButtonFlags(
    ElaAppBarType::ButtonFlags buttonFlags
);

// 获取窗口按钮组
ElaAppBarType::ButtonFlags 
    getWindowButtonFlags() const;
```

## 信号
```cpp
// 返回按钮点击
Q_SIGNAL void routeBackButtonClicked();

// 导航按钮点击
Q_SIGNAL void navigationButtonClicked();

// 主题切换按钮点击
Q_SIGNAL void themeChangeButtonClicked();

// 关闭按钮点击
Q_SIGNAL void closeButtonClicked();
```

## 使用示例

### 基础用法

#### 1. 创建窗口
```cpp
// 创建窗口
ElaWidget* window = new ElaWidget();

// 设置标题
window->setWindowTitle("My Window");

// 设置大小
window->resize(800, 600);

// 移动到屏幕中心
window->moveToCenter();
```

#### 2. 窗口按钮
```cpp
// 设置单个按钮
window->setWindowButtonFlag(
    ElaAppBarType::MinimizeButtonHint, 
    true
);

// 设置按钮组
window->setWindowButtonFlags(
    ElaAppBarType::StayTopButtonHint | 
    ElaAppBarType::MinimizeButtonHint | 
    ElaAppBarType::MaximizeButtonHint | 
    ElaAppBarType::CloseButtonHint
);
```

### 高级用法

#### 1. 主题处理
```cpp
// 连接主题变更信号
connect(eTheme, 
    &ElaTheme::themeModeChanged,
    this, [=](ElaThemeType::ThemeMode mode) {
    // 处理主题变更
    update();
});
```

#### 2. 事件处理
```cpp
// 连接按钮事件
connect(window, 
    &ElaWidget::navigationButtonClicked,
    this, [=]() {
    // 处理导航
});

connect(window,
    &ElaWidget::themeChangeButtonClicked,
    this, [=]() {
    // 处理主题切换
});
```

## 自定义外观

### 1. 背景绘制
```cpp
void paintEvent(QPaintEvent* event) {
    if (!isEnableMica) {
        QPainter painter(this);
        painter.setRenderHints(
            QPainter::Antialiasing | 
            QPainter::TextAntialiasing
        );
        
        // 绘制背景
        painter.setPen(Qt::NoPen);
        painter.setBrush(
            ElaThemeColor(themeMode, WindowBase)
        );
        painter.drawRect(rect());
    }
}
```

### 2. 标题栏定制
```cpp
// 获取标题栏
ElaAppBar* appBar = window->findChild<
    ElaAppBar*>();

// 设置标题栏属性
appBar->setIsStayTop(true);
appBar->setIsFixedSize(false);
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. Mica 效果
7. 窗口状态
8. 按钮控制

## 最佳实践
1. 合理设置大小
2. 优化绘制性能
3. 控制刷新频率
4. 注意内存使用
5. 统一交互方式
6. 保持风格一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QWidget: 基类
- ElaAppBar: 标题栏组件
- ElaTheme: 主题系统
- ElaApplication: 应用程序
```