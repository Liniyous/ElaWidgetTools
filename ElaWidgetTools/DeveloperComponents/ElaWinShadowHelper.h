#ifndef ELAWINSHADOWHELPER_H
#define ELAWINSHADOWHELPER_H
#include "ElaDef.h"
#include "ElaSingleton.h"
#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>

#define Win7_Origin "6.1.0"
#define Win8_Origin "6.2.0"
#define Win10_Origin "10.0.0"
#define Win10_1809 "10.0.17763"
#define Win10_1903 "10.0.18362"
#define Win10_20H1 "10.0.19041"
#define Win11_Origin "10.0.22000"
#define Win11_22H2 "10.0.22621"

#define eWinHelper ElaWinShadowHelper::getInstance()
class ElaWinShadowHelper : public QObject
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsWinVersionGreater10)
    Q_PRIVATE_CREATE(bool, IsWinVersionGreater11)
    Q_SINGLETON_CREATE(ElaWinShadowHelper)
private:
    explicit ElaWinShadowHelper(QObject* parent = nullptr);
    ~ElaWinShadowHelper() override;

public:
    bool initWinAPI();

    void setWindowShadow(quint64 hwnd);
    void setWindowThemeMode(quint64 hwnd, bool isLightMode);
    void setWindowDisplayMode(QWidget* widget, ElaApplicationType::WindowDisplayMode displayMode, ElaApplicationType::WindowDisplayMode lastDisplayMode);
    bool getIsCompositionEnabled() const;
    bool getIsFullScreen(const HWND& hwnd);
    MONITORINFOEXW getMonitorForWindow(const HWND& hwnd);
    quint32 getResizeBorderThickness(const HWND& hwnd);
    quint32 getDpiForWindow(const HWND& hwnd);
    int getSystemMetricsForDpi(const HWND& hwnd, const int index);
    bool compareWindowsVersion(const QString& windowsVersion) const;

private:
    enum _DWM_SYSTEMBACKDROP_TYPE
    {
        _DWMSBT_AUTO,
        _DWMSBT_NONE,
        _DWMSBT_MAINWINDOW,
        _DWMSBT_TRANSIENTWINDOW,
        _DWMSBT_TABBEDWINDOW
    };
    enum _DWMWINDOWATTRIBUTE
    {
        _DWMWA_USE_HOSTBACKDROPBRUSH = 17,
        _DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19,
        _DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
        _DWMWA_WINDOW_CORNER_PREFERENCE = 33,
        _DWMWA_VISIBLE_FRAME_BORDER_THICKNESS = 37,
        _DWMWA_SYSTEMBACKDROP_TYPE = 38,
        _DWMWA_MICA_EFFECT = 1029
    };
    enum _WINDOWCOMPOSITIONATTRIB
    {
        _WCA_UNDEFINED = 0,
        _WCA_NCRENDERING_ENABLED = 1,
        _WCA_NCRENDERING_POLICY = 2,
        _WCA_TRANSITIONS_FORCEDISABLED = 3,
        _WCA_ALLOW_NCPAINT = 4,
        _WCA_CAPTION_BUTTON_BOUNDS = 5,
        _WCA_NONCLIENT_RTL_LAYOUT = 6,
        _WCA_FORCE_ICONIC_REPRESENTATION = 7,
        _WCA_EXTENDED_FRAME_BOUNDS = 8,
        _WCA_HAS_ICONIC_BITMAP = 9,
        _WCA_THEME_ATTRIBUTES = 10,
        _WCA_NCRENDERING_EXILED = 11,
        _WCA_NCADORNMENTINFO = 12,
        _WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
        _WCA_VIDEO_OVERLAY_ACTIVE = 14,
        _WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
        _WCA_DISALLOW_PEEK = 16,
        _WCA_CLOAK = 17,
        _WCA_CLOAKED = 18,
        _WCA_ACCENT_POLICY = 19,
        _WCA_FREEZE_REPRESENTATION = 20,
        _WCA_EVER_UNCLOAKED = 21,
        _WCA_VISUAL_OWNER = 22,
        _WCA_HOLOGRAPHIC = 23,
        _WCA_EXCLUDED_FROM_DDA = 24,
        _WCA_PASSIVEUPDATEMODE = 25,
        _WCA_USEDARKMODECOLORS = 26,
        _WCA_CORNER_STYLE = 27,
        _WCA_PART_COLOR = 28,
        _WCA_DISABLE_MOVESIZE_FEEDBACK = 29,
        _WCA_LAST = 30
    };

    enum _ACCENT_STATE
    {
        _ACCENT_DISABLED = 0,
        _ACCENT_ENABLE_GRADIENT = 1,
        _ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        _ACCENT_ENABLE_BLURBEHIND = 3,
        _ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
        _ACCENT_ENABLE_HOST_BACKDROP = 5,
        _ACCENT_INVALID_STATE = 6
    };
    enum _ACCENT_FLAG
    {
        _ACCENT_NONE = 0,
        _ACCENT_ENABLE_ACRYLIC = 1,
        _ACCENT_ENABLE_ACRYLIC_WITH_LUMINOSITY = 482
    };
    struct _ACCENT_POLICY {
        DWORD dwAccentState;
        DWORD dwAccentFlags;
        DWORD dwGradientColor;
        DWORD dwAnimationId;
    };
    struct _WINDOWCOMPOSITIONATTRIBDATA {
        _WINDOWCOMPOSITIONATTRIB Attrib;
        PVOID pvData;
        SIZE_T cbData;
    };
    enum _MONITOR_DPI_TYPE
    {
        MDT_EFFECTIVE_DPI = 0,
        MDT_ANGULAR_DPI = 1,
        MDT_RAW_DPI = 2,
        MDT_DEFAULT = MDT_EFFECTIVE_DPI
    };

    using DwmExtendFrameIntoClientAreaFunc = HRESULT(WINAPI*)(HWND hWnd, const MARGINS* pMarInset);
    using DwmSetWindowAttributeFunc = HRESULT(WINAPI*)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    using DwmIsCompositionEnabledFunc = HRESULT(WINAPI*)(BOOL* pfEnabled);
    using RtlGetVersionFunc = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW);
    using DwmEnableBlurBehindWindowFunc = HRESULT(WINAPI*)(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
    using SetWindowCompositionAttributeFunc = BOOL(WINAPI*)(HWND hwnd, const _WINDOWCOMPOSITIONATTRIBDATA*);
    using GetDpiForWindowFunc = UINT(WINAPI*)(HWND hwnd);
    using GetDpiForMonitorFunc = HRESULT(WINAPI*)(HMONITOR hmonitor, _MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);
    using GetSystemMetricsForDpiFunc = int(WINAPI*)(int index, UINT dpi);

    DwmExtendFrameIntoClientAreaFunc _dwmExtendFrameIntoClientArea{nullptr};
    DwmSetWindowAttributeFunc _dwmSetWindowAttribute{nullptr};
    DwmIsCompositionEnabledFunc _dwmIsCompositionEnabled{nullptr};
    DwmEnableBlurBehindWindowFunc _dwmEnableBlurBehindWindow{nullptr};
    SetWindowCompositionAttributeFunc _setWindowCompositionAttribute{nullptr};
    GetDpiForWindowFunc _getDpiForWindow{nullptr};
    GetDpiForMonitorFunc _getDpiForMonitor{nullptr};
    GetSystemMetricsForDpiFunc _getSystemMetricsForDpi{nullptr};
    RTL_OSVERSIONINFOW _windowsVersion{};
    void _externWindowMargins(HWND hwnd);
};

#endif
#endif // ELAWINSHADOWHELPER_H
