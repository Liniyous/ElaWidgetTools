# ElaListView 组件文档

## 概述
ElaListView 是一个现代化的列表视图组件,继承自 QListView。它提供了自定义样式、主题适配、滚动条美化等特性。

## 特性
- 自定义项高度
- 透明背景选项
- 主题适配
- 平滑滚动
- 选中特效
- 悬停效果
- 圆角边框
- 自定义滚动条

## 属性

### 基础属性
```cpp
// 项目高度
Q_PROPERTY(int itemHeight READ getItemHeight 
    WRITE setItemHeight)

// 是否透明
Q_PROPERTY(bool isTransparent READ getIsTransparent 
    WRITE setIsTransparent)
```

## 默认值
- 项目高度: 35px
- 左内边距: 11px
- 圆角半径: 4px
- 滚动模式: 像素滚动
- 透明背景: false
- 鼠标追踪: 启用

## 使用示例

### 基础用法

#### 1. 创建列表视图
```cpp
// 创建列表视图
ElaListView* listView = new ElaListView(this);

// 设置模型
listView->setModel(new MyListModel(this));

// 设置项目高度
listView->setItemHeight(40);

// 设置透明背景
listView->setIsTransparent(true);
```

#### 2. 滚动条配置
```cpp
// 创建浮动滚动条
ElaScrollBar* floatScrollBar = 
    new ElaScrollBar(
        listView->verticalScrollBar(), 
        listView
    );

// 启用动画
floatScrollBar->setIsAnimation(true);

// 设置滚动条策略
listView->setHorizontalScrollBarPolicy(
    Qt::ScrollBarAsNeeded);
listView->setVerticalScrollBarPolicy(
    Qt::ScrollBarAsNeeded);
```

### 高级用法

#### 1. 自定义样式
```cpp
class CustomListViewStyle : public ElaListViewStyle {
protected:
    void drawControl(
        ControlElement element,
        const QStyleOption* option,
        QPainter* painter,
        const QWidget* widget) const override 
    {
        if (element == CE_ItemViewItem) {
            // 自定义项目绘制
            drawCustomItem(option, painter, widget);
        } else {
            ElaListViewStyle::drawControl(
                element, option, painter, widget);
        }
    }
    
    QSize sizeFromContents(
        ContentsType type,
        const QStyleOption* option,
        const QSize& size,
        const QWidget* widget) const override 
    {
        // 自定义项目尺寸
        return customSize;
    }
};
```

#### 2. 交互处理
```cpp
class CustomListView : public ElaBaseListView {
protected:
    void mousePressEvent(QMouseEvent* event) override {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            // 处理点击事件
            emit mousePress(index);
        }
        ElaBaseListView::mousePressEvent(event);
    }
};
```

## 样式系统

### ElaListViewStyle
自定义代理样式类,负责绘制列表视图外观。

```cpp
// 项目背景
void drawPrimitive(PE_PanelItemViewItem) {
    // 绘制选中状态
    if (state & State_Selected) {
        if (state & State_MouseOver) {
            // 选中+悬停
            painter->fillPath(path, selectedHoverColor);
        } else {
            // 选中
            painter->fillPath(path, selectedColor);
        }
    }
    // 绘制悬停状态
    else if (state & State_MouseOver) {
        painter->fillPath(path, hoverColor);
    }
}

// 项目内容
void drawControl(CE_ItemViewItem) {
    // 绘制图标
    if (!icon.isNull()) {
        icon.paint(painter, iconRect, alignment, mode);
    }
    
    // 绘制文本
    if (!text.isEmpty()) {
        painter->drawText(textRect, alignment, text);
    }
    
    // 绘制选中指示器
    if (state & State_Selected) {
        painter->drawRoundedRect(
            indicatorRect, 3, 3);
    }
}
```

## 布局算法

### 项目布局
```cpp
QSize sizeFromContents(CT_ItemViewItem) {
    QSize itemSize = baseSize;
    
    // 列表模式添加左边距
    if (viewMode == ListMode) {
        itemSize.setWidth(
            itemSize.width() + leftPadding);
    }
    
    // 设置固定高度
    itemSize.setHeight(itemHeight);
    
    return itemSize;
}
```

## 注意事项
1. 继承自 QListView
2. 自动处理主题切换
3. 支持像素级滚动
4. 自动处理选中状态
5. 支持交替行颜色
6. 注意内存管理
7. 性能优化
8. 样式继承

## 最佳实践
1. 合理设置项目
