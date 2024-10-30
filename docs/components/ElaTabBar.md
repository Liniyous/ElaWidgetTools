# ElaTabBar 组件文档

## 概述
ElaTabBar 是一个现代化的标签栏组件,继承自 QTabBar。它提供了拖拽支持、主题适配、动画效果等特性。

## 特性
- 拖拽支持
- 主题适配
- 动画效果
- 关闭按钮
- 图标支持
- 文本省略
- 自定义样式
- 状态反馈

## 信号
```cpp
// 标签按下
Q_SIGNAL void tabBarPress(int index);

// 标签拖拽创建
Q_SIGNAL void tabDragCreate(QDrag* drag);

// 标签拖拽放下
Q_SIGNAL void tabDragDrop(
    const QMimeData* mimeData
);
```

## 样式系统

### 标签形状
```cpp
void drawControl(CE_TabBarTabShape) {
    // 选中状态
    if (state & State_Selected) {
        // 绘制圆角背景
        path.moveTo(x, bottom + 1);
        path.arcTo(leftCorner, -90, 90);
        path.lineTo(x + margin, y + radius);
        path.arcTo(topLeftCorner, 180, -90);
        path.lineTo(right - radius, y);
        path.arcTo(topRightCorner, 90, -90);
        painter.drawPath(path);
    }
    // 悬浮状态
    else if (state & State_MouseOver) {
        painter.setBrush(hoverColor);
        painter.drawRoundedRect(rect);
    }
}
```

### 标签内容
```cpp
void drawControl(CE_TabBarTabLabel) {
    // 绘制图标
    if (!icon.isNull()) {
        painter.drawPixmap(
            iconRect,
            icon.pixmap(iconSize)
        );
    }
    
    // 绘制文本
    painter.setPen(textColor);
    QString text = metrics.elidedText(
        text, Qt::ElideRight, width
    );
    painter.drawText(
        textRect, 
        Qt::AlignLeft | Qt::AlignVCenter,
        text
    );
}
```

### 关闭按钮
```cpp
void drawPrimitive(PE_IndicatorTabClose) {
    // 悬浮效果
    if (state & State_MouseOver) {
        painter.setBrush(hoverColor);
        painter.drawRoundedRect(rect, 2, 2);
    }
    
    // 绘制图标
    painter.setFont(iconFont);
    painter.drawText(
        rect,
        Qt::AlignCenter, 
        closeIcon
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建标签栏
```cpp
// 创建标签栏
ElaTabBar* tabBar = new ElaTabBar(this);

// 添加标签
tabBar->addTab("标签1");
tabBar->addTab(icon, "标签2");

// 设置当前标签
tabBar->setCurrentIndex(0);
```

#### 2. 拖拽处理
```cpp
// 拖拽创建
connect(tabBar, 
    &ElaTabBar::tabDragCreate,
    this, [=](QDrag* drag) {
    drag->exec(Qt::MoveAction);
});

// 拖拽放下
connect(tabBar, 
    &ElaTabBar::tabDragDrop,
    this, [=](const QMimeData* data) {
    int fromIndex = data->property(
        "TabIndex"
    ).toInt();
    int toIndex = data->property(
        "TabDropIndex"
    ).toInt();
    // 处理标签移动
});
```

#### 3. 标签事件
```cpp
// 标签按下
connect(tabBar, 
    &ElaTabBar::tabBarPress,
    this, [=](int index) {
    qDebug() << "Pressed:" << index;
});

// 标签关闭
connect(tabBar, 
    &ElaTabBar::tabCloseRequested,
    this, [=](int index) {
    tabBar->removeTab(index);
});
```

## 注意事项
1. 继承自 QTabBar
2. 自动处理主题切换
3. 固定标签大小
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置大小
2. 控制标签数量
3. 优化拖拽体验
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QTabBar: 基类
- QDrag: 拖拽系统
- QMimeData: 数据容器
- QProxyStyle: 样式系统
