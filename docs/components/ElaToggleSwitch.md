# ElaToggleSwitch 组件文档

## 概述
ElaToggleSwitch 是一个现代化的开关组件,提供了流畅的动画效果、主题适配、手势支持等特性。它适用于开关状态的切换场景。

## 特性
- 动画效果
- 主题适配
- 手势支持
- 圆角边框
- 悬浮效果
- 点击反馈
- 禁用状态
- 自适应大小

## 属性

### 基础属性
```cpp
// 开关状态
void setIsToggled(bool isToggled);
bool getIsToggled() const;
```

## 信号
```cpp
// 状态改变
Q_SIGNAL void toggled(bool checked);
```

## 动画系统

### 位置动画
```cpp
void _startPosAnimation(
    qreal startX, 
    qreal endX, 
    bool isToggle
) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            this, "circleCenterX"
        );
    animation->setStartValue(startX);
    animation->setEndValue(endX);
    animation->setEasingCurve(
        QEasingCurve::InOutSine
    );
    animation->start();
}
```

### 半径动画
```cpp
void _startRadiusAnimation(
    qreal startRadius, 
    qreal endRadius
) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            this, "circleRadius"
        );
    animation->setStartValue(startRadius);
    animation->setEndValue(endRadius);
    animation->setEasingCurve(
        QEasingCurve::InOutSine
    );
    animation->start();
}
```

## 渲染系统

### 背景渲染
```cpp
void paintEvent() {
    // 绘制背景路径
    QPainterPath path;
    path.moveTo(width - height - margin, 
        height - margin);
    path.arcTo(rightRect, -90, 180);
    path.lineTo(height/2 + margin, margin);
    path.arcTo(leftRect, 90, 180);
    path.closeSubpath();
    
    // 设置样式
    painter.setPen(isToggled ? 
        borderColor : borderDeepColor);
    painter.setBrush(isEnabled ? 
        (isToggled ? primaryColor : 
            (hover ? hoverColor : baseColor)) 
        : disableColor);
    
    // 绘制背景
    painter.drawPath(path);
}
```

### 圆心渲染
```cpp
void paintEvent() {
    // 设置样式
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled ? 
        (isToggled ? textInvertColor : 
            centerColor) 
        : textDisableColor);
    
    // 绘制圆心
    painter.drawEllipse(
        QPointF(circleCenterX, height/2),
        circleRadius, 
        circleRadius
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建开关
```cpp
// 创建开关
ElaToggleSwitch* toggle = 
    new ElaToggleSwitch(this);

// 设置状态
toggle->setIsToggled(true);

// 获取状态
bool isOn = toggle->getIsToggled();
```

#### 2. 处理事件
```cpp
// 状态改变
connect(toggle, &ElaToggleSwitch::toggled,
    this, [=](bool checked) {
    qDebug() << "Toggled:" << checked;
});
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
2. 注意性能影响
3. 统一交互方式
4. 保持样式一致
5. 动画流畅性
6. 响应式布局
7. 优化用户体验
8. 主题适配

## 相关组件
- QWidget: 基类
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
- QPainterPath: 绘制系统
