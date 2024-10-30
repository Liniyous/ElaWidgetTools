# ElaTheme 组件文档

## 概述
ElaTheme 是一个全局主题管理组件,采用单例模式设计。它提供了完整的明暗主题切换方案,以及丰富的主题色彩系统,可以轻松实现应用程序的主题定制。

## 特性
- 支持明暗主题动态切换
- 提供完整的主题色彩系统
- 支持自定义主题颜色
- 提供阴影效果绘制
- 全局单例访问
- 主题切换事件通知

## 主题色系统

### 基础颜色
| 颜色类型 | 明亮模式 | 暗黑模式 | 用途 |
|---------|---------|----------|------|
| BasicText | #000000 | #FFFFFF | 基础文本颜色 |
| BasicTextInvert | #FFFFFF | #000000 | 反色文本 |
| BasicDetailsText | #878787 | #ADADB0 | 详细说明文本 |
| BasicTextNoFocus | #86868A | #86868A | 未聚焦文本 |
| BasicTextDisable | #B6B6B6 | #A7A7A7 | 禁用状态文本 |
| BasicTextPress | #5A5A5D | #BBBBBF | 按下状态文本 |

### 边框颜色
| 颜色类型 | 明亮模式 | 暗黑模式 | 用途 |
|---------|---------|----------|------|
| BasicBorder | #E5E5E5 | #4B4B4B | 基础边框 |
| BasicBorderDeep | #A8A8A8 | #5C5C5C | 深色边框 |
| BasicBorderHover | #DADADA | #575757 | 悬停边框 |

### 背景颜色
| 颜色类型 | 明亮模式 | 暗黑模式 | 用途 |
|---------|---------|----------|------|
| WindowBase | #F3F3F3 | #202020 | 窗口背景 |
| DialogBase | #FFFFFF | #1F1F1F | 对话框背景 |
| PopupBase | #FAFAFA | #2C2C2C | 弹出窗口背景 |
| BasicBase | #FDFDFD | #343434 | 基础背景 |

## 属性和方法

### ThemeMode
当前主题模式。

**类型:** `ElaThemeType::ThemeMode`  
**可选值:** 
- `Light`: 明亮主题
- `Dark`: 暗黑主题

**访问方法:**
```cpp
void setThemeMode(ElaThemeType::ThemeMode themeMode);
ElaThemeType::ThemeMode getThemeMode() const;
```

### 主题色获取与设置

#### getThemeColor()
获取指定主题模式下的主题颜色。

```cpp
const QColor& getThemeColor(ElaThemeType::ThemeMode themeMode, 
                          ElaThemeType::ThemeColor themeColor);
```

#### setThemeColor()
设置指定主题模式下的主题颜色。

```cpp
void setThemeColor(ElaThemeType::ThemeMode themeMode,
                  ElaThemeType::ThemeColor themeColor, 
                  QColor newColor);
```

### 特效绘制

#### drawEffectShadow()
绘制阴影效果。

```cpp
void drawEffectShadow(QPainter* painter, 
                     QRect widgetRect,
                     int shadowBorderWidth, 
                     int borderRadius);
```

## 使用示例

### 基础用法

#### 1. 获取主题色
```cpp
// 使用宏简化调用
QColor textColor = ElaThemeColor(ElaThemeType::Light, BasicText);

// 或直接调用方法
QColor textColor = eTheme->getThemeColor(ElaThemeType::Light, 
                                       ElaThemeType::BasicText);
```

#### 2. 切换主题
```cpp
// 切换到暗色主题
eTheme->setThemeMode(ElaThemeType::Dark);

// 监听主题变化
connect(eTheme, &ElaTheme::themeModeChanged, this, 
        [this](ElaThemeType::ThemeMode mode) {
    updateThemeStyle(mode);
});
```

#### 3. 自定义主题色
```cpp
// 修改明亮主题下的主要颜色
eTheme->setThemeColor(ElaThemeType::Light,
                     ElaThemeType::PrimaryNormal,
                     QColor("#1E90FF"));
```

### 高级用法

#### 1. 绘制阴影
```cpp
void CustomWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // 绘制阴影效果
    eTheme->drawEffectShadow(&painter, rect(), 10, 4);
    
    // 继续绘制其他内容
    // ...
}
```

#### 2. 主题色系统扩展
```cpp
// 添加自定义主题色
enum CustomThemeColor {
    CustomPrimary = ElaThemeType::ThemeColorCount,
    CustomSecondary
};

// 设置自定义主题色
eTheme->setThemeColor(ElaThemeType::Light, 
                     CustomPrimary,
                     QColor("#FF6B6B"));
```

## 注意事项
1. 组件采用单例模式,通过 `eTheme` 宏访问实例
2. 主题切换会触发 themeModeChanged 信号
3. 自定义主题色需要在应用启动时设置
4. 阴影绘制会影响性能,建议适度使用
5. 颜色值的 Alpha 通道会影响最终显示效果

## 相关组件
- ElaApplication: 程序初始化组件
- ElaWindow: 支持主题的窗口组件
- ElaWidget: 支持主题的基础组件
