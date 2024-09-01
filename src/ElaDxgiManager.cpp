#include "ElaDxgiManager.h"
#ifdef Q_OS_WIN
#include <QScreen>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QThread>
#include <QUrl>


#include "ElaDxgi.h"
#include "ElaDxgiManagerPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaDxgiManager);
ElaDxgiManager::ElaDxgiManager(QObject* parent)
    : QObject{parent}, d_ptr(new ElaDxgiManagerPrivate())
{
    Q_D(ElaDxgiManager);
    d->q_ptr = this;
    d->_dxgiThread = new QThread(this);
    d->_dxgi = new ElaDxgi();
    setGrabArea(0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
    bool ret = d->_dxgi->initialize(0, 0);
    if (!ret)
    {
        for (int i = 1; i < d->_dxgi->getDxDeviceList().count(); i++)
        {
            bool ret = d->_dxgi->initialize(i, 0);
            if (ret)
            {
                break;
            }
        }
    }
    if (!ret)
    {
        d->_dxgi->initialize(0, 0);
        qCritical() << "No available screenshot devices";
    }
    d->_dxgi->moveToThread(d->_dxgiThread);
    d->_dxgiThread->start();
    connect(d, &ElaDxgiManagerPrivate::grabScreen, d->_dxgi, &ElaDxgi::onGrabScreen);
    connect(d->_dxgi, &ElaDxgi::grabScreenOver, this, &ElaDxgiManager::grabImageUpdate);
}

ElaDxgiManager::~ElaDxgiManager()
{
    Q_D(ElaDxgiManager);
    if (d->_dxgi)
    {
        d->_dxgi->setIsGrabActive(false);
    }
    if (d->_dxgiThread->isRunning())
    {
        d->_dxgiThread->quit();
        d->_dxgiThread->wait();
    }
    delete d->_dxgi;
    d->_dxgi = nullptr; // 防止悬空指针
    d->_dxgiThread = nullptr;
}

QStringList ElaDxgiManager::getDxDeviceList() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getDxDeviceList();
}

QStringList ElaDxgiManager::getOutputDeviceList() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getOutputDeviceList();
}

QImage ElaDxgiManager::grabScreenToImage() const
{
    Q_D(const ElaDxgiManager);
    if (!d->_dxgi->getIsInitSuccess())
    {
        return QImage();
    }
    return d->_dxgi->getGrabImage();
}

void ElaDxgiManager::startGrabScreen()
{
    Q_D(ElaDxgiManager);
    d->_isAllowedGrabScreen = true;
    if (!d->_dxgi->getIsGrabActive())
    {
        d->_dxgi->setIsGrabActive(true);
        Q_EMIT d->grabScreen();
    }
}

void ElaDxgiManager::stopGrabScreen()
{
    Q_D(ElaDxgiManager);
    d->_isAllowedGrabScreen = false;
    d->_dxgi->setIsGrabActive(false);
}

bool ElaDxgiManager::setDxDeviceID(int dxID)
{
    Q_D(ElaDxgiManager);
    if (dxID < 0 || d->_dxgi->getDxDeviceList().count() <= dxID)
    {
        return false;
    }
    d->_dxgi->setIsGrabActive(false);
    while (!d->_dxgi->getIsGrabStoped())
    {
        //等待任务结束
        QApplication::processEvents();
    }
    if (d->_dxgi->initialize(dxID, d->_dxgi->getOutputDeviceID()))
    {
        if (d->_isAllowedGrabScreen)
        {
            d->_dxgi->setIsGrabActive(true);
            Q_EMIT d->grabScreen();
        }
        return true;
    }
    return false;
}

int ElaDxgiManager::getDxDeviceID() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getDxDeviceID();
}

bool ElaDxgiManager::setOutputDeviceID(int deviceID)
{
    Q_D(ElaDxgiManager);
    if (deviceID < 0 || d->_dxgi->getOutputDeviceList().count() <= deviceID)
    {
        return false;
    }
    
    d->_dxgi->setIsGrabActive(false);
    
    while (!d->_dxgi->getIsGrabStoped())
    {
        setGrabArea(0,0,deviceID);
        //等待任务结束
        QApplication::processEvents();
    }
    if (d->_dxgi->initialize(d->_dxgi->getDxDeviceID(), deviceID))
    {
        
        if (d->_isAllowedGrabScreen)
        {
            
            d->_dxgi->setIsGrabActive(true);
            Q_EMIT d->grabScreen();
        }
        return true;
    }
    return false;
}

int ElaDxgiManager::getOutputDeviceID() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getOutputDeviceID();
}

void ElaDxgiManager::setGrabArea(int width, int height)
{
    Q_D(ElaDxgiManager);
    
    //auto screenIndex=d_dx->getOutputDeviceID();
    d->_dxgi->setIsGrabActive(false);
    
    while (!d->_dxgi->getIsGrabStoped())
    {
        setGrabArea(0,0,d->_dxgi->getOutputDeviceID());
        //等待任务结束
        QApplication::processEvents();
    }
    // // 获取指定索引的屏幕
    // QScreen* screen = QGuiApplication::screens().at(d->_dxgi->getOutputDeviceID());

    // // 获取屏幕的实际几何尺寸（包括任务栏）
    // QRect screenGeometry = screen->geometry();

    // // 获取屏幕DPI缩放因子
    // qreal dpiFactor = screen->devicePixelRatio();

    // // 获取屏幕的真实分辨率（包括DPI缩放）
    // int screenWidth = screenGeometry.width() * dpiFactor;
    // int screenHeight = screenGeometry.height() * dpiFactor;

    // // 根据DPI因子调整宽高
    // if (width <= 0 || width > screenWidth)
    // {
    //     width = screenWidth;
    // }
    // if (height <= 0 || height > screenHeight)
    // {
    //     height = screenHeight;
    // }
    
    // d->_dxgi->setGrabArea(QRect(0, 0, width, height));
}
void ElaDxgiManager::setGrabArea(int width, int height,int screenIndex)
{
    Q_D(ElaDxgiManager);
    //auto screenIndex=d->_dxgi->getOutputDeviceID();
    
    // 获取指定索引的屏幕
    QScreen* screen = QGuiApplication::screens().at(screenIndex);

    // 获取屏幕的实际几何尺寸（包括任务栏）
    QRect screenGeometry = screen->geometry();

    // 获取屏幕DPI缩放因子
    qreal dpiFactor = screen->devicePixelRatio();

    // 获取屏幕的真实分辨率（包括DPI缩放）
    int screenWidth = screenGeometry.width() * dpiFactor;
    int screenHeight = screenGeometry.height() * dpiFactor;

    // 根据DPI因子调整宽高
    if (width <= 0 || width > screenWidth)
    {
        width = screenWidth;
    }
    if (height <= 0 || height > screenHeight)
    {
        height = screenHeight;
    }

    d->_dxgi->setGrabArea(QRect(0, 0, width, height));

}
void ElaDxgiManager::setGrabArea(int x, int y, int width, int height)
{
    Q_D(ElaDxgiManager);
    auto screenIndex=d->_dxgi->getOutputDeviceID();
    
  // 获取指定索引的屏幕
    QScreen* screen = QGuiApplication::screens().at(screenIndex);

    // 获取屏幕的实际几何尺寸（包括任务栏）
    QRect screenGeometry = screen->geometry();

    // 获取屏幕DPI缩放因子
    qreal dpiFactor = screen->devicePixelRatio();

    // 获取屏幕的真实分辨率（包括DPI缩放）
    int screenWidth = screenGeometry.width() * dpiFactor;
    int screenHeight = screenGeometry.height() * dpiFactor;

    // 根据DPI因子调整宽高
    if (width <= 0 || width > screenWidth)
    {
        width = screenWidth;
    }
    if (height <= 0 || height > screenHeight)
    {
        height = screenHeight;
    }

    // 确保起始位置在屏幕范围内
    if (x < screenGeometry.left())
    {
        x = screenGeometry.left();
    }
    if (y < screenGeometry.top())
    {
        y = screenGeometry.top();
    }

    // 打印调试信息
    //qDebug() << "Screen Geometry: " << screenGeometry;
    //qDebug() << "Grab Area: " << QRect(x, y, width, height);

    // 设置捕获区域
    d->_dxgi->setGrabArea(QRect(x, y, width, height));
}

QRect ElaDxgiManager::getGrabArea() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getGrabArea();
}

void ElaDxgiManager::setGrabFrameRate(int frameRateValue)
{
    Q_D(ElaDxgiManager);
    if (frameRateValue > 0)
    {
        d->_dxgi->setGrabFrameRate(frameRateValue);
    }
}

int ElaDxgiManager::getGrabFrameRate() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getGrabFrameRate();
}

void ElaDxgiManager::setTimeoutMsValue(int timeoutValue)
{
    Q_D(ElaDxgiManager);
    if (timeoutValue > 0)
    {
        d->_dxgi->setTimeoutMsValue(timeoutValue);
    }
}

int ElaDxgiManager::getTimeoutMsValue() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getTimeoutMsValue();
}

Q_PROPERTY_CREATE_Q_CPP(ElaDxgiScreen, int, BorderRadius)
ElaDxgiScreen::ElaDxgiScreen(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaDxgiScreenPrivate())
{
    Q_D(ElaDxgiScreen);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    d->_dxgiManager = ElaDxgiManager::getInstance();
    setFixedSize(700, 500);
    connect(d->_dxgiManager, &ElaDxgiManager::grabImageUpdate, this, [=] {
        if (isVisible())
        {
            update();
        }
    });
}

ElaDxgiScreen::~ElaDxgiScreen()
{
    
}

void ElaDxgiScreen::paintEvent(QPaintEvent* event)
{
    
    Q_D(ElaDxgiScreen);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    painter.drawImage(rect(), d->_dxgiManager->grabScreenToImage());
    painter.restore();

}
// void ElaDxgiScreen::paintEvent(QPaintEvent* event)
// {
//     Q_D(ElaDxgiScreen);

//     // 创建并启动计时器
//     QElapsedTimer timer;
//     timer.start();

//     // 执行绘制操作
//     QPainter painter(this);
//     painter.save();
//     painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, false);

//     // 使用 QPixmap 进行绘制
//     QPixmap pixmap = QPixmap::fromImage(d->_dxgiManager->grabScreenToImage());
//     painter.drawPixmap(rect(), pixmap);

//     painter.restore();

//     // 计算并输出绘制耗时
//     qint64 elapsed = timer.elapsed();
    
//     // 打印绘制耗时
//     //qDebug() << "绘制操作耗时：" << elapsed << "毫秒";
    
//     // 绘制耗时文本
//     painter.save();
//     painter.setPen(Qt::red); // 设置文本颜色
//     painter.setFont(QFont("Arial", 12)); // 设置字体和字号
//     painter.drawText(rect().adjusted(10, 10, 0, 0), Qt::AlignLeft, QString("绘制耗时：%1 ms").arg(elapsed));
//     painter.restore();
// }
void ElaDxgiScreen::setIsSyncGrabSize(bool isSyncGrabSize)
{
    Q_D(ElaDxgiScreen);
    if (isSyncGrabSize)
    {
        setFixedSize(d->_dxgiManager->getGrabArea().size());
    }
}

bool ElaDxgiScreen::getIsSyncGrabSize() const
{
    Q_D(const ElaDxgiScreen);
    return d->_isSyncGrabSize;
}
#endif
