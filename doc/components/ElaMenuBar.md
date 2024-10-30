# ElaMenuBar 组件文档

## 概述
ElaMenuBar 是一个现代化的菜单栏组件,继承自 QMenuBar。它提供了图标支持、主题适配、样式美化等增强特性。

## 特性
- 图标支持
- 主题适配
- 悬停效果
- 圆角边框
- 快捷键支持
- 子菜单
- 分隔符
- 自定义高度

## 方法

### 添加菜单
```cpp
// 添加菜单
ElaMenu* addMenu(const QString& title);
ElaMenu* addMenu(const QIcon& icon, 
    const QString& title);
ElaMenu* addMenu(ElaIconType::IconName icon, 
    const QString& title);

// 添加图标动作
QAction* addElaIconAction(
    ElaIconType::IconName icon, 
    const QString& text);
QAction* addElaIconAction(
    ElaIconType::IconName icon,
    const QString& text,
    const QKeySequence& shortcut);
```

## 使用示例

### 基础用法

#### 1. 创建菜单栏
```cpp
// 创建菜单栏
ElaMenuBar* menuBar = new ElaMenuBar(this);
menuBar->setFixedHeight(30);

// 添加到窗口
setMenuBar(menuBar);

// 添加菜单项
menuBar->addElaIconAction(
    ElaIconType::Home, 
    "主页"
);
```

#### 2. 添加子菜单
```cpp
// 添加带图标的子菜单
ElaMenu* fileMenu = menuBar->addMenu(
    ElaIconType::File,
    "文件(&F)"
);

// 添加菜单项
fileMenu->addElaIconAction(
    ElaIconType::Save,
    "保存",
    QKeySequence::Save
);

fileMenu->addElaIconAction(
    ElaIconType::Open,
    "打开",
    QKeySequence::Open
);
```

### 高级用法

#### 1. 多级菜单
```cpp
// 创建一级菜单
ElaMenu* viewMenu = menuBar->addMenu(
    ElaIconType::View,
    "视图(&V)"
);

// 创建二级菜单
ElaMenu* themeMenu = viewMenu->addMenu(
    ElaIconType::Theme,
    "主题"
);

// 添加菜单项
themeMenu->addElaIconAction(
    ElaIconType::Sun,
    "浅色"
);

themeMenu->addElaIconAction(
    ElaIconType::Moon,
    "深色"
);
```

#### 2. 分组菜单
```cpp
// 添加分隔符
menuBar->addSeparator();

// 添加编辑菜单组
ElaMenu* editMenu = menuBar->addMenu(
    "编辑(&E)"
);

editMenu->addElaIconAction(
    ElaIconType::Cut,
    "剪切",
    QKeySequence::Cut
);

editMenu->addElaIconAction(
    ElaIconType::Copy,
    "复制",
    QKeySequence::Copy
);
```

## 样式系统

### 菜单项样式
```cpp
void drawControl(CE_MenuBarItem) {
    // 绘制背景
    if (selected || pressed) {
        painter->setBrush(hoverColor);
        painter->drawRoundedRect(rect, 3, 3);
    }
    
    // 绘制图标
    if (!icon.isNull()) {
        icon.paint(painter, iconRect, 
            alignment, mode);
    }
    
    // 绘制文本
    painter->setPen(textColor);
    painter->drawText(textRect, 
        alignment, text);
}
```

### 布局算法
```cpp
QSize sizeFromContents(CT_MenuBarItem) {
    QSize size = baseSize;
    
    // 设置固定高度
    size.setHeight(menuBarHeight);
    
    // 添加图标空间
    if (!icon.isNull()) {
        size.setWidth(
            size.width() + iconWidth + margin);
    }
    
    return size;
}
```

## 注意事项
1. 继承自 QMenuBar
2. 自动处理主题切换
3. 支持快捷键
4. 智能布局
5. 样式继承
6. 注意内存管理
7. 性能优化
8. 主题适配

## 最佳实践
1. 合理使用图标
2. 添加快捷键
3. 控制菜单层级
4. 分组管理
5. 注意命名规范
6. 合理使用分隔符
7. 保持样式一致
8. 优化用户体验

## 相关组件
- QMenuBar: 基类
- ElaMenu: 菜单组件
- ElaIcon: 图标系统
- ElaTheme: 主题管理
