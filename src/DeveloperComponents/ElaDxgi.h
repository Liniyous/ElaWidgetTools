#ifndef ELADXGI_H
#define ELADXGI_H

#include <QObject>
#ifdef Q_OS_WIN
#include <d3d11.h>
#include <dxgi1_6.h>

#include <QPixmap>

#include "stdafx.h"

class ElaDxgi : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QStringList, DxDeviceList)
    Q_PROPERTY_CREATE(QStringList, OutputDeviceList)
    Q_PROPERTY_CREATE(int, DxDeviceID);
    Q_PROPERTY_CREATE(int, OutputDeviceID);
    Q_PROPERTY_CREATE(QString, LastError)
    Q_PROPERTY_CREATE(bool, IsGrabActive)
    Q_PROPERTY_CREATE(QRect, GrabArea);
    Q_PROPERTY_CREATE(int, GrabFrameRate);  // 截图帧数
    Q_PROPERTY_CREATE(int, TimeoutMsValue); // 超时等待
    Q_PROPERTY_CREATE(bool, IsInitSuccess);
    Q_PROPERTY_CREATE(bool, IsGrabStoped);

public:
    explicit ElaDxgi(QObject* parent = nullptr);
    ~ElaDxgi();
    bool initialize(int dxID, int outputID);
    QImage getGrabImage() const;
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver();

private:
    IDXGIOutputDuplication* _duplication{nullptr};
    ID3D11Device* _device{nullptr};
    ID3D11DeviceContext* _context{nullptr};
    ID3D11Texture2D* _texture{nullptr};
    qreal _lastGrabTime{0};
    qreal _cpuSleepTime{0};
    QImage _grabImg;
    void releaseInterface();
    void cpuSleep(int usec);
};
#endif
#endif // ELADXGI_H
