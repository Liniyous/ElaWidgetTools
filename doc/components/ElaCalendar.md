# ElaCalendar 组件文档

## 概述
ElaCalendar 是一个现代化的日历组件,提供了年/月/日三种显示模式和丰富的日期选择功能。它支持动画效果、主题适配、日期标记等特性。

## 特性
- 年/月/日三级显示模式
- 动画切换效果
- 今日日期高亮
- 选中日期标记
- 日期描述文本
- 主题适配
- 自定义缩放比例
- 星期标题栏
- 快速导航
- 弹出式选择器(ElaCalendarPicker)

## 属性

### ZoomRatio
日历缩放比例。

**类型:** `qreal`  
**默认值:** `1.0`

### PixOpacity
日历透明度。

**类型:** `qreal`  
**默认值:** `1.0`

### BorderRadius
边框圆角半径。

**类型:** `int`  
**默认值:** `3`

## 公共方法

### 日期操作
```cpp
// 设置选中日期
void setSelectedDate(const QDate& date);

// 获取选中日期
QDate getSelectedDate() const;

// 设置显示模式
void setDisplayMode(ElaCalendarMode mode);

// 获取显示模式
ElaCalendarMode getDisplayMode() const;

// 设置日期描述
void setDateDescription(const QDate& date, 
                       const QString& description);
```

## 信号

### selectedDateChanged
日期选择变化时触发。

**参数:**
- `const QDate& date`: 选中的日期

### displayModeChanged
显示模式变化时触发。

## 使用示例

### 基础用法

#### 1. 创建日历
```cpp
// 基础构造
ElaCalendar* calendar = new ElaCalendar(this);

// 设置初始日期
calendar->setSelectedDate(QDate::currentDate());

// 监听日期选择
connect(calendar, &ElaCalendar::selectedDateChanged,
    this, [](const QDate& date) {
        qDebug() << "Selected date:" << date.toString();
});
```

#### 2. 日期选择器
```cpp
// 创建日期选择器
ElaCalendarPicker* picker = new ElaCalendarPicker(this);

// 监听日期选择
connect(picker, &ElaCalendarPicker::selectedDateChanged,
    this, [](const QDate& date) {
        updateDateLabel(date);
});
```

### 高级用法

#### 1. 日期标记
```cpp
// 添加日期描述
calendar->setDateDescription(QDate(2024, 3, 15), "会议");
calendar->setDateDescription(QDate(2024, 3, 20), "假期");

// 批量添加标记
void markHolidays(ElaCalendar* calendar, 
                  const QList<QDate>& holidays) {
    for(const auto& date : holidays) {
        calendar->setDateDescription(date, "节假日");
    }
}
```

#### 2. 动画控制
```cpp
// 设置缩放比例
calendar->setZoomRatio(1.2);  // 放大 20%

// 设置透明度
calendar->setPixOpacity(0.8); // 80% 不透明度

// 动画切换
QPropertyAnimation* animation = 
    new QPropertyAnimation(calendar, "zoomRatio");
animation->setDuration(300);
animation->setStartValue(1.0);
animation->setEndValue(1.2);
animation->start(QAbstractAnimation::DeleteWhenStopped);
```

#### 3. 自定义显示
```cpp
// 设置显示模式
calendar->setDisplayMode(ElaCalendarMode::YearMode);

// 模式切换监听
connect(calendar, &ElaCalendar::displayModeChanged,
    this, [=]() {
        updateModeLabel(calendar->getDisplayMode());
});
```

## 注意事项
1. 默认显示当前月份
2. 年模式显示 15 年
3. 月模式显示 12 个月
4. 日模式显示当月所有日期
5. 今日日期自动高亮显示
6. 支持键盘导航
7. 主题切换会自动更新样式
8. 日期描述文本长度应适中

## 相关组件
- ElaCalendarPicker: 日期选择器
- ElaCalendarDelegate: 日历项代理
- ElaCalendarModel: 日历数据模型
- ElaTheme: 主题管理器
