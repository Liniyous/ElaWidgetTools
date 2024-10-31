# ElaDoubleSpinBox 组件文档

## 概述
ElaDoubleSpinBox 是一个现代化的双精度数值输入框组件,继承自 QDoubleSpinBox。它提供了自定义样式、右键菜单、主题适配等特性。

## 特性
- 自定义样式
- 右键菜单
- 主题适配
- 居中对齐
- 透明背景
- 数值验证
- 步进控制
- 快捷键支持

## 基础设置
```cpp
// 创建时的基础设置
setFixedSize(120, 30);
setStyle(new ElaSpinBoxStyle(style()));
lineEdit()->setAlignment(Qt::AlignCenter);
lineEdit()->setStyleSheet(
    "background-color:transparent"
);
```

## 右键菜单

### 菜单项
```cpp
// 编辑操作
- 撤销 (Ctrl+Z)
- 恢复 (Ctrl+Y)
- 剪切 (Ctrl+X)
- 复制 (Ctrl+C)
- 粘贴 (Ctrl+V)
- 删除
- 全选 (Ctrl+A)

// 数值操作
- 增加
- 减少
```

## 使用示例

### 基础用法

#### 1. 创建数值框
```cpp
// 创建数值框
ElaDoubleSpinBox* spinBox = 
    new ElaDoubleSpinBox(this);

// 设置范围
spinBox->setRange(0.0, 100.0);

// 设置步进值
spinBox->setSingleStep(0.1);

// 设置小数位数
spinBox->setDecimals(2);
```

#### 2. 主题适配
```cpp
// 连接主题变更信号
connect(eTheme, 
    &ElaTheme::themeModeChanged,
    this, [=](ElaThemeType::ThemeMode mode) {
    QPalette palette;
    palette.setColor(
        QPalette::Base, 
        Qt::transparent
    );
    palette.setColor(
        QPalette::Text, 
        ElaThemeColor(mode, BasicText)
    );
    lineEdit()->setPalette(palette);
});
```

### 高级用法

#### 1. 自定义右键菜单
```cpp
void contextMenuEvent(
    QContextMenuEvent* event
) override {
    // 创建菜单
    ElaMenu* menu = createStandardContextMenu();
    
    // 添加分隔符
    menu->addSeparator();
    
    // 添加数值操作
    const uint se = stepEnabled();
    QAction* up = menu->addElaIconAction(
        ElaIconType::Plus, 
        tr("增加")
    );
    up->setEnabled(se & StepUpEnabled);
    
    QAction* down = menu->addElaIconAction(
        ElaIconType::Minus, 
        tr("减少")
    );
    down->setEnabled(se & StepDownEnabled);
    
    // 执行菜单
    const QAction* action = menu->exec(pos);
    if (action) {
        if (action == up) {
            stepBy(1);
        } else if (action == down) {
            stepBy(-1);
        }
    }
}
```

#### 2. 私有实现
```cpp
ElaMenu* createStandardContextMenu() {
    // 创建菜单
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    
    // 添加编辑操作
    addEditActions(menu);
    
    // 添加分隔符
    if (!menu->isEmpty()) {
        menu->addSeparator();
    }
    
    // 添加全选操作
    QAction* action = menu->addAction(
        tr("全选")
    );
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(
        !text().isEmpty() && 
        !(selectedText() == text())
    );
    
    return menu;
}
```

## 注意事项
1. 继承自 QDoubleSpinBox
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 菜单管理

## 最佳实践
1. 合理设置范围
2. 控制小数位数
3. 优化步进值
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QDoubleSpinBox: 基类
- ElaMenu: 菜单组件
- ElaSpinBoxStyle: 样式组件
- ElaTheme: 主题系统
