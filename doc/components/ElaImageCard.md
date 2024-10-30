# ElaImageCard 组件文档

## 概述
ElaImageCard 是一个现代化的图片卡片组件,提供了圆角、裁剪、纵横比控制等特性。它能够自动处理图片的显示效果,并支持主题切换。

## 特性
- 圆角边框
- 纵横比控制
- 智能裁剪
- 平滑渲染
- 主题适配
- 最小尺寸限制
- 抗锯齿
- 图片缩放

## 属性

### 基础属性
```cpp
// 卡片图片
Q_PROPERTY(QImage cardImage READ getCardImage 
    WRITE setCardImage)

// 边框圆角
Q_PROPERTY(int borderRadius READ getBorderRadius 
    WRITE setBorderRadius)

// 是否保持纵横比裁剪
Q_PROPERTY(bool isPreserveAspectCrop 
    READ getIsPreserveAspectCrop 
    WRITE setIsPreserveAspectCrop)

// 最大纵横比
Q_PROPERTY(qreal maximumAspectRatio 
    READ getMaximumAspectRatio 
    WRITE setMaximumAspectRatio)
```

## 默认值
- 最小尺寸: 350x260
- 边框圆角: 6px
- 最大纵横比: 2.2
- 保持纵横比: true

## 使用示例

### 基础用法

#### 1. 创建图片卡片
```cpp
// 创建卡片
ElaImageCard* imageCard = new ElaImageCard(this);

// 设置图片
imageCard->setCardImage(QImage(":/images/background.png"));

// 设置圆角
imageCard->setBorderRadius(10);

// 设置固定大小
imageCard->setFixedSize(400, 300);
```

#### 2. 纵横比控制
```cpp
// 设置最大纵横比
imageCard->setMaximumAspectRatio(1.7);

// 控制是否保持纵横比
imageCard->setIsPreserveAspectCrop(true);
```

### 高级用法

#### 1. 自定义渲染
```cpp
class CustomImageCard : public ElaImageCard {
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHints(
            QPainter::SmoothPixmapTransform | 
            QPainter::Antialiasing
        );
        
        // 绘制背景
        QPainterPath path;
        path.addRoundedRect(rect(), 
            getBorderRadius(), 
            getBorderRadius());
        painter.setClipPath(path);
        
        // 自定义绘制逻辑
        if (getIsPreserveAspectCrop()) {
            // 计算纵横比
            qreal aspectRatio = (qreal)width() / height();
            if (aspectRatio < getMaximumAspectRatio()) {
                // 处理纵横比限制
                // ...
            }
        }
        
        // 绘制图片
        painter.drawImage(rect(), getCardImage());
    }
};
```

#### 2. 主题适配
```cpp
// 连接主题变化信号
connect(ElaTheme::getInstance(), 
    &ElaTheme::themeModeChanged,
    this, [=](ElaThemeType::ThemeMode mode) {
        // 更新主题相关样式
        update();
});
```

## 图片处理算法

### 纵横比裁剪
```cpp
if (isPreserveAspectCrop) {
    qreal itemAspectRatio = (qreal)width() / height();
    if (itemAspectRatio < maximumAspectRatio) {
        // 使用最大纵横比
        qreal cropHeight = image.width() / maximumAspectRatio;
        painter.drawImage(
            QRect(0, 0, height() * maximumAspectRatio, height()),
            image,
            QRectF(0, 0, image.width(), cropHeight)
        );
    } else {
        // 使用当前纵横比
        qreal cropHeight = image.width() / itemAspectRatio;
        painter.drawImage(
            rect(),
            image,
            QRectF(0, 0, image.width(), cropHeight)
        );
    }
}
```

## 注意事项
1. 图片自动缩放适应
2. 保持纵横比时会裁剪图片
3. 最小尺寸限制
4. 主题切换自动更新
5. 抗锯齿渲染
6. 平滑缩放
7. 圆角裁剪
8. 性能优化

## 最佳实践
1. 使用合适的图片尺寸
2. 控制纵横比范围
3. 注意内存使用
4. 合理设置圆角
5. 优化图片质量
6. 处理主题切换
7. 考虑性能影响
8. 适当缓存图片

## 相关组件
- QPainter: 绘制系统
- QPainterPath: 路径绘制
- QImage: 图片处理
- ElaTheme: 主题管理
