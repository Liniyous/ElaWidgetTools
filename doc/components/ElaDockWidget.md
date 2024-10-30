# ElaDockWidget 组件文档

## 概述
ElaDockWidget 是一个现代化的停靠窗口组件,继承自 QDockWidget。它提供了自定义标题栏、主题适配、Mica 效果等特性,可用于创建可停靠、可浮动的子窗口。

## 特性
- 自定义标题栏
- 主题适配
- Windows Mica 效果
- 窗口阴影
- 浮动/停靠切换
- 无边框设计
- 窗口缩放
- 标题图标
- 关闭/浮动按钮

## 构造函数
```cpp
// 基础构造
ElaDockWidget(QWidget* parent = nullptr, 
              Qt::WindowFlags flags = Qt::WindowFlags());

// 带标题构造
ElaDockWidget(const QString& title, 
              QWidget* parent = nullptr,
              Qt::WindowFlags flags = Qt::WindowFlags());
```

## 尺寸规格

- 标题栏高度: 32px
- 图标尺寸: 18x18px
- 按钮尺寸: 32x26px
- 标题字体大小: 13px
- 边距: 8px
- 阴影宽度: 6px

## 使用示例

### 基础用法

#### 1. 创建停靠窗口
```cpp
// 创建停靠窗口
ElaDockWidget* dockWidget = new ElaDockWidget("日志窗口", this);

// 设置内容组件
QTextEdit* logView = new QTextEdit(dockWidget);
dockWidget->setWidget(logView);

// 添加到主窗口
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

// 设置初始大小
resizeDocks({dockWidget}, {200}, Qt::Horizontal);
```

#### 2. 设置特性
```cpp
// 设置停靠特性
dockWidget->setFeatures(QDockWidget::DockWidgetClosable | 
                       QDockWidget::DockWidgetFloatable |
                       QDockWidget::DockWidgetMovable);

// 设置允许的停靠区域
dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | 
                           Qt::RightDockWidgetArea);
```

### 高级用法

#### 1. 自定义标题栏
```cpp
class CustomDockTitleBar : public ElaDockWidgetTitleBar {
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 自定义背景
        if (dockWidget()->isFloating()) {
            painter.setBrush(customFloatingColor);
        } else {
            painter.setBrush(customDockedColor);
        }
        painter.drawRect(rect());
        
        // 绘制其他自定义元素
    }
};

// 使用自定义标题栏
dockWidget->setTitleBarWidget(new CustomDockTitleBar(dockWidget));
```

#### 2. 状态监听
```cpp
// 监听浮动状态变化
connect(dockWidget, &ElaDockWidget::topLevelChanged,
    this, [](bool floating) {
        qDebug() << "Dock widget floating:" << floating;
});

// 监听可见性变化
connect(dockWidget, &ElaDockWidget::visibilityChanged,
    this, [](bool visible) {
        updateDockWidgetStatus(visible);
});
```

#### 3. 布局管理
```cpp
// 创建多个停靠窗口
ElaDockWidget* logDock = new ElaDockWidget("日志", this);
ElaDockWidget* propDock = new ElaDockWidget("属性", this);
ElaDockWidget* toolDock = new ElaDockWidget("工具", this);

// 设置布局
addDockWidget(Qt::RightDockWidgetArea, logDock);
addDockWidget(Qt::RightDockWidgetArea, propDock);
addDockWidget(Qt::LeftDockWidgetArea, toolDock);

// 标签页模式
tabifyDockWidget(logDock, propDock);

// 调整大小
resizeDocks({logDock, toolDock}, 
            {200, 250}, 
            Qt::Horizontal);
```

## 注意事项
1. 默认支持关闭和浮动功能
2. 浮动时自动添加阴影效果
3. Windows 平台支持 Mica 效果
4. 主题切换会自动更新样式
5. 标题栏按钮根据特性自动显示/隐藏
6. 支持拖拽调整大小
7. 支持键盘导航
8. 浮动窗口保持在父窗口之上

## 相关组件
- ElaDockWidgetTitleBar: 停靠窗口标题栏
- ElaIconButton: 图标按钮
- ElaText: 文本标签
- ElaTheme: 主题管理器
