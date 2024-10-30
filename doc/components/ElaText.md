# ElaText 组件文档

## 概述
ElaText 是一个继承自 QLabel 的文本显示组件,提供了更丰富的文本样式和排版功能。它支持多种预设文本样式、自定义字体大小、字间距调整以及自动换行等特性。

## 特性
- 支持多种预设文本样式
- 自定义字体大小(像素/点)
- 可调整字间距
- 支持自动换行
- 支持任意位置换行
- 自动适应明暗主题
- 透明背景

## 属性

### TextStyle
文本样式预设。

**类型:** `ElaTextType::TextStyle`  
**默认值:** `NoStyle`  
**可选值:**
- `NoStyle`: 无样式
- `Caption`: 说明文本 (12px)
- `Body`: 正文文本 (14px)
- `BodyStrong`: 加粗正文 (14px Bold)
- `Subtitle`: 副标题 (20px)
- `Title`: 标题 (28px DemiBold)
- `TitleLarge`: 大标题 (40px DemiBold)
- `Display`: 展示文本 (48px DemiBold)

### IsWrapAnywhere
是否允许在任意位置换行。

**类型:** `bool`  
**默认值:** `false`

### TextPixelSize
文本大小(像素)。

**类型:** `int`  
**默认值:** 根据样式决定

## 构造函数

### 基础构造
```cpp
ElaText(QWidget* parent = nullptr);
```

### 带文本构造
```cpp
ElaText(QString text, QWidget* parent = nullptr);
```

### 带文本和大小构造
```cpp
ElaText(QString text, int pixelSize, QWidget* parent = nullptr);
```

## 公共方法

### 文本样式设置
```cpp
void setTextStyle(ElaTextType::TextStyle textStyle);
ElaTextType::TextStyle getTextStyle() const;
```

### 字体大小设置
```cpp
void setTextPixelSize(int size);
int getTextPixelSize() const;
void setTextPointSize(int size);
int getTextPointSize() const;
```

### 换行控制
```cpp
void setIsWrapAnywhere(bool isWrapAnywhere);
bool getIsWrapAnywhere() const;
```

## 使用示例

### 基础用法

#### 1. 创建文本
```cpp
// 基础创建
ElaText* text = new ElaText("Hello World");

// 指定大小
ElaText* text = new ElaText("Hello World", 16);

// 设置样式
text->setTextStyle(ElaTextType::Title);
```

#### 2. 自动换行
```cpp
ElaText* text = new ElaText();
text->setText("这是一段很长的文本，需要自动换行显示...");
text->setWordWrap(true);
text->setIsWrapAnywhere(true);  // 允许在任意位置换行
```

### 高级用法

#### 1. 组合样式
```cpp
ElaText* text = new ElaText();
text->setTextStyle(ElaTextType::BodyStrong);
text->setTextPixelSize(18);  // 覆盖预设大小
```

#### 2. 主题适配
```cpp
// 主题切换会自动更新文本颜色
connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode mode){
    // 可以在这里添加额外的主题相关处理
    updateCustomStyle();
});
```

## 注意事项
1. 设置 TextStyle 会覆盖之前的字体大小设置
2. IsWrapAnywhere 需要配合 WordWrap 使用才能生效
3. 组件会自动处理主题切换，无需手动更新文本颜色
4. 背景默认透明，可以通过样式表修改
5. 字间距默认为 0.5 像素

## 相关组件
- ElaTheme: 主题管理器
- ElaLineEdit: 文本输入框
- ElaPlainTextEdit: 多行文本编辑框
