# ElaTabWidget 组件文档

## 概述
ElaTabWidget 是一个现代化的标签页组件,继承自 QTabWidget。它提供了拖拽分离、主题适配、自定义标签栏等特性。

## 特性
- 拖拽分离
- 主题适配
- 自定义标签栏
- 标签关闭
- 标签移动
- 浮动窗口
- 状态恢复
- 透明背景

## 属性

### 基础属性
```cpp
// 标签栏透明
Q_PROPERTY(bool isTabTransparent 
    READ getIsTabTransparent 
    WRITE setIsTabTransparent)
```

## 公共方法

### 标签位置
```cpp
// 设置标签位置(仅支持上下)
void setTabPosition(TabPosition position);
```

## 拖拽系统

### 拖拽数据
```cpp
QMimeData* mimeData {
    "DragType": "ElaTabBarDrag",
    "TabIndex": int,
    "TabIcon": QIcon,
    "TabText": QString,
    "DragWidget": QWidget*,
    "ElaTabWidgetObject": ElaTabWidget*,
    "ElaTabBarObject": ElaTabBar*
};
```

### 浮动窗口
```cpp
// 创建浮动窗口
ElaCustomTabWidget* floatWidget = 
    new ElaCustomTabWidget(originTabWidget);

// 添加拖拽标签
floatWidget->addTab(
    dragWidget, 
    tabIcon, 
    tabText
);

// 设置位置
floatWidget->move(
    cursorX - width/2, 
    cursorY - 40
);
```

## 使用示例

### 基础用法

#### 1. 创建标签页
```cpp
// 创建标签页
ElaTabWidget* tabWidget = 
    new ElaTabWidget(this);

// 添加标签
tabWidget->addTab(widget1, "标签1");
tabWidget->addTab(widget2, icon, "标签2");

// 设置当前标签
tabWidget->setCurrentIndex(0);
```

#### 2. 标签栏设置
```cpp
// 设置标签位置
tabWidget->setTabPosition(
    QTabWidget::North
);

// 设置透明背景
tabWidget->setIsTabTransparent(true);
```

#### 3. 拖拽处理
```cpp
// 标签拖拽
connect(tabBar, 
    &ElaTabBar::tabDragCreate,
    this, [=](QDrag* drag) {
    // 处理拖拽创建
});

// 标签放下
connect(tabBar, 
    &ElaTabBar::tabDragDrop,
    this, [=](const QMimeData* data) {
    // 处理标签放下
});
```

#### 4. 关闭处理
```cpp
// 标签关闭
connect(tabBar, 
    &ElaTabBar::tabCloseRequested,
    this, [=](int index) {
    // 处理标签关闭
    QWidget* widget = 
        tabWidget->widget(index);
    // 恢复到原始标签页
    if (originTabWidget) {
        originTabWidget->addTab(
            widget, icon, text
        );
    } else {
        tabWidget->removeTab(index);
        widget->deleteLater();
    }
});
```

## 注意事项
1. 继承自 QTabWidget
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 状态恢复

## 最佳实践
1. 合理设置大小
2. 控制标签数量
3. 优化拖拽体验
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QTabWidget: 基类
- ElaTabBar: 标签栏组件
- ElaCustomTabWidget: 浮动窗口
- QDrag: 拖拽系统
