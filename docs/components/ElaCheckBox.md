# ElaCheckBox 组件文档

## 概述
ElaCheckBox 是一个现代化的复选框组件,继承自 QCheckBox。它提供了美观的视觉效果和流畅的交互体验,支持三态切换、主题适配等特性。

## 特性
- 现代化圆角设计
- 三态切换支持
- 自动主题适配
- 悬停效果
- 按压效果
- 禁用状态样式
- 自定义边框圆角
- 图标指示器

## 属性

### BorderRadius
复选框边框圆角半径。

**类型:** `int`  
**默认值:** `3`

### CheckIndicatorWidth
复选框指示器宽度。

**类型:** `int`  
**默认值:** `21`

## 状态颜色

### 选中状态
- 默认: Primary 主题色
- 悬停: PrimaryHover
- 按压: PrimaryPress

### 未选中状态
- 默认: BasicBase
- 悬停: BasicHover
- 边框: BasicBorderDeep

### 禁用状态
- 文本: BasicTextDisable

## 使用示例

### 基础用法

#### 1. 创建复选框
```cpp
// 基础构造
ElaCheckBox* checkBox = new ElaCheckBox(this);
checkBox->setText("Basic CheckBox");

// 带文本构造
ElaCheckBox* checkBox = new ElaCheckBox("Custom CheckBox", this);
```

#### 2. 状态处理
```cpp
// 设置选中状态
checkBox->setChecked(true);

// 设置部分选中状态
checkBox->setTristate(true);
checkBox->setCheckState(Qt::PartiallyChecked);

// 监听状态变化
connect(checkBox, &ElaCheckBox::stateChanged, 
    this, [](int state) {
        switch(state) {
            case Qt::Checked:
                handleChecked();
                break;
            case Qt::Unchecked:
                handleUnchecked();
                break;
            case Qt::PartiallyChecked:
                handlePartiallyChecked();
                break;
        }
});
```

### 高级用法

#### 1. 自定义样式
```cpp
ElaCheckBox* checkBox = new ElaCheckBox(this);
checkBox->setBorderRadius(4);  // 设置圆角半径

// 设置字体
QFont font = checkBox->font();
font.setPixelSize(16);
checkBox->setFont(font);
```

#### 2. 批量管理
```cpp
// 创建复选框组
QList<ElaCheckBox*> checkBoxGroup;
for(int i = 0; i < 3; i++) {
    auto box = new ElaCheckBox(QString("Option %1").arg(i+1), this);
    checkBoxGroup.append(box);
    
    // 连接信号
    connect(box, &ElaCheckBox::toggled, 
        this, [=](bool checked) {
            updateGroupState(i, checked);
    });
}

// 全选/取消全选
void setAllChecked(bool checked) {
    for(auto box : checkBoxGroup) {
        box->setChecked(checked);
    }
}
```

#### 3. 禁用状态管理
```cpp
// 设置禁用状态
checkBox->setEnabled(false);

// 条件禁用
connect(someCondition, &Condition::changed, 
    checkBox, &ElaCheckBox::setEnabled);
```

## 注意事项
1. 默认字体大小为 15 像素
2. 指示器固定大小为 21x21 像素
3. 文本与指示器间距为 10 像素
4. 主题切换会自动更新样式
5. 禁用状态下保持当前选中状态
6. 部分选中状态使用横线表示

## 相关组件
- ElaRadioButton: 单选按钮
- ElaToggleSwitch: 开关按钮
- ElaToggleButton: 切换按钮
- ElaTheme: 主题管理器
