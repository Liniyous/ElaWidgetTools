# ElaInteractiveCard 组件文档

## 概述
ElaInteractiveCard 是一个交互式卡片组件,继承自 QPushButton。它提供了图标、标题、副标题的组合展示,支持多种图标样式和主题适配。

## 特性
- 鼠标悬停效果
- 多种图标样式
- 标题和副标题
- 圆角边框
- 主题适配
- 抗锯齿渲染
- 自动布局
- 文本自动换行

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 标题
Q_PROPERTY(QString title READ getTitle 
    WRITE setTitle)

// 副标题
Q_PROPERTY(QString subTitle READ getSubTitle 
    WRITE setSubTitle)

// 图标
Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap 
    WRITE setCardPixmap)
```

### 样式属性
```cpp
// 标题字号
Q_PROPERTY(int titlePixelSize READ getTitlePixelSize 
    WRITE setTitlePixelSize)

// 副标题字号
Q_PROPERTY(int subTitlePixelSize 
    READ getSubTitlePixelSize 
    WRITE setSubTitlePixelSize)

// 标题间距
Q_PROPERTY(int titleSpacing READ getTitleSpacing 
    WRITE setTitleSpacing)

// 图标尺寸
Q_PROPERTY(QSize cardPixmapSize 
    READ getCardPixmapSize 
    WRITE setCardPixmapSize)

// 图标圆角
Q_PROPERTY(int cardPixmapBorderRadius 
    READ getCardPixmapBorderRadius 
    WRITE setCardPixmapBorderRadius)

// 图标模式
Q_PROPERTY(ElaCardPixType::PixMode cardPixMode 
    READ getCardPixMode 
    WRITE setCardPixMode)
```

## 默认值
- 最小尺寸: 270x80
- 边框圆角: 6px
- 标题字号: 15px
- 副标题字号: 12px
- 图标尺寸: 64x64
- 标题间距: 2px
- 图标圆角: 6px
- 图标模式: Ellipse

## 使用示例

### 基础用法

#### 1. 创建卡片
```cpp
// 创建卡片
ElaInteractiveCard* card = new ElaInteractiveCard(this);

// 设置文本
card->setTitle("主标题");
card->setSubTitle("副标题描述");

// 设置图标
card->setCardPixmap(QPixmap(":/icons/avatar.png"));

// 连接点击信号
connect(card, &ElaInteractiveCard::clicked, 
    this, &MyWidget::onCardClicked);
```

#### 2. 自定义样式
```cpp
// 设置图标样式
card->setCardPixMode(ElaCardPixType::PixMode::RoundedRect);
card->setCardPixmapBorderRadius(10);
card->setCardPixmapSize(QSize(48, 48));

// 设置文本样式
card->setTitlePixelSize(16);
card->setSubTitlePixelSize(13);
card->setTitleSpacing(4);
```

### 高级用法

#### 1. 图标模式
```cpp
// 圆形图标
card->setCardPixMode(ElaCardPixType::PixMode::Ellipse);

// 圆角矩形
card->setCardPixMode(ElaCardPixType::PixMode::RoundedRect);

// 默认矩形
card->setCardPixMode(ElaCardPixType::PixMode::Default);
```

#### 2. 主题适配
```cpp
// 连接主题变化信号
connect(ElaTheme::getInstance(), 
    &ElaTheme::themeModeChanged,
    this, [=](ElaThemeType::ThemeMode mode) {
        update();
});
```

## 布局算法

### 组件布局
```cpp
// 图标位置
int iconX = 10;
int iconY = (height() - iconSize.height()) / 2;

// 文本起始位置
int textStartX = iconSize.width() + 26;
int textWidth = width() - textStartX;

// 标题区域
QRect titleRect(
    textStartX,
    rect().y(),
    textWidth,
    height() / 2 - titleSpacing
);

// 副标题区域
QRect subTitleRect(
    textStartX,
    height() / 2 + titleSpacing,
    textWidth,
    height() / 2 - titleSpacing
);
```

## 注意事项
1. 继承自 QPushButton
2. 自动处理鼠标事件
3. 支持文本自动换行
4. 图标居中显示
5. 主题切换自动更新
6. 抗锯齿渲染
7. 标题自动加粗
8. 性能优化

## 最佳实践
1. 合理设置文本长度
2. 选择合适的图标尺寸
3. 注意图标质量
4. 控制卡片大小
5. 处理主题切换
6. 优化点击响应
7. 合理使用间距
8. 保持样式一致

## 相关组件
- QPushButton: 基类
- QPainter: 绘制系统
- QPainterPath: 路径绘制
- ElaTheme: 主题管理
