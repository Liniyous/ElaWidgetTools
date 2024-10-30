# ElaExponentialBlur 组件文档

## 概述
ElaExponentialBlur 是一个高性能的指数模糊组件,提供了图像模糊处理功能。它使用指数算法实现快速且高质量的模糊效果。

## 特性
- 指数模糊算法
- 高性能处理
- 可调节模糊半径
- 支持透明度
- 单例模式
- 行列扫描
- 精确控制
- 内存优化

## 公共方法

### 模糊处理
```cpp
// 执行指数模糊
static QPixmap doExponentialBlur(
    QImage img,           // 源图像
    const quint16& radius // 模糊半径
);
```

## 使用示例

### 基础用法

#### 1. 模糊处理
```cpp
// 加载原始图像
QImage image("image.png");

// 执行模糊处理
QPixmap blurred = 
    ElaExponentialBlur::doExponentialBlur(
        image,  // 源图像
        10      // 模糊半径
    );

// 使用模糊后的图像
label->setPixmap(blurred);
```

#### 2. 调整模糊程度
```cpp
// 轻度模糊
QPixmap lightBlur = 
    ElaExponentialBlur::doExponentialBlur(
        image, 5
    );

// 中度模糊
QPixmap mediumBlur = 
    ElaExponentialBlur::doExponentialBlur(
        image, 10
    );

// 重度模糊
QPixmap heavyBlur = 
    ElaExponentialBlur::doExponentialBlur(
        image, 20
    );
```

### 高级用法

#### 1. 性能优化
```cpp
// 预处理图像格式
QImage optimizedImage = 
    sourceImage.convertToFormat(
        QImage::Format_ARGB32
    );

// 执行模糊
QPixmap blurred = 
    ElaExponentialBlur::doExponentialBlur(
        optimizedImage, 
        radius
    );
```

#### 2. 模糊参数
```cpp
// 模糊算法参数
int aprec = 12;  // 精度控制
int zprec = 7;   // 缩放控制

// 计算alpha值
float factor = 2.3f;
int alpha = (int)((1 << aprec) * 
    (1.0f - std::exp(-factor / 
        (radius + 1.f))));
```

## 实现细节

### 模糊算法
```cpp
void drawExponentialBlur(
    QImage& image, 
    const quint16& radius
) {
    // 转换图像格式
    image = image.convertToFormat(
        QImage::Format_ARGB32_Premultiplied
    );
    
    // 计算模糊系数
    int alpha = calculateAlpha(radius);
    
    // 行模糊处理
    for (int row = 0; row < height; row++) {
        drawRowBlur(image, row, alpha);
    }
    
    // 列模糊处理
    for (int col = 0; col < width; col++) {
        drawColumnBlur(image, col, alpha);
    }
}
```

## 注意事项
1. 单例模式设计
2. 内存管理优化
3. 性能考虑
4. 图像格式兼容
5. 参数调优
6. 精度控制
7. 边界处理
8. 透明度处理

## 最佳实践
1. 合理设置半径
2. 优化图像大小
3. 控制处理频率
4. 注意性能
5. 缓存结果
6. 异步处理
7. 内存复用
8. 批量处理

## 相关组件
- QImage: 图像处理
- QPixmap: 图像显示
- QPainter: 绘制系统
- QRgb: 颜色处理
```