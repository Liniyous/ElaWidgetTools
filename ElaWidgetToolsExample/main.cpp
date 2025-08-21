#include <QApplication>
#include <QScreen>

#include "ElaApplication.h"
#include "ElaLog.h"
#include "mainwindow.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
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
    eApp->init();
    //ElaLog::getInstance()->initMessageLog(true);
    MainWindow w;
    w.show();
#ifdef Q_OS_WIN
//    HWND handle = FindWindowA(NULL, "ElaWidgetTool");
//    if (handle != NULL)
//    {
//        SetWindowDisplayAffinity(handle, WDA_EXCLUDEFROMCAPTURE);
//    }
#endif
    return a.exec();
}
