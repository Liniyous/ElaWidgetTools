# ElaCalendarPicker 组件文档

## 概述
ElaCalendarPicker 是一个日期选择器组件,继承自 QPushButton。它提供了一个简洁的日期输入控件,点击后弹出日历面板进行日期选择。

## 特性
- 弹出式日历面板
- 动画效果
- 悬停效果
- 日期格式化显示
- 主题适配
- 阴影效果
- 自定义边框圆角
- 日历图标

## 属性

### BorderRadius
选择器边框圆角半径。

**类型:** `int`  
**默认值:** `3`

### SelectedDate
当前选中的日期。

**类型:** `QDate`  
**默认值:** `QDate::currentDate()`

## 公共方法

### 日期操作
```cpp
// 设置选中日期
void setSelectedDate(QDate date);

// 获取选中日期
QDate getSelectedDate() const;
```

## 信号

### selectedDateChanged
日期选择变化时触发。

**参数:**
- `QDate date`: 选中的日期

## 尺寸规格

- 组件宽度: 120px
- 组件高度: 30px
- 日历面板宽度: 317px
- 日历面板高度: 352px
- 内边距: 6px
- 文本左边距: 10px
- 图标宽度: 15px

## 使用示例

### 基础用法

#### 1. 创建日期选择器
```cpp
// 基础构造
ElaCalendarPicker* picker = new ElaCalendarPicker(this);

// 设置初始日期
picker->setSelectedDate(QDate::currentDate());

// 监听日期选择
connect(picker, &ElaCalendarPicker::selectedDateChanged,
    this, [](const QDate& date) {
        qDebug() << "Selected date:" << date.toString();
});
```

### 高级用法

#### 1. 日期格式化
```cpp
// 自定义日期显示格式
class CustomCalendarPicker : public ElaCalendarPicker {
protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        // ... 基础绘制代码 ...
        
        // 自定义日期格式
        QDate date = getSelectedDate();
        QString formattedDate = date.toString("yyyy年MM月dd日");
        painter.drawText(textRect, Qt::AlignVCenter, 
                        formattedDate);
    }
};
```

#### 2. 日期范围限制
```cpp
// 创建带范围限制的选择器
ElaCalendarPicker* picker = new ElaCalendarPicker(this);
ElaCalendar* calendar = picker->findChild<ElaCalendar*>();

// 设置最小日期
calendar->setMinimumDate(QDate::currentDate());

// 设置最大日期
calendar->setMaximumDate(
    QDate::currentDate().addYears(1));
```

#### 3. 动画自定义
```cpp
// 自定义弹出动画
void customShowAnimation(ElaCalendarPickerContainer* container,
                        const QPoint& endPoint) {
    QPropertyAnimation* animation = 
        new QPropertyAnimation(container, "pos");
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->setDuration(500);
    animation->setStartValue(QPoint(endPoint.x(), 
                                  endPoint.y() - 50));
    animation->setEndValue(endPoint);
    container->show();
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
```

## 注意事项
1. 默认使用当前日期
2. 日期格式为 "yyyy/MM/dd"
3. 弹出动画持续 250ms
4. 使用 OutCubic 缓动曲线
5. 点击选择后自动关闭面板
6. 主题切换会自动更新样式
7. 面板位置自动居中对齐
8. 支持键盘导航

## 相关组件
- ElaCalendar: 日历组件
- ElaCalendarPickerContainer: 选择器容器
- ElaTheme: 主题管理器
