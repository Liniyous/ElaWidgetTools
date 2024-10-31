# ElaScrollPageArea 组件文档

## 概述
ElaScrollPageArea 是一个现代化的滚动页面区域组件,继承自 QWidget。它提供了圆角边框、主题适配、背景绘制等特性。

## 特性
- 圆角边框
- 主题适配
- 背景绘制
- 边框样式
- 透明效果
- 固定高度
- 自动更新
- 性能优化

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius 
    READ getBorderRadius 
    WRITE setBorderRadius)
```

## 样式系统

### 区域样式
```cpp
void paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // 设置画笔和画刷
    painter.setPen(
        ElaThemeColor(themeMode, BasicBorder)
    );
    painter.setBrush(
        ElaThemeColor(themeMode, BasicBaseAlpha)
    );
    
    // 绘制圆角矩形
    QRect foregroundRect(
        1, 1, 
        width() - 2, 
        height() - 2
    );
    painter.drawRoundedRect(
        foregroundRect,
        borderRadius,
        borderRadius
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建区域
```cpp
// 创建区域
ElaScrollPageArea* area = 
    new ElaScrollPageArea(this);

// 设置圆角
area->setBorderRadius(6);

// 设置大小
area->setFixedHeight(75);
```

#### 2. 主题处理
```cpp
// 连接主题变更信号
connect(eTheme, 
    &ElaTheme::themeModeChanged,
    this, [=](ElaThemeType::ThemeMode mode) {
    // 更新主题
    update();
});
```

### 高级用法

#### 1. 自定义样式
```cpp
// 设置背景透明
area->setStyleSheet(
    "background:transparent;"
);

// 添加阴影效果
auto shadow = new QGraphicsDropShadowEffect(
    area
);
shadow->setBlurRadius(10);
shadow->setColor(QColor(0, 0, 0, 60));
shadow->setOffset(0, 2);
area->setGraphicsEffect(shadow);
```

#### 2. 布局设置
```cpp
// 创建布局
QVBoxLayout* layout = 
    new QVBoxLayout(area);
layout->setContentsMargins(
    12, 12, 12, 12
);
layout->setSpacing(6);

// 添加内容
layout->addWidget(contentWidget);
```

## 私有实现

### 私有类
```cpp
class ElaScrollPageAreaPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaScrollPageArea)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    
private:
    // 主题模式
    ElaThemeType::ThemeMode _themeMode;
};
```

### 初始化
```cpp
ElaScrollPageArea::ElaScrollPageArea(
    QWidget* parent
) : QWidget{parent}
  , d_ptr(new ElaScrollPageAreaPrivate())
{
    Q_D(ElaScrollPageArea);
    d->q_ptr = this;
    
    // 初始化属性
    d->_pBorderRadius = 6;
    setFixedHeight(75);
    
    // 初始化主题
    d->_themeMode = eTheme->getThemeMode();
    
    // 连接主题信号
    connect(eTheme, 
        &ElaTheme::themeModeChanged,
        this, [=](auto themeMode) {
        d->_themeMode = themeMode;
    });
}
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 绘制优化

## 最佳实践
1. 合理设置大小
2. 优化绘制性能
3. 控制刷新频率
4. 注意内存使用
5. 统一交互方式
6. 保持风格一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QWidget: 基类
- ElaTheme: 主题系统
- ElaScrollPage: 滚动页面
- QPainter: 绘制系统
```