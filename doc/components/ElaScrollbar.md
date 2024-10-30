# ElaScrollBar 组件文档

## 概述
ElaScrollBar 是一个现代化的滚动条组件,继承自 QScrollBar。它提供了平滑滚动、动画效果、主题适配等特性。

## 特性
- 平滑滚动
- 动画效果
- 主题适配
- 悬浮展开
- 速度限制
- 右键菜单
- 滚轮支持
- 自动隐藏

## 属性

### 基础属性
```cpp
// 启用动画
Q_PROPERTY(bool isAnimation 
    READ getIsAnimation 
    WRITE setIsAnimation)

// 速度限制
Q_PROPERTY(qreal speedLimit 
    READ getSpeedLimit 
    WRITE setSpeedLimit)
```

## 构造函数

```cpp
// 基础构造
ElaScrollBar(QWidget* parent = nullptr);

// 指定方向构造
ElaScrollBar(Qt::Orientation orientation, 
    QWidget* parent = nullptr);

// 替换现有滚动条
ElaScrollBar(QScrollBar* originScrollBar, 
    QAbstractScrollArea* parent = nullptr);
```

## 样式系统

### 滚动条样式
```cpp
void drawComplexControl(CC_ScrollBar) {
    // 展开状态
    if (isExpand) {
        // 绘制背景
        painter.setBrush(baseColor);
        painter.drawRoundedRect(
            rect, radius, radius
        );
        
        // 绘制箭头指示器
        painter.setBrush(handleColor);
        drawArrowIndicator();
    }
    
    // 绘制滑块
    painter.setBrush(handleColor);
    painter.drawRoundedRect(
        sliderRect, radius, radius
    );
}
```

### 动画系统
```cpp
void startExpandAnimation(bool isExpand) {
    // 透明度动画
    opacityAnimation->setStartValue(opacity);
    opacityAnimation->setEndValue(
        isExpand ? 1.0 : 0.0
    );
    
    // 滑块大小动画
    extentAnimation->setStartValue(extent);
    extentAnimation->setEndValue(
        isExpand ? maxExtent : minExtent
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建滚动条
```cpp
// 创建垂直滚动条
ElaScrollBar* scrollBar = 
    new ElaScrollBar(Qt::Vertical, this);

// 启用动画
scrollBar->setIsAnimation(true);

// 设置速度限制
scrollBar->setSpeedLimit(20);
```

#### 2. 替换现有滚动条
```cpp
// 替换 QScrollArea 的滚动条
ElaScrollBar* scrollBar = new ElaScrollBar(
    area->verticalScrollBar(), 
    area
);

// 替换 QTableView 的滚动条
ElaScrollBar* scrollBar = new ElaScrollBar(
    view->verticalScrollBar(), 
    view
);
```

#### 3. 信号连接
```cpp
// 范围动画完成
connect(scrollBar, 
    &ElaScrollBar::rangeAnimationFinished,
    this, [=]() {
    // 处理动画完成
});

// 值改变
connect(scrollBar, 
    &QScrollBar::valueChanged,
    this, [=](int value) {
    // 处理值改变
});
```

## 注意事项
1. 继承自 QScrollBar
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 动画流畅

## 最佳实践
1. 合理设置速度
2. 控制动画时长
3. 优化滚动体验
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QScrollBar: 基类
- QAbstractScrollArea: 滚动区域
- QPropertyAnimation: 动画系统
- QProxyStyle: 样式系统
