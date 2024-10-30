# ElaColorDialog 组件文档

## 概述
ElaColorDialog 是一个现代化的颜色选择对话框组件,提供了丰富的颜色选择功能。它支持 RGB/HSV 颜色模式、基础/自定义颜色面板、颜色预览等特性。

## 特性
- RGB/HSV 双色彩模式
- 颜色选择器面板
- 亮度调节滑块
- 颜色预览
- 基础颜色面板
- 自定义颜色面板
- 十六进制颜色输入
- 数值精确调节
- 主题适配
- 模态对话框

## 属性

### CurrentColor
当前选中的颜色。

**类型:** `QColor`  
**默认值:** `QColor(0x80, 0xFF, 0xEF)`

## 公共方法

### 颜色操作
```cpp
// 设置当前颜色
void setCurrentColor(QColor color);

// 获取当前颜色
QColor getCurrentColor() const;

// 获取当前颜色的 RGB 值(十六进制)
QString getCurrentColorRGB() const;

// 获取自定义颜色列表
QList<QColor> getCustomColorList() const;

// 获取指定索引的自定义颜色
QColor getCustomColor(int index) const;
```

## 信号

### colorSelected
颜色被选中时触发。

**参数:**
- `const QColor& color`: 选中的颜色

### pCurrentColorChanged
当前颜色发生变化时触发。

## 使用示例

### 基础用法

#### 1. 创建颜色对话框
```cpp
// 创建对话框
ElaColorDialog* colorDialog = new ElaColorDialog(this);

// 设置初始颜色
colorDialog->setCurrentColor(QColor(255, 0, 0));

// 显示对话框
if (colorDialog->exec() == QDialog::Accepted) {
    QColor selectedColor = colorDialog->getCurrentColor();
    // 处理选中的颜色
}
```

#### 2. 监听颜色选择
```cpp
connect(colorDialog, &ElaColorDialog::colorSelected,
    this, [](const QColor& color) {
        qDebug() << "Selected color:" << color;
        qDebug() << "RGB hex:" << colorDialog->getCurrentColorRGB();
});
```

### 高级用法

#### 1. 自定义颜色管理
```cpp
// 获取自定义颜色列表
QList<QColor> customColors = colorDialog->getCustomColorList();

// 保存自定义颜色
void saveCustomColors() {
    QSettings settings;
    settings.beginWriteArray("CustomColors");
    QList<QColor> colors = colorDialog->getCustomColorList();
    for (int i = 0; i < colors.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("color", colors[i]);
    }
    settings.endArray();
}
```

#### 2. 颜色预览集成
```cpp
ElaPushButton* previewButton = new ElaPushButton(this);
previewButton->setFixedSize(35, 35);

connect(colorDialog, &ElaColorDialog::colorSelected,
    this, [=](const QColor& color) {
        // 更新按钮颜色
        previewButton->setLightDefaultColor(color);
        previewButton->setDarkDefaultColor(color);
        
        // 更新颜色文本
        colorLabel->setText(colorDialog->getCurrentColorRGB());
});
```

#### 3. 实时颜色更新
```cpp
connect(colorDialog, &ElaColorDialog::pCurrentColorChanged,
    this, [=]() {
        QColor color = colorDialog->getCurrentColor();
        updatePreview(color);
});
```

## 注意事项
1. 对话框固定大小为 620x630 像素
2. 标题栏高度为 30 像素
3. 支持 RGB/HSV 双色彩模式切换
4. 基础颜色面板为只读
5. 自定义颜色面板支持 24 个颜色
6. 颜色值支持十六进制和数值输入
7. 主题切换会自动更新界面样式

## 相关组件
- ElaColorPicker: 颜色选择器
- ElaColorPreview: 颜色预览
- ElaAppBar: 标题栏组件
- ElaTheme: 主题管理器
