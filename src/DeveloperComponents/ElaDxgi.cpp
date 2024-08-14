#include "ElaDxgi.h"
#ifdef Q_OS_WIN
#include <QDateTime>
#include <QDebug>

ElaDxgi::ElaDxgi(QObject* parent)
    : QObject(parent)
{
    _pIsInitSuccess = false;
    _pIsGrabActive = false;
    _pGrabFrameRate = 120;
    _pTimeoutMsValue = 50;
    _pIsGrabStoped = true;
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
    CreateDXGIFactory(__uuidof(dxgiFactory), reinterpret_cast<void**>(&dxgiFactory));
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
        for (auto adapter : dxgiAdapterVector)
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
    for (auto adapter : dxgiAdapterVector)
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
        for (auto output : dxgiOutputVector)
        {
            output->Release();
        }
        return false;
    }
    IDXGIOutput6* dxgiOutput6 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput6), reinterpret_cast<void**>(&dxgiOutput6));
    for (auto output : dxgiOutputVector)
    {
        output->Release();
    }
    if (FAILED(hr))
    {
        _pLastError = "Failed to QueryInterface IDXGIOutput6 ErrorCode = " + QString::number(uint(hr), 16);
        qDebug() << _pLastError;
        d3dDevice->Release();
        d3dContext->Release();
        dxgiOutput6->Release();
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

QImage ElaDxgi::getGrabImage() const
{
    return _grabImg;
}

void ElaDxgi::onGrabScreen()
{
    qreal startTime = 0;
    qreal endTime = 0;
    if (!_duplication || !_device || !_context)
    {
        setIsGrabStoped(true);
        return;
    }
    _pIsGrabStoped = false;
    while (_pIsGrabActive)
    {
        IDXGIResource* desktopRes = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        while (true)
        {
            startTime = QDateTime::currentMSecsSinceEpoch();
            _duplication->ReleaseFrame();
            HRESULT hr = _duplication->AcquireNextFrame(_pTimeoutMsValue, &frameInfo, &desktopRes);
            if (FAILED(hr))
            {
                if (hr != DXGI_ERROR_WAIT_TIMEOUT)
                {
                    if (desktopRes)
                    {
                        desktopRes->Release();
                        desktopRes = nullptr;
                    }
                    while (true)
                    {
                        if (!_duplication || !_device || !_context)
                        {
                            setIsGrabStoped(true);
                            return;
                        }
                        if (initialize(_pDxDeviceID, _pOutputDeviceID))
                        {
                            break;
                        }
                    }
                }
                continue;
            }
            if (frameInfo.LastPresentTime.QuadPart)
            {
                break;
            }
        }
        D3D11_TEXTURE2D_DESC desc;
        ID3D11Texture2D* textrueRes = nullptr;
        HRESULT hr = desktopRes->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&textrueRes));
        desktopRes->Release();
        if (FAILED(hr))
        {
            qDebug() << "Failed to ID3D11Texture2D result =" << QString::number(uint(hr), 16);
            continue;
        }
        textrueRes->GetDesc(&desc);
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(texDesc));
        texDesc.Width = desc.Width;
        texDesc.Height = desc.Height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_STAGING;
        texDesc.Format = desc.Format;
        texDesc.BindFlags = 0;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        texDesc.MiscFlags = 0;
        _device->CreateTexture2D(&texDesc, nullptr, &_texture);
        _context->CopyResource(_texture, textrueRes);
        IDXGISurface1* surface = nullptr;
        hr = _texture->QueryInterface(__uuidof(IDXGISurface1), reinterpret_cast<void**>(&surface));
        if (FAILED(hr))
        {
            qDebug() << "Failed to QueryInterface IDXGISurface1 ErrorCode ="
                     << QString::number(uint(hr), 16);
            continue;
        }

        DXGI_MAPPED_RECT map;
        surface->Map(&map, DXGI_MAP_READ);
        QImage grabImage(static_cast<uchar*>(map.pBits), int(desc.Width), int(desc.Height),
                         QImage::Format_ARGB32);
        surface->Unmap();
        surface->Release();
        _texture->Release();
        _grabImg = grabImage.copy(_pGrabArea);
        Q_EMIT grabScreenOver();
        endTime = QDateTime::currentMSecsSinceEpoch();
        if (_lastGrabTime == 0)
        {
            _lastGrabTime = endTime - startTime; // 毫秒
        }
        else
        {
            _lastGrabTime = ((endTime - startTime) + _lastGrabTime) / 2.0;
        }
        _cpuSleepTime = (1000 - _lastGrabTime * _pGrabFrameRate) / _pGrabFrameRate;
        cpuSleep(_cpuSleepTime * 1000);
        // qDebug() << _cpuSleepTime << _lastGrabTime;
    }
    setIsGrabStoped(true);
}

void ElaDxgi::releaseInterface()
{
    if (_duplication)
    {
        _duplication->Release();
        _duplication = nullptr;
    }
    if (_device)
    {
        _device->Release();
        _device = nullptr;
    }
    if (_context)
    {
        _context->Release();
        _context = nullptr;
    }
}

void ElaDxgi::cpuSleep(int usec)
{
    if (usec <= 0)
    {
        return;
    }
    LARGE_INTEGER cpuFerq;
    LARGE_INTEGER startTime;
    LARGE_INTEGER endTime;
    QueryPerformanceFrequency(&cpuFerq);
    QueryPerformanceCounter(&startTime);
    while (1)
    {
        QueryPerformanceCounter(&endTime);
        if (((endTime.QuadPart - startTime.QuadPart) * 1000000) / cpuFerq.QuadPart > usec)
        {
            break;
        }
    }
}
#endif
