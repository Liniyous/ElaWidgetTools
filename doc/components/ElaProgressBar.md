# ElaProgressBar 组件文档

## 概述
ElaProgressBar 是一个现代化的进度条组件,继承自 QProgressBar。它提供了主题适配、动画效果、忙碌状态等增强特性。

## 特性
- 主题适配
- 动画效果
- 忙碌状态
- 水平/垂直
- 圆角边框
- 平滑过渡
- 反向显示
- 自定义样式

## 属性继承
```cpp
// 继承自 QProgressBar 的属性
Q_PROPERTY(int minimum READ minimum 
    WRITE setMinimum)
Q_PROPERTY(int maximum READ maximum 
    WRITE setMaximum)
Q_PROPERTY(int value READ value 
    WRITE setValue)
Q_PROPERTY(QString format READ format 
    WRITE setFormat)
Q_PROPERTY(Qt::Alignment alignment 
    READ alignment 
    WRITE setAlignment)
Q_PROPERTY(bool textVisible READ isTextVisible 
    WRITE setTextVisible)
Q_PROPERTY(Qt::Orientation orientation 
    READ orientation 
    WRITE setOrientation)
Q_PROPERTY(bool invertedAppearance 
    READ invertedAppearance 
    WRITE setInvertedAppearance)
```

## 动画系统

### 忙碌动画
```cpp
void paintEvent() {
    if (minimum() == 0 && maximum() == 0) {
        // 创建忙碌动画
        QPropertyAnimation* busyAnimation = 
            new QPropertyAnimation(
                style, 
                "busyStartValue"
            );
        busyAnimation->setDuration(2000);
        busyAnimation->setLoopCount(-1);
        busyAnimation->setStartValue(-75);
        busyAnimation->setEndValue(width());
        busyAnimation->start();
    }
}
```

## 样式系统

### 轨道样式
```cpp
void drawControl(CE_ProgressBarGroove) {
    // 绘制背景轨道
    painter->setPen(Qt::NoPen);
    painter->setBrush(chuteColor);
    painter->drawRoundedRect(
        rect, 3, 3);
}
```

### 进度样式
```cpp
void drawControl(CE_ProgressBarContents) {
    // 绘制进度
    painter->setPen(Qt::NoPen);
    painter->setBrush(primaryColor);
    
    if (busy) {
        // 忙碌状态动画
        painter->drawRoundedRect(
            QRectF(startValue, y, 
                busyWidth, height), 
            3, 3);
    } else {
        // 正常进度
        painter->drawRoundedRect(
            QRectF(x, y, 
                width * ratio, height), 
            3, 3);
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建进度条
```cpp
// 创建进度条
ElaProgressBar* bar = 
    new ElaProgressBar(this);

// 设置范围和值
bar->setMinimum(0);
bar->setMaximum(100);
bar->setValue(50);

// 设置方向
bar->setOrientation(Qt::Horizontal);
```

#### 2. 忙碌状态
```cpp
// 设置为忙碌状态
bar->setMinimum(0);
bar->setMaximum(0);

// 恢复正常状态
bar->setMinimum(0);
bar->setMaximum(100);
```

#### 3. 反向显示
```cpp
// 设置反向显示
bar->setInvertedAppearance(true);

// 显示文本
bar->setTextVisible(true);
bar->setFormat("%p%");
```

## 注意事项
1. 继承自 QProgressBar
2. 自动处理主题切换
3. 动画时长2000ms
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置范围
2. 平滑更新值
3. 注意性能影响
4. 统一交互方式
5. 保持样式一致
6. 动画流畅性
7. 响应式布局
8. 优化用户体验

## 相关组件
- QProgressBar: 基类
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
- QProxyStyle: 样式系统
