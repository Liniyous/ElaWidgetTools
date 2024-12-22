#include "mainwindow.h"
#include "ElaApplication.h"
#include <QApplication>
#include <QScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
    qputenv("QT_SCALE_FACTOR", "1.5"); //根据实际屏幕缩放比例更改
#endif
#endif

    QApplication a(argc, argv);
    eApp->init();
    a.setWindowIcon(QIcon(":/Resource/Image/logo.png"));
    //eTheme->setThemeMode(static_cast<ElaThemeType::ThemeMode>(DataBase::getInstance()->getConf("theme").toInt()));
    MainWindow::getInstance();
    return a.exec();
}
