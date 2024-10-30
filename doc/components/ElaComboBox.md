# ElaComboBox 组件文档

## 概述
ElaComboBox 是一个现代化的下拉框组件,继承自 QComboBox。它提供了美观的视觉效果和流畅的动画效果,支持自定义样式、主题适配等特性。

## 特性
- 现代化圆角设计
- 展开/收起动画
- 指示器旋转动画
- 阴影效果
- 自动主题适配
- 悬停效果
- 自定义滚动条
- 自定义边框圆角
- 支持图标
- 支持多选模式(ElaMultiSelectComboBox)

## 属性

### BorderRadius
下拉框边框圆角半径。

**类型:** `int`  
**默认值:** `3`

### ExpandIconRotate
展开指示器旋转角度。

**类型:** `qreal`  
**默认值:** `0`

### ExpandMarkWidth
展开标记宽度。

**类型:** `qreal`  
**默认值:** `0`

## 尺寸规格

- 组件高度: 35px
- 文本左边距: 16px
- 文本右边距: 15px
- 指示器区域宽度: 25px
- 阴影边框宽度: 6px
- 项目高度: 35px

## 使用示例

### 基础用法

#### 1. 创建下拉框
```cpp
// 基础构造
ElaComboBox* comboBox = new ElaComboBox(this);

// 添加选项
comboBox->addItem("Option 1");
comboBox->addItems(QStringList{"Option 2", "Option 3", "Option 4"});

// 添加带图标的选项
comboBox->addItem(QIcon(":/icons/user.png"), "User");
```

#### 2. 多选模式
```cpp
// 创建多选下拉框
ElaMultiSelectComboBox* multiBox = new ElaMultiSelectComboBox(this);

// 添加选项
multiBox->addItems(QStringList{"Item 1", "Item 2", "Item 3"});

// 设置当前选中项
multiBox->setCurrentSelection(QStringList{"Item 1", "Item 3"});

// 监听选择变化
connect(multiBox, &ElaMultiSelectComboBox::currentTextListChanged,
    this, [](QStringList selected) {
        qDebug() << "Selected items:" << selected;
});
```

### 高级用法

#### 1. 自定义样式
```cpp
ElaComboBox* comboBox = new ElaComboBox(this);
comboBox->setBorderRadius(4);  // 设置圆角半径

// 设置固定宽度
comboBox->setFixedWidth(200);

// 设置最大显示项数
comboBox->setMaxVisibleItems(8);
```

#### 2. 动态更新选项
```cpp
void updateComboBoxItems(ElaComboBox* comboBox, 
                        const QStringList& items) {
    comboBox->clear();
    comboBox->addItems(items);
    
    // 保持之前的选择
    if (!previousSelection.isEmpty() && 
        items.contains(previousSelection)) {
        comboBox->setCurrentText(previousSelection);
    }
}
```

#### 3. 自定义显示格式
```cpp
// 设置项目代理
class CustomItemDelegate : public QStyledItemDelegate {
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override {
        // 自定义绘制逻辑
    }
};

comboBox->setItemDelegate(new CustomItemDelegate(comboBox));
```

## 注意事项
1. 默认高度固定为 35 像素
2. 展开动画持续时间为 300ms
3. 主题切换会自动更新样式
4. 多选模式下支持空选
5. 指示器使用 180° 旋转动画
6. 弹出框具有阴影效果
7. 滚动采用像素级滚动

## 相关组件
- ElaMultiSelectComboBox: 多选下拉框
- ElaScrollBar: 滚动条组件
- ElaTheme: 主题管理器
