#include "ElaDxgiManager.h"
#ifdef Q_OS_WIN
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
            ret = d->_dxgi->initialize(i, 0);
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

bool ElaDxgiManager::getIsGrabScreen() const
{
    Q_D(const ElaDxgiManager);
    return d->_dxgi->getIsGrabActive();
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
    int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_dxgi->setIsGrabCenter(true);
    d->_dxgi->setGrabArea(QRect(0, 0, width, height));
}

void ElaDxgiManager::setGrabArea(int x, int y, int width, int height)
{
    Q_D(ElaDxgiManager);
    int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_dxgi->setIsGrabCenter(false);
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
    connect(d->_dxgiManager, &ElaDxgiManager::grabImageUpdate, this, [=](QImage img) {
        if (isVisible())
        {
            d->_img = std::move(img);
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
    if (d->_dxgiManager->getIsGrabScreen())
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter.drawImage(rect(), d->_img);
        painter.restore();
    }
}

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
