# ElaMessageBar 组件文档

## 概述
ElaMessageBar 是一个现代化的消息提示组件,提供了成功、警告、信息、错误四种类型的消息提示,支持动画效果和自动消失。

## 特性
- 四种消息类型
- 自动消失
- 动画效果
- 位置策略
- 堆叠显示
- 主题适配
- 圆角边框
- 阴影效果

## 静态方法

### 消息显示
```cpp
// 成功消息
static void success(
    ElaMessageBarType::PositionPolicy policy,
    QString title,
    QString text,
    int displayMsec,
    QWidget* parent = nullptr
);

// 警告消息
static void warning(
    ElaMessageBarType::PositionPolicy policy,
    QString title,
    QString text,
    int displayMsec,
    QWidget* parent = nullptr
);

// 信息消息
static void information(
    ElaMessageBarType::PositionPolicy policy,
    QString title,
    QString text,
    int displayMsec,
    QWidget* parent = nullptr
);

// 错误消息
static void error(
    ElaMessageBarType::PositionPolicy policy,
    QString title,
    QString text,
    int displayMsec,
    QWidget* parent = nullptr
);
```

## 位置策略
```cpp
enum PositionPolicy {
    Top,        // 顶部居中
    Bottom,     // 底部居中
    Left,       // 左侧居中
    Right,      // 右侧居中
    TopLeft,    // 左上角
    TopRight,   // 右上角
    BottomLeft, // 左下角
    BottomRight // 右下角
};
```

## 使用示例

### 基础用法

#### 1. 成功消息
```cpp
ElaMessageBar::success(
    ElaMessageBarType::TopRight,
    "成功",
    "操作已完成",
    2000  // 2秒后消失
);
```

#### 2. 警告消息
```cpp
ElaMessageBar::warning(
    ElaMessageBarType::Top,
    "警告",
    "磁盘空间不足",
    3000
);
```

### 高级用法

#### 1. 多消息堆叠
```cpp
// 连续显示多个消息
ElaMessageBar::information(
    ElaMessageBarType::Right,
    "提示1",
    "第一条消息",
    2000
);

ElaMessageBar::information(
    ElaMessageBarType::Right,
    "提示2", 
    "第二条消息",
    2000
);
```

#### 2. 错误消息
```cpp
ElaMessageBar::error(
    ElaMessageBarType::BottomRight,
    "错误",
    "网络连接失败\n请检查网络设置",
    5000
);
```

## 动画系统

### 显示动画
```cpp
void messageBarCreate(int displayMsec) {
    // 创建位置动画
    QPropertyAnimation* posAnimation = 
        new QPropertyAnimation(this, "pos");
    posAnimation->setDuration(300);
    posAnimation->setEasingCurve(
        QEasingCurve::OutCubic);
    
    // 创建透明度动画
    QPropertyAnimation* opacityAnimation =
        new QPropertyAnimation(this, "opacity");
    opacityAnimation->setDuration(300);
    
    // 启动动画组
    QParallelAnimationGroup* group = 
        new QParallelAnimationGroup(this);
    group->addAnimation(posAnimation);
    group->addAnimation(opacityAnimation);
    group->start();
}
```

### 消失动画
```cpp
void messageBarEnd() {
    // 创建透明度动画
    QPropertyAnimation* animation = 
        new QPropertyAnimation(this, "opacity");
    animation->setDuration(300);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    
    // 动画结束后删除组件
    connect(animation, &QPropertyAnimation::finished,
        this, &QObject::deleteLater);
        
    animation->start();
}
```

## 样式系统

### 成功样式
```cpp
void drawSuccess(QPainter* painter) {
    // 绘制背景
    painter->setBrush(QColor(0xE0, 0xF6, 0xDD));
    painter->drawRoundedRect(rect, 6, 6);
    
    // 绘制图标
    painter->setPen(Qt::white);
    painter->setBrush(QColor(0x11, 0x77, 0x10));
    painter->drawEllipse(iconRect);
    
    // 绘制文本
    painter->setPen(Qt::black);
    painter->drawText(textRect, text);
}
```

## 注意事项
1. 自动管理内存
2. 动画时长300ms
3. 堆叠顺序管理
4. 位置自适应
5. 主题适配
6. 性能优化
7. 事件处理
8. 样式继承

## 最佳实践
1. 合理设置时间
2. 控制消息数量
3. 选择合适位置
4. 简洁明了文本
5. 处理窗口变化
6. 避免频繁调用
7. 注意性能影响
8. 保持样式一致

## 相关组件
- QWidget: 基类
- ElaIcon: 图标系统
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
