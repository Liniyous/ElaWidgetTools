# ElaSuggestBox 组件文档

## 概述
ElaSuggestBox 是一个现代化的搜索建议框组件,提供了实时搜索、建议列表、动画效果等特性。它适用于需要搜索和建议功能的场景。

## 特性
- 实时搜索
- 建议列表
- 动画效果
- 主题适配
- 图标支持
- 自定义数据
- 大小写敏感
- 阴影效果

## 属性

### 基础属性
```cpp
// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 大小写敏感
Q_PROPERTY(Qt::CaseSensitivity caseSensitivity 
    READ getCaseSensitivity 
    WRITE setCaseSensitivity)
```

## 公共方法

### 建议管理
```cpp
// 添加建议
void addSuggestion(
    const QString& suggestText,
    const QVariantMap& suggestData = {}
);

// 添加带图标的建议
void addSuggestion(
    ElaIconType::IconName icon,
    const QString& suggestText,
    const QVariantMap& suggestData = {}
);

// 移除建议
void removeSuggestion(
    const QString& suggestText
);
void removeSuggestion(int index);

// 设置占位符
void setPlaceholderText(
    const QString& placeholderText
);
```

## 信号
```cpp
// 建议被点击
Q_SIGNAL void suggestionClicked(
    QString suggestText, 
    QVariantMap suggestData
);
```

## 动画系统

### 大小动画
```cpp
void _startSizeAnimation(
    QSize oldSize, 
    QSize newSize
) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            widget, "size"
        );
    animation->setDuration(300);
    animation->setEasingCurve(
        QEasingCurve::InOutSine
    );
    animation->setStartValue(oldSize);
    animation->setEndValue(newSize);
    animation->start();
}
```

### 展开/收起动画
```cpp
void _startExpandAnimation() {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(
            view, "pos"
        );
    animation->setDuration(300);
    animation->setStartValue(startPos);
    animation->setEndValue(endPos);
    animation->start();
}
```

## 渲染系统

### 容器渲染
```cpp
void paintEvent() {
    // 绘制阴影
    theme->drawEffectShadow(
        &painter, rect, 6, 8
    );
    
    // 绘制背景
    painter.setPen(borderColor);
    painter.setBrush(baseColor);
    painter.drawRoundedRect(
        foregroundRect, 8, 8
    );
}
```

## 使用示例

### 基础用法

#### 1. 创建搜索框
```cpp
// 创建搜索框
ElaSuggestBox* box = 
    new ElaSuggestBox(this);

// 设置占位符
box->setPlaceholderText("搜索...");

// 设置大小写敏感
box->setCaseSensitivity(
    Qt::CaseInsensitive
);
```

#### 2. 添加建议
```cpp
// 添加文本建议
box->addSuggestion("建议1");

// 添加带图标的建议
box->addSuggestion(
    ElaIconType::Search,
    "建议2"
);

// 添加带数据的建议
QVariantMap data;
data["key"] = "value";
box->addSuggestion("建议3", data);
```

#### 3. 处理事件
```cpp
// 建议点击
connect(box, 
    &ElaSuggestBox::suggestionClicked,
    this, [=](QString text, 
        QVariantMap data) {
    qDebug() << "Clicked:" << text;
    qDebug() << "Data:" << data;
});
```

## 注意事项
1. 继承自 QWidget
2. 自动处理主题切换
3. 动画时长300ms
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 主题适配
8. 样式继承

## 最佳实践
1. 合理设置大小
2. 控制建议数量
3. 优化搜索逻辑
4. 注意性能影响
5. 统一交互方式
6. 保持样式一致
7. 动画流畅性
8. 响应式布局

## 相关组件
- QWidget: 基类
- ElaLineEdit: 输入框组件
- ElaBaseListView: 列表组件
- ElaTheme: 主题管理
- QPropertyAnimation: 动画系统