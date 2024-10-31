# ElaStatusBar 组件文档

## 概述
ElaStatusBar 是一个现代化的状态栏组件,继承自 QStatusBar。它提供了自定义样式、主题适配、分隔符等特性。

## 特性
- 自定义样式
- 主题适配
- 分隔符绘制
- 固定高度
- 边距控制
- 背景绘制
- 抗锯齿
- 性能优化

## 基础设置
```cpp
// 创建时的基础设置
setFixedHeight(28);
setContentsMargins(20, 0, 0, 0);
setStyle(new ElaStatusBarStyle(style()));
```

## 样式系统

### 状态栏样式
```cpp
void drawPrimitive(
    PrimitiveElement element,
    const QStyleOption* option,
    QPainter* painter,
    const QWidget* widget
) const override {
    switch (element) {
    case PE_PanelStatusBar:
        // 绘制背景
        painter->setPen(
            ElaThemeColor(themeMode, BasicBorder)
        );
        painter->setBrush(
            ElaThemeColor(themeMode, BasicBaseAlpha)
        );
        painter->drawRect(statusBarRect);
        break;
        
    case PE_FrameStatusBarItem:
        // 绘制分隔符
        painter->setBrush(
            ElaThemeColor(themeMode, PrimaryNormal)
        );
        painter->drawRoundedRect(
            separatorRect, 2, 2
        );
        break;
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建状态栏
```cpp
// 创建状态栏
ElaStatusBar* statusBar = 
    new ElaStatusBar(this);

// 添加永久信息
statusBar->addPermanentWidget(
    new QLabel("永久信息")
);

// 显示临时信息
statusBar->showMessage(
    "临时信息", 
    3000  // 显示3秒
);
```

#### 2. 添加部件
```cpp
// 添加标签
QLabel* label = new QLabel("状态:");
statusBar->addWidget(label);

// 添加进度条
QProgressBar* progress = 
    new QProgressBar();
progress->setFixedWidth(100);
statusBar->addWidget(progress);

// 添加按钮
QPushButton* button = 
    new QPushButton("操作");
statusBar->addWidget(button);
```

### 高级用法

#### 1. 自定义分隔符
```cpp
// 设置分隔符可见性
statusBar->setSizeGripEnabled(false);

// 添加带分隔符的部件
QLabel* label = new QLabel("CPU:");
statusBar->addPermanentWidget(
    label, 
    0  // 拉伸因子
);

// 移除分隔符
statusBar->removeWidget(widget);
```

#### 2. 状态更新
```cpp
// 定时更新状态
QTimer* timer = new QTimer(this);
connect(timer, &QTimer::timeout, 
    this, [=]() {
    // 更新状态信息
    statusBar->showMessage(
        QString("当前时间: %1")
        .arg(QTime::currentTime()
            .toString())
    );
});
timer->start(1000);
```

## 样式定制

### 1. 背景绘制
```cpp
void drawBackground(QPainter* painter) {
    painter->setRenderHints(
        QPainter::Antialiasing | 
        QPainter::TextAntialiasing
    );
    
    // 绘制背景
    painter->setPen(
        ElaThemeColor(themeMode, BasicBorder)
    );
    painter->setBrush(
        ElaThemeColor(themeMode, BasicBaseAlpha)
    );
    painter->drawRect(rect());
}
```

### 2. 分隔符绘制
```cpp
void drawSeparator(
    QPainter* painter, 
    const QRect& rect
) {
    painter->setPen(Qt::NoPen);
    painter->setBrush(
        ElaThemeColor(themeMode, PrimaryNormal)
    );
    
    // 计算分隔符区域
    QRectF separatorRect(
        rect.right() - 3,
        rect.y() + rect.height() * 0.1,
        3,
        rect.height() * 0.8
    );
    
    // 绘制圆角分隔符
    painter->drawRoundedRect(
        separatorRect, 
        2, 
        2
    );
}
```

## 注意事项
1. 继承自 QStatusBar
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 部件管理

## 最佳实践
1. 合理设置高度
2. 控制部件数量
3. 优化更新频率
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QStatusBar: 基类
- ElaStatusBarStyle: 样式组件
- ElaTheme: 主题系统
- QLabel: 标签组件
