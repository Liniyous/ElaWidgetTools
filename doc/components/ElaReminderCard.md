# ElaReminderCard 组件文档

## 概述
ElaReminderCard 是一个现代化的提醒卡片组件,继承自 QPushButton。它提供了图片展示、标题副标题、主题适配等特性。

## 特性
- 图片展示
- 标题副标题
- 主题适配
- 阴影效果
- 圆角边框
- 悬浮效果
- 焦点指示
- 自适应布局

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius 
    READ getBorderRadius 
    WRITE setBorderRadius)

// 标题
Q_PROPERTY(QString title 
    READ getTitle 
    WRITE setTitle)

// 副标题
Q_PROPERTY(QString subTitle 
    READ getSubTitle 
    WRITE setSubTitle)

// 标题字号
Q_PROPERTY(int titlePixelSize 
    READ getTitlePixelSize 
    WRITE setTitlePixelSize)

// 副标题字号
Q_PROPERTY(int subTitlePixelSize 
    READ getSubTitlePixelSize 
    WRITE setSubTitlePixelSize)

// 标题间距
Q_PROPERTY(int titleSpacing 
    READ getTitleSpacing 
    WRITE setTitleSpacing)

// 卡片图片
Q_PROPERTY(QPixmap cardPixmap 
    READ getCardPixmap 
    WRITE setCardPixmap)

// 图片大小
Q_PROPERTY(QSize cardPixmapSize 
    READ getCardPixmapSize 
    WRITE setCardPixmapSize)

// 图片圆角
Q_PROPERTY(int cardPixmapBorderRadius 
    READ getCardPixmapBorderRadius 
    WRITE setCardPixmapBorderRadius)

// 图片模式
Q_PROPERTY(ElaCardPixType::PixMode cardPixMode 
    READ getCardPixMode 
    WRITE setCardPixMode)
```

## 样式系统

### 卡片样式
```cpp
void paintEvent() {
    // 绘制阴影
    eTheme->drawEffectShadow(
        &painter, rect(), 
        shadowWidth, radius
    );
    
    // 绘制背景
    painter.setBrush(
        underMouse() ? hoverColor : baseColor
    );
    painter.drawRoundedRect(
        rect, radius, radius
    );
    
    // 绘制图片
    if (!pixmap.isNull()) {
        switch(pixMode) {
            case Ellipse:
                path.addEllipse(...);
                break;
            case RoundedRect:
                path.addRoundedRect(...);
                break;
            case Default:
                painter.drawPixmap(...);
                break;
        }
    }
    
    // 绘制焦点指示器
    painter.setBrush(primaryColor);
    painter.drawEllipse(
        point, radius, radius
    );
    
    // 绘制标题
    painter.setFont(titleFont);
    painter.drawText(
        rect, Qt::AlignLeft, title
    );
    
    // 绘制副标题
    painter.setFont(subTitleFont);
    painter.drawText(
        rect, Qt::AlignLeft, subTitle
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建卡片
```cpp
// 创建卡片
ElaReminderCard* card = 
    new ElaReminderCard(this);

// 设置大小
card->setFixedSize(330, 105);

// 设置圆角
card->setBorderRadius(6);
```

#### 2. 设置内容
```cpp
// 设置标题
card->setTitle("重要提醒");

// 设置副标题
card->setSubTitle("这是一条重要提醒");

// 设置图片
card->setCardPixmap(pixmap);

// 设置图片大小
card->setCardPixmapSize(79, 79);
```

#### 3. 样式设置
```cpp
// 设置字号
card->setTitlePixelSize(15);
card->setSubTitlePixelSize(12);

// 设置间距
card->setTitleSpacing(3);

// 设置图片模式
card->setCardPixMode(
    ElaCardPixType::PixMode::RoundedRect
);

// 设置图片圆角
card->setCardPixmapBorderRadius(6);
```

## 注意事项
1. 继承自 QPushButton
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 布局优化

## 最佳实践
1. 合理设置大小
2. 控制文本长度
3. 优化图片大小
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QPushButton: 基类
- ElaTheme: 主题系统
- QProxyStyle: 样式系统
- QPainter: 绘制系统
