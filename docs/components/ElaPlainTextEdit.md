# ElaPlainTextEdit 组件文档

## 概述
ElaPlainTextEdit 是一个现代化的纯文本编辑器组件,继承自 QPlainTextEdit。它提供了主题适配、动画效果、右键菜单等增强特性。

## 特性
- 主题适配
- 焦点动画
- 右键菜单
- 滚动条
- 圆角边框
- 底部标记
- 复制粘贴
- 撤销重做

## 构造函数
```cpp
// 默认构造
explicit ElaPlainTextEdit(QWidget* parent = nullptr);

// 带文本构造
explicit ElaPlainTextEdit(
    const QString& text, 
    QWidget* parent = nullptr
);
```

## 右键菜单

### 菜单项
```cpp
// 创建右键菜单
ElaMenu* menu = new ElaMenu(this);
menu->setMenuItemHeight(27);

// 撤销/重做
menu->addElaIconAction(
    ElaIconType::ArrowRotateLeft, 
    "撤销", 
    QKeySequence::Undo
);
menu->addElaIconAction(
    ElaIconType::ArrowRotateRight, 
    "恢复", 
    QKeySequence::Redo
);

// 剪切/复制/粘贴
menu->addElaIconAction(
    ElaIconType::KnifeKitchen, 
    "剪切", 
    QKeySequence::Cut
);
menu->addElaIconAction(
    ElaIconType::Copy, 
    "复制", 
    QKeySequence::Copy
);
menu->addElaIconAction(
    ElaIconType::Paste, 
    "粘贴", 
    QKeySequence::Paste
);

// 删除/全选
menu->addElaIconAction(
    ElaIconType::DeleteLeft, 
    "删除"
);
menu->addAction("全选");
```

## 动画系统

### 焦点动画
```cpp
void focusInEvent(QFocusEvent* event) {
    // 创建标记动画
    QPropertyAnimation* markAnimation = 
        new QPropertyAnimation(style, "expandMarkWidth");
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(
        QEasingCurve::InOutSine);
    markAnimation->setStartValue(0);
    markAnimation->setEndValue(width() / 2 - 3);
    markAnimation->start();
}
```

## 样式系统

### 边框样式
```cpp
void drawControl(CE_ShapedFrame) {
    // 绘制边框
    painter->setBrush(borderColor);
    painter->drawRoundedRect(editRect, 6, 6);
    
    // 绘制背景
    painter->setBrush(backgroundColor);
    painter->drawRoundedRect(
        innerRect, 6, 6);
    
    // 绘制底边线
    painter->setBrush(hemlineColor);
    painter->drawPath(hemlinePath);
    
    // 绘制焦点标记
    painter->setBrush(primaryColor);
    painter->drawRoundedRect(
        markRect, 2, 2);
}
```

## 使用示例

### 基础用法

#### 1. 创建编辑器
```cpp
// 创建编辑器
ElaPlainTextEdit* editor = 
    new ElaPlainTextEdit(this);

// 设置文本
editor->setPlainText("Hello World");

// 设置只读
editor->setReadOnly(true);
```

#### 2. 处理事件
```cpp
// 文本变化
connect(editor, 
    &ElaPlainTextEdit::textChanged,
    this, [=]() {
    qDebug() << "Text changed:" 
        << editor->toPlainText();
});

// 选择变化
connect(editor,
    &ElaPlainTextEdit::selectionChanged,
    this, [=]() {
    qDebug() << "Selection:" 
        << editor->textCursor().selectedText();
});
```

## 注意事项
1. 继承自 QPlainTextEdit
2. 自动处理主题切换
3. 动画时长300ms
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置大小
2. 处理文本变化
3. 注意性能影响
4. 统一交互方式
5. 保持样式一致
6. 动画流畅性
7. 响应式布局
8. 优化用户体验

## 相关组件
- QPlainTextEdit: 基类
- ElaMenu: 右键菜单
- ElaScrollBar: 滚动条
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统
