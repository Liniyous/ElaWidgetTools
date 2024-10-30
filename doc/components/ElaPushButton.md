# ElaPushButton 组件文档

## 概述
ElaPushButton 是一个继承自 QPushButton 的按钮组件,提供了现代化的外观设计和丰富的自定义选项。它支持圆角、阴影效果、多种状态颜色以及明暗主题适配。

## 特性
- 现代化圆角设计
- 内置阴影效果
- 支持悬停和按压状态
- 自动适应明暗主题
- 可自定义边框圆角
- 支持禁用状态样式
- 自定义文本颜色
- 底部边线装饰

## 属性

### BorderRadius
- 类型: `int`
- 默认值: `3`
- 说明: 按钮边框圆角半径

### 主题颜色属性
#### 明亮主题
- `LightDefaultColor`: 默认背景色
- `LightHoverColor`: 悬停状态色
- `LightPressColor`: 按压状态色
- `LightTextColor`: 文本颜色

#### 暗黑主题
- `DarkDefaultColor`: 默认背景色
- `DarkHoverColor`: 悬停状态色
- `DarkPressColor`: 按压状态色
- `DarkTextColor`: 文本颜色

## 构造函数

### 基础构造
```cpp
ElaPushButton(QWidget* parent = nullptr);
```

### 带文本构造
```cpp
ElaPushButton(QString text, QWidget* parent = nullptr);
```

## 公共方法

### 文本颜色设置
```cpp
void setLightTextColor(QColor color);
QColor getLightTextColor() const;

void setDarkTextColor(QColor color);
QColor getDarkTextColor() const;
```

### 主题色设置
```cpp
void setLightDefaultColor(QColor color);
void setDarkDefaultColor(QColor color);
void setLightHoverColor(QColor color);
void setDarkHoverColor(QColor color);
void setLightPressColor(QColor color);
void setDarkPressColor(QColor color);
```

## 使用示例

### 基础用法

#### 1. 创建按钮
```cpp
// 基础创建
ElaPushButton* btn = new ElaPushButton("确定");

// 设置固定宽度
btn->setFixedWidth(120);
```

#### 2. 自定义样式
```cpp
ElaPushButton* btn = new ElaPushButton();
btn->setBorderRadius(6);  // 设置圆角
btn->setLightTextColor(QColor("#1E90FF"));  // 设置明亮主题文本颜色
btn->setDarkTextColor(QColor("#87CEEB"));   // 设置暗黑主题文本颜色
```

### 高级用法

#### 1. 主题色定制
```cpp
ElaPushButton* btn = new ElaPushButton("自定义主题");
// 设置明亮主题颜色
btn->setLightDefaultColor(QColor("#E3F2FD"));
btn->setLightHoverColor(QColor("#BBDEFB"));
btn->setLightPressColor(QColor("#90CAF9"));

// 设置暗黑主题颜色
btn->setDarkDefaultColor(QColor("#1976D2"));
btn->setDarkHoverColor(QColor("#1E88E5"));
btn->setDarkPressColor(QColor("#2196F3"));
```

#### 2. 状态处理
```cpp
ElaPushButton* btn = new ElaPushButton("状态按钮");
// 禁用状态
btn->setEnabled(false);

// 监听点击事件
connect(btn, &ElaPushButton::clicked, this, [=](){
    btn->setEnabled(false);
    // 处理点击事件
    processClick();
    btn->setEnabled(true);
});
```

## 注意事项
1. 按钮默认高度为 38 像素
2. 默认字体大小为 15 像素
3. 阴影边框宽度固定为 3 像素
4. 禁用状态会自动使用主题的 BasicDisable 颜色
5. 底部边线在按压状态下会隐藏
6. 主题切换时会自动更新按钮样式

## 相关组件
- ElaTheme: 主题管理器
- ElaIconButton: 图标按钮
- ElaToolButton: 工具栏按钮
- ElaMessageButton: 消息按钮
