# ElaFlowLayout 组件文档

## 概述
ElaFlowLayout 是一个流式布局管理器,继承自 QLayout。它能够自动将子组件按照从左到右、从上到下的顺序排列,并支持动画效果。

## 特性
- 流式布局
- 自动换行
- 动画效果
- 智能间距
- 自适应大小
- 高度自适应
- 边距控制
- 方向扩展

## 构造函数
```cpp
// 带父组件的构造
explicit ElaFlowLayout(
    QWidget* parent,
    int margin = -1,
    int hSpacing = -1,
    int vSpacing = -1
);

// 无父组件的构造
explicit ElaFlowLayout(
    int margin = -1,
    int hSpacing = -1,
    int vSpacing = -1
);
```

## 公共方法

### 布局控制
```cpp
// 添加布局项
void addItem(QLayoutItem* item) override;

// 获取水平间距
int horizontalSpacing() const;

// 获取垂直间距
int verticalSpacing() const;

// 设置动画效果
void setIsAnimation(bool isAnimation);
```

### 布局信息
```cpp
// 获取项目数量
int count() const override;

// 获取指定索引的布局项
QLayoutItem* itemAt(int index) const override;

// 移除指定索引的布局项
QLayoutItem* takeAt(int index) override;

// 获取最小尺寸
QSize minimumSize() const override;

// 获取建议尺寸
QSize sizeHint() const override;
```

## 使用示例

### 基础用法

#### 1. 创建流式布局
```cpp
// 创建布局
ElaFlowLayout* flowLayout = new ElaFlowLayout(
    this,       // 父组件
    10,         // 外边距
    5,          // 水平间距
    5           // 垂直间距
);

// 设置布局边距
flowLayout->setContentsMargins(30, 0, 0, 0);

// 启用动画
flowLayout->setIsAnimation(true);

// 添加组件
flowLayout->addWidget(card1);
flowLayout->addWidget(card2);
flowLayout->addWidget(card3);
```

#### 2. 自适应布局
```cpp
// 创建容器组件
QWidget* container = new QWidget(this);
container->setStyleSheet(
    "background-color: transparent;"
);

// 创建流式布局
ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
container->setLayout(flowLayout);

// 添加多个卡片
for(int i = 0; i < 10; i++) {
    ElaPopularCard* card = new ElaPopularCard(this);
    card->setFixedSize(200, 280);
    flowLayout->addWidget(card);
}
```

### 高级用法

#### 1. 动态添加和移除
```cpp
class DynamicFlow : public QWidget {
private:
    ElaFlowLayout* flowLayout;
    
public:
    void addCard(ElaCard* card) {
        flowLayout->setIsAnimation(true);
    }
};
