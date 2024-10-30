# ElaAcrylicUrlCard 组件文档

## 概述
ElaAcrylicUrlCard 是一个基于 Qt 的亚克力效果 URL 卡片组件，继承自 QPushButton。它提供了一个带有图片、标题、副标题的交互式卡片，支持点击跳转到指定 URL。

## 特性
- 亚克力磨砂玻璃效果背景
- 可自定义图片及其显示模式
- 支持标题和副标题文本
- 支持 URL 跳转
- 支持明暗主题切换
- 鼠标悬停效果
- 右下角带有跳转图标

## 属性

### 基础属性

#### BorderRadius
- 类型: `int`
- 默认值: `5`
- 说明: 卡片边框圆角半径

#### MainOpacity
- 类型: `qreal`
- 默认值: `0.95`
- 说明: 主体背景透明度

#### NoiseOpacity
- 类型: `qreal`
- 默认值: `0.06`
- 说明: 噪点效果透明度

#### BrushAlpha
- 类型: `int`
- 默认值: `245`
- 说明: 画刷透明度

### 文本相关属性

#### Title
- 类型: `QString`
- 说明: 卡片标题文本

#### SubTitle
- 类型: `QString`
- 说明: 卡片副标题文本

#### TitlePixelSize
- 类型: `int`
- 默认值: `15`
- 说明: 标题文字大小

#### SubTitlePixelSize
- 类型: `int`
- 默认值: `12`
- 说明: 副标题文字大小

#### TitleSpacing
- 类型: `int`
- 默认值: `10`
- 说明: 标题间距

#### SubTitleSpacing
- 类型: `int`
- 默认值: `10`
- 说明: 副标题间距

### 图片相关属性

#### CardPixmap
- 类型: `QPixmap`
- 说明: 卡片显示的图片

#### CardPixmapSize
- 类型: `QSize`
- 默认值: `QSize(54, 54)`
- 说明: 图片显示大小

#### CardPixmapBorderRadius
- 类型: `int`
- 默认值: `6`
- 说明: 图片边框圆角半径

#### CardPixMode
- 类型: `ElaCardPixType::PixMode`
- 默认值: `ElaCardPixType::PixMode::Ellipse`
- 说明: 图片显示模式
- 可选值:
  - `Ellipse`: 圆形
  - `Default`: 默认矩形
  - `RoundedRect`: 圆角矩形

### URL 相关属性

#### Url
- 类型: `QString`
- 说明: 点击卡片跳转的 URL

## 方法

### setCardPixmapSize
设置卡片图片大小。

```cpp
void setCardPixmapSize(int width, int height);
```

## 使用示例

### 基础用法
```cpp
// 创建卡片
ElaAcrylicUrlCard* urlCard = new ElaAcrylicUrlCard(this);

// 设置基本属性
urlCard->setFixedSize(195, 225);
urlCard->setCardPixmapSize(QSize(62, 62));

// 设置内容
urlCard->setTitle("GitHub");
urlCard->setSubTitle("Visit our GitHub repository");
urlCard->setUrl("https://github.com/example/repo");
urlCard->setCardPixmap(QPixmap(":/images/github.png"));
```

### 自定义样式
```cpp
ElaAcrylicUrlCard* urlCard = new ElaAcrylicUrlCard(this);

// 自定义外观
urlCard->setBorderRadius(10);
urlCard->setMainOpacity(0.9);
urlCard->setNoiseOpacity(0.08);

// 自定义文字样式
urlCard->setTitlePixelSize(17);
urlCard->setTitleSpacing(25);
urlCard->setSubTitleSpacing(13);

// 自定义图片样式
urlCard->setCardPixmapBorderRadius(8);
urlCard->setCardPixMode(ElaCardPixType::PixMode::RoundedRect);
```

### 添加工具提示
```cpp
ElaAcrylicUrlCard* urlCard = new ElaAcrylicUrlCard(this);
// ... 设置其他属性 ...

// 添加工具提示
ElaToolTip* toolTip = new ElaToolTip(urlCard);
toolTip->setToolTip("点击访问网站");
```

## 注意事项
1. 组件默认大小为 180x200，可通过 setFixedSize 调整
2. 图片默认以圆形方式显示，可通过 CardPixMode 更改显示方式
3. 点击卡片会自动使用默认浏览器打开设置的 URL
4. 组件会自动适应明暗主题切换
5. 鼠标悬停时会显示边框高亮效果
6. 右下角会自动显示一个跳转图标

## 相关组件
- ElaToolTip: 可配合使用添加工具提示
- ElaTheme: 主题管理器，控制明暗主题