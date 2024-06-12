#include <QApplication>

#include "mainwindow.h"
int main(int argc, char* argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // HWND handle = FindWindowA(NULL, "ElaWidgetTools");
    // if (handle != NULL)
    // {
    //     SetWindowDisplayAffinity(handle, WDA_EXCLUDEFROMCAPTURE);
    // }
    return a.exec();
}
