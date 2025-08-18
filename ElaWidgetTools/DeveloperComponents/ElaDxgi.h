#ifndef ELADXGI_H
#define ELADXGI_H

#include <QObject>
#ifdef Q_OS_WIN
#include <QElapsedTimer>
#include <QPixmap>
#include <d3d11.h>
#include <dxgi1_6.h>

#include "ElaProperty.h"

class ElaDxgi : public QObject
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QStringList, DxDeviceList)
    Q_PRIVATE_CREATE(QStringList, OutputDeviceList)
    Q_PRIVATE_CREATE(int, DxDeviceID);
    Q_PRIVATE_CREATE(int, OutputDeviceID);
    Q_PRIVATE_CREATE(QString, LastError)
    Q_PRIVATE_CREATE(bool, IsGrabActive)
    Q_PRIVATE_CREATE(QRect, GrabArea);
    Q_PRIVATE_CREATE(int, GrabFrameRate);  // 截图帧数
    Q_PRIVATE_CREATE(int, TimeoutMsValue); // 超时等待
    Q_PRIVATE_CREATE(bool, IsInitSuccess);
    Q_PRIVATE_CREATE(bool, IsGrabStoped);
    Q_PRIVATE_CREATE(bool, IsGrabCenter);

public:
    explicit ElaDxgi(QObject* parent = nullptr);
    ~ElaDxgi() override;
    bool initialize(int dxID, int outputID);
    QImage getGrabImage() const;
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver(QImage img);

private:
    IDXGIOutputDuplication* _duplication{nullptr};
    ID3D11Device* _device{nullptr};
    ID3D11DeviceContext* _context{nullptr};
    ID3D11Texture2D* _texture{nullptr};
    QElapsedTimer _grabTimer;
    qint64 _lastGrabTime{0};
    qint64 _cpuSleepTime{0};
    uchar* _imageBits{nullptr};
    int _descWidth{0};
    int _descHeight{0};
    void releaseInterface();
    void cpuSleep(qint64 usec);
};
#endif
#endif // ELADXGI_H
