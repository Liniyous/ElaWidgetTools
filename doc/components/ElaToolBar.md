# ElaToolBar 组件文档

## 概述
ElaToolBar 是一个现代化的工具栏组件,继承自 QToolBar。它提供了自定义样式、浮动支持、主题适配等特性。

## 特性
- 自定义样式
- 浮动支持
- 主题适配
- 阴影效果
- 分隔符
- 拖动句柄
- 图标支持
- 透明背景

## 属性

### 基础属性
```cpp
// 工具栏间距
void setToolBarSpacing(int spacing);
int getToolBarSpacing() const;
```

## 公共方法

### 操作方法
```cpp
// 添加图标动作
QAction* addElaIconAction(
    ElaIconType::IconName icon, 
    const QString& text
);

// 添加带快捷键的图标动作
QAction* addElaIconAction(
    ElaIconType::IconName icon,
    const QString& text,
    const QKeySequence& shortcut
);
```

## 样式系统

### 工具栏样式
```cpp
void paintEvent() {
    if (isFloating()) {
        // 绘制阴影
        eTheme->drawEffectShadow(
            &painter, rect(), 
            shadowWidth, radius
        );
        
        // 绘制背景
        painter.setPen(borderColor);
        painter.setBrush(backgroundColor);
        painter.drawRoundedRect(
            foregroundRect, radius, radius
        );
        
        // 绘制拖动句柄
        if (handleRect.isValid()) {
            style->drawPrimitive(
                PE_IndicatorToolBarHandle
            );
        }
    }
}
```

### 分隔符样式
```cpp
void drawPrimitive(PE_IndicatorToolBarSeparator) {
    // 垂直分隔符
    if (orientation == Qt::Vertical) {
        painter.drawRect(
            x + width * 0.1,
            center.y() - 0.9,
            width * 0.8,
            1.8
        );
    } 
    // 水平分隔符
    else {
        painter.drawRect(
            center.x() - 0.9,
            y + height * 0.1,
            1.8,
            height * 0.8
        );
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建工具栏
```cpp
// 创建工具栏
ElaToolBar* toolBar = 
    new ElaToolBar("工具栏", this);

// 设置间距
toolBar->setToolBarSpacing(10);

// 设置边距
toolBar->layout()->setContentsMargins(
    3, 3, 3, 3
);
```

#### 2. 添加动作
```cpp
// 添加图标动作
QAction* action = toolBar->addElaIconAction(
    ElaIconType::Save,
    "保存"
);

// 添加带快捷键的动作
QAction* action = toolBar->addElaIconAction(
    ElaIconType::Open,
    "打开",
    QKeySequence::Open
);
```

#### 3. 浮动设置
```cpp
// 允许浮动
toolBar->setFloatable(true);

// 浮动状态改变
connect(toolBar, 
    &ElaToolBar::topLevelChanged,
    this, [=](bool floating) {
    if (floating) {
        // 设置浮动时的边距
        toolBar->layout()
            ->setContentsMargins(9, 9, 9, 9);
    } else {
        // 设置停靠时的边距
        toolBar->layout()
            ->setContentsMargins(3, 3, 3, 3);
    }
});
```

## 注意事项
1. 继承自 QToolBar
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 浮动支持

## 最佳实践
1. 合理设置大小
2. 控制动作数量
3. 优化布局
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QToolBar: 基类
- ElaToolButton: 工具按钮
- ElaIcon: 图标系统
- QProxyStyle: 样式系统
