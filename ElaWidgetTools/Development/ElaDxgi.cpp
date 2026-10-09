#include "ElaDxgi.h"

#ifdef Q_OS_WIN
#include <QDebug>
#include <QElapsedTimer>
#include <QScopeGuard>

#include <limits>

ElaDxgi::ElaDxgi(QObject* parent)
    : QObject(parent)
{
    _pIsInitSuccess = false;
    _pGrabFrameRate = 120;
    _pTimeoutMsValue = 50;
    _pIsGrabCenter = false;
    _pDxDeviceID = 0;
    _pOutputDeviceID = 0;
}

ElaDxgi::~ElaDxgi()
{
    releaseInterface();
}

bool ElaDxgi::initialize(int dxID, int outputID)
{
    _pIsInitSuccess = false;
    _pDxDeviceID = dxID;
    _pOutputDeviceID = outputID;
    releaseInterface();
    ID3D11Device* d3dDevice = nullptr;
    ID3D11DeviceContext* d3dContext = nullptr;
    D3D_FEATURE_LEVEL feat = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
                                   D3D11_SDK_VERSION, &d3dDevice, &feat, &d3dContext);
    if (FAILED(hr))
    {
        _pLastError = "Failed to D3D11CreateDevice ErrorCode = " + QString::number(uint(hr), 16);
        qDebug() << _pLastError;
        if (d3dDevice)
        {
            d3dDevice->Release();
        }
        if (d3dContext)
        {
            d3dContext->Release();
        }
        return false;
    }

    IDXGIFactory* dxgiFactory = nullptr;
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
    if (FAILED(hr))
    {
        _pLastError = "创建 DXGI 工厂失败，错误码：" + QString::number(static_cast<uint>(hr), 16);
        qWarning() << _pLastError;
        d3dContext->Release();
        d3dDevice->Release();
        return false;
    }
    IDXGIAdapter* dxgiAdapter = nullptr;
    QVector<IDXGIAdapter*> dxgiAdapterVector;
    for (uint i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        dxgiAdapterVector.append(dxgiAdapter);
    }
    dxgiFactory->Release();
    dxgiAdapter = nullptr;
    _pDxDeviceList.clear();
    for (int i = 0; i < dxgiAdapterVector.count(); i++)
    {
        IDXGIAdapter* adapt = dxgiAdapterVector.at(i);
        DXGI_ADAPTER_DESC desc;
        adapt->GetDesc(&desc);
        _pDxDeviceList.append(QString::fromWCharArray(desc.Description));
    }
    if (dxID >= 0 && dxID < dxgiAdapterVector.count())
    {
        dxgiAdapter = dxgiAdapterVector.at(dxID);
    }

    if (!dxgiAdapter)
    {
        _pLastError = "Failed to found gpu";
        qDebug() << _pLastError;
        d3dDevice->Release();
        d3dContext->Release();
        for (auto adapter: dxgiAdapterVector)
        {
            adapter->Release();
        }
        return false;
    }

    IDXGIOutput* dxgiOutput = nullptr;
    QVector<IDXGIOutput*> dxgiOutputVector;
    for (uint i = 0; dxgiAdapter->EnumOutputs(i, &dxgiOutput) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        dxgiOutputVector.append(dxgiOutput);
    }
    for (auto adapter: dxgiAdapterVector)
    {
        adapter->Release();
    }
    dxgiOutput = nullptr;
    _pOutputDeviceList.clear();
    for (int i = 0; i < dxgiOutputVector.count(); i++)
    {
        const auto& output = dxgiOutputVector.at(i);
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);
        _pOutputDeviceList.append(QString::fromWCharArray(desc.DeviceName));
    }
    if (outputID >= 0 && outputID < dxgiOutputVector.count())
    {
        dxgiOutput = dxgiOutputVector.at(outputID);
    }

    if (!dxgiOutput)
    {
        _pLastError = "Failed to found screen!";
        qDebug() << _pLastError;
        d3dDevice->Release();
        d3dContext->Release();
        for (auto output: dxgiOutputVector)
        {
            output->Release();
        }
        return false;
    }
    IDXGIOutput6* dxgiOutput6 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput6), reinterpret_cast<void**>(&dxgiOutput6));
    for (auto output: dxgiOutputVector)
    {
        output->Release();
    }
    if (FAILED(hr))
    {
        _pLastError = "Failed to QueryInterface IDXGIOutput6 ErrorCode = " + QString::number(uint(hr), 16);
        qDebug() << _pLastError;
        d3dDevice->Release();
        d3dContext->Release();
        return false;
    }
    UINT supportedFormatsCount = 1; // 支持的格式数
    DXGI_FORMAT supportedFormats[1] = {DXGI_FORMAT_B8G8R8A8_UNORM};
    hr = dxgiOutput6->DuplicateOutput1(d3dDevice, 0, supportedFormatsCount, supportedFormats, &_duplication);
    dxgiOutput6->Release();
    if (FAILED(hr))
    {
        _pLastError = "Failed to DuplicateOutput ErrorCode = " + QString::number(uint(hr), 16);
        qDebug() << _pLastError;
        d3dDevice->Release();
        d3dContext->Release();
        if (_duplication)
        {
            _duplication->Release();
            _duplication = nullptr;
        }
        return false;
    }
    _device = d3dDevice;
    _context = d3dContext;
    _pIsInitSuccess = true;
    return true;
}

void ElaDxgi::setIsGrabActive(bool isGrabActive)
{
    _isGrabActive = isGrabActive;
}

bool ElaDxgi::getIsGrabActive() const
{
    return _isGrabActive;
}

void ElaDxgi::setIsGrabStopped(bool const& isGrabStopped)
{
    _pIsGrabStopped.store(isGrabStopped, std::memory_order_release);
}

bool ElaDxgi::getIsGrabStopped() const
{
    return _pIsGrabStopped.load(std::memory_order_acquire);
}

QImage ElaDxgi::getGrabImage()
{
    QMutexLocker grabLocker(&_grabMutex);
    return _grabImage;
}

void ElaDxgi::onGrabScreen()
{
    if (!_duplication || !_device || !_context)
    {
        setIsGrabStopped(true);
        return;
    }
    setIsGrabStopped(false);
    const auto stoppedGuard = qScopeGuard([this]() {
        setIsGrabActive(false);
        setIsGrabStopped(true);
    });
    while (_isGrabActive)
    {
        _grabTimer.start();
        IDXGIResource* desktopRes = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo{};
        HRESULT hr = _duplication->AcquireNextFrame(_pTimeoutMsValue, &frameInfo, &desktopRes);
        if (FAILED(hr))
        {
            if (desktopRes)
            {
                desktopRes->Release();
            }
            if (hr == DXGI_ERROR_WAIT_TIMEOUT)
            {
                continue;
            }
            if (!_isGrabActive || !initialize(_pDxDeviceID, _pOutputDeviceID))
            {
                return;
            }
            continue;
        }

        // 本轮所有退出路径均释放帧；正常路径延至帧间等待结束，减少 DXGI 的重复更新。
        const auto frameGuard = qScopeGuard([this, desktopRes]() {
            if (desktopRes)
            {
                desktopRes->Release();
            }
            _duplication->ReleaseFrame();
        });
        if (!_isGrabActive || !desktopRes || !frameInfo.LastPresentTime.QuadPart)
        {
            continue;
        }
        const QImage grabImage = _captureFrame(desktopRes);
        if (grabImage.isNull())
        {
            continue;
        }
        {
            QMutexLocker grabLocker(&_grabMutex);
            _grabImage = grabImage;
        }
        Q_EMIT grabScreenOver(grabImage);
        if (_lastGrabTime == 0)
        {
            _lastGrabTime = static_cast<qreal>(_grabTimer.elapsed()); // 毫秒
        }
        else
        {
            _lastGrabTime = (static_cast<qreal>(_grabTimer.elapsed()) + _lastGrabTime) / 2;
        }
        _cpuSleepTime = (1000 - _lastGrabTime * _pGrabFrameRate) / _pGrabFrameRate;
        waitElapsedTime(_cpuSleepTime * 1000);
        // qDebug() << _cpuSleepTime << _lastGrabTime;
    }
}

QImage ElaDxgi::_captureFrame(IDXGIResource* desktopResource)
{
    ID3D11Texture2D* textureRes = nullptr;
    HRESULT hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&textureRes));
    if (FAILED(hr))
    {
        qWarning() << "获取桌面纹理失败，错误码：" << QString::number(static_cast<uint>(hr), 16);
        return {};
    }
    const auto textureGuard = qScopeGuard([textureRes]() {
        textureRes->Release();
    });
    D3D11_TEXTURE2D_DESC desc{};
    textureRes->GetDesc(&desc);
    if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM || desc.Width == 0 || desc.Height == 0
        || desc.Width > static_cast<UINT>((std::numeric_limits<int>::max)() / 4) || desc.Height > static_cast<UINT>((std::numeric_limits<int>::max)()))
    {
        qWarning() << "桌面纹理格式或尺寸无效";
        return {};
    }
    if (_texture)
    {
        D3D11_TEXTURE2D_DESC cachedDesc{};
        _texture->GetDesc(&cachedDesc);
        if (cachedDesc.Width != desc.Width || cachedDesc.Height != desc.Height || cachedDesc.Format != desc.Format)
        {
            _texture->Release();
            _texture = nullptr;
        }
    }
    if (!_texture)
    {
        D3D11_TEXTURE2D_DESC texDesc{};
        texDesc.Width = desc.Width;
        texDesc.Height = desc.Height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_STAGING;
        texDesc.Format = desc.Format;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        hr = _device->CreateTexture2D(&texDesc, nullptr, &_texture);
        if (FAILED(hr))
        {
            qWarning() << "创建暂存纹理失败，错误码：" << QString::number(static_cast<uint>(hr), 16);
            return {};
        }
    }
    _context->CopyResource(_texture, textureRes);
    IDXGISurface* surface = nullptr;
    hr = _texture->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&surface));
    if (FAILED(hr))
    {
        qWarning() << "获取暂存表面失败，错误码：" << QString::number(static_cast<uint>(hr), 16);
        return {};
    }
    const auto surfaceGuard = qScopeGuard([surface]() {
        surface->Release();
    });
    DXGI_MAPPED_RECT map{};
    hr = surface->Map(&map, DXGI_MAP_READ);
    if (FAILED(hr))
    {
        qWarning() << "映射桌面纹理失败，错误码：" << QString::number(static_cast<uint>(hr), 16);
        return {};
    }
    const auto mapGuard = qScopeGuard([surface]() {
        surface->Unmap();
    });
    const int width = static_cast<int>(desc.Width);
    const int height = static_cast<int>(desc.Height);
    if (!map.pBits || map.Pitch < width * 4)
    {
        return {};
    }
    const QImage mappedImage(static_cast<const uchar*>(map.pBits), width, height, map.Pitch, QImage::Format_ARGB32);
    QRect grabArea = _pGrabArea;
    if (_pIsGrabCenter)
    {
        grabArea.moveTo((width - grabArea.width()) / 2, (height - grabArea.height()) / 2);
    }
    // 像素只在映射有效时复制一次；返回值和后续快照不再引用 DXGI 内存。
    return mappedImage.copy(grabArea);
}

void ElaDxgi::releaseInterface()
{
    {
        QMutexLocker grabLocker(&_grabMutex);
        _grabImage = QImage();
    }
    if (_texture)
    {
        _texture->Release();
        _texture = nullptr;
    }
    if (_duplication)
    {
        _duplication->Release();
        _duplication = nullptr;
    }
    if (_context)
    {
        _context->Release();
        _context = nullptr;
    }
    if (_device)
    {
        _device->Release();
        _device = nullptr;
    }
}

void ElaDxgi::waitElapsedTime(qreal usec)
{
    if (usec <= 0)
    {
        return;
    }
    static QElapsedTimer timer;
    timer.start();
    while (static_cast<qreal>(timer.nsecsElapsed()) / 1000.0 < usec)
    {
    }
}
#endif
