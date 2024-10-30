# ElaEventBus 组件文档

## 概述
ElaEventBus 是一个轻量级的事件总线组件,用于实现组件间的解耦通信。它支持事件的注册、发布和订阅,并提供了自动连接类型和参数传递功能。

## 主要组件

### ElaEventBus
事件总线管理器,负责事件的分发和管理。

### ElaEvent
事件对象,用于定义和注册具体的事件。

## 特性
- 单例模式
- 支持多订阅者
- 自动连接类型
- 参数传递
- 事件名管理
- 自动注销
- 线程安全
- 类型安全

## 公共方法

### ElaEventBus

```cpp
// 发布事件
ElaEventBusType::EventBusReturnType post(
    const QString& eventName, 
    const QVariantMap& data = {});

// 获取已注册的事件名列表
QStringList getRegisteredEventsName() const;
```

### ElaEvent

```cpp
// 构造函数
explicit ElaEvent(
    QString eventName,
    QString functionName,
    QObject* parent = nullptr);

// 注册并初始化事件
ElaEventBusType::EventBusReturnType registerAndInit();

// 属性设置
void setEventName(const QString& eventName);
void setFunctionName(const QString& functionName);
void setConnectionType(Qt::ConnectionType connectionType);
```

## 使用示例

### 基础用法

#### 1. 注册事件
```cpp
class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget(QWidget* parent = nullptr) : QWidget(parent) {
        // 创建并注册事件
        ElaEvent* event = new ElaEvent(
            "updateUI",           // 事件名
            "onUpdateUI",         // 处理函数名
            this                  // 父对象
        );
        event->registerAndInit();
    }
    
public slots:
    // 事件处理函数
    void onUpdateUI(QVariantMap data) {
        QString message = data["message"].toString();
        qDebug() << "Received update:" << message;
    }
};
```

#### 2. 发布事件
```cpp
// 获取事件总线实例
ElaEventBus* eventBus = ElaEventBus::getInstance();

// 发布事件
QVariantMap data;
data["message"] = "UI needs update";
data["timestamp"] = QDateTime::currentDateTime();

eventBus->post("updateUI", data);
```

### 高级用法

#### 1. 自定义连接类型
```cpp
// 创建事件对象
ElaEvent* event = new ElaEvent(this);
event->setEventName("asyncUpdate");
event->setFunctionName("onAsyncUpdate");
event->setConnectionType(Qt::QueuedConnection);
event->registerAndInit();
```

#### 2. 多订阅者模式
```cpp
// 第一个订阅者
ElaEvent* event1 = new ElaEvent(
    "globalUpdate", 
    "onUpdate",
    subscriber1
);
event1->registerAndInit();

// 第二个订阅者
ElaEvent* event2 = new ElaEvent(
    "globalUpdate", 
    "onUpdate",
    subscriber2
);
event2->registerAndInit();

// 发布事件 - 两个订阅者都会收到
eventBus->post("globalUpdate", data);
```

#### 3. 事件管理
```cpp
// 获取所有已注册的事件名
QStringList events = eventBus->getRegisteredEventsName();

// 打印事件列表
qDebug() << "Registered events:";
for(const QString& eventName : events) {
    qDebug() << " -" << eventName;
}
```

## 返回值类型

### ElaEventBusType::EventBusReturnType
```cpp
enum class EventBusReturnType {
    Success,           // 操作成功
    EventInvalid,      // 事件无效
    EventNameInvalid   // 事件名无效
};
```

## 注意事项
1. 事件名不能为空
2. 函数名必须是有效的槽函数
3. 事件对象会自动注销
4. 支持跨线程事件分发
5. 参数通过 QVariantMap 传递
6. 注意内存管理
7. 避免循环依赖
8. 建议使用有意义的事件名

## 最佳实践
1. 使用常量定义事件名
2. 合理组织事件层级
3. 及时清理不用的事件
4. 避免过度使用
5. 文档化事件接口
6. 处理错误返回值
7. 合理使用连接类型
8. 注意参数类型安全

## 相关组件
- QObject: 基类支持
- QVariant: 参数传递
- QMetaObject: 元对象系统
- Qt::ConnectionType: 连接类型
