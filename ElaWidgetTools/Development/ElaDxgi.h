#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADXGI_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADXGI_H_

#include <QObject>
#ifdef Q_OS_WIN
#include <QElapsedTimer>
#include <QImage>
#include <atomic>
#include <d3d11.h>
#include <dxgi1_6.h>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"

#include <QMutex>

class ElaDxgi : public QObject
{
    Q_OBJECT
    ELA_VARIABLE(QStringList, DxDeviceList)
    ELA_VARIABLE(QStringList, OutputDeviceList)
    ELA_VARIABLE(int, DxDeviceID)
    ELA_VARIABLE(int, OutputDeviceID)
    ELA_VARIABLE(QString, LastError)
    ELA_VARIABLE(QRect, GrabArea)
    ELA_VARIABLE(int, GrabFrameRate)  // 截图帧数
    ELA_VARIABLE(int, TimeoutMsValue) // 超时等待
    ELA_VARIABLE(bool, IsInitSuccess)
    ELA_VARIABLE(bool, IsGrabCenter)

public:
    explicit ElaDxgi(QObject* parent = nullptr);
    ~ElaDxgi() override;
    bool initialize(int dxID, int outputID);

    void setIsGrabActive(bool isGrabActive);
    bool getIsGrabActive() const;
    void setIsGrabStopped(bool const& isGrabStopped);
    bool getIsGrabStopped() const;

    QImage getGrabImage();
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver(const QImage& img);

private:
    QMutex _grabMutex;
    std::atomic_bool _isGrabActive{false};
    std::atomic_bool _pIsGrabStopped{true};
    IDXGIOutputDuplication* _duplication{nullptr};
    ID3D11Device* _device{nullptr};
    ID3D11DeviceContext* _context{nullptr};
    ID3D11Texture2D* _texture{nullptr}; // 同尺寸、同格式的 CPU 读取纹理在帧间复用。
    QElapsedTimer _grabTimer;
    qreal _lastGrabTime{0};
    qreal _cpuSleepTime{0};
    QImage _grabImage; // 持有独立像素的裁剪快照；发布后只读，访问由 _grabMutex 保护。
    QImage _captureFrame(IDXGIResource* desktopResource);
    void releaseInterface();
    static void waitElapsedTime(qreal usec);
};
#endif
#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADXGI_H_
