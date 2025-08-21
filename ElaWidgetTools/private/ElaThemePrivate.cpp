#include "ElaThemePrivate.h"

ElaThemePrivate::ElaThemePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaThemePrivate::~ElaThemePrivate()
{
}

void ElaThemePrivate::_initThemeColor()
{
    //ElaScrollBar
    _lightThemeColorList[ElaThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
    _darkThemeColorList[ElaThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    //ElaToggleSwitch
    _lightThemeColorList[ElaThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
    _darkThemeColorList[ElaThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

    // 主题颜色
    _lightThemeColorList[ElaThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
    _darkThemeColorList[ElaThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
    _lightThemeColorList[ElaThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
    _darkThemeColorList[ElaThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
    _lightThemeColorList[ElaThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
    _darkThemeColorList[ElaThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

    // 通用颜色
    // 普通窗体
    _lightThemeColorList[ElaThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[ElaThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
    _lightThemeColorList[ElaThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 80);
    _darkThemeColorList[ElaThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

    // 浮动窗体
    _lightThemeColorList[ElaThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[ElaThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
    _lightThemeColorList[ElaThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
    _darkThemeColorList[ElaThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
    _lightThemeColorList[ElaThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
    _darkThemeColorList[ElaThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
    _lightThemeColorList[ElaThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
    _darkThemeColorList[ElaThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

    // Dialog窗体
    _lightThemeColorList[ElaThemeType::DialogBase] = Qt::white;
    _darkThemeColorList[ElaThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
    _lightThemeColorList[ElaThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[ElaThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

    // 基础颜色
    _lightThemeColorList[ElaThemeType::BasicText] = Qt::black;
    _darkThemeColorList[ElaThemeType::BasicText] = Qt::white;
    _lightThemeColorList[ElaThemeType::BasicTextInvert] = Qt::white;
    _darkThemeColorList[ElaThemeType::BasicTextInvert] = Qt::black;
    _lightThemeColorList[ElaThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[ElaThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
    _lightThemeColorList[ElaThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _darkThemeColorList[ElaThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _lightThemeColorList[ElaThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
    _darkThemeColorList[ElaThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
    _lightThemeColorList[ElaThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
    _darkThemeColorList[ElaThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
    _lightThemeColorList[ElaThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
    _darkThemeColorList[ElaThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
    _lightThemeColorList[ElaThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
    _darkThemeColorList[ElaThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
    _lightThemeColorList[ElaThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
    _darkThemeColorList[ElaThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
    _lightThemeColorList[ElaThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ElaThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
    _lightThemeColorList[ElaThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
    _darkThemeColorList[ElaThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
    _lightThemeColorList[ElaThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
    _darkThemeColorList[ElaThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
    _lightThemeColorList[ElaThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[ElaThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
    _darkThemeColorList[ElaThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
    _lightThemeColorList[ElaThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
    _darkThemeColorList[ElaThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
    _lightThemeColorList[ElaThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
    _darkThemeColorList[ElaThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
    _lightThemeColorList[ElaThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _darkThemeColorList[ElaThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _lightThemeColorList[ElaThemeType::BasicChute] = QColor(0xB3, 0xB3, 0xB3);
    _darkThemeColorList[ElaThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

    // 基础透明
    _lightThemeColorList[ElaThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
    _darkThemeColorList[ElaThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
    _lightThemeColorList[ElaThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
    _darkThemeColorList[ElaThemeType::BasicBaseAlpha] = QColor(0x2D, 0x2D, 0x2D, 95);
    _lightThemeColorList[ElaThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
    _darkThemeColorList[ElaThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
    _lightThemeColorList[ElaThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    _darkThemeColorList[ElaThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[ElaThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[ElaThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
    _lightThemeColorList[ElaThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    _darkThemeColorList[ElaThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[ElaThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[ElaThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

    // 状态颜色
    _lightThemeColorList[ElaThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
    _darkThemeColorList[ElaThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);

    // Win10顶部边框颜色
    _lightThemeColorList[ElaThemeType::Win10BorderActive] = QColor(0x6E, 0x6E, 0x6E);
    _darkThemeColorList[ElaThemeType::Win10BorderActive] = QColor(0x33, 0x33, 0x33);
    _lightThemeColorList[ElaThemeType::Win10BorderInactive] = QColor(0xA7, 0xA7, 0xA7);
    _darkThemeColorList[ElaThemeType::Win10BorderInactive] = QColor(0x3D, 0x3D, 0x3D);
}
