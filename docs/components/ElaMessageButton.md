# ElaMessageButton 组件文档

## 概述
ElaMessageButton 是一个用于触发消息提示的按钮组件,继承自 QPushButton。它提供了便捷的消息配置和触发功能,支持多种消息类型和位置策略。

## 特性
- 消息类型配置
- 位置策略设置
- 主题适配
- 圆角边框
- 阴影效果
- 悬停效果
- 按压效果
- 禁用状态

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 消息标题
Q_PROPERTY(QString barTitle READ getBarTitle 
    WRITE setBarTitle)

// 消息内容
Q_PROPERTY(QString barText READ getBarText 
    WRITE setBarText)

// 显示时长(毫秒)
Q_PROPERTY(int displayMsec READ getDisplayMsec 
    WRITE setDisplayMsec)

// 目标窗口
Q_PROPERTY(QWidget* messageTargetWidget 
    READ getMessageTargetWidget 
    WRITE setMessageTargetWidget)

// 消息类型
Q_PROPERTY(ElaMessageBarType::MessageMode messageMode 
    READ getMessageMode 
    WRITE setMessageMode)

// 位置策略
Q_PROPERTY(ElaMessageBarType::PositionPolicy positionPolicy 
    READ getPositionPolicy 
    WRITE setPositionPolicy)
```

## 构造函数
```cpp
// 默认构造
explicit ElaMessageButton(QWidget* parent = nullptr);

// 带文本构造
explicit ElaMessageButton(QString text, 
    QWidget* parent = nullptr);
```

## 使用示例

### 基础用法

#### 1. 创建按钮
```cpp
// 创建成功消息按钮
ElaMessageButton* btn = new ElaMessageButton("提示");
btn->setBarTitle("成功");
btn->setBarText("操作已完成");
btn->setMessageMode(ElaMessageBarType::Success);
btn->setPositionPolicy(ElaMessageBarType::TopRight);
btn->setDisplayMsec(2000);
```

#### 2. 自定义样式
```cpp
// 设置圆角和尺寸
btn->setBorderRadius(5);
btn->setFixedSize(100, 40);

// 设置字体
QFont font = btn->font();
font.setPixelSize(15);
btn->setFont(font);
```

### 高级用法

#### 1. 多类型消息
```cpp
// 错误消息按钮
ElaMessageButton* errorBtn = new ElaMessageButton();
errorBtn->setMessageMode(ElaMessageBarType::Error);
errorBtn->setBarTitle("错误");
errorBtn->setBarText("网络连接失败");
errorBtn->setDisplayMsec(3000);

// 警告消息按钮
ElaMessageButton* warnBtn = new ElaMessageButton();
warnBtn->setMessageMode(ElaMessageBarType::Warning);
warnBtn->setBarTitle("警告");
warnBtn->setBarText("磁盘空间不足");
```

#### 2. 指定目标窗口
```cpp
// 在指定窗口显示消息
QWidget* targetWindow = new QWidget();
btn->setMessageTargetWidget(targetWindow);
```

## 样式系统

### 按钮样式
```cpp
void paintEvent(QPaintEvent* event) {
    // 绘制阴影
    eTheme->drawEffectShadow(
        &painter, 
        rect(), 
        shadowWidth, 
        borderRadius
    );
    
    // 绘制背景
    painter.setBrush(isEnabled() ? 
        underMouse() ? hoverColor : baseColor 
        : disableColor);
    painter.drawRoundedRect(
        foregroundRect, 
        borderRadius, 
        borderRadius
    );
    
    // 绘制文本
    painter.setPen(isEnabled() ? 
        isPressed ? pressTextColor : textColor 
        : disableTextColor);
    painter.drawText(rect(), 
        Qt::AlignCenter, 
        text()
    );
}
```

## 注意事项
1. 继承自 QPushButton
2. 自动处理主题切换
3. 智能内存管理
4. 样式继承
5. 事件处理
6. 性能优化
7. 主题适配
8. 状态管理

## 最佳实践
1. 合理设置时长
2. 选择合适位置
3. 简洁消息文本
4. 处理窗口变化
5. 注意性能影响
6. 统一按钮样式
7. 合理使用禁用
8. 保持交互一致

## 相关组件
- QPushButton: 基类
- ElaMessageBar: 消息组件
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
