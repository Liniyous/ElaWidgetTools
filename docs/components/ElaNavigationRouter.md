# ElaNavigationRouter 组件文档

## 概述
ElaNavigationRouter 是一个现代化的导航路由组件,提供了路由管理、状态追踪、数据传递等特性。它适用于管理应用程序的导航状态和页面跳转。

## 特性
- 路由管理
- 状态追踪
- 数据传递
- 回退支持
- 队列管理
- 单例模式
- 连接类型
- 最大限制

## 属性

### 基础属性
```cpp
// 最大路由数量
Q_PROPERTY(int maxRouteCount 
    READ getMaxRouteCount 
    WRITE setMaxRouteCount)
```

## 公共方法

### 路由管理
```cpp
// 导航路由
ElaNavigationRouterType::NavigationRouteType 
navigationRoute(
    QObject* routeObject,
    QString routeFunctionName,
    const QVariantMap& routeData = {},
    Qt::ConnectionType connectionType = 
        Qt::AutoConnection
);

// 路由回退
void navigationRouteBack();
```

## 信号
```cpp
// 路由状态改变
Q_SIGNAL void navigationRouterStateChanged(
    bool state
);
```

## 路由类型
```cpp
namespace ElaNavigationRouterType {
    enum NavigationRouteType {
        Success,            // 成功
        ObjectInvalid,     // 对象无效
        FunctionNameInvalid // 函数名无效
    };
}
```

## 数据结构

### 路由数据
```cpp
QVariantMap routeData {
    "ElaRouteObject": QObject*,      // 路由对象
    "ElaRouteFunctionName": QString, // 函数名
    "ElaRouteData": QVariantMap,    // 传递数据
    "ElaRouteConnectionType": Qt::ConnectionType 
                                    // 连接类型
};
```

## 使用示例

### 基础用法

#### 1. 获取实例
```cpp
// 获取路由实例
auto router = ElaNavigationRouter::getInstance();

// 设置最大路由数
router->setMaxRouteCount(25);
```

#### 2. 导航路由
```cpp
// 基础路由
router->navigationRoute(
    targetObject,
    "onNavigate"
);

// 带数据路由
QVariantMap data;
data["key"] = "value";
router->navigationRoute(
    targetObject,
    "onNavigate",
    data
);

// 指定连接类型
router->navigationRoute(
    targetObject,
    "onNavigate",
    data,
    Qt::QueuedConnection
);
```

#### 3. 路由回退
```cpp
// 回退到上一个路由
router->navigationRouteBack();

// 监听路由状态
connect(router, 
    &ElaNavigationRouter::navigationRouterStateChanged,
    this, [=](bool state) {
    qDebug() << "Router State:" << state;
});
```

#### 4. 接收路由
```cpp
// 在目标对象中定义槽函数
public slots:
void onNavigate(QVariantMap data) {
    QString value = data["key"].toString();
    // 处理导航逻辑
}
```

## 注意事项
1. 单例模式
2. 队列管理
3. 最大限制25
4. 智能内存管理
5. 事件处理
6. 性能优化
7. 状态追踪
8. 数据传递

## 最佳实践
1. 合理设置限制
2. 控制路由数量
3. 及时清理路由
4. 注意性能影响
5. 统一交互方式
6. 保持状态一致
7. 异步处理
8. 错误处理

## 相关组件
- QObject: 基类
- QQueue: 队列管理
- QVariantMap: 数据容器
- QMetaObject: 元对象系统
