# ElaMenu 组件文档

## 概述
ElaMenu 是一个现代化的菜单组件,继承自 QMenu。它提供了图标支持、动画效果、主题适配等增强特性。

## 特性
- 图标支持
- 弹出动画
- 主题适配
- 快捷键支持
- 子菜单
- 分隔符
- 自定义高度
- 圆角边框

## 属性

### 基础属性
```cpp
// 菜单项高度
void setMenuItemHeight(int height);
int getMenuItemHeight() const;
```

## 方法

### 添加菜单项
```cpp
// 添加子菜单
ElaMenu* addMenu(const QString& title);
ElaMenu* addMenu(const QIcon& icon, const QString& title);
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

### 状态查询
```cpp
// 是否包含子菜单
bool isHasChildMenu() const;

// 是否包含图标
bool isHasIcon() const;
```

## 信号
```cpp
// 菜单显示信号
void menuShow();
```

## 使用示例

### 基础用法

#### 1. 创建菜单
```cpp
// 创建菜单
ElaMenu* menu = new ElaMenu(this);

// 设置菜单项高度
menu->setMenuItemHeight(27);

// 添加菜单项
menu->addElaIconAction(
    ElaIconType::Copy, 
    "复制",
    QKeySequence::Copy
);
```

#### 2. 子菜单
```cpp
// 添加子菜单
ElaMenu* subMenu = menu->addMenu(
    ElaIconType::Settings,
    "设置"
);

// 添加子菜单项
subMenu->addElaIconAction(
    ElaIconType::Theme,
    "主题设置"
);
```

### 高级用法

#### 1. 动态菜单
```cpp
void contextMenuEvent(QContextMenuEvent* event) {
    ElaMenu* menu = new ElaMenu(this);
    
    // 根据状态添加菜单项
    if (hasSelection()) {
        menu->addElaIconAction(
            ElaIconType::Copy,
            "复制选中内容"
        );
    }
    
    // 添加分隔符
    if (!menu->isEmpty()) {
        menu->addSeparator();
    }
    
    // 弹出菜单
    menu->popup(event->globalPos());
}
```

#### 2. 快捷键支持
```cpp
// 添加带快捷键的菜单项
menu->addElaIconAction(
    ElaIconType::Save,
    "保存",
    QKeySequence::Save
);

menu->addElaIconAction(
    ElaIconType::Undo,
    "撤销",
    QKeySequence::Undo
);
```

## 动画系统

### 弹出动画
```cpp
void showEvent(QShowEvent* event) {
    // 创建位置动画
    QPropertyAnimation* posAnimation = 
        new QPropertyAnimation(this, "y");
    posAnimation->setDuration(400);
    posAnimation->setEasingCurve(
        QEasingCurve::OutCubic);
    
    // 设置动画范围
    if (isTopPopup) {
        posAnimation->setStartValue(-targetY);
    } else {
        posAnimation->setStartValue(targetY);
    }
    posAnimation->setEndValue(0);
    
    // 启动动画
    posAnimation->start(
        QAbstractAnimation::DeleteWhenStopped);
}
```

## 注意事项
1. 继承自 QMenu
2. 自动处理主题切换
3. 支持快捷键
4. 动画时长400ms
5. 智能分隔符
6. 注意内存管理
7. 性能优化
8. 样式继承

## 最佳实践
1. 合理使用图标
2. 控制菜单层级
3. 添加快捷键
4. 处理动画效果
5. 注意菜单定位
6. 合理使用分隔符
7. 保持样式一致
8. 优化用户体验

## 相关组件
- QMenu: 基类
- ElaIcon: 图标系统
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
