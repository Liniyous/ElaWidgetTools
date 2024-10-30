# ElaLineEdit 组件文档

## 概述
ElaLineEdit 是一个现代化的单行文本输入组件,继承自 QLineEdit。它提供了动画效果、主题适配、右键菜单等增强特性。

## 特性
- 焦点动画
- 主题适配
- 自定义样式
- 右键菜单
- 清除按钮
- 圆角边框
- 底部指示线
- 文本间距控制

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 清除按钮启用
Q_PROPERTY(bool isClearButtonEnable 
    READ getIsClearButtonEnable 
    WRITE setIsClearButtonEnable)
```

## 信号
```cpp
// 获得焦点
void focusIn(QString text);

// 失去焦点
void focusOut(QString text);

// 窗口失去焦点
void wmFocusOut(QString text);
```

## 默认值
- 边框圆角: 6px
- 文本间距: 0.5px
- 清除按钮: 启用
- 菜单项高度: 27px
- 焦点策略: StrongFocus
- 左内边距: 10px

## 使用示例

### 基础用法

#### 1. 创建输入框
```cpp
// 创建输入框
ElaLineEdit* lineEdit = new ElaLineEdit(this);

// 设置占位符文本
lineEdit->setPlaceholderText("请输入...");

// 设置初始文本
lineEdit->setText("Hello World");

// 连接信号
connect(lineEdit, &ElaLineEdit::focusOut, 
    this, &MyWidget::onTextChanged);
```

#### 2. 自定义样式
```cpp
// 设置圆角
lineEdit->setBorderRadius(8);

// 控制清除按钮
lineEdit->setIsClearButtonEnable(true);

// 设置只读
lineEdit->setReadOnly(true);

// 设置最大长度
lineEdit->setMaxLength(100);
```

### 高级用法

#### 1. 焦点处理
```cpp
// 焦点获得
connect(lineEdit, &ElaLineEdit::focusIn, 
    this, [](const QString& text) {
        qDebug() << "Focus in:" << text;
});

// 焦点失去
connect(lineEdit, &ElaLineEdit::focusOut, 
    this, [](const QString& text) {
        qDebug() << "Focus out:" << text;
});

// 窗口焦点
connect(lineEdit, &ElaLineEdit::wmFocusOut, 
    this, [](const QString& text) {
        qDebug() << "Window focus out:" << text;
});
```

#### 2. 右键菜单
```cpp
// 自定义右键菜单
class CustomLineEdit : public ElaLineEdit {
protected:
    void contextMenuEvent(
        QContextMenuEvent* event) override {
        ElaMenu* menu = new ElaMenu(this);
        menu->setMenuItemHeight(27);
        
        // 添加自定义菜单项
        menu->addElaIconAction(
            ElaIconType::Copy, 
            "复制",
            QKeySequence::Copy
        );
        
        menu->popup(event->globalPos());
    }
};
```

## 样式系统

### ElaLineEditStyle
自定义代理样式类,负责绘制输入框外观。

```cpp
void drawPrimitive(PrimitiveElement element, 
    const QStyleOption* option,
    QPainter* painter,
    const QWidget* widget) const override 
{
    // 绘制边框
    painter->setBrush(borderColor);
    painter->drawRoundedRect(rect, 6, 6);
    
    // 绘制背景
    painter->setBrush(backgroundColor);
    painter->drawRoundedRect(innerRect, 6, 6);
    
    // 绘制底边线
    painter->setBrush(hemlineColor);
    painter->drawPath(bottomPath);
}
```

## 动画效果

### 焦点动画
```cpp
QPropertyAnimation* markAnimation = 
    new QPropertyAnimation(d, "pExpandMarkWidth");
markAnimation->setDuration(300);
markAnimation->setEasingCurve(
    QEasingCurve::InOutSine);
markAnimation->setStartValue(currentWidth);
markAnimation->setEndValue(targetWidth);
```

## 注意事项
1. 继承自 QLineEdit
2. 自动处理主题切换
3. 支持撤销/重做
4. 支持剪切板操作
5. 动画时长300ms
6. 使用事件总线
7. 注意内存管理
8. 性能优化

## 最佳实践
1. 合理设置文本长度
2. 处理焦点变化
3. 注意输入验证
4. 控制动画效果
5. 处理主题切换
6. 优化用户体验
7. 合理使用事件
8. 保持样式一致

## 相关组件
- QLineEdit: 基类
- ElaMenu: 右键菜单
- ElaTheme: 主题管理
- ElaEventBus: 事件总线
