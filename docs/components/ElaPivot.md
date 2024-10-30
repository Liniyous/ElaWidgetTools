# ElaPivot 组件文档

## 概述
ElaPivot 是一个现代化的轴心导航组件,提供了水平滚动、动画效果、主题适配等特性。它适用于创建类似标签页切换的导航界面。

## 特性
- 水平滚动
- 动画效果
- 主题适配
- 手势支持
- 标记指示
- 悬停效果
- 点击反馈
- 自定义间距

## 属性

### 基础属性
```cpp
// 文本大小
Q_PROPERTY(int textPixelSize READ getTextPixelSize 
    WRITE setTextPixelSize)

// 当前索引
Q_PROPERTY(int currentIndex READ getCurrentIndex 
    WRITE setCurrentIndex)

// 项目间距
Q_PROPERTY(int pivotSpacing READ getPivotSpacing 
    WRITE setPivotSpacing)

// 标记宽度
Q_PROPERTY(int markWidth READ getMarkWidth 
    WRITE setMarkWidth)
```

## 方法

### 项目管理
```cpp
// 添加项目
void appendPivot(QString pivotTitle);

// 移除项目
void removePivot(QString pivotTitle);
```

## 信号
```cpp
// 点击信号
Q_SIGNAL void pivotClicked(int index);

// 双击信号
Q_SIGNAL void pivotDoubleClicked(int index);
```

## 使用示例

### 基础用法

#### 1. 创建导航
```cpp
// 创建导航组件
ElaPivot* pivot = new ElaPivot(this);
pivot->setFixedHeight(40);

// 添加项目
pivot->appendPivot("首页");
pivot->appendPivot("音乐");
pivot->appendPivot("视频");
pivot->appendPivot("设置");

// 设置当前项
pivot->setCurrentIndex(0);
```

#### 2. 自定义样式
```cpp
// 设置文本大小
pivot->setTextPixelSize(20);

// 设置间距
pivot->setPivotSpacing(8);

// 设置标记宽度
pivot->setMarkWidth(75);
```

### 高级用法

#### 1. 处理点击事件
```cpp
connect(pivot, &ElaPivot::pivotClicked, 
    this, [=](int index) {
    // 处理点击事件
    switch(index) {
        case 0:
            showHomePage();
            break;
        case 1:
            showMusicPage();
            break;
        // ...
    }
});
```

## 动画系统

### 标记动画
```cpp
void doCurrentIndexChangedAnimation(
    const QModelIndex& index) {
    // 创建位置动画
    QPropertyAnimation* markAnimation = 
        new QPropertyAnimation(this, "markX");
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(
        QEasingCurve::InOutSine);
    
    // 创建宽度动画
    QPropertyAnimation* widthAnimation =
        new QPropertyAnimation(this, 
            "markAnimationWidth");
    widthAnimation->setDuration(300);
    
    // 启动动画组
    QParallelAnimationGroup* group = 
        new QParallelAnimationGroup(this);
    group->addAnimation(markAnimation);
    group->addAnimation(widthAnimation);
    group->start();
}
```

## 样式系统

### 项目样式
```cpp
void drawControl(CE_ItemViewItem) {
    // 绘制文本
    if (pressed) {
        painter->setPen(pressTextColor);
    } else if (current || hovered) {
        painter->setPen(activeTextColor);
    } else {
        painter->setPen(normalTextColor);
    }
    
    painter->drawText(textRect, 
        Qt::AlignCenter, text);
}
```

### 标记样式
```cpp
void paintEvent() {
    // 绘制标记
    painter.setPen(Qt::NoPen);
    painter.setBrush(markColor);
    painter.drawRoundedRect(
        markRect, 3, 3);
}
```

## 注意事项
1. 继承自 QWidget
2. 使用 MVC 模式
3. 自动内存管理
4. 动画时长300ms
5. 手势支持
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置间距
2. 控制项目数量
3. 处理长文本
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QWidget: 基类
- QListView: 视图组件
- QPropertyAnimation: 动画系统
- ElaTheme: 主题管理
