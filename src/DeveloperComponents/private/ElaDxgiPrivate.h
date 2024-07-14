#ifndef ELADXGIPRIVATE_H
#define ELADXGIPRIVATE_H

#include <d3d11.h>
#include <dxgi1_6.h>

#include <QObject>
#include <QPixmap>

#include "stdafx.h"
class ElaDxgi;
class ElaDxgiPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDxgi)
    Q_PROPERTY_CREATE_D(QStringList, DxDeviceList)
    Q_PROPERTY_CREATE_D(QStringList, OutputDeviceList)
    Q_PROPERTY_CREATE_D(int, DxDeviceID);
    Q_PROPERTY_CREATE_D(int, OutputDeviceID);
    Q_PROPERTY_CREATE_D(QString, LastError)
    Q_PROPERTY_CREATE_D(bool, IsGrabActive)
    Q_PROPERTY_CREATE_D(QRect, GrabArea);
    Q_PROPERTY_CREATE_D(int, GrabFrameRate);  //截图帧数
    Q_PROPERTY_CREATE_D(int, TimeoutMsValue); //超时等待
    Q_PROPERTY_CREATE_D(bool, IsInitSuccess);
    Q_PROPERTY_CREATE_D(bool, IsGrabStoped);

public:
    explicit ElaDxgiPrivate(QObject* parent = nullptr);
    ~ElaDxgiPrivate();

private:
    IDXGIOutputDuplication* _duplication{nullptr};
    ID3D11Device* _device{nullptr};
    ID3D11DeviceContext* _context{nullptr};
    ID3D11Texture2D* _texture{nullptr};
    qreal _lastGrabTime{0};
    qreal _cpuSleepTime{0};
    QImage _grabImg;
};

#endif // ELADXGIPRIVATE_H
