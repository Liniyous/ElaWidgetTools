# ElaPopularCard 组件文档

## 概述
ElaPopularCard 是一个现代化的卡片组件,提供了悬浮展开、动画效果、主题适配等特性。它适用于展示产品、文章等内容的预览卡片。

## 特性
- 悬浮展开
- 动画效果
- 主题适配
- 圆角边框
- 阴影效果
- 图片支持
- 按钮交互
- 自适应布局

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 卡片图片
Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap 
    WRITE setCardPixmap)

// 标题
Q_PROPERTY(QString title READ getTitle 
    WRITE setTitle)

// 副标题
Q_PROPERTY(QString subTitle READ getSubTitle 
    WRITE setSubTitle)

// 交互提示
Q_PROPERTY(QString interactiveTips 
    READ getInteractiveTips 
    WRITE setInteractiveTips)

// 详细文本
Q_PROPERTY(QString detailedText 
    READ getDetailedText 
    WRITE setDetailedText)

// 按钮文本
Q_PROPERTY(QString cardButtonText 
    READ getCardButtonText 
    WRITE setCardButtonText)

// 悬浮区域
Q_PROPERTY(QWidget* cardFloatArea 
    READ getCardFloatArea 
    WRITE setCardFloatArea)

// 悬浮图片
Q_PROPERTY(QPixmap cardFloatPixmap 
    READ getCardFloatPixmap 
    WRITE setCardFloatPixmap)
```

## 信号
```cpp
// 卡片点击
Q_SIGNAL void popularCardClicked();

// 按钮点击
Q_SIGNAL void popularCardButtonClicked();
```

## 动画系统

### 悬浮动画
```cpp
void showFloater() {
    // 位置动画
    QPropertyAnimation* geometryAnimation = 
        new QPropertyAnimation(this, "geometry");
    geometryAnimation->setDuration(300);
    geometryAnimation->setEasingCurve(
        QEasingCurve::OutQuad);
    
    // 透明度动画
    QPropertyAnimation* opacityAnimation = 
        new QPropertyAnimation(this, "opacity");
    opacityAnimation->setDuration(300);
    
    // 按钮动画
    QPropertyAnimation* buttonAnimation = 
        new QPropertyAnimation(button, "geometry");
    buttonAnimation->setDuration(300);
    
    // 启动动画组
    QParallelAnimationGroup* group = 
        new QParallelAnimationGroup(this);
    group->addAnimation(geometryAnimation);
    group->addAnimation(opacityAnimation);
    group->addAnimation(buttonAnimation);
    group->start();
}
```

## 样式系统

### 卡片样式
```cpp
void paintEvent() {
    // 绘制阴影
    painter.setOpacity(hoverOpacity);
    eTheme->drawEffectShadow(
        &painter, shadowRect, 
        shadowWidth, borderRadius
    );
    
    // 绘制背景
    painter.setPen(hovered ? 
        hoverBorderColor : borderColor);
    painter.setBrush(backgroundColor);
    painter.drawRoundedRect(
        foregroundRect, 
        borderRadius, 
        borderRadius
    );
    
    // 绘制图片
    painter.setClipPath(pixPath);
    painter.drawPixmap(pixRect, 
        cardPixmap, 
        cardPixmap.rect()
    );
    
    // 绘制文本
    painter.drawText(titleRect, title);
    painter.drawText(subTitleRect, subTitle);
    painter.drawText(detailsRect, details);
}
```

## 使用示例

### 基础用法

#### 1. 创建卡片
```cpp
// 创建卡片
ElaPopularCard* card = 
    new ElaPopularCard(this);

// 设置内容
card->setCardPixmap(QPixmap(":/image.png"));
card->setTitle("标题文本");
card->setSubTitle("副标题文本");
card->setDetailedText("详细描述...");
card->setCardButtonText("查看详情");

// 设置悬浮区域
card->setCardFloatArea(this);
```

#### 2. 处理事件
```cpp
// 卡片点击
connect(card, 
    &ElaPopularCard::popularCardClicked,
    this, [=]() {
    qDebug() << "Card clicked";
});

// 按钮点击
connect(card,
    &ElaPopularCard::popularCardButtonClicked,
    this, [=]() {
    qDebug() << "Button clicked";
});
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 动画时长300ms
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
- ElaPushButton: 按钮组件
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统