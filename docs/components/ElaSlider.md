# ElaSlider 组件文档

## 概述
ElaSlider 是一个现代化的滑块组件,继承自 QSlider。它提供了自定义样式、动画效果、主题适配等特性。

## 特性
- 自定义样式
- 动画效果
- 主题适配
- 平滑滑动
- 圆形滑块
- 渐变滑槽
- 悬浮效果
- 点击定位

## 构造函数
```cpp
// 基础构造
explicit ElaSlider(
    QWidget* parent = nullptr
);

// 指定方向构造
explicit ElaSlider(
    Qt::Orientation orientation, 
    QWidget* parent = nullptr
);
```

## 样式系统

### 滑块样式
```cpp
void drawComplexControl(
    ComplexControl control, 
    const QStyleOptionComplex* option, 
    QPainter* painter, 
    const QWidget* widget
) const override {
    // 绘制滑槽
    painter->setBrush(
        ElaThemeColor(themeMode, BasicChute)
    );
    painter->drawRoundedRect(...);
    
    // 绘制已滑过区域
    painter->setBrush(
        ElaThemeColor(themeMode, PrimaryNormal)
    );
    painter->drawRoundedRect(...);
    
    // 绘制滑块
    painter->setBrush(
        ElaThemeColor(themeMode, BasicBase)
    );
    painter->drawEllipse(...);
}
```

### 动画效果
```cpp
void startRadiusAnimation(
    qreal startRadius, 
    qreal endRadius, 
    QWidget* widget
) const {
    // 创建动画
    auto animation = new QPropertyAnimation(
        this, 
        "circleRadius"
    );
    
    // 设置动画属性
    animation->setEasingCurve(
        QEasingCurve::InOutSine
    );
    animation->setStartValue(startRadius);
    animation->setEndValue(endRadius);
    
    // 启动动画
    animation->start(
        QAbstractAnimation::DeleteWhenStopped
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建滑块
```cpp
// 创建水平滑块
ElaSlider* slider = 
    new ElaSlider(Qt::Horizontal, this);

// 设置范围
slider->setRange(0, 100);

// 设置步长
slider->setSingleStep(1);

// 设置页步长
slider->setPageStep(10);
```

#### 2. 值处理
```cpp
// 连接值变化信号
connect(slider, 
    &ElaSlider::valueChanged,
    this, [=](int value) {
    // 处理值变化
});

// 设置当前值
slider->setValue(50);

// 获取当前值
int value = slider->value();
```

### 高级用法

#### 1. 自定义外观
```cpp
// 设置方向
slider->setOrientation(Qt::Vertical);

// 设置刻度位置
slider->setTickPosition(
    QSlider::TicksBothSides
);

// 设置刻度间隔
slider->setTickInterval(10);
```

#### 2. 事件处理
```cpp
// 鼠标按下事件
void mousePressEvent(
    QMouseEvent* event
) override {
    if (event->button() == Qt::LeftButton) {
        // 计算点击位置对应的值
        double pos = event->pos().x() / 
            (double)width();
        setValue(minimum() + 
            (maximum() - minimum()) * pos);
    }
}
```

## 样式定制

### 1. 尺寸设置
```cpp
int pixelMetric(
    PixelMetric metric,
    const QStyleOption* option,
    const QWidget* widget
) const override {
    switch (metric) {
    case PM_SliderLength:
        return 20;
    case PM_SliderThickness:
        return 20;
    default:
        break;
    }
    return QProxyStyle::pixelMetric(
        metric, 
        option, 
        widget
    );
}
```

### 2. 行为设置
```cpp
int styleHint(
    StyleHint hint,
    const QStyleOption* option,
    const QWidget* widget,
    QStyleHintReturn* returnData
) const override {
    if (hint == SH_Slider_AbsoluteSetButtons) {
        return Qt::LeftButton;
    }
    return QProxyStyle::styleHint(
        hint, 
        option, 
        widget, 
        returnData
    );
}
```

## 注意事项
1. 继承自 QSlider
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 动画流畅

## 最佳实践
1. 合理设置范围
2. 优化步进值
3. 控制动画时长
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QSlider: 基类
- ElaSliderStyle: 样式组件
- ElaTheme: 主题系统
- QPropertyAnimation: 动画系统
