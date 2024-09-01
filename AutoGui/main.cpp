#include <QApplication>
#include <QScreen>
#include "ElaApplication.h"
#include "mainwindow.h"
#include "T_AppConfig.h"
#include "T_mainwindow.h"
int main(int argc, char* argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
    //根据实际屏幕缩放比例更改
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif
    QApplication a(argc, argv);
    // 设置编辑模式
    
    eApp->init();
   // 根据编辑模式创建窗口对象
    QWidget* mainWindow = nullptr;  // 使用 QWidget* 来指向不同的窗口类型

    if (T_AppConfig::getEditMode()) {
        mainWindow = new T_mainwindow();  // 创建编辑模式窗口
    } else {
        mainWindow = new MainWindow();  // 创建正常模式窗口
    }

    // 显示窗口
    mainWindow->show();

    // 启动应用程序事件循环
    int result = a.exec();

    // 清理
    delete mainWindow;
}
