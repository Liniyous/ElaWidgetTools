# ElaToolButton 组件文档

## 概述
ElaToolButton 是一个现代化的工具按钮组件,继承自 QToolButton。它提供了自定义样式、动画效果、主题适配等特性。

## 特性
- 自定义样式
- 动画效果
- 主题适配
- 圆角边框
- 透明背景
- 菜单支持
- 图标支持
- 文本支持

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius 
    READ getBorderRadius 
    WRITE setBorderRadius)

// 选中状态
Q_PROPERTY(bool isSelected 
    READ getIsSelected 
    WRITE setIsSelected)
```

## 公共方法

### 样式设置
```cpp
// 设置透明背景
void setIsTransparent(bool isTransparent);

// 设置菜单
void setMenu(ElaMenu* menu);

// 设置图标
void setElaIcon(ElaIconType::IconName icon);
```

## 样式系统

### 按钮样式
```cpp
void drawComplexControl(CC_ToolButton) {
    // 背景绘制
    if (state & State_Enabled) {
        if (state & State_Sunken) {
            painter.setBrush(pressColor);
        } else if (isSelected) {
            painter.setBrush(selectedColor);
        } else if (state & State_MouseOver) {
            painter.setBrush(hoverColor);
        }
        painter.drawRoundedRect(
            rect, radius, radius
        );
    }
}
```

### 菜单指示器
```cpp
void drawIndicator() {
    // 设置字体
    painter.setFont("ElaAwesome");
    
    // 绘制展开图标
    painter.drawText(
        rect,
        Qt::AlignCenter,
        AngleDown
    );
    
    // 旋转动画
    painter.rotate(expandIconRotate);
}
```

## 使用示例

### 基础用法

#### 1. 创建按钮
```cpp
// 创建按钮
ElaToolButton* button = 
    new ElaToolButton(this);

// 设置图标
button->setElaIcon(
    ElaIconType::Broom
);

// 设置文本
button->setText("清理");
```

#### 2. 样式设置
```cpp
// 设置圆角
button->setBorderRadius(4);

// 设置透明背景
button->setIsTransparent(true);

// 设置图标大小
button->setIconSize(QSize(22, 22));

// 设置弹出模式
button->setPopupMode(
    QToolButton::InstantPopup
);
```

#### 3. 菜单设置
```cpp
// 创建菜单
ElaMenu* menu = new ElaMenu(this);
menu->addAction("选项1");
menu->addAction("选项2");

// 设置菜单
button->setMenu(menu);

// 设置菜单项高度
menu->setMenuItemHeight(27);
```

#### 4. 状态设置
```cpp
// 设置选中状态
button->setIsSelected(true);

// 设置禁用状态
button->setEnabled(false);

// 设置按钮样式
button->setToolButtonStyle(
    Qt::ToolButtonTextBesideIcon
);
```

## 注意事项
1. 继承自 QToolButton
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 主题适配
7. 样式继承
8. 动画流畅

## 最佳实践
1. 合理设置大小
2. 控制菜单项数量
3. 优化动画效果
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QToolButton: 基类
- ElaMenu: 菜单组件
- ElaIcon: 图标系统
- QProxyStyle: 样式系统
