#ifndef ELAWINSHADOWHELPER_H
#define ELAWINSHADOWHELPER_H
#include <qglobal.h>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>
#endif
#ifdef Q_OS_WIN
[[maybe_unused]] static inline void setShadow(HWND hwnd)
{
    const MARGINS shadow = {1, 0, 0, 0};
    typedef HRESULT(WINAPI * DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS* pMarInset);
    HMODULE module = LoadLibraryW(L"dwmapi.dll");
    if (module)
    {
        DwmExtendFrameIntoClientAreaPtr dwm_extendframe_into_client_area_;
        dwm_extendframe_into_client_area_ = reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(module, "DwmExtendFrameIntoClientArea"));
        if (dwm_extendframe_into_client_area_)
        {
            dwm_extendframe_into_client_area_(hwnd, &shadow);
        }
    }
}
#endif

#endif // ELAWINSHADOWHELPER_H
