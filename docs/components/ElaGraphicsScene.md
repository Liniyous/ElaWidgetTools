# ElaGraphicsScene 组件文档

## 概述
ElaGraphicsScene 是一个现代化的图形场景组件,继承自 QGraphicsScene。它提供了图形项管理、连线功能、序列化等特性。

## 特性
- 图形项管理
- 连线功能
- 序列化支持
- 场景模式
- 事件处理
- 选择管理
- 坐标转换
- 性能优化

## 属性

### 基础属性
```cpp
// 检查连接端口
Q_PROPERTY(bool isCheckLinkPort 
    READ getIsCheckLinkPort 
    WRITE setIsCheckLinkPort)

// 序列化路径
Q_PROPERTY(QString serializePath 
    READ getSerializePath 
    WRITE setSerializePath)
```

## 公共方法

### 图形项操作
```cpp
// 添加图形项
void addItem(ElaGraphicsItem* item);

// 移除图形项
void removeItem(ElaGraphicsItem* item);

// 移除选中项
void removeSelectedItems();

// 清空场景
void clear();

// 创建并添加图形项
QList<ElaGraphicsItem*> createAndAddItem(
    int width, 
    int height, 
    int count = 1
);
```

### 图形项查询
```cpp
// 获取选中的图形项
QList<ElaGraphicsItem*> getSelectedElaItems() const;

// 获取所有图形项
QList<ElaGraphicsItem*> getElaItems();

// 获取指定位置的图形项
QList<ElaGraphicsItem*> getElaItems(QPoint pos);
QList<ElaGraphicsItem*> getElaItems(QPointF pos);
QList<ElaGraphicsItem*> getElaItems(QRect rect);
QList<ElaGraphicsItem*> getElaItems(QRectF rect);
```

### 连线操作
```cpp
// 获取连线列表
QList<QVariantMap> getItemLinkList() const;

// 添加连线
bool addItemLink(
    ElaGraphicsItem* item1,
    ElaGraphicsItem* item2,
    int port1 = 0,
    int port2 = 0
);

// 移除连线
bool removeItemLink(ElaGraphicsItem* item1);
bool removeItemLink(
    ElaGraphicsItem* item1,
    ElaGraphicsItem* item2,
    int port1 = 0,
    int port2 = 0
);
```

### 序列化
```cpp
// 序列化场景
void serialize();

// 反序列化场景
void deserialize();
```

## 信号
```cpp
// 显示连线
void showItemLink();

// 鼠标事件
void mouseLeftClickedItem(ElaGraphicsItem* item);
void mouseRightClickedItem(ElaGraphicsItem* item);
void mouseDoubleClickedItem(ElaGraphicsItem* item);
```

## 使用示例

### 基础用法

#### 1. 创建场景
```cpp
// 创建场景
ElaGraphicsScene* scene = 
    new ElaGraphicsScene();

// 设置场景模式
scene->setSceneMode(
    ElaGraphicsSceneType::SceneMode::Default
);

// 添加到视图
view->setScene(scene);
```

#### 2. 图形项管理
```cpp
// 创建图形项
auto items = scene->createAndAddItem(
    100,  // 宽度
    100,  // 高度
    2     // 数量
);

// 移除选中项
scene->removeSelectedItems();

// 清空场景
scene->clear();
```

#### 3. 连线操作
```cpp
// 添加连线
scene->addItemLink(
    item1,  // 第一个图形项
    item2,  // 第二个图形项
    0,      // 端口1
    1       // 端口2
);

// 移除连线
scene->removeItemLink(item1, item2);
```

### 高级用法

#### 1. 序列化
```cpp
// 设置序列化路径
scene->setSerializePath("./scene.dat");

// 保存场景
scene->serialize();

// 加载场景
scene->deserialize();
```

#### 2. 事件处理
```cpp
// 连接信号
connect(scene, 
    &ElaGraphicsScene::mouseLeftClickedItem,
    this, [](ElaGraphicsItem* item) {
    // 处理左键点击
});

connect(scene,
    &ElaGraphicsScene::showItemLink,
    this, []{
    // 显示连线对话框
});
```

## 注意事项
1. 继承自 QGraphicsScene
2. 自动处理主题切换
3. 智能内存管理
4. 事件处理
5. 性能优化
6. 连线检查
7. 序列化完整性
8. 坐标转换

## 最佳实践
1. 合理设置大小
2. 控制图形项数量
3. 优化连线显示
4. 注意性能
5. 统一交互
6. 保持一致
7. 动画流畅
8. 响应式布局

## 相关组件
- QGraphicsScene: 基类
- ElaGraphicsItem: 图形项
- ElaGraphicsLineItem: 连线项
- QGraphicsView: 视图组件
```