# ElaTreeView 组件文档

## 概述
ElaTreeView 是一个现代化的树形视图组件,继承自 QTreeView。它提供了自定义样式、动画效果、主题适配等特性。

## 特性
- 自定义样式
- 动画效果
- 主题适配
- 展开/收起
- 选中特效
- 滚动优化
- 缩进控制
- 性能优化

## 属性

### 基础属性
```cpp
// 项目高度
Q_PROPERTY(int itemHeight 
    READ getItemHeight 
    WRITE setItemHeight)

// 表头边距
Q_PROPERTY(int headerMargin 
    READ getHeaderMargin 
    WRITE setHeaderMargin)
```

## 样式系统

### 项目样式
```cpp
void drawPrimitive(PE_PanelItemViewItem) {
    // 背景绘制
    if (state & State_Selected) {
        if (state & State_MouseOver) {
            painter.fillPath(path, selectedHoverColor);
        } else {
            painter.fillPath(path, selectedColor);
        }
    } else if (state & State_MouseOver) {
        painter.fillPath(path, hoverColor);
    }
}
```

### 展开指示器
```cpp
void drawPrimitive(PE_IndicatorBranch) {
    if (state & State_Children) {
        // 设置字体
        painter.setFont("ElaAwesome");
        
        // 绘制图标
        painter.drawText(
            rect,
            state & State_Open ? 
                AngleDown : AngleRight
        );
    }
}
```

### 选中特效
```cpp
void drawControl(CE_ItemViewItem) {
    // 绘制选中指示器
    if (state & State_Selected) {
        painter.setBrush(primaryColor);
        painter.drawRoundedRect(
            indicator, radius, radius
        );
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建树形视图
```cpp
// 创建视图
ElaTreeView* treeView = 
    new ElaTreeView(this);

// 设置模型
treeView->setModel(model);

// 设置动画
treeView->setAnimated(true);
```

#### 2. 外观设置
```cpp
// 设置项目高度
treeView->setItemHeight(35);

// 设置表头边距
treeView->setHeaderMargin(5);

// 设置缩进
treeView->setIndentation(20);

// 设置表头字体
QFont headerFont = 
    treeView->header()->font();
headerFont.setPixelSize(16);
treeView->header()->setFont(headerFont);
```

#### 3. 滚动设置
```cpp
// 创建滚动条
ElaScrollBar* scrollBar = 
    new ElaScrollBar(treeView);
scrollBar->setIsAnimation(true);

// 设置滚动条
treeView->setVerticalScrollBar(scrollBar);
treeView->setHorizontalScrollBar(scrollBar);

// 设置滚动策略
treeView->setVerticalScrollBarPolicy(
    Qt::ScrollBarAsNeeded
);
```

#### 4. 展开控制
```cpp
// 展开全部
treeView->expandAll();

// 收起全部
treeView->collapseAll();

// 展开指定项
treeView->expand(index);

// 收起指定项
treeView->collapse(index);
```

## 注意事项
1. 继承自 QTreeView
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 滚动优化

## 最佳实践
1. 合理设置大小
2. 控制数据量
3. 优化渲染
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QTreeView: 基类
- ElaScrollBar: 滚动条组件
- QProxyStyle: 样式系统
- QHeaderView: 表头组件
