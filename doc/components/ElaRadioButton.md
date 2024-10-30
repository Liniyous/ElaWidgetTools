# ElaRadioButton 组件文档

## 概述
ElaRadioButton 是一个现代化的单选按钮组件,继承自 QRadioButton。它提供了动画效果、主题适配、悬浮反馈等特性。

## 特性
- 动画效果
- 主题适配
- 悬浮反馈
- 点击效果
- 圆形样式
- 禁用状态
- 文本支持
- 自适应大小

## 构造函数
```cpp
// 默认构造
explicit ElaRadioButton(
    QWidget* parent = nullptr
);

// 带文本构造
explicit ElaRadioButton(
    const QString& text,
    QWidget* parent = nullptr
);
```

## 样式系统

### 基础样式
```cpp
void drawPrimitive(
    PrimitiveElement element,
    const QStyleOption* option,
    QPainter* painter,
    const QWidget* widget
) {
    // 未选中状态
    if (state & State_Off) {
        // 绘制边框
        painter.setPen(borderColor);
        
        // 绘制背景
        if (state & State_MouseOver) {
            painter.setBrush(hoverColor);
        } else {
            painter.setBrush(baseColor);
        }
        
        // 绘制圆形
        painter.drawEllipse(
            center, 8.5, 8.5
        );
    }
    // 选中状态
    else {
        // 绘制外圆
        painter.setBrush(primaryColor);
        painter.drawEllipse(
            center, radius, radius
        );
        
        // 绘制内圆
        painter.setBrush(invertColor);
        if (state & State_MouseOver) {
            if (state & State_Sunken) {
                radius = width/4.5;
            } else {
                radius = width/3.5;
            }
        } else {
            radius = width/4;
        }
        painter.drawEllipse(
            center, radius, radius
        );
    }
}
```

### 度量系统
```cpp
int pixelMetric(
    PixelMetric metric,
    const QStyleOption* option,
    const QWidget* widget
) {
    // 指示器宽度
    case PM_ExclusiveIndicatorWidth:
        return 20;
        
    // 指示器高度
    case PM_ExclusiveIndicatorHeight:
        return 20;
}
```

## 使用示例

### 基础用法

#### 1. 创建单选按钮
```cpp
// 创建按钮
ElaRadioButton* radio = 
    new ElaRadioButton(this);

// 设置文本
radio->setText("选项1");

// 设置状态
radio->setChecked(true);
```

#### 2. 按钮组
```cpp
// 创建按钮组
QButtonGroup* group = 
    new QButtonGroup(this);

// 添加按钮
group->addButton(radio1);
group->addButton(radio2);
group->addButton(radio3);

// 设置互斥
group->setExclusive(true);
```

#### 3. 处理事件
```cpp
// 状态改变
connect(radio, 
    &ElaRadioButton::toggled,
    this, [=](bool checked) {
    qDebug() << "Checked:" << checked;
});
```

## 注意事项
1. 继承自 QRadioButton
2. 自动处理主题切换
3. 固定高度20px
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理分组
2. 控制数量
3. 统一样式
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QRadioButton: 基类
- QButtonGroup: 按钮组
- ElaTheme: 主题管理
- QProxyStyle: 样式系统
