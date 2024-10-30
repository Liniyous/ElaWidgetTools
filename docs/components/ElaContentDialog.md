# ElaContentDialog 组件文档

## 概述
ElaContentDialog 是一个现代化的内容对话框组件,继承自 QDialog。它提供了一个带有遮罩层、动画效果和三个按钮的模态对话框。

## 特性
- 遮罩层效果
- 显示/隐藏动画
- 三按钮布局
- 自定义内容区域
- 主题适配
- 无边框设计
- Windows 原生事件处理
- 模态对话框

## 公共方法

### 按钮文本设置
```cpp
// 设置左侧按钮文本
void setLeftButtonText(QString text);

// 设置中间按钮文本
void setMiddleButtonText(QString text);

// 设置右侧按钮文本
void setRightButtonText(QString text);
```

### 内容设置
```cpp
// 设置中心内容组件
void setCentralWidget(QWidget* centralWidget);
```

## 信号

### buttonClicked 系列
```cpp
// 左侧按钮点击信号
void leftButtonClicked();

// 中间按钮点击信号
void middleButtonClicked();

// 右侧按钮点击信号
void rightButtonClicked();
```

## 尺寸规格

- 默认宽度: 400px
- 按钮高度: 38px
- 按钮圆角: 6px
- 内容边距: 15px(左右), 25px(上), 10px(下)
- 按钮区域高度: 60px
- 遮罩层边距: 8px

## 使用示例

### 基础用法

#### 1. 创建确认对话框
```cpp
// 创建对话框
ElaContentDialog* dialog = new ElaContentDialog(this);

// 设置按钮文本
dialog->setLeftButtonText(tr("取消"));
dialog->setMiddleButtonText(tr("最小化"));
dialog->setRightButtonText(tr("确定"));

// 显示对话框
dialog->exec();
```

#### 2. 自定义内容
```cpp
// 创建内容组件
QWidget* content = new QWidget;
QVBoxLayout* layout = new QVBoxLayout(content);

// 添加标题
ElaText* title = new ElaText("提示", content);
title->setTextStyle(ElaTextType::Title);

// 添加描述
ElaText* description = new ElaText("确定要执行此操作吗?", content);
description->setTextStyle(ElaTextType::Body);

layout->addWidget(title);
layout->addSpacing(2);
layout->addWidget(description);
layout->addStretch();

// 设置到对话框
dialog->setCentralWidget(content);
```

### 高级用法

#### 1. 退出确认对话框
```cpp
class MainWindow : public QMainWindow {
private:
    ElaContentDialog* _closeDialog;
    
    void setupCloseDialog() {
        _closeDialog = new ElaContentDialog(this);
        
        // 连接按钮信号
        connect(_closeDialog, 
                &ElaContentDialog::rightButtonClicked,
                this, &MainWindow::close);
        connect(_closeDialog, 
                &ElaContentDialog::middleButtonClicked,
                this, &MainWindow::showMinimized);
                
        // 拦截关闭事件
        connect(this, &MainWindow::closeButtonClicked,
                _closeDialog, &ElaContentDialog::exec);
    }
};
```

#### 2. 自定义动画效果
```cpp
class CustomContentDialog : public ElaContentDialog {
protected:
    void showEvent(QShowEvent* event) override {
        // 自定义显示动画
        QPropertyAnimation* animation = 
            new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(300);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        
        ElaContentDialog::showEvent(event);
    }
};
```

## 注意事项
1. 默认为模态对话框
2. 按钮区域使用圆角设计
3. 支持 Windows 原生窗口事件
4. 遮罩层自动适应父窗口大小
5. 右侧按钮使用主题强调色
6. 主题切换会自动更新样式
7. 按钮点击自动关闭对话框
8. 支持自定义内容组件

## 相关组件
- ElaMaskWidget: 遮罩层组件
- ElaPushButton: 按钮组件
- ElaText: 文本组件
- ElaTheme: 主题管理器
