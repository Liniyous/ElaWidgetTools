# ElaBreadcrumbBar 组件文档

## 概述
ElaBreadcrumbBar 是一个面包屑导航组件,提供了层级路径的可视化展示和交互功能。它支持动态添加/删除路径项、自动移除功能以及点击导航等特性。

## 特性
- 水平滚动布局
- 动态添加/删除路径
- 自动移除功能
- 点击导航
- 路径分隔符
- 主题适配
- 自定义文本大小
- 最后一项特殊样式

## 属性

### TextPixelSize
文本大小(像素)。

**类型:** `int`  
**默认值:** `18`

### IsAutoRemove
是否启用自动移除功能。

**类型:** `bool`  
**默认值:** `true`

## 公共方法

### 路径操作
```cpp
// 设置完整路径列表
void setBreadcrumbList(QStringList breadcrumbList);

// 添加单个路径
QStringList appendBreadcrumb(QString breadcrumb);

// 移除指定路径
QStringList removeBreadcrumb(QString breadcrumb);

// 获取路径数量
int getBreadcrumbListCount() const;

// 获取当前路径列表
QStringList getBreadcrumbList() const;
```

## 信号

### breadcrumbClicked
路径项被点击时触发。

**参数:**
- `QString breadcrumb`: 被点击的路径项文本
- `QStringList lastBreadcrumbList`: 点击前的完整路径列表

## 使用示例

### 基础用法

#### 1. 创建面包屑导航
```cpp
ElaBreadcrumbBar* breadcrumbBar = new ElaBreadcrumbBar(this);
breadcrumbBar->setFixedHeight(37);

// 设置初始路径
QStringList paths = {"Home", "Documents", "Projects"};
breadcrumbBar->setBreadcrumbList(paths);
```

#### 2. 动态添加路径
```cpp
// 添加单个路径
breadcrumbBar->appendBreadcrumb("New Project");

// 批量设置路径
QStringList newPaths = {"Home", "Pictures", "2024"};
breadcrumbBar->setBreadcrumbList(newPaths);
```

### 高级用法

#### 1. 自定义样式
```cpp
ElaBreadcrumbBar* breadcrumbBar = new ElaBreadcrumbBar(this);
breadcrumbBar->setTextPixelSize(16);  // 设置文本大小
breadcrumbBar->setIsAutoRemove(false); // 禁用自动移除
```

#### 2. 路径导航处理
```cpp
connect(breadcrumbBar, &ElaBreadcrumbBar::breadcrumbClicked, 
    this, [=](QString breadcrumb, QStringList lastList) {
        // 处理路径导航
        navigateToPath(breadcrumb);
        
        // 可以获取点击前的完整路径
        qDebug() << "Previous path:" << lastList;
});
```

#### 3. 路径同步
```cpp
// 监听路径变化
void MainWindow::onPathChanged(const QString& path) {
    QStringList pathParts = path.split('/');
    breadcrumbBar->setBreadcrumbList(pathParts);
}

// 同步文件系统导航
connect(fileSystem, &FileSystem::currentPathChanged,
        this, &MainWindow::onPathChanged);
```

## 注意事项
1. 组件固定高度为 37 像素
2. 自动移除功能会在点击非最后一项时移除后续路径
3. 重复添加相同路径会被忽略
4. 点击最后一项不会触发导航
5. 路径分隔符使用 ">" 符号
6. 主题切换会自动更新样式

## 相关组件
- ElaNavigationBar: 导航栏组件
- ElaPivot: 数据透视组件
- ElaTabWidget: 标签页组件
- ElaScrollPageArea: 滚动区域组件
