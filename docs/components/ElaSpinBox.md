# ElaSpinBox/ElaDoubleSpinBox 组件文档

## 概述
ElaSpinBox 和 ElaDoubleSpinBox 是一对现代化的数值输入组件,分别用于整数和浮点数的输入。它们提供了自定义样式、右键菜单、主题适配等特性。

## 特性
- 整数/浮点数输入
- 自定义样式
- 右键菜单
- 主题适配
- 居中对齐
- 圆角边框
- 增减按钮
- 键盘导航
- 复制/粘贴支持

## 尺寸规格

- 组件宽度: 120px
- 组件高度: 30px
- 按钮宽度: 30px
- 边框圆角: 4px
- 文本居中对齐
- 菜单项高度: 27px

## 使用示例

### 基础用法

#### 1. 整数输入框
```cpp
// 创建整数输入框
ElaSpinBox* spinBox = new ElaSpinBox(this);

// 设置范围
spinBox->setRange(0, 100);

// 设置步长
spinBox->setSingleStep(1);

// 设置初始值
spinBox->setValue(50);
```

#### 2. 浮点数输入框
```cpp
// 创建浮点数输入框
ElaDoubleSpinBox* doubleSpinBox = new ElaDoubleSpinBox(this);

// 设置范围
doubleSpinBox->setRange(0.0, 1.0);

// 设置步长
doubleSpinBox->setSingleStep(0.1);

// 设置小数位数
doubleSpinBox->setDecimals(2);
```

### 高级用法

#### 1. 自定义格式
```cpp
class CustomSpinBox : public ElaSpinBox {
    QString textFromValue(int value) const override {
        // 自定义显示格式
        return QString("%1%").arg(value);
    }
    
    int valueFromText(const QString &text) const override {
        // 自定义解析逻辑
        QString numStr = text;
        numStr.remove('%');
        return numStr.toInt();
    }
};
```

#### 2. 值变化监听
```cpp
// 监听值变化
connect(spinBox, 
    QOverload<int>::of(&ElaSpinBox::valueChanged),
    this, [](int value) {
        qDebug() << "New value:" << value;
});

// 监听编辑完成
connect(spinBox, &ElaSpinBox::editingFinished,
    this, [=]() {
        validateInput(spinBox->value());
});
```

#### 3. 前缀和后缀
```cpp
// 设置前缀
doubleSpinBox->setPrefix("$");

// 设置后缀
doubleSpinBox->setSuffix(" kg");

// 特殊显示
doubleSpinBox->setSpecialValueText("Empty");
```

## 右键菜单功能

### 编辑操作
- 撤销 (Ctrl+Z)
- 恢复 (Ctrl+Y)
- 剪切 (Ctrl+X)
- 复制 (Ctrl+C)
- 粘贴 (Ctrl+V)
- 删除
- 全选 (Ctrl+A)

### 数值操作
- 增加
- 减少

## 注意事项
1. 默认固定大小为 120x30
2. 文本自动居中对齐
3. 支持键盘导航
4. 支持鼠标滚轮调节
5. 主题切换会自动更新样式
6. 右键菜单根据状态自动启用/禁用
7. 支持 Undo/Redo 操作
8. 按钮状态自动更新

## 相关组件
- ElaSpinBoxStyle: 输入框样式
- ElaMenu: 右键菜单
- ElaTheme: 主题管理器
