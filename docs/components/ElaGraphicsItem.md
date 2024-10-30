# ElaGraphics 图形组件文档

## 概述
ElaGraphics 系列组件包含 ElaGraphicsItem(图形项)和 ElaGraphicsLineItem(连线项)两个主要组件,用于构建可视化图形编辑场景。

## ElaGraphicsItem

### 特性
- 可移动/选择
- 图像显示
- 连接端口
- 序列化支持
- 事件处理
- 状态管理
- 数据路由
- 唯一标识

### 属性
```cpp
// 基础属性
Q_PROPERTY(int width READ getWidth WRITE setWidth)
Q_PROPERTY(int height READ getHeight WRITE setHeight)
Q_PROPERTY(QImage itemImage READ getItemImage WRITE setItemImage)
Q_PROPERTY(QImage itemSelectedImage READ getItemSelectedImage WRITE setItemSelectedImage)
Q_PROPERTY(QString itemName READ getItemName WRITE setItemName)
Q_PROPERTY(QVariantMap dataRoutes READ getDataRoutes WRITE setDataRoutes)
Q_PROPERTY(int maxLinkPortCount READ getMaxLinkPortCount WRITE setMaxLinkPortCount)
```

### 公共方法

#### 端口操作
```cpp
// 设置端口状态
void setLinkPortState(bool isFullLink);
void setLinkPortState(bool isLink, int portIndex);

// 获取端口状态
bool getLinkPortState(int portIndex) const;
QVector<bool> getLinkPortState() const;

// 获取已用/未用端口
int getUsedLinkPortCount() const;
QVector<int> getUsedLinkPort() const;
int getUnusedLinkPortCount() const;
QVector<int> getUnusedLinkPort() const;
```

## ElaGraphicsLineItem

### 特性
- 贝塞尔曲线
- 端口连接
- 抗锯齿
- 可选择
- 状态管理
- 双向连接
- 端口映射
- 自动更新

### 属性
```cpp
// 基础属性
Q_PROPERTY(QPointF startPoint READ getStartPoint WRITE setStartPoint)
Q_PROPERTY(QPointF endPoint READ getEndPoint WRITE setEndPoint)
Q_PROPERTY(ElaGraphicsItem* startItem READ getStartItem WRITE setStartItem)
Q_PROPERTY(ElaGraphicsItem* endItem READ getEndItem WRITE setEndItem)
Q_PROPERTY(int startItemPort READ getStartItemPort WRITE setStartItemPort)
Q_PROPERTY(int endItemPort READ getEndItemPort WRITE setEndItemPort)
```

### 公共方法

#### 连接检查
```cpp
// 检查目标连接
bool isTargetLink(ElaGraphicsItem* item) const;
bool isTargetLink(
    ElaGraphicsItem* item1, 
    ElaGraphicsItem* item2
) const;
bool isTargetLink(
    ElaGraphicsItem* item1,
    ElaGraphicsItem* item2,
    int port1,
    int port2
) const;
```

## 使用示例

### 图形项

#### 1. 创建图形项
```cpp
// 创建基础图形项
auto item = new ElaGraphicsItem();

// 创建指定大小的图形项
auto item = new ElaGraphicsItem(100, 100);

// 设置图形项属性
item->setItemName("MyItem");
item->setMaxLinkPortCount(4);
item->setItemImage(QImage("item.png"));
```

#### 2. 端口管理
```cpp
// 设置端口状态
item->setLinkPortState(true, 0); // 设置端口0为已连接
item->setLinkPortState(false);   // 设置所有端口为未连接

// 获取可用端口
auto unusedPorts = item->getUnusedLinkPort();
if (unusedPorts.count() > 0) {
    int port = unusedPorts.first();
    // 使用端口
}
```

### 连线项

#### 1. 创建连线
```cpp
// 创建项之间的连线
auto line = new ElaGraphicsLineItem(
    startItem,    // 起始项
    endItem,      // 结束项
    0,           // 起始端口
    1            // 结束端口
);

// 创建点之间的连线
auto line = new ElaGraphicsLineItem(
    QPointF(0, 0),    // 起始点
    QPointF(100, 100) // 结束点
);
```

#### 2. 连接检查
```cpp
// 检查项是否连接
if (line->isTargetLink(item)) {
    // 项已连接
}

// 检查特定端口连接
if (line->isTargetLink(
    item1, item2, port1, port2)) {
    // 指定端口已连接
}
```

## 注意事项
1. 图形项管理
2. 端口状态
3. 连线更新
4. 性能优化
5. 内存管理
6. 事件处理
7. 序列化
8. 状态同步

## 最佳实践
1. 合理设置端口数
2. 优化连线绘制
3. 控制图形项数量
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QGraphicsObject: 图形项基类
- QGraphicsPathItem: 路径项基类
- QGraphicsScene: 场景组件
- QGraphicsView: 视图组件
```