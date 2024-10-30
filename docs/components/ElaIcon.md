# ElaIcon & ElaIconButton 组件文档

## 概述
ElaIcon 是一个基于 Qt 的图标管理组件,提供了一套完整的图标字体解决方案。ElaIconButton 则是基于 ElaIcon 实现的图标按钮组件,继承自 QPushButton。两者结合使用可以轻松实现各种图标按钮效果。

## 特性
- 内置大量常用图标字体
- 支持自定义图标大小
- 支持 PNG/JPG 等位图图标
- 支持明暗主题动态切换
- 支持鼠标悬停动画效果
- 支持圆角和透明度设置
- 支持选中状态

## ElaIcon 属性与方法

### 静态方法

#### createIcon()
创建图标实例。

**语法:**
```cpp
static QIcon createIcon(ElaIconType::IconName awesome, 
                       int pixelSize = 15,
                       QColor iconColor = QColor());
```

**参数:**
- `awesome`: 图标枚举值
- `pixelSize`: 图标大小(像素)
- `iconColor`: 图标颜色

**返回值:**
- `QIcon`: 创建的图标对象

## ElaIconButton 属性

### BorderRadius
- 类型: `int`
- 默认值: `0`
- 说明: 按钮边框圆角半径

### Opacity
- 类型: `qreal`
- 默认值: `1.0`
- 说明: 按钮整体透明度

### IsSelected
- 类型: `bool`
- 默认值: `false`
- 说明: 按钮选中状态

### 主题相关属性

#### LightHoverColor / DarkHoverColor
- 类型: `QColor`
- 默认值: 主题色 + BasicHoverAlpha
- 说明: 鼠标悬停时的背景色

#### LightIconColor / DarkIconColor  
- 类型: `QColor`
- 默认值: 主题色 + BasicText
- 说明: 图标颜色

#### LightHoverIconColor / DarkHoverIconColor
- 类型: `QColor`
- 默认值: 主题色 + BasicText
- 说明: 鼠标悬停时的图标颜色

## ElaIconButton 构造函数

### 使用图片构造
```cpp
ElaIconButton(QPixmap pix, QWidget* parent = nullptr);
```

### 使用图标字体构造
```cpp
// 基础构造
ElaIconButton(ElaIconType::IconName awesome, QWidget* parent = nullptr);

// 指定大小构造
ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent = nullptr);

// 指定大小和固定尺寸构造
ElaIconButton(ElaIconType::IconName awesome, int pixelSize, 
              int fixedWidth, int fixedHeight, QWidget* parent = nullptr);
```

## 使用示例

### 基础用法

#### 1. 创建图标按钮
```cpp
// 使用图标字体
ElaIconButton* btn1 = new ElaIconButton(ElaIconType::Close);

// 使用图片
ElaIconButton* btn2 = new ElaIconButton(QPixmap(":/images/icon.png"));

// 自定义大小
ElaIconButton* btn3 = new ElaIconButton(ElaIconType::Settings, 20);

// 固定尺寸
ElaIconButton* btn4 = new ElaIconButton(ElaIconType::Search, 16, 32, 32);
```

#### 2. 自定义样式
```cpp
ElaIconButton* btn = new ElaIconButton(ElaIconType::Home);
btn->setBorderRadius(4);
btn->setOpacity(0.9);
btn->setLightHoverColor(QColor(0, 0, 0, 20));
btn->setDarkHoverColor(QColor(255, 255, 255, 20));
```

#### 3. 处理选中状态
```cpp
ElaIconButton* btn = new ElaIconButton(ElaIconType::Star);
btn->setIsSelected(true);
connect(btn, &ElaIconButton::clicked, this, [=](){
    btn->setIsSelected(!btn->getIsSelected());
});
```

### 高级用法

#### 1. 动态切换图标
```cpp
class CustomButton : public ElaIconButton {
public:
    CustomButton() : ElaIconButton(ElaIconType::Play) {
        connect(this, &CustomButton::clicked, this, &CustomButton::toggleIcon);
    }
    
    void toggleIcon() {
        static bool isPlaying = false;
        isPlaying = !isPlaying;
        setAwesome(isPlaying ? ElaIconType::Pause : ElaIconType::Play);
    }
};
```

#### 2. 主题适配
```cpp
ElaIconButton* btn = new ElaIconButton(ElaIconType::Sun);
connect(eTheme, &ElaTheme::themeModeChanged, btn, [=](ElaThemeType::ThemeMode mode){
    btn->setAwesome(mode == ElaThemeType::Light ? ElaIconType::Sun : ElaIconType::Moon);
});
```

## 注意事项
1. 使用前需确保已加载 "ElaAwesome.ttf" 字体文件
2. 图标按钮默认无边框,需要边框效果请设置 BorderRadius
3. 选中状态会影响按钮的外观表现
4. 主题切换时会自动更新按钮样式
5. 动画效果的持续时间固定为 175ms
6. 禁用状态下会使用 BasicTextDisable 颜色

## 相关组件
- ElaTheme: 主题管理器
- ElaPushButton: 基础按钮组件
- ElaToolButton: 工具栏按钮组件
