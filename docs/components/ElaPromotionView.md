# ElaPromotionView 组件文档

## 概述
ElaPromotionView 是一个现代化的推广视图组件,提供了卡片切换、自动滚动、动画效果等特性。它适用于展示多个推广卡片的轮播展示。

## 特性
- 卡片切换
- 自动滚动
- 动画效果
- 主题适配
- 指示器
- 手势支持
- 无限循环
- 响应式布局

## 属性

### 基础属性
```cpp
// 展开卡片宽度
Q_PROPERTY(int cardExpandWidth 
    READ getCardExpandWidth 
    WRITE setCardExpandWidth)

// 收起卡片宽度
Q_PROPERTY(int cardCollapseWidth 
    READ getCardCollapseWidth 
    WRITE setCardCollapseWidth)

// 当前索引
Q_PROPERTY(int currentIndex 
    READ getCurrentIndex 
    WRITE setCurrentIndex)

// 自动滚动
Q_PROPERTY(bool isAutoScroll 
    READ getIsAutoScroll 
    WRITE setIsAutoScroll)

// 滚动间隔
Q_PROPERTY(int autoScrollInterval 
    READ getAutoScrollInterval 
    WRITE setAutoScrollInterval)
```

## 公共方法

### 卡片管理
```cpp
// 添加推广卡片
void appendPromotionCard(
    ElaPromotionCard* card
);
```

## 动画系统

### 卡片切换动画
```cpp
void _startCardGeometryAnimation(
    ElaPromotionCard* card, 
    QRect start, 
    QRect end
) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            card, "geometry"
        );
    animation->setDuration(650);
    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->start();
}
```

### 图片比例动画
```cpp
void _startHorizontalCardPixmapRatioAnimation(
    ElaPromotionCard* card, 
    qreal start, 
    qreal end
) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            card, "horizontalCardPixmapRatio"
        );
    animation->setDuration(650);
    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->start();
}
```

## 渲染系统

### 指示器渲染
```cpp
void paintEvent() {
    // 计算起始点
    QPoint startPoint = isCountOdd ? 
        QPoint(width/2 - count/2 * spacing, 
            height - margin/2) : 
        QPoint(width/2 - count/2 * spacing 
            - spacing/2, height - margin/2);
    
    // 绘制指示器
    for (int i = 0; i < count; i++) {
        if (i == currentIndex) {
            painter.drawEllipse(
                startPoint + i * spacing, 
                3.5, 3.5
            );
        } else {
            painter.drawEllipse(
                startPoint + i * spacing, 
                2.5, 2.5
            );
        }
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建视图
```cpp
// 创建视图
ElaPromotionView* view = 
    new ElaPromotionView(this);

// 设置卡片宽度
view->setCardExpandWidth(600);
view->setCardCollapseWidth(300);

// 添加卡片
view->appendPromotionCard(card1);
view->appendPromotionCard(card2);
view->appendPromotionCard(card3);
```

#### 2. 自动滚动
```cpp
// 启用自动滚动
view->setIsAutoScroll(true);
view->setAutoScrollInterval(5000);

// 禁用自动滚动
view->setIsAutoScroll(false);
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 动画时长650ms
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置宽度
2. 控制卡片数量
3. 优化图片大小
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QWidget: 基类
- ElaPromotionCard: 卡片组件
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
