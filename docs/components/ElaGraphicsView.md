# ElaGraphicsView 组件文档

## 概述
ElaGraphicsView 是一个现代化的图形视图组件,继承自 QGraphicsView。它提供了缩放、平移、抗锯齿等特性,适用于图形编辑场景。

## 特性
- 平滑缩放
- 拖拽平移
- 抗锯齿渲染
- 橡皮筋选择
- 自定义滚动条
- 鼠标追踪
- 键盘控制
- 性能优化

## 属性

### 基础属性
```cpp
// 最大缩放比例
Q_PROPERTY(qreal maxTransform 
    READ getMaxTransform 
    WRITE setMaxTransform)

// 最小缩放比例
Q_PROPERTY(qreal minTransform 
    READ getMinTransform 
    WRITE setMinTransform)
```

## 构造函数
```cpp
// 基础构造
explicit ElaGraphicsView(
    QWidget* parent = nullptr
);

// 指定场景构造
explicit ElaGraphicsView(
    QGraphicsScene* scene, 
    QWidget* parent = nullptr
);
```

## 事件处理

### 鼠标滚轮事件
```cpp
void wheelEvent(QWheelEvent* event) override {
    // Ctrl + 滚轮实现缩放
    if (event->modifiers() == Qt::CTRL) {
        if (event->angleDelta().y() > 0) {
            // 放大
            scale(1.1, 1.1);
        } else {
            // 缩小
            scale(1.0/1.1, 1.0/1.1);
        }
    }
}
```

### 键盘事件
```cpp
void keyPressEvent(QKeyEvent* event) override {
    // Alt 键按下切换为手型工具
    if (event->key() == Qt::Key_Alt) {
        setDragMode(
            QGraphicsView::ScrollHandDrag
        );
    }
}

void keyReleaseEvent(QKeyEvent* event) override {
    // Alt 键释放恢复橡皮筋选择
    if (event->key() == Qt::Key_Alt) {
        setDragMode(
            QGraphicsView::RubberBandDrag
        );
    }
}
```

## 使用示例

### 基础用法

#### 1. 创建视图
```cpp
// 创建视图
ElaGraphicsView* view = 
    new ElaGraphicsView(this);

// 设置场景
view->setScene(scene);

// 设置缩放范围
view->setMaxTransform(5.0);
view->setMinTransform(0.15);
```

#### 2. 视图设置
```cpp
// 设置渲染提示
view->setRenderHints(
    QPainter::Antialiasing |
    QPainter::TextAntialiasing |
    QPainter::SmoothPixmapTransform
);

// 设置变换锚点
view->setTransformationAnchor(
    QGraphicsView::AnchorUnderMouse
);

// 设置调整锚点
view->setResizeAnchor(
    QGraphicsView::AnchorUnderMouse
);
```

#### 3. 交互控制
```cpp
// 启用鼠标追踪
view->setMouseTracking(true);

// 设置拖拽模式
view->setDragMode(
    QGraphicsView::RubberBandDrag
);

// 自定义滚动条
view->setVerticalScrollBar(
    new ElaScrollBar(view)
);
view->setHorizontalScrollBar(
    new ElaScrollBar(view)
);
```

### 高级用法

#### 1. 缩放控制
```cpp
// 自定义缩放步长
void customZoom(qreal factor) {
    qreal currentScale = transform().m11();
    if (factor > 1.0 && 
        currentScale <= maxTransform) {
        scale(factor, factor);
    } 
    else if (factor < 1.0 && 
        currentScale >= minTransform) {
        scale(factor, factor);
    }
}
```

#### 2. 视图变换
```cpp
// 重置视图
void resetView() {
    resetTransform();
    centerOn(0, 0);
}

// 适应视图
void fitView() {
    fitInView(
        scene()->sceneRect(),
        Qt::KeepAspectRatio
    );
}
```

## 注意事项
1. 继承自 QGraphicsView
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 缩放限制
7. 平滑渲染
8. 交互控制

## 最佳实践
1. 合理设置缩放范围
2. 优化渲染性能
3. 控制刷新频率
4. 注意内存使用
5. 统一交互方式
6. 保持流畅度
7. 动画平滑
8. 响应式布局

## 相关组件
- QGraphicsView: 基类
- ElaGraphicsScene: 场景组件
- ElaScrollBar: 滚动条组件
- QGraphicsItem: 图形项基类
```