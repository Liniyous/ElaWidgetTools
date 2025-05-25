#include "ElaWinShadowHelper.h"

#ifdef Q_OS_WIN
#include <QDebug>
#include <QWidget>

ElaWinShadowHelper::ElaWinShadowHelper(QObject* parent)
    : QObject(parent)
{
    HMODULE module = LoadLibraryW(L"ntdll.dll");
    if (module)
    {
        auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(::GetProcAddress(module, "RtlGetVersion"));
        Q_ASSERT(pRtlGetVersion);
        _windowsVersion.dwOSVersionInfoSize = sizeof(_windowsVersion);
        pRtlGetVersion(&_windowsVersion);
    }
}

ElaWinShadowHelper::~ElaWinShadowHelper()
{
}

bool ElaWinShadowHelper::initDWMAPI()
{
    HMODULE dwmModule = LoadLibraryW(L"dwmapi.dll");
    if (dwmModule)
    {
        if (!_dwmExtendFrameIntoClientArea)
        {
            _dwmExtendFrameIntoClientArea = reinterpret_cast<DwmExtendFrameIntoClientAreaFunc>(GetProcAddress(dwmModule, "DwmExtendFrameIntoClientArea"));
        }
        if (!_dwmSetWindowAttribute)
        {
            _dwmSetWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFunc>(GetProcAddress(dwmModule, "DwmSetWindowAttribute"));
        }
        if (!_dwmIsCompositionEnabled)
        {
            _dwmIsCompositionEnabled = reinterpret_cast<DwmIsCompositionEnabledFunc>(GetProcAddress(dwmModule, "DwmIsCompositionEnabled"));
        }
        if (!_dwmEnableBlurBehindWindow)
        {
            _dwmEnableBlurBehindWindow = reinterpret_cast<DwmEnableBlurBehindWindowFunc>(GetProcAddress(dwmModule, "DwmEnableBlurBehindWindow"));
        }
        if (!(_dwmExtendFrameIntoClientArea && _dwmSetWindowAttribute && _dwmIsCompositionEnabled && _dwmEnableBlurBehindWindow))
        {
            qCritical() << "Dwm Func Init Fail!";
            return false;
        }
    }
    else
    {
        qCritical() << "dwmapi.dll Load Fail!";
        return false;
    }
    HMODULE user32Module = LoadLibraryW(L"user32.dll");
    if (user32Module)
    {
        if (!_setWindowCompositionAttribute)
        {
            _setWindowCompositionAttribute = reinterpret_cast<SetWindowCompositionAttributeFunc>(GetProcAddress(user32Module, "SetWindowCompositionAttribute"));
        }
        if (!(_setWindowCompositionAttribute))
        {
            qCritical() << "User32 Func Init Fail!";
            return false;
        }
    }
    else
    {
        qCritical() << "user32.dll Load Fail!";
        return false;
    }
    return true;
}

void ElaWinShadowHelper::setWindowShadow(quint64 hwnd)
{
    static const MARGINS shadow = {1, 0, 0, 0};
    _dwmExtendFrameIntoClientArea((HWND)hwnd, &shadow);
}

void ElaWinShadowHelper::setWindowThemeMode(quint64 hwnd, bool isLightMode)
{
    if (!compareWindowsVersion(Win10_1809))
    {
        return;
    }
    BOOL bIsLightMode = !isLightMode;
    _DWMWINDOWATTRIBUTE dwAttritube = compareWindowsVersion(Win10_20H1) ? _DWMWA_USE_IMMERSIVE_DARK_MODE : _DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1;
    _dwmSetWindowAttribute((HWND)hwnd, dwAttritube, &bIsLightMode, sizeof(bIsLightMode));
}

void ElaWinShadowHelper::setWindowDisplayMode(QWidget* widget, ElaApplicationType::WindowDisplayMode displayMode, ElaApplicationType::WindowDisplayMode lastDisplayMode)
{
    HWND winHwnd = (HWND)widget->winId();
    switch (lastDisplayMode)
    {
    case ElaApplicationType::Mica:
    {
        if (!compareWindowsVersion(Win11_Origin))
        {
            break;
        }
        if (compareWindowsVersion(Win11_22H2))
        {
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
            _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        }
        else
        {
            const BOOL isEnable = FALSE;
            _dwmSetWindowAttribute(winHwnd, _DWMWA_MICA_EFFECT, &isEnable, sizeof(isEnable));
        }
        break;
    }
    case ElaApplicationType::MicaAlt:
    {
        if (!compareWindowsVersion(Win11_22H2))
        {
            break;
        }
        const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
        _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        break;
    }
    case ElaApplicationType::Acrylic:
    {
        if (!compareWindowsVersion(Win11_Origin))
        {
            break;
        }
        const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
        _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        break;
    }
    case ElaApplicationType::DWMBlur:
    {
        if (compareWindowsVersion(Win8_Origin))
        {
            _ACCENT_POLICY policy{};
            policy.dwAccentState = _ACCENT_DISABLED;
            policy.dwAccentFlags = _ACCENT_NONE;
            _WINDOWCOMPOSITIONATTRIBDATA wcad{};
            wcad.Attrib = _WCA_ACCENT_POLICY;
            wcad.pvData = &policy;
            wcad.cbData = sizeof(policy);
            _setWindowCompositionAttribute(winHwnd, &wcad);
        }
        else
        {
            DWM_BLURBEHIND bb{};
            bb.fEnable = FALSE;
            bb.dwFlags = DWM_BB_ENABLE;
            _dwmEnableBlurBehindWindow(winHwnd, &bb);
        }
        break;
    }
    default:
    {
        break;
    }
    }

    switch (displayMode)
    {
    case ElaApplicationType::Mica:
    {
        if (!compareWindowsVersion(Win11_Origin))
        {
            break;
        }
        _externWindowMargins(winHwnd);
        if (compareWindowsVersion(Win11_22H2))
        {
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_MAINWINDOW;
            _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        }
        else
        {
            const BOOL enable = TRUE;
            _dwmSetWindowAttribute(winHwnd, _DWMWA_MICA_EFFECT, &enable, sizeof(enable));
        }
        break;
    }
    case ElaApplicationType::MicaAlt:
    {
        if (!compareWindowsVersion(Win11_22H2))
        {
            break;
        }
        _externWindowMargins(winHwnd);
        const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_TABBEDWINDOW;
        _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        break;
    }
    case ElaApplicationType::Acrylic:
    {
        if (!compareWindowsVersion(Win11_Origin))
        {
            break;
        }
        _externWindowMargins(winHwnd);
        const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_TRANSIENTWINDOW;
        _dwmSetWindowAttribute(winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        break;
    }
    case ElaApplicationType::DWMBlur:
    {
        MARGINS windowMargins = {0, 1, 0, 0};
        _dwmExtendFrameIntoClientArea(winHwnd, &windowMargins);
        if (compareWindowsVersion(Win8_Origin))
        {
            _ACCENT_POLICY policy{};
            policy.dwAccentState = _ACCENT_ENABLE_BLURBEHIND;
            policy.dwAccentFlags = _ACCENT_NONE;
            _WINDOWCOMPOSITIONATTRIBDATA wcad{};
            wcad.Attrib = _WCA_ACCENT_POLICY;
            wcad.pvData = &policy;
            wcad.cbData = sizeof(policy);
            _setWindowCompositionAttribute(winHwnd, &wcad);
        }
        else
        {
            DWM_BLURBEHIND bb{};
            bb.fEnable = TRUE;
            bb.dwFlags = DWM_BB_ENABLE;
            _dwmEnableBlurBehindWindow(winHwnd, &bb);
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

bool ElaWinShadowHelper::isCompositionEnabled() const
{
    BOOL isCompositionEnabled = false;
    _dwmIsCompositionEnabled(&isCompositionEnabled);
    return isCompositionEnabled;
}

bool ElaWinShadowHelper::compareWindowsVersion(const QString& windowsVersion) const
{
    QStringList versionList = windowsVersion.split(".");
    if (versionList.count() != 3)
    {
        return false;
    }
    return (_windowsVersion.dwMajorVersion > versionList[0].toUInt()) || (_windowsVersion.dwMajorVersion == versionList[0].toUInt() && (_windowsVersion.dwMinorVersion > versionList[1].toUInt() || _windowsVersion.dwBuildNumber >= versionList[2].toUInt()));
}

void ElaWinShadowHelper::_externWindowMargins(HWND hwnd)
{
    static const MARGINS margins = {65536, 0, 0, 0};
    _dwmExtendFrameIntoClientArea(hwnd, &margins);
}
#endif