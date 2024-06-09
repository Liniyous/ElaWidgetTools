#include <QApplication>

#include "mainwindow.h"
int main(int argc, char* argv[])
{
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
