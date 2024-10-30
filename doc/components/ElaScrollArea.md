# ElaScrollArea 组件文档

## 概述
ElaScrollArea 是一个现代化的滚动区域组件,继承自 QScrollArea。它提供了平滑滚动、手势支持、动画效果等特性。

## 特性
- 平滑滚动
- 手势支持
- 动画效果
- 回弹效果
- 透明背景
- 自定义滚动条
- 触摸支持
- 性能优化

## 公共方法

### 手势设置
```cpp
// 启用/禁用手势
void setIsGrabGesture(
    bool isEnable, 
    qreal mousePressEventDelay = 0.5
);

// 设置回弹效果
void setIsOverShoot(
    Qt::Orientation orientation, 
    bool isEnable
);

// 获取回弹状态
bool getIsOverShoot(
    Qt::Orientation orientation
) const;

// 设置动画效果
void setIsAnimation(
    Qt::Orientation orientation, 
    bool isAnimation
);

// 获取动画状态
bool getIsAnimation(
    Qt::Orientation orientation
) const;
```

## 使用示例

### 基础用法

#### 1. 创建滚动区域
```cpp
// 创建滚动区域
ElaScrollArea* area = 
    new ElaScrollArea(this);

// 设置内容部件
QWidget* content = new QWidget();
area->setWidget(content);

// 设置透明背景
area->setStyleSheet(
    "background:transparent;"
);
```

#### 2. 手势设置
```cpp
// 启用手势
area->setIsGrabGesture(true);

// 设置手势延迟
area->setIsGrabGesture(true, 0.5);

// 设置回弹效果
area->setIsOverShoot(
    Qt::Vertical, true
);
area->setIsOverShoot(
    Qt::Horizontal, true
);
```

#### 3. 动画设置
```cpp
// 启用垂直动画
area->setIsAnimation(
    Qt::Vertical, true
);

// 启用水平动画
area->setIsAnimation(
    Qt::Horizontal, true
);

// 获取动画状态
bool hasVerticalAnimation = 
    area->getIsAnimation(Qt::Vertical);
```

### 高级用法

#### 1. 滚动属性设置
```cpp
// 获取滚动器
QScroller* scroller = 
    QScroller::scroller(
        area->viewport()
    );

// 设置滚动属性
QScrollerProperties props = 
    scroller->scrollerProperties();

// 设置回弹阻力
props.setScrollMetric(
    QScrollerProperties::
    OvershootDragResistanceFactor, 
    0.35
);

// 设置回弹时间
props.setScrollMetric(
    QScrollerProperties::
    OvershootScrollTime, 
    0.5
);

// 设置帧率
props.setScrollMetric(
    QScrollerProperties::Fps60,
    QScrollerProperties::FrameRate
);

// 应用属性
scroller->setScrollerProperties(props);
```

## 注意事项
1. 继承自 QScrollArea
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 手势支持

## 最佳实践
1. 合理设置大小
2. 控制内容大小
3. 优化滚动体验
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QScrollArea: 基类
- ElaScrollBar: 滚动条组件
- QScroller: 滚动系统
- QScrollerProperties: 滚动属性
