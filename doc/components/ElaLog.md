# ElaLog 组件文档

## 概述
ElaLog 是一个单例日志管理组件,提供了文件日志记录、消息格式化、线程安全等功能。它可以捕获 Qt 的消息系统并将其保存到文件。

## 特性
- 单例模式
- 文件日志
- 消息格式化
- 线程安全
- 时间戳
- 自动清理
- 路径配置
- 实时输出

## 属性

### 基础属性
```cpp
// 日志保存路径
Q_PROPERTY(QString logSavePath READ getLogSavePath 
    WRITE setLogSavePath)

// 日志文件名
Q_PROPERTY(QString logFileName READ getLogFileName 
    WRITE setLogFileName)

// 是否使用时间戳文件名
Q_PROPERTY(bool isLogFileNameWithTime 
    READ getIsLogFileNameWithTime 
    WRITE setIsLogFileNameWithTime)
```

## 信号
```cpp
// 日志消息信号
void logMessage(QString log);
```

## 默认值
- 日志文件名: "ElaLog"
- 保存路径: 当前目录
- 时间戳文件名: false
- 消息互斥锁: 启用
- 日期时间格式: "yyyy-MM-dd hh:mm:ss"

## 使用示例

### 基础用法

#### 1. 初始化日志系统
```cpp
// 获取日志实例
ElaLog* logger = ElaLog::getInstance();

// 配置日志
logger->setLogFileName("MyApp");
logger->setLogSavePath("D:/logs");
logger->setIsLogFileNameWithTime(true);

// 启用日志记录
logger->initMessageLog(true);
```

#### 2. 记录日志
```cpp
// 使用 Qt 的消息系统
qDebug() << "Debug message";
qWarning() << "Warning message";
qCritical() << "Critical message";

// 监听日志消息
connect(logger, &ElaLog::logMessage, 
    this, [](const QString& log) {
        // 处理日志消息
        processLog(log);
});
```

### 高级用法

#### 1. 自定义日志格式
```cpp
// 日志格式示例
QString logFormat = QString(
    "[%1-%2](函数: %3, 行数: %4) -> %5")
    .arg(logLevel)
    .arg(timestamp)
    .arg(function)
    .arg(line)
    .arg(message);
```

#### 2. 线程安全写入
```cpp
void writeLogSafely(const QString& logInfo) {
    QMutexLocker locker(messageLogMutex);
    QFile logfile(getLogFilePath());
    if (logfile.open(QIODevice::Append)) {
        QTextStream stream(&logfile);
        stream << logInfo << Qt::endl;
        logfile.close();
    }
}
```

## 日志格式

### 消息格式
```cpp
// 调试信息
[信息-2024-01-01 12:00:00](函数: main, 行数: 42) 
    -> Debug message

// 警告信息
[警告-2024-01-01 12:00:00](函数: main, 行数: 43) 
    -> Warning message

// 错误信息
[错误-2024-01-01 12:00:00](函数: main, 行数: 44) 
    -> Critical message
```

### 文件名格式
```cpp
// 普通文件名
MyApp.txt

// 带时间戳文件名
MyApp_2024-01-01_12-00-00.txt
```

## 实现细节

### 消息处理器
```cpp
void messageLogHandler(
    QtMsgType type, 
    const QMessageLogContext& ctx, 
    const QString& msg) 
{
    // 获取时间戳
    QString timestamp = QDateTime::currentDateTime()
        .toString("yyyy-MM-dd hh:mm:ss");
    
    // 格式化消息
    QString logInfo = formatLogMessage(
        type, timestamp, ctx, msg);
    
    // 发送信号
    emit logMessage(logInfo);
    
    // 写入文件
    writeLogToFile(logInfo);
}
```

## 注意事项
1. 单例模式实现
2. 线程安全考虑
3. 文件操作异常
4. 路径分隔符
5. 文件命名冲突
6. 磁盘空间管理
7. 性能影响
8. Qt 版本兼容

## 最佳实践
1. 合理配置路径
2. 控制日志级别
3. 定期清理日志
4. 处理写入失败
5. 监控文件大小
6. 使用异步写入
7. 避免频繁写入
8. 做好错误处理

## 相关组件
- QFile: 文件操作
- QMutex: 互斥锁
- QDateTime: 时间处理
- QTextStream: 文本流
