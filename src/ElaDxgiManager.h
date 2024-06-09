#ifndef ELADXGIMANAGER_H
#define ELADXGIMANAGER_H

#include <QtQml/qqml.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <windows.h>

#include <QLabel>
#include <QPixmap>
#include <QThread>
#include <QTimer>

#include "singleton.h"
#include "stdafx.h"
class ElaDxgiPrivate;
class ElaDxgi : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaDxgi)
    Q_PROPERTY_CREATE_Q_H(QStringList, DxDeviceList)
    Q_PROPERTY_CREATE_Q_H(QStringList, OutputDeviceList)
    Q_PROPERTY_CREATE_Q_H(int, DxDeviceID);
    Q_PROPERTY_CREATE_Q_H(int, OutputDeviceID);
    Q_PROPERTY_CREATE_Q_H(QString, LastError)
    Q_PROPERTY_CREATE_Q_H(bool, IsGrabActive)
    Q_PROPERTY_CREATE_Q_H(QRect, GrabArea);
    Q_PROPERTY_CREATE_Q_H(int, GrabFrameRate);  // 截图帧数
    Q_PROPERTY_CREATE_Q_H(int, TimeoutMsValue); // 超时等待

public:
    explicit ElaDxgi(QObject* parent = nullptr);
    ~ElaDxgi();
    bool initialize(int dxID, int outputID);
    QImage getGrabImage() const;
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver();

private:
    void releaseInterface();
    void cpuSleep(int usec);
};

class ELA_EXPORT ElaDxgiManager : public QObject
{
    Q_OBJECT
private:
    explicit ElaDxgiManager(QObject* parent = nullptr);
    ~ElaDxgiManager();

public:
    Q_SINGLETON_CREATE(ElaDxgiManager);
    static ElaDxgiManager* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine) { return getInstance(); }
    QStringList getDxDeviceList() const;
    QStringList getOutputDeviceList() const;
    QImage grabScreenToImage() const;
    void startGrabScreen();
    void stopGrabScreen();
    bool setDxDeviceID(int dxID);
    int getDxDeviceID() const;
    bool setOutputDeviceID(int deviceID);
    int getOutputDeviceID() const;
    void setGrabArea(int width, int height);
    void setGrabArea(int x, int y, int width, int height);
    QRect getGrabArea() const;
    void setGrabFrameRate(int frameRateValue);
    int getGrabFrameRate() const;
    void setTimeoutMsValue(int timeoutValue);
    int getTimeoutMsValue() const;

    Q_SIGNAL void grabScreen();
    Q_SIGNAL void sendUpdate();

private:
    ElaDxgi* _dxgi{nullptr};
    QThread* _dxgiThread{nullptr};
};

class ELA_EXPORT ElaDxgiScreen : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, BorderRadius)
public:
    explicit ElaDxgiScreen(QWidget* parent = nullptr);
    ~ElaDxgiScreen();
    void setIsSyncGrabSize(bool isSyncGrabSize);
    bool getIsSyncGrabSize() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ElaDxgiManager* _dxgiManager{nullptr};
    bool _isSyncGrabSize{false};
};

#endif // ELADXGIMANAGER_H
