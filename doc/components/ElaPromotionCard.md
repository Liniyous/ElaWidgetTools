# ElaPromotionCard 组件文档

## 概述
ElaPromotionCard 是一个现代化的推广卡片组件,提供了悬浮效果、点击动画、主题适配等特性。它适用于展示产品推广、活动宣传等内容。

## 特性
- 悬浮效果
- 点击动画
- 主题适配
- 圆角边框
- 阴影效果
- 图片支持
- 渐变背景
- 多层文本

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 卡片图片
Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap 
    WRITE setCardPixmap)

// 图片比例
Q_PROPERTY(qreal horizontalCardPixmapRatio 
    READ getHorizontalCardPixmapRatio 
    WRITE setHorizontalCardPixmapRatio)
Q_PROPERTY(qreal verticalCardPixmapRatio 
    READ getVerticalCardPixmapRatio 
    WRITE setVerticalCardPixmapRatio)
```

### 文本属性
```cpp
// 卡片标题
Q_PROPERTY(QString cardTitle READ getCardTitle 
    WRITE setCardTitle)

// 推广标题
Q_PROPERTY(QString promotionTitle 
    READ getPromotionTitle 
    WRITE setPromotionTitle)

// 主标题
Q_PROPERTY(QString title READ getTitle 
    WRITE setTitle)

// 副标题
Q_PROPERTY(QString subTitle READ getSubTitle 
    WRITE setSubTitle)
```

### 样式属性
```cpp
// 文本颜色
Q_PROPERTY(QColor cardTitleColor 
    READ getCardTitleColor 
    WRITE setCardTitleColor)
Q_PROPERTY(QColor promotionTitleColor 
    READ getPromotionTitleColor 
    WRITE setPromotionTitleColor)
Q_PROPERTY(QColor titleColor 
    READ getTitleColor 
    WRITE setTitleColor)
Q_PROPERTY(QColor subTitleColor 
    READ getSubTitleColor 
    WRITE setSubTitleColor)

// 文本大小
Q_PROPERTY(int cardTitlePixelSize 
    READ getCardTitlePixelSize 
    WRITE setCardTitlePixelSize)
Q_PROPERTY(int promotionTitlePixelSize 
    READ getPromotionTitlePixelSize 
    WRITE setPromotionTitlePixelSize)
Q_PROPERTY(int titlePixelSize 
    READ getTitlePixelSize 
    WRITE setTitlePixelSize)
Q_PROPERTY(int subTitlePixelSize 
    READ getSubTitlePixelSize 
    WRITE setSubTitlePixelSize)
```

## 信号
```cpp
// 卡片点击
Q_SIGNAL void promotionCardClicked();
```

## 动画系统

### 悬浮动画
```cpp
void _startHoverOpacityAnimation(bool isVisible) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(this, "opacity");
    animation->setDuration(250);
    animation->setStartValue(currentOpacity);
    animation->setEndValue(isVisible ? 1 : 0);
    animation->start();
}
```

### 点击动画
```cpp
void mousePressEvent() {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(this, "pressRadius");
    animation->setDuration(350);
    animation->setStartValue(0);
    animation->setEndValue(longestDistance);
    animation->setEasingCurve(
        QEasingCurve::OutQuad);
    animation->start();
}
```

## 渲染系统

### 背景渲染
```cpp
void paintEvent() {
    // 绘制阴影
    eTheme->drawEffectShadow(
        &painter, rect(), 
        shadowWidth, borderRadius
    );
    
    // 绘制背景图
    painter.drawPixmap(
        foregroundRect,
        cardPixmap, 
        sourceRect
    );
    
    // 绘制渐变
    painter.setBrush(*gradient);
    painter.drawEllipse(
        center, radius, radius
    );
}
```

### 文本渲染
```cpp
void paintEvent() {
    // 卡片标题
    painter.setFont(cardTitleFont);
    painter.drawText(rect, cardTitle);
    
    // 推广标题
    painter.drawRoundedRect(
        promotionTitleBaseRect, 8, 8);
    painter.drawText(rect, promotionTitle);
    
    // 主标题
    painter.drawText(titleRect, title);
    
    // 副标题
    painter.drawText(subTitleRect, subTitle);
}
```

## 使用示例

### 基础用法

#### 1. 创建卡片
```cpp
// 创建卡片
ElaPromotionCard* card = 
    new ElaPromotionCard(this);

// 设置图片
card->setCardPixmap(QPixmap(":/image.png"));
card->setHorizontalCardPixmapRatio(0.8);
card->setVerticalCardPixmapRatio(0.8);

// 设置文本
card->setCardTitle("卡片标题");
card->setPromotionTitle("推广标题");
card->setTitle("主标题");
card->setSubTitle("副标题文本");
```

#### 2. 自定义样式
```cpp
// 设置颜色
card->setCardTitleColor(Qt::white);
card->setPromotionTitleColor(Qt::white);
card->setTitleColor(Qt::white);
card->setSubTitleColor(Qt::white);

// 设置字号
card->setCardTitlePixelSize(22);
card->setPromotionTitlePixelSize(12);
card->setTitlePixelSize(25);
card->setSubTitlePixelSize(16);
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 动画时长250ms
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置大小
2. 控制文本长度
3. 优化图片大小
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QWidget: 基类
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
- QRadialGradient: 渐变系统
