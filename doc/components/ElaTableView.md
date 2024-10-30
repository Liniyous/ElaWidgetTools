# ElaTableView 组件文档

## 概述
ElaTableView 是一个现代化的表格视图组件,继承自 QTableView。它提供了自定义样式、行悬浮、主题适配等特性。

## 特性
- 自定义样式
- 行悬浮效果
- 主题适配
- 隔行变色
- 选中特效
- 滚动优化
- 表头自定义
- 性能优化

## 属性

### 基础属性
```cpp
// 表头边距
Q_PROPERTY(int headerMargin 
    READ getHeaderMargin 
    WRITE setHeaderMargin)
```

## 信号
```cpp
// 视图显示
Q_SIGNAL void tableViewShow();

// 视图隐藏
Q_SIGNAL void tableViewHide();
```

## 样式系统

### 表格样式
```cpp
void drawControl(CE_ItemViewItem) {
    // 背景绘制
    if (state & State_Selected) {
        painter.setBrush(selectedColor);
    } else if (state & State_MouseOver) {
        painter.setBrush(hoverColor);
    }
    
    // 图标绘制
    if (!icon.isNull()) {
        icon.paint(painter, iconRect);
    }
    
    // 文本绘制
    painter.setPen(textColor);
    painter.drawText(textRect, text);
    
    // 选中特效
    if (state & State_Selected) {
        painter.setBrush(primaryColor);
        painter.drawRoundedRect(
            indicator, radius, radius
        );
    }
}
```

### 表头样式
```cpp
void drawControl(CE_HeaderSection) {
    // 背景绘制
    painter.setBrush(headerColor);
    painter.drawRect(rect);
    
    // 悬浮效果
    if (state & State_MouseOver) {
        painter.setBrush(hoverColor);
    }
    
    // 按下效果
    if (state & State_Sunken) {
        painter.setBrush(pressColor);
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建表格
```cpp
// 创建表格
ElaTableView* tableView = 
    new ElaTableView(this);

// 设置模型
tableView->setModel(model);

// 设置选择模式
tableView->setSelectionBehavior(
    QAbstractItemView::SelectRows
);
```

#### 2. 表头设置
```cpp
// 设置表头字体
QFont headerFont = 
    tableView->horizontalHeader()->font();
headerFont.setPixelSize(16);
tableView->horizontalHeader()
    ->setFont(headerFont);

// 隐藏垂直表头
tableView->verticalHeader()
    ->setHidden(true);

// 设置列宽模式
tableView->horizontalHeader()
    ->setSectionResizeMode(
        QHeaderView::Interactive
    );
```

#### 3. 外观设置
```cpp
// 设置隔行变色
tableView->setAlternatingRowColors(true);

// 设置图标大小
tableView->setIconSize(QSize(38, 38));

// 设置最小尺寸
tableView->horizontalHeader()
    ->setMinimumSectionSize(60);
tableView->verticalHeader()
    ->setMinimumSectionSize(46);
```

#### 4. 列宽设置
```cpp
// 显示时设置列宽
connect(tableView, 
    &ElaTableView::tableViewShow,
    this, [=]() {
    tableView->setColumnWidth(0, 60);
    tableView->setColumnWidth(1, 205);
    tableView->setColumnWidth(2, 170);
    tableView->setColumnWidth(3, 150);
    tableView->setColumnWidth(4, 60);
});
```

## 注意事项
1. 继承自 QTableView
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
- QTableView: 基类
- ElaScrollBar: 滚动条组件
- QProxyStyle: 样式系统
- QHeaderView: 表头组件
