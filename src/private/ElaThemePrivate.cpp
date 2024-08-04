#include "ElaThemePrivate.h"

using namespace ElaThemeType;

ElaThemePrivate::ElaThemePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaThemePrivate::~ElaThemePrivate()
{
}

void ElaThemePrivate::_initThemeColor()
{
    //ElaAcrylicUrlCard
    _lightThemeColorList[AcrylicUrlCardBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[AcrylicUrlCardBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[AcrylicUrlCardBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[AcrylicUrlCardBase] = QColor(0x04, 0x09, 0x11);
    _lightThemeColorList[AcrylicUrlCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[AcrylicUrlCardHover] = QColor(0x17, 0x17, 0x17);
    _lightThemeColorList[AcrylicUrlCardSubTitleText] = Qt::black;
    _darkThemeColorList[AcrylicUrlCardSubTitleText] = QColor(0x79, 0x72, 0x68);

    //ElaBreadcrumbBar
    _lightThemeColorList[BreadcrumbBarText] = QColor(0x5B, 0x5B, 0x5E);
    _darkThemeColorList[BreadcrumbBarText] = QColor(0xDE, 0xDE, 0xDE);
    _lightThemeColorList[BreadcrumbBarTextHasFocus] = QColor(0x86, 0x86, 0x8A);
    _darkThemeColorList[BreadcrumbBarTextHasFocus] = QColor(0x86, 0x86, 0x8A);

    //ElaCheckBox
    _lightThemeColorList[CheckBoxBorder] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[CheckBoxBorder] = QColor(0xA0, 0xA0, 0xA0);
    _lightThemeColorList[CheckBoxCheckedBorder] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[CheckBoxCheckedBorder] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[CheckBoxPressBorder] = QColor(0xBF, 0xBF, 0xBF);
    _darkThemeColorList[CheckBoxPressBorder] = QColor(0x5A, 0x5A, 0x5A);
    _lightThemeColorList[CheckBoxBase] = QColor(0xF7, 0xF7, 0xF7);
    _darkThemeColorList[CheckBoxBase] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[CheckBoxHover] = QColor(0xEC, 0xEC, 0xEC);
    _darkThemeColorList[CheckBoxHover] = QColor(0x48, 0x48, 0x48);
    _lightThemeColorList[CheckBoxCheckedBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[CheckBoxCheckedBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[CheckBoxCheckedHover] = QColor(0x00, 0x75, 0xCF);
    _darkThemeColorList[CheckBoxCheckedHover] = QColor(0x42, 0x8B, 0xC3);
    _lightThemeColorList[CheckBoxCheckedPress] = QColor(0x00, 0x85, 0xEA);
    _darkThemeColorList[CheckBoxCheckedPress] = QColor(0x3A, 0x7B, 0xAC);

    //ElaComboBox ElaMultiComboBoxs
    _lightThemeColorList[ComboBoxMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ComboBoxMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ComboBoxBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ComboBoxBorder] = QColor(0x4B, 0x4B, 0x4D);
    _lightThemeColorList[ComboBoxBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ComboBoxBase] = QColor(0x3B, 0x3B, 0x3B);
    _lightThemeColorList[ComboBoxHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ComboBoxHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ComboBoxShapedFrameBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ComboBoxShapedFrameBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ComboBoxShapedFrameBase] = QColor(0xF9, 0xF9, 0xF9);
    _darkThemeColorList[ComboBoxShapedFrameBase] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ComboBoxItemViewItemHover] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ComboBoxItemViewItemHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ComboBoxItemViewItemSelected] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ComboBoxItemViewItemSelected] = QColor(0x4F, 0x4F, 0x4F);
    _lightThemeColorList[ComboBoxItemViewItemSelectedHover] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ComboBoxItemViewItemSelectedHover] = QColor(0x40, 0x40, 0x40);

    //ElaContentDialog
    _lightThemeColorList[ContentDialogBase] = Qt::white;
    _darkThemeColorList[ContentDialogBase] = QColor(0x2B, 0x2B, 0x2B);
    _lightThemeColorList[ContentDialogButtonAreaBase] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[ContentDialogButtonAreaBase] = QColor(0x20, 0x20, 0x20);
    _lightThemeColorList[ContentDialogRightButtonBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ContentDialogRightButtonBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ContentDialogRightButtonText] = Qt::white;
    _darkThemeColorList[ContentDialogRightButtonText] = Qt::white;
    _lightThemeColorList[ContentDialogRightButtonHover] = QColor(0x00, 0x70, 0xC6);
    _darkThemeColorList[ContentDialogRightButtonHover] = QColor(0x45, 0x91, 0xCC);
    _lightThemeColorList[ContentDialogRightButtonPress] = QColor(0x00, 0x7A, 0xD8);
    _darkThemeColorList[ContentDialogRightButtonPress] = QColor(0x3F, 0x85, 0xBB);

    //ElaDockWidget
    _lightThemeColorList[DockWidgetBaseStart] = QColor(0xF3, 0xF2, 0xF9);
    _darkThemeColorList[DockWidgetBaseStart] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[DockWidgetBaseEnd] = QColor(0xF4, 0xF1, 0xF8);
    _darkThemeColorList[DockWidgetBaseEnd] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[DockWidgetTitleBarBase] = QColor(0xF5, 0xF1, 0xFF);
    _darkThemeColorList[DockWidgetTitleBarBase] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[DockWidgetTitleBarFloatButtonHover] = QColor(0xEF, 0xE6, 0xED);
    _darkThemeColorList[DockWidgetTitleBarFloatButtonHover] = QColor(0x2F, 0x2E, 0x2C);
    _lightThemeColorList[DockWidgetTitleBarCloseButtonHover] = QColor(0xC4, 0x2B, 0x1C);
    _darkThemeColorList[DockWidgetTitleBarCloseButtonHover] = QColor(0xC4, 0x2B, 0x1C);
    _lightThemeColorList[DockWidgetFloatBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[DockWidgetFloatBorder] = QColor(0x52, 0x50, 0x52);

    //ElaIconButton
    _lightThemeColorList[IconButtonDefaultHover] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[IconButtonDefaultHover] = QColor(0x2F, 0x2E, 0x2C);
    _lightThemeColorList[IconButtonDefaultIconText] = Qt::black;
    _darkThemeColorList[IconButtonDefaultIconText] = Qt::white;

    //ElaInteractiveCard
    _lightThemeColorList[InteractiveCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[InteractiveCardHover] = QColor(0x27, 0x27, 0x27);

    //ElaLineEdit
    _lightThemeColorList[LineEditMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[LineEditMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[LineEditBorder] = QColor(0xE1, 0xE6, 0xEA);
    _darkThemeColorList[LineEditBorder] = QColor(0x4C, 0x4C, 0x4C);
    _lightThemeColorList[LineEditBase] = QColor(0xFA, 0xFC, 0xFD);
    _darkThemeColorList[LineEditBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[LineEditHover] = QColor(0xF4, 0xF7, 0xF9);
    _darkThemeColorList[LineEditHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[LineEditHasFocus] = QColor(0xFF, 0xFF, 0xFF);
    _darkThemeColorList[LineEditHasFocus] = QColor(0x24, 0x24, 0x24);
    _lightThemeColorList[LineEditHemline] = QColor(0x86, 0x86, 0x86);
    _darkThemeColorList[LineEditHemline] = QColor(0x86, 0x86, 0x86);

    //ElaListView
    _lightThemeColorList[ListViewItemHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ListViewItemHover] = QColor(0x35, 0x35, 0x35);
    _lightThemeColorList[ListViewItemSelected] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ListViewItemSelected] = QColor(0x35, 0x35, 0x35);
    _lightThemeColorList[ListViewItemSelectedHover] = QColor(0xED, 0xEC, 0xF3);
    _darkThemeColorList[ListViewItemSelectedHover] = QColor(0x2F, 0x2F, 0x2F);

    //ElaMenu
    _lightThemeColorList[MenuBaseStart] = QColor(0xF2, 0xE7, 0xF5);
    _darkThemeColorList[MenuBaseStart] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[MenuBaseEnd] = QColor(0xED, 0xF3, 0xF9);
    _darkThemeColorList[MenuBaseEnd] = QColor(0x3D, 0x3D, 0x3D);
    _lightThemeColorList[MenuBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[MenuBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[MenuSeparator] = QColor(0xB3, 0xB3, 0xB3);
    _darkThemeColorList[MenuSeparator] = QColor(0xB3, 0xB3, 0xB3);
    _lightThemeColorList[MenuHover] = QColor(0xE5, 0xE3, 0xE6);
    _darkThemeColorList[MenuHover] = QColor(0x46, 0x46, 0x46);

    //ElaMenuBar
    _lightThemeColorList[MenuBarExpandButtonHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[MenuBarExpandButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[MenuBarItemSelected] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[MenuBarItemSelected] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[MenuBarItemPress] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[MenuBarItemPress] = QColor(0x33, 0x33, 0x33);

    //ElaMessageButton
    _lightThemeColorList[MessageButtonBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[MessageButtonBorder] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[MessageButtonBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[MessageButtonBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[MessageButtonHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[MessageButtonHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[MessageButtonTextPress] = QColor(0x64, 0x66, 0x73);
    _darkThemeColorList[MessageButtonTextPress] = QColor(0xA1, 0xA2, 0xA2);

    //ElaNavigationBar
    _lightThemeColorList[NavigationBaseStart] = QColor(0xF3, 0xF2, 0xF9);
    _darkThemeColorList[NavigationBaseStart] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[NavigationBaseEnd] = QColor(0xF4, 0xF1, 0xF8);
    _darkThemeColorList[NavigationBaseEnd] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[NavigationHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[NavigationHover] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[NavigationSelected] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[NavigationSelected] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[NavigationSelectedHover] = QColor(0xED, 0xEC, 0xF3);
    _darkThemeColorList[NavigationSelectedHover] = QColor(0x28, 0x27, 0x33);
    _lightThemeColorList[NavigationExpanderNodeKeyPoint] = QColor(0xFF, 0x4D, 0x4F);
    _darkThemeColorList[NavigationExpanderNodeKeyPoint] = QColor(0xFF, 0x4D, 0x4F);
    _lightThemeColorList[NavigationKeyPointBase] = Qt::white;
    _darkThemeColorList[NavigationKeyPointBase] = Qt::white;
    _lightThemeColorList[NavigationKeyPointCenter] = QColor(0xFF, 0x4D, 0x4F);
    _darkThemeColorList[NavigationKeyPointCenter] = QColor(0xFF, 0x4D, 0x4F);
    _lightThemeColorList[NavigationKeyPointText] = Qt::white;
    _darkThemeColorList[NavigationKeyPointText] = Qt::white;
    _lightThemeColorList[NavigationMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[NavigationMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[NavigationFooterBaseLine] = QColor(0xD2, 0xD2, 0xD2);
    _darkThemeColorList[NavigationFooterBaseLine] = QColor(0x50, 0x50, 0x50);

    //ElaPopularCard
    _lightThemeColorList[PopularCardBorder] = QColor(0xEA, 0xEA, 0xED);
    _darkThemeColorList[PopularCardBorder] = QColor(0x26, 0x24, 0x2A);
    _lightThemeColorList[PopularCardFloaterBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[PopularCardFloaterBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[PopularCardBase] = QColor(0xFD, 0xFD, 0xFE);
    _darkThemeColorList[PopularCardBase] = QColor(0x34, 0x30, 0x39);
    _lightThemeColorList[PopularCardInteractiveTipsBase] = QColor(0xE6, 0xE6, 0xE6);
    _darkThemeColorList[PopularCardInteractiveTipsBase] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[PopularCardInteractiveTips] = Qt::black;
    _darkThemeColorList[PopularCardInteractiveTips] = Qt::white;
    _lightThemeColorList[PopularCardFloaterBaseLine] = QColor(0xF1, 0xF1, 0xF1);
    _darkThemeColorList[PopularCardFloaterBaseLine] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[PopularCardDetailedText] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[PopularCardDetailedText] = QColor(0xAD, 0xAD, 0xB0);

    //ElaProgressBar
    _lightThemeColorList[ProgressBarBase] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[ProgressBarBase] = QColor(0x63, 0x63, 0x63);
    _lightThemeColorList[ProgressBarContentsBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ProgressBarContentsBase] = QColor(0x4C, 0xA0, 0xE0);

    //ElaPromotionCard
    _lightThemeColorList[PromotionCardDefaultHighlightStart] = QColor(0xFF, 0xFF, 0xFF, 40);
    _darkThemeColorList[PromotionCardDefaultHighlightStart] = QColor(0xFF, 0xFF, 0xFF, 40);
    _lightThemeColorList[PromotionCardDefaultHighlightEnd] = QColor(0xFF, 0xFF, 0xFF, 0);
    _darkThemeColorList[PromotionCardDefaultHighlightEnd] = QColor(0xFF, 0xFF, 0xFF, 0);

    //ElaPromotionView
    _lightThemeColorList[PromotionViewIndicator] = QColor(0x75, 0x7C, 0x87);
    _darkThemeColorList[PromotionViewIndicator] = QColor(0x75, 0x7C, 0x87);

    //ElaPushButton
    _lightThemeColorList[PushButtonBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[PushButtonBorder] = Qt::transparent;
    _lightThemeColorList[PushButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _darkThemeColorList[PushButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _lightThemeColorList[PushButtonDefaultBase] = QColor(0xFE, 0xFE, 0xFE);
    _darkThemeColorList[PushButtonDefaultBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[PushButtonDefaultHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[PushButtonDefaultHover] = QColor(0x4F, 0x4F, 0x4F);
    _lightThemeColorList[PushButtonDefaultPress] = QColor(0xF2, 0xF2, 0xF2);
    _darkThemeColorList[PushButtonDefaultPress] = QColor(0x1C, 0x1C, 0x1C);
    _lightThemeColorList[PushButtonDefaultText] = Qt::black;
    _darkThemeColorList[PushButtonDefaultText] = Qt::white;

    //ElaRadioButton
    _lightThemeColorList[RadioButtonStateOffBorder] = QColor(0x94, 0x94, 0x94);
    _darkThemeColorList[RadioButtonStateOffBorder] = QColor(0x94, 0x94, 0x94);
    _lightThemeColorList[RadioButtonStateOffBase] = Qt::white;
    _darkThemeColorList[RadioButtonStateOffBase] = Qt::white;
    _lightThemeColorList[RadioButtonStateOffHover] = QColor(0xDE, 0xDE, 0xDE);
    _darkThemeColorList[RadioButtonStateOffHover] = QColor(0xDE, 0xDE, 0xDE);
    _lightThemeColorList[RadioButtonStateOnBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[RadioButtonStateOnBorder] = Qt::transparent;
    _lightThemeColorList[RadioButtonStateOnBorder] = Qt::transparent;
    _darkThemeColorList[RadioButtonStateOnBase] = QColor(0x00, 0x66, 0xB4);
    _lightThemeColorList[RadioButtonStateOnCenter] = QColor(0xEA, 0xEA, 0xEB);
    _darkThemeColorList[RadioButtonStateOnCenter] = QColor(0xEA, 0xEA, 0xEB);

    //ElaReminderCard
    _lightThemeColorList[ReminderCardBorder] = Qt::transparent;
    _darkThemeColorList[ReminderCardBorder] = QColor(0x43, 0x43, 0x43);
    _lightThemeColorList[ReminderCardBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[ReminderCardBase] = QColor(0x23, 0x22, 0x24);
    _lightThemeColorList[ReminderCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ReminderCardHover] = QColor(0x47, 0x46, 0x47);
    _lightThemeColorList[ReminderCardMark] = QColor(0x00, 0x67, 0xC0);
    _darkThemeColorList[ReminderCardMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ReminderCardSubTitleText] = Qt::black;
    _darkThemeColorList[ReminderCardSubTitleText] = QColor(0x79, 0x72, 0x68);

    //ElaScrollBar
    _lightThemeColorList[ScrollBarHandle] = QColor(0x8A, 0x84, 0x89);
    _darkThemeColorList[ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    //ElaScrollPageArea
    _lightThemeColorList[ScrollPageAreaBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ScrollPageAreaBorder] = QColor(0x37, 0x37, 0x37);
    _lightThemeColorList[ScrollPageAreaBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[ScrollPageAreaBase] = QColor(0x23, 0x22, 0x24);

    //ElaSlider
    _lightThemeColorList[SliderBaseNoValue] = QColor(0x8A, 0x8A, 0x8A);
    _darkThemeColorList[SliderBaseNoValue] = QColor(0x8A, 0x8A, 0x8A);
    _lightThemeColorList[SliderBaseHasValue] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[SliderBaseHasValue] = QColor(0x00, 0x66, 0xB4);
    _lightThemeColorList[SliderHandleBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[SliderHandleBase] = QColor(0x00, 0x66, 0xB4);
    _lightThemeColorList[SliderHandleCenter] = QColor(0xEA, 0xEA, 0xEB);
    _darkThemeColorList[SliderHandleCenter] = QColor(0xEA, 0xEA, 0xEB);

    //ElaSpinBox ElaDoubleSpinBox
    _lightThemeColorList[SpinBoxBase] = Qt::white;
    _darkThemeColorList[SpinBoxBase] = QColor(0x24, 0x24, 0x24);
    _lightThemeColorList[SpinBoxButtonBase] = QColor(0xE8, 0xE8, 0xE8);
    _darkThemeColorList[SpinBoxButtonBase] = QColor(0x38, 0x38, 0x38);
    _lightThemeColorList[SpinBoxButtonHover] = QColor(0xE0, 0xE0, 0xE0);
    _darkThemeColorList[SpinBoxButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[SpinBoxButtonPress] = QColor(0xD8, 0xD8, 0xD8);
    _darkThemeColorList[SpinBoxButtonPress] = QColor(0x48, 0x48, 0x48);
    _lightThemeColorList[SpinBoxMarkNoFocus] = QColor(0xB7, 0xB7, 0xB7);
    _darkThemeColorList[SpinBoxMarkNoFocus] = QColor(0xA6, 0xA6, 0xA6);
    _lightThemeColorList[SpinBoxMarkHasFocus] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[SpinBoxMarkHasFocus] = QColor(0x4C, 0xA0, 0xE0);

    //ElaStatusBar
    _lightThemeColorList[StatusBarBase] = QColor(0xFC, 0xF7, 0xFC);
    _darkThemeColorList[StatusBarBase] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[StatusBarBorder] = QColor(0xE8, 0xE2, 0xEA);
    _darkThemeColorList[StatusBarBorder] = Qt::transparent;
    _lightThemeColorList[StatusBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[StatusBarSeparator] = QColor(0x4C, 0xA0, 0xE0);

    //ElaSuggestBox
    _lightThemeColorList[SuggestBoxViewItemHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[SuggestBoxViewItemHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[SuggestBoxViewItemSelected] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[SuggestBoxViewItemSelected] = QColor(0x35, 0x35, 0x35);
    _lightThemeColorList[SuggestBoxViewItemSelectedHover] = QColor(0xED, 0xEC, 0xF3);
    _darkThemeColorList[SuggestBoxViewItemSelectedHover] = QColor(0x2F, 0x2F, 0x2F);
    _lightThemeColorList[SuggestBoxSearchViewContainerBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[SuggestBoxSearchViewContainerBorder] = QColor(0x37, 0x37, 0x37);
    _lightThemeColorList[SuggestBoxSearchViewContainerBase] = QColor(0xF6, 0xF6, 0xFA);
    _darkThemeColorList[SuggestBoxSearchViewContainerBase] = QColor(0x32, 0x32, 0x32);

    //ElaTabBar ElaTabWidget
    _lightThemeColorList[TabBarCloseButtonHover] = QColor(0xDC, 0xDC, 0xDD);
    _darkThemeColorList[TabBarCloseButtonHover] = QColor(0x5C, 0x5A, 0x56);
    _lightThemeColorList[TabBarTabBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[TabBarTabBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[TabBarTabHover] = QColor(0xFC, 0xF8, 0xFF);
    _darkThemeColorList[TabBarTabHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[TabBarTabSelected] = Qt::white;
    _darkThemeColorList[TabBarTabSelected] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[TabBarTabMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[TabBarTabMark] = QColor(0x4C, 0xA0, 0xE0);

    //ElaToggleButton
    _lightThemeColorList[ToggleButtonNoToggledBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ToggleButtonNoToggledBorder] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[ToggleButtonNoToggledBase] = QColor(0xFE, 0xFE, 0xFE);
    _darkThemeColorList[ToggleButtonNoToggledBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ToggleButtonNoToggledPress] = QColor(0xF2, 0xF2, 0xF2);
    _darkThemeColorList[ToggleButtonNoToggledPress] = QColor(0x4D, 0x4D, 0x4D);
    _lightThemeColorList[ToggleButtonNoToggledHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ToggleButtonNoToggledHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ToggleButtonNoToggledText] = QColor(0x45, 0x44, 0x41);
    _darkThemeColorList[ToggleButtonNoToggledText] = QColor(0xFE, 0xFE, 0xFE);
    _lightThemeColorList[ToggleButtonToggledBorder] = Qt::transparent;
    _darkThemeColorList[ToggleButtonToggledBorder] = Qt::transparent;
    _lightThemeColorList[ToggleButtonToggledBase] = QColor(0x00, 0x66, 0xB9);
    _darkThemeColorList[ToggleButtonToggledBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ToggleButtonToggledPress] = QColor(0x00, 0x7A, 0xD8);
    _darkThemeColorList[ToggleButtonToggledPress] = QColor(0x00, 0x7A, 0xD8);
    _lightThemeColorList[ToggleButtonToggledHover] = QColor(0x00, 0x70, 0xC6);
    _darkThemeColorList[ToggleButtonToggledHover] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ToggleButtonToggledText] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ToggleButtonToggledText] = QColor(0x01, 0x01, 0x02);
    _lightThemeColorList[ToggleButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _darkThemeColorList[ToggleButtonHemline] = QColor(0x52, 0x52, 0x52);

    //ElaToggleSwitch
    _lightThemeColorList[ToggleSwitchNoToggledBorder] = QColor(0xA5, 0xA5, 0xA7);
    _darkThemeColorList[ToggleSwitchNoToggledBorder] = QColor(0xA1, 0xA1, 0xA1);
    _lightThemeColorList[ToggleSwitchNoToggledBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ToggleSwitchNoToggledBase] = QColor(0x32, 0x32, 0x32);
    _lightThemeColorList[ToggleSwitchNoToggledHover] = QColor(0xF0, 0xF0, 0xF0);
    _darkThemeColorList[ToggleSwitchNoToggledHover] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ToggleSwitchNoToggledCenter] = QColor(0x5D, 0x5D, 0x60);
    _darkThemeColorList[ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);
    _lightThemeColorList[ToggleSwitchToggledBorder] = QColor(0x00, 0x64, 0xB0);
    _darkThemeColorList[ToggleSwitchToggledBorder] = Qt::transparent;
    _lightThemeColorList[ToggleSwitchToggledBase] = QColor(0x00, 0x64, 0xB0);
    _darkThemeColorList[ToggleSwitchToggledBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ToggleSwitchToggledCenter] = QColor(0xFD, 0xFF, 0xF3);
    _darkThemeColorList[ToggleSwitchToggledCenter] = QColor(0x00, 0x00, 0x00);

    //ElaToolBar
    _lightThemeColorList[ToolBarExpandButtonHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ToolBarExpandButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ToolBarWindowBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ToolBarWindowBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ToolBarWindowBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[ToolBarWindowBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ToolBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ToolBarSeparator] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ToolBarHandle] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ToolBarHandle] = QColor(0x4C, 0xA0, 0xE0);

    //ElaToolButton
    _lightThemeColorList[ToolButtonHover] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ToolButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ToolButtonPress] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ToolButtonPress] = QColor(0x33, 0x33, 0x33);
    _lightThemeColorList[ToolButtonIndicator] = QColor(0xE0, 0xE0, 0xE0);
    _darkThemeColorList[ToolButtonIndicator] = QColor(0x40, 0x40, 0x40);

    //ElaWindow
    _lightThemeColorList[WindowText] = Qt::black;
    _darkThemeColorList[WindowText] = Qt::white;
    _lightThemeColorList[WindowTextDisable] = Qt::gray;
    _darkThemeColorList[WindowTextDisable] = Qt::gray;
    _lightThemeColorList[WindowBaseStart] = QColor(0xF2, 0xF2, 0xF9);
    _darkThemeColorList[WindowBaseStart] = QColor(0x1F, 0x1F, 0x2B);
    _lightThemeColorList[WindowBaseEnd] = QColor(0xF4, 0xF1, 0xF9);
    _darkThemeColorList[WindowBaseEnd] = QColor(0x1F, 0x1F, 0x29);
    _lightThemeColorList[WindowBaseLine] = QColor(0xE4, 0xE4, 0xEA);
    _darkThemeColorList[WindowBaseLine] = QColor(0x26, 0x2B, 0x2F);
    _lightThemeColorList[WindowDockWidgetResizeHandle] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[WindowDockWidgetResizeHandle] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[WindowDockWidgetResizeHandleHover] = QColor(0x6F, 0xAA, 0xDE);
    _darkThemeColorList[WindowDockWidgetResizeHandleHover] = QColor(0x6F, 0xAA, 0xDE);
    _lightThemeColorList[WindowRubberBand] = QColor(0xFF, 0xFF, 0xFF, 128);
    _darkThemeColorList[WindowRubberBand] = QColor(0x85, 0x85, 0x85, 128);
    _lightThemeColorList[WindowTabBarBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[WindowTabBarBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[WindowTabBarHover] = QColor(0xFC, 0xF8, 0xFF);
    _darkThemeColorList[WindowTabBarHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[WindowTabBarSelected] = Qt::white;
    _darkThemeColorList[WindowTabBarSelected] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[WindowTabBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[WindowTabBarSeparator] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[WindowCentralStackBaseStart] = QColor(0xF9, 0xF9, 0xFC);
    _darkThemeColorList[WindowCentralStackBaseStart] = QColor(0x24, 0x29, 0x2D);
    _lightThemeColorList[WindowCentralStackBaseEnd] = QColor(0xFA, 0xF8, 0xFC);
    _darkThemeColorList[WindowCentralStackBaseEnd] = QColor(0x2E, 0x26, 0x33);
}
