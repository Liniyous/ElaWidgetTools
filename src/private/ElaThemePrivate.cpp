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
    //ElaAcrylicUrlCard
    _lightThemeColorList[ElaThemeType::AcrylicUrlCardBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::AcrylicUrlCardBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::AcrylicUrlCardBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[ElaThemeType::AcrylicUrlCardBase] = QColor(0x04, 0x09, 0x11);
    _lightThemeColorList[ElaThemeType::AcrylicUrlCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::AcrylicUrlCardHover] = QColor(0x17, 0x17, 0x17);
    _lightThemeColorList[ElaThemeType::AcrylicUrlCardSubTitleText] = Qt::black;
    _darkThemeColorList[ElaThemeType::AcrylicUrlCardSubTitleText] = QColor(0x79, 0x72, 0x68);

    //ElaBreadcrumbBar
    _lightThemeColorList[ElaThemeType::BreadcrumbBarText] = QColor(0x5B, 0x5B, 0x5E);
    _darkThemeColorList[ElaThemeType::BreadcrumbBarText] = QColor(0xDE, 0xDE, 0xDE);
    _lightThemeColorList[ElaThemeType::BreadcrumbBarTextHasFocus] = QColor(0x86, 0x86, 0x8A);
    _darkThemeColorList[ElaThemeType::BreadcrumbBarTextHasFocus] = QColor(0x86, 0x86, 0x8A);

    //ElaCheckBox
    _lightThemeColorList[ElaThemeType::CheckBoxBorder] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[ElaThemeType::CheckBoxBorder] = QColor(0xA0, 0xA0, 0xA0);
    _lightThemeColorList[ElaThemeType::CheckBoxCheckedBorder] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::CheckBoxCheckedBorder] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::CheckBoxPressBorder] = QColor(0xBF, 0xBF, 0xBF);
    _darkThemeColorList[ElaThemeType::CheckBoxPressBorder] = QColor(0x5A, 0x5A, 0x5A);
    _lightThemeColorList[ElaThemeType::CheckBoxBase] = QColor(0xF7, 0xF7, 0xF7);
    _darkThemeColorList[ElaThemeType::CheckBoxBase] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::CheckBoxHover] = QColor(0xEC, 0xEC, 0xEC);
    _darkThemeColorList[ElaThemeType::CheckBoxHover] = QColor(0x48, 0x48, 0x48);
    _lightThemeColorList[ElaThemeType::CheckBoxCheckedBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::CheckBoxCheckedBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::CheckBoxCheckedHover] = QColor(0x00, 0x75, 0xCF);
    _darkThemeColorList[ElaThemeType::CheckBoxCheckedHover] = QColor(0x42, 0x8B, 0xC3);
    _lightThemeColorList[ElaThemeType::CheckBoxCheckedPress] = QColor(0x00, 0x85, 0xEA);
    _darkThemeColorList[ElaThemeType::CheckBoxCheckedPress] = QColor(0x3A, 0x7B, 0xAC);

    //ElaComboBox ElaMultiComboBoxs
    _lightThemeColorList[ElaThemeType::ComboBoxMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::ComboBoxMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ComboBoxBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::ComboBoxBorder] = QColor(0x4B, 0x4B, 0x4D);
    _lightThemeColorList[ElaThemeType::ComboBoxBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ElaThemeType::ComboBoxBase] = QColor(0x3B, 0x3B, 0x3B);
    _lightThemeColorList[ElaThemeType::ComboBoxHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ElaThemeType::ComboBoxHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ElaThemeType::ComboBoxShapedFrameBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::ComboBoxShapedFrameBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::ComboBoxShapedFrameBase] = QColor(0xF9, 0xF9, 0xF9);
    _darkThemeColorList[ElaThemeType::ComboBoxShapedFrameBase] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::ComboBoxItemViewItemHover] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::ComboBoxItemViewItemHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::ComboBoxItemViewItemSelected] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::ComboBoxItemViewItemSelected] = QColor(0x4F, 0x4F, 0x4F);
    _lightThemeColorList[ElaThemeType::ComboBoxItemViewItemSelectedHover] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::ComboBoxItemViewItemSelectedHover] = QColor(0x40, 0x40, 0x40);

    //ElaContentDialog
    _lightThemeColorList[ElaThemeType::ContentDialogBase] = Qt::white;
    _darkThemeColorList[ElaThemeType::ContentDialogBase] = QColor(0x2B, 0x2B, 0x2B);
    _lightThemeColorList[ElaThemeType::ContentDialogButtonAreaBase] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[ElaThemeType::ContentDialogButtonAreaBase] = QColor(0x20, 0x20, 0x20);
    _lightThemeColorList[ElaThemeType::ContentDialogRightButtonBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::ContentDialogRightButtonBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ContentDialogRightButtonText] = Qt::white;
    _darkThemeColorList[ElaThemeType::ContentDialogRightButtonText] = Qt::white;
    _lightThemeColorList[ElaThemeType::ContentDialogRightButtonHover] = QColor(0x00, 0x70, 0xC6);
    _darkThemeColorList[ElaThemeType::ContentDialogRightButtonHover] = QColor(0x45, 0x91, 0xCC);
    _lightThemeColorList[ElaThemeType::ContentDialogRightButtonPress] = QColor(0x00, 0x7A, 0xD8);
    _darkThemeColorList[ElaThemeType::ContentDialogRightButtonPress] = QColor(0x3F, 0x85, 0xBB);

    //ElaDockWidget
    _lightThemeColorList[ElaThemeType::DockWidgetBaseStart] = QColor(0xF3, 0xF2, 0xF9);
    _darkThemeColorList[ElaThemeType::DockWidgetBaseStart] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::DockWidgetBaseEnd] = QColor(0xF4, 0xF1, 0xF8);
    _darkThemeColorList[ElaThemeType::DockWidgetBaseEnd] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::DockWidgetTitleBarBase] = QColor(0xF5, 0xF1, 0xFF);
    _darkThemeColorList[ElaThemeType::DockWidgetTitleBarBase] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::DockWidgetTitleBarFloatButtonHover] = QColor(0xEF, 0xE6, 0xED);
    _darkThemeColorList[ElaThemeType::DockWidgetTitleBarFloatButtonHover] = QColor(0x2F, 0x2E, 0x2C);
    _lightThemeColorList[ElaThemeType::DockWidgetTitleBarCloseButtonHover] = QColor(0xC4, 0x2B, 0x1C);
    _darkThemeColorList[ElaThemeType::DockWidgetTitleBarCloseButtonHover] = QColor(0xC4, 0x2B, 0x1C);
    _lightThemeColorList[ElaThemeType::DockWidgetFloatBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::DockWidgetFloatBorder] = QColor(0x52, 0x50, 0x52);

    //ElaIconButton
    _lightThemeColorList[ElaThemeType::IconButtonDefaultHover] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::IconButtonDefaultHover] = QColor(0x2F, 0x2E, 0x2C);
    _lightThemeColorList[ElaThemeType::IconButtonDefaultIconText] = Qt::black;
    _darkThemeColorList[ElaThemeType::IconButtonDefaultIconText] = Qt::white;

    //ElaInteractiveCard
    _lightThemeColorList[ElaThemeType::InteractiveCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::InteractiveCardHover] = QColor(0x27, 0x27, 0x27);

    //ElaLineEdit
    _lightThemeColorList[ElaThemeType::LineEditMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::LineEditMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::LineEditBorder] = QColor(0xE1, 0xE6, 0xEA);
    _darkThemeColorList[ElaThemeType::LineEditBorder] = QColor(0x4C, 0x4C, 0x4C);
    _lightThemeColorList[ElaThemeType::LineEditBase] = QColor(0xFA, 0xFC, 0xFD);
    _darkThemeColorList[ElaThemeType::LineEditBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ElaThemeType::LineEditHover] = QColor(0xF4, 0xF7, 0xF9);
    _darkThemeColorList[ElaThemeType::LineEditHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ElaThemeType::LineEditHasFocus] = QColor(0xFF, 0xFF, 0xFF);
    _darkThemeColorList[ElaThemeType::LineEditHasFocus] = QColor(0x24, 0x24, 0x24);
    _lightThemeColorList[ElaThemeType::LineEditHemline] = QColor(0x86, 0x86, 0x86);
    _darkThemeColorList[ElaThemeType::LineEditHemline] = QColor(0x86, 0x86, 0x86);

    //ElaListView
    _lightThemeColorList[ElaThemeType::ListViewBase] = QColor(0xF0, 0xF0, 0xF5);
    _darkThemeColorList[ElaThemeType::ListViewBase] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::ListViewBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::ListViewBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::ListViewItemHover] = QColor(0xE4, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::ListViewItemHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::ListViewItemSelected] = QColor(0xE4, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::ListViewItemSelected] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::ListViewItemSelectedHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::ListViewItemSelectedHover] = QColor(0x3A, 0x3A, 0x3A);

    //ElaMenu
    _lightThemeColorList[ElaThemeType::MenuBaseStart] = QColor(0xF2, 0xE7, 0xF5);
    _darkThemeColorList[ElaThemeType::MenuBaseStart] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::MenuBaseEnd] = QColor(0xED, 0xF3, 0xF9);
    _darkThemeColorList[ElaThemeType::MenuBaseEnd] = QColor(0x3D, 0x3D, 0x3D);
    _lightThemeColorList[ElaThemeType::MenuBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::MenuBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::MenuSeparator] = QColor(0xB3, 0xB3, 0xB3);
    _darkThemeColorList[ElaThemeType::MenuSeparator] = QColor(0xB3, 0xB3, 0xB3);
    _lightThemeColorList[ElaThemeType::MenuHover] = QColor(0xE5, 0xE3, 0xE6);
    _darkThemeColorList[ElaThemeType::MenuHover] = QColor(0x46, 0x46, 0x46);

    //ElaMenuBar
    _lightThemeColorList[ElaThemeType::MenuBarExpandButtonHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::MenuBarExpandButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::MenuBarItemSelected] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::MenuBarItemSelected] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::MenuBarItemPress] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::MenuBarItemPress] = QColor(0x33, 0x33, 0x33);

    //ElaMessageButton
    _lightThemeColorList[ElaThemeType::MessageButtonBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::MessageButtonBorder] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[ElaThemeType::MessageButtonBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ElaThemeType::MessageButtonBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ElaThemeType::MessageButtonHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ElaThemeType::MessageButtonHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ElaThemeType::MessageButtonTextPress] = QColor(0x64, 0x66, 0x73);
    _darkThemeColorList[ElaThemeType::MessageButtonTextPress] = QColor(0xA1, 0xA2, 0xA2);

    //ElaNavigationBar
    _lightThemeColorList[ElaThemeType::NavigationBaseStart] = QColor(0xF3, 0xF2, 0xF9);
    _darkThemeColorList[ElaThemeType::NavigationBaseStart] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::NavigationBaseEnd] = QColor(0xF4, 0xF1, 0xF8);
    _darkThemeColorList[ElaThemeType::NavigationBaseEnd] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::NavigationHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::NavigationHover] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[ElaThemeType::NavigationSelected] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::NavigationSelected] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[ElaThemeType::NavigationSelectedHover] = QColor(0xEC, 0xEC, 0xF3);
    _darkThemeColorList[ElaThemeType::NavigationSelectedHover] = QColor(0x28, 0x27, 0x33);
    _lightThemeColorList[ElaThemeType::NavigationExpanderNodeKeyPoint] = QColor(0xFF, 0x4D, 0x4F);
    _darkThemeColorList[ElaThemeType::NavigationExpanderNodeKeyPoint] = QColor(0xFF, 0x4D, 0x4F);
    _lightThemeColorList[ElaThemeType::NavigationKeyPointBase] = Qt::white;
    _darkThemeColorList[ElaThemeType::NavigationKeyPointBase] = Qt::white;
    _lightThemeColorList[ElaThemeType::NavigationKeyPointCenter] = QColor(0xFF, 0x4D, 0x4F);
    _darkThemeColorList[ElaThemeType::NavigationKeyPointCenter] = QColor(0xFF, 0x4D, 0x4F);
    _lightThemeColorList[ElaThemeType::NavigationKeyPointText] = Qt::white;
    _darkThemeColorList[ElaThemeType::NavigationKeyPointText] = Qt::white;
    _lightThemeColorList[ElaThemeType::NavigationMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::NavigationMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::NavigationFooterBaseLine] = QColor(0xD2, 0xD2, 0xD2);
    _darkThemeColorList[ElaThemeType::NavigationFooterBaseLine] = QColor(0x50, 0x50, 0x50);

    //ElaPopularCard
    _lightThemeColorList[ElaThemeType::PopularCardBorder] = QColor(0xEA, 0xEA, 0xED);
    _darkThemeColorList[ElaThemeType::PopularCardBorder] = QColor(0x26, 0x24, 0x2A);
    _lightThemeColorList[ElaThemeType::PopularCardFloaterBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::PopularCardFloaterBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::PopularCardBase] = QColor(0xFD, 0xFD, 0xFE);
    _darkThemeColorList[ElaThemeType::PopularCardBase] = QColor(0x34, 0x30, 0x39);
    _lightThemeColorList[ElaThemeType::PopularCardInteractiveTipsBase] = QColor(0xE6, 0xE6, 0xE6);
    _darkThemeColorList[ElaThemeType::PopularCardInteractiveTipsBase] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[ElaThemeType::PopularCardInteractiveTips] = Qt::black;
    _darkThemeColorList[ElaThemeType::PopularCardInteractiveTips] = Qt::white;
    _lightThemeColorList[ElaThemeType::PopularCardFloaterBaseLine] = QColor(0xF1, 0xF1, 0xF1);
    _darkThemeColorList[ElaThemeType::PopularCardFloaterBaseLine] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[ElaThemeType::PopularCardDetailedText] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[ElaThemeType::PopularCardDetailedText] = QColor(0xAD, 0xAD, 0xB0);

    //ElaProgressBar
    _lightThemeColorList[ElaThemeType::ProgressBarBase] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[ElaThemeType::ProgressBarBase] = QColor(0x63, 0x63, 0x63);
    _lightThemeColorList[ElaThemeType::ProgressBarContentsBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::ProgressBarContentsBase] = QColor(0x4C, 0xA0, 0xE0);

    //ElaPromotionCard
    _lightThemeColorList[ElaThemeType::PromotionCardDefaultHighlightStart] = QColor(0xFF, 0xFF, 0xFF, 40);
    _darkThemeColorList[ElaThemeType::PromotionCardDefaultHighlightStart] = QColor(0xFF, 0xFF, 0xFF, 40);
    _lightThemeColorList[ElaThemeType::PromotionCardDefaultHighlightEnd] = QColor(0xFF, 0xFF, 0xFF, 0);
    _darkThemeColorList[ElaThemeType::PromotionCardDefaultHighlightEnd] = QColor(0xFF, 0xFF, 0xFF, 0);

    //ElaPromotionView
    _lightThemeColorList[ElaThemeType::PromotionViewIndicator] = QColor(0x75, 0x7C, 0x87);
    _darkThemeColorList[ElaThemeType::PromotionViewIndicator] = QColor(0x75, 0x7C, 0x87);

    //ElaPushButton
    _lightThemeColorList[ElaThemeType::PushButtonBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::PushButtonBorder] = Qt::transparent;
    _lightThemeColorList[ElaThemeType::PushButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _darkThemeColorList[ElaThemeType::PushButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _lightThemeColorList[ElaThemeType::PushButtonDefaultBase] = QColor(0xFE, 0xFE, 0xFE);
    _darkThemeColorList[ElaThemeType::PushButtonDefaultBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ElaThemeType::PushButtonDefaultHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ElaThemeType::PushButtonDefaultHover] = QColor(0x4F, 0x4F, 0x4F);
    _lightThemeColorList[ElaThemeType::PushButtonDefaultPress] = QColor(0xF2, 0xF2, 0xF2);
    _darkThemeColorList[ElaThemeType::PushButtonDefaultPress] = QColor(0x1C, 0x1C, 0x1C);
    _lightThemeColorList[ElaThemeType::PushButtonDefaultText] = Qt::black;
    _darkThemeColorList[ElaThemeType::PushButtonDefaultText] = Qt::white;

    //ElaRadioButton
    _lightThemeColorList[ElaThemeType::RadioButtonStateOffBorder] = QColor(0x94, 0x94, 0x94);
    _darkThemeColorList[ElaThemeType::RadioButtonStateOffBorder] = QColor(0x94, 0x94, 0x94);
    _lightThemeColorList[ElaThemeType::RadioButtonStateOffBase] = Qt::white;
    _darkThemeColorList[ElaThemeType::RadioButtonStateOffBase] = Qt::white;
    _lightThemeColorList[ElaThemeType::RadioButtonStateOffHover] = QColor(0xDE, 0xDE, 0xDE);
    _darkThemeColorList[ElaThemeType::RadioButtonStateOffHover] = QColor(0xDE, 0xDE, 0xDE);
    _lightThemeColorList[ElaThemeType::RadioButtonStateOnBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::RadioButtonStateOnBorder] = Qt::transparent;
    _lightThemeColorList[ElaThemeType::RadioButtonStateOnBorder] = Qt::transparent;
    _darkThemeColorList[ElaThemeType::RadioButtonStateOnBase] = QColor(0x00, 0x66, 0xB4);
    _lightThemeColorList[ElaThemeType::RadioButtonStateOnCenter] = QColor(0xEA, 0xEA, 0xEB);
    _darkThemeColorList[ElaThemeType::RadioButtonStateOnCenter] = QColor(0xEA, 0xEA, 0xEB);

    //ElaReminderCard
    _lightThemeColorList[ElaThemeType::ReminderCardBorder] = Qt::transparent;
    _darkThemeColorList[ElaThemeType::ReminderCardBorder] = QColor(0x43, 0x43, 0x43);
    _lightThemeColorList[ElaThemeType::ReminderCardBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[ElaThemeType::ReminderCardBase] = QColor(0x23, 0x22, 0x24);
    _lightThemeColorList[ElaThemeType::ReminderCardHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::ReminderCardHover] = QColor(0x47, 0x46, 0x47);
    _lightThemeColorList[ElaThemeType::ReminderCardMark] = QColor(0x00, 0x67, 0xC0);
    _darkThemeColorList[ElaThemeType::ReminderCardMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ReminderCardSubTitleText] = Qt::black;
    _darkThemeColorList[ElaThemeType::ReminderCardSubTitleText] = QColor(0x79, 0x72, 0x68);

    //ElaScrollBar
    _lightThemeColorList[ElaThemeType::ScrollBarBase] = QColor(0xF1, 0xF2, 0xF3);
    _darkThemeColorList[ElaThemeType::ScrollBarBase] = QColor(0x38, 0x38, 0x38);
    _lightThemeColorList[ElaThemeType::ScrollBarHandle] = QColor(0x8A, 0x84, 0x89);
    _darkThemeColorList[ElaThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    //ElaScrollPageArea
    _lightThemeColorList[ElaThemeType::ScrollPageAreaBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::ScrollPageAreaBorder] = QColor(0x37, 0x37, 0x37);
    _lightThemeColorList[ElaThemeType::ScrollPageAreaBase] = QColor(0xFB, 0xFB, 0xFD);
    _darkThemeColorList[ElaThemeType::ScrollPageAreaBase] = QColor(0x23, 0x22, 0x24);

    //ElaSlider
    _lightThemeColorList[ElaThemeType::SliderBaseNoValue] = QColor(0xCF, 0xCF, 0xCF);
    _darkThemeColorList[ElaThemeType::SliderBaseNoValue] = QColor(0x63, 0x63, 0x63);
    _lightThemeColorList[ElaThemeType::SliderBaseHasValue] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::SliderBaseHasValue] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::SliderHandleBase] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::SliderHandleBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::SliderHandleCenter] = QColor(0xEA, 0xEA, 0xEB);
    _darkThemeColorList[ElaThemeType::SliderHandleCenter] = QColor(0xEA, 0xEA, 0xEB);

    //ElaSpinBox ElaDoubleSpinBox
    _lightThemeColorList[ElaThemeType::SpinBoxBase] = Qt::white;
    _darkThemeColorList[ElaThemeType::SpinBoxBase] = QColor(0x24, 0x24, 0x24);
    _lightThemeColorList[ElaThemeType::SpinBoxButtonBase] = QColor(0xE8, 0xE8, 0xE8);
    _darkThemeColorList[ElaThemeType::SpinBoxButtonBase] = QColor(0x38, 0x38, 0x38);
    _lightThemeColorList[ElaThemeType::SpinBoxButtonHover] = QColor(0xE0, 0xE0, 0xE0);
    _darkThemeColorList[ElaThemeType::SpinBoxButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::SpinBoxButtonPress] = QColor(0xD8, 0xD8, 0xD8);
    _darkThemeColorList[ElaThemeType::SpinBoxButtonPress] = QColor(0x48, 0x48, 0x48);
    _lightThemeColorList[ElaThemeType::SpinBoxMarkNoFocus] = QColor(0xB7, 0xB7, 0xB7);
    _darkThemeColorList[ElaThemeType::SpinBoxMarkNoFocus] = QColor(0xA6, 0xA6, 0xA6);
    _lightThemeColorList[ElaThemeType::SpinBoxMarkHasFocus] = QColor(0x00, 0x66, 0xB4);
    _darkThemeColorList[ElaThemeType::SpinBoxMarkHasFocus] = QColor(0x4C, 0xA0, 0xE0);

    //ElaStatusBar
    _lightThemeColorList[ElaThemeType::StatusBarBase] = QColor(0xFC, 0xF7, 0xFC);
    _darkThemeColorList[ElaThemeType::StatusBarBase] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::StatusBarBorder] = QColor(0xE8, 0xE2, 0xEA);
    _darkThemeColorList[ElaThemeType::StatusBarBorder] = Qt::transparent;
    _lightThemeColorList[ElaThemeType::StatusBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::StatusBarSeparator] = QColor(0x4C, 0xA0, 0xE0);

    //ElaSuggestBox
    _lightThemeColorList[ElaThemeType::SuggestBoxViewItemHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::SuggestBoxViewItemHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[ElaThemeType::SuggestBoxViewItemSelected] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::SuggestBoxViewItemSelected] = QColor(0x35, 0x35, 0x35);
    _lightThemeColorList[ElaThemeType::SuggestBoxViewItemSelectedHover] = QColor(0xED, 0xEC, 0xF3);
    _darkThemeColorList[ElaThemeType::SuggestBoxViewItemSelectedHover] = QColor(0x2F, 0x2F, 0x2F);
    _lightThemeColorList[ElaThemeType::SuggestBoxSearchViewContainerBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::SuggestBoxSearchViewContainerBorder] = QColor(0x37, 0x37, 0x37);
    _lightThemeColorList[ElaThemeType::SuggestBoxSearchViewContainerBase] = QColor(0xF6, 0xF6, 0xFA);
    _darkThemeColorList[ElaThemeType::SuggestBoxSearchViewContainerBase] = QColor(0x32, 0x32, 0x32);

    //ElaTabBar ElaTabWidget
    _lightThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0xDC, 0xDC, 0xDD);
    _darkThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0x5C, 0x5A, 0x56);
    _lightThemeColorList[ElaThemeType::TabBarTabBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[ElaThemeType::TabBarTabBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::TabBarTabHover] = QColor(0xFC, 0xF8, 0xFF);
    _darkThemeColorList[ElaThemeType::TabBarTabHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[ElaThemeType::TabBarTabSelected] = Qt::white;
    _darkThemeColorList[ElaThemeType::TabBarTabSelected] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::TabBarTabMark] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::TabBarTabMark] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::TabBarPanelButtonToolHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::TabBarPanelButtonToolHover] = QColor(0x2B, 0x2C, 0x36);
    _lightThemeColorList[ElaThemeType::TabBarPanelButtonToolPress] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::TabBarPanelButtonToolPress] = QColor(0x2B, 0x2C, 0x36);

    //ElaToggleButton
    _lightThemeColorList[ElaThemeType::ToggleButtonNoToggledBorder] = QColor(0xDF, 0xDF, 0xDF);
    _darkThemeColorList[ElaThemeType::ToggleButtonNoToggledBorder] = QColor(0x50, 0x50, 0x50);
    _lightThemeColorList[ElaThemeType::ToggleButtonNoToggledBase] = QColor(0xFE, 0xFE, 0xFE);
    _darkThemeColorList[ElaThemeType::ToggleButtonNoToggledBase] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ElaThemeType::ToggleButtonNoToggledPress] = QColor(0xF2, 0xF2, 0xF2);
    _darkThemeColorList[ElaThemeType::ToggleButtonNoToggledPress] = QColor(0x4D, 0x4D, 0x4D);
    _lightThemeColorList[ElaThemeType::ToggleButtonNoToggledHover] = QColor(0xF6, 0xF6, 0xF6);
    _darkThemeColorList[ElaThemeType::ToggleButtonNoToggledHover] = QColor(0x44, 0x44, 0x44);
    _lightThemeColorList[ElaThemeType::ToggleButtonNoToggledText] = QColor(0x45, 0x44, 0x41);
    _darkThemeColorList[ElaThemeType::ToggleButtonNoToggledText] = QColor(0xFE, 0xFE, 0xFE);
    _lightThemeColorList[ElaThemeType::ToggleButtonToggledBorder] = Qt::transparent;
    _darkThemeColorList[ElaThemeType::ToggleButtonToggledBorder] = Qt::transparent;
    _lightThemeColorList[ElaThemeType::ToggleButtonToggledBase] = QColor(0x00, 0x66, 0xB9);
    _darkThemeColorList[ElaThemeType::ToggleButtonToggledBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ToggleButtonToggledPress] = QColor(0x00, 0x7A, 0xD8);
    _darkThemeColorList[ElaThemeType::ToggleButtonToggledPress] = QColor(0x00, 0x7A, 0xD8);
    _lightThemeColorList[ElaThemeType::ToggleButtonToggledHover] = QColor(0x00, 0x70, 0xC6);
    _darkThemeColorList[ElaThemeType::ToggleButtonToggledHover] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ToggleButtonToggledText] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ElaThemeType::ToggleButtonToggledText] = QColor(0x01, 0x01, 0x02);
    _lightThemeColorList[ElaThemeType::ToggleButtonHemline] = QColor(0xBC, 0xBC, 0xBC);
    _darkThemeColorList[ElaThemeType::ToggleButtonHemline] = QColor(0x52, 0x52, 0x52);

    //ElaToggleSwitch
    _lightThemeColorList[ElaThemeType::ToggleSwitchNoToggledBorder] = QColor(0xA5, 0xA5, 0xA7);
    _darkThemeColorList[ElaThemeType::ToggleSwitchNoToggledBorder] = QColor(0xA1, 0xA1, 0xA1);
    _lightThemeColorList[ElaThemeType::ToggleSwitchNoToggledBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[ElaThemeType::ToggleSwitchNoToggledBase] = QColor(0x32, 0x32, 0x32);
    _lightThemeColorList[ElaThemeType::ToggleSwitchNoToggledHover] = QColor(0xF0, 0xF0, 0xF0);
    _darkThemeColorList[ElaThemeType::ToggleSwitchNoToggledHover] = QColor(0x3E, 0x3E, 0x3E);
    _lightThemeColorList[ElaThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5D, 0x5D, 0x60);
    _darkThemeColorList[ElaThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);
    _lightThemeColorList[ElaThemeType::ToggleSwitchToggledBorder] = QColor(0x00, 0x64, 0xB0);
    _darkThemeColorList[ElaThemeType::ToggleSwitchToggledBorder] = Qt::transparent;
    _lightThemeColorList[ElaThemeType::ToggleSwitchToggledBase] = QColor(0x00, 0x64, 0xB0);
    _darkThemeColorList[ElaThemeType::ToggleSwitchToggledBase] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ToggleSwitchToggledCenter] = QColor(0xFD, 0xFF, 0xF3);
    _darkThemeColorList[ElaThemeType::ToggleSwitchToggledCenter] = QColor(0x00, 0x00, 0x00);

    //ElaToolBar
    _lightThemeColorList[ElaThemeType::ToolBarExpandButtonHover] = QColor(0xE9, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::ToolBarExpandButtonHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::ToolBarWindowBorder] = QColor(0xBE, 0xBA, 0xBE);
    _darkThemeColorList[ElaThemeType::ToolBarWindowBorder] = QColor(0x52, 0x50, 0x52);
    _lightThemeColorList[ElaThemeType::ToolBarWindowBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[ElaThemeType::ToolBarWindowBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::ToolBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::ToolBarSeparator] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::ToolBarHandle] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::ToolBarHandle] = QColor(0x4C, 0xA0, 0xE0);

    //ElaToolButton
    _lightThemeColorList[ElaThemeType::ToolButtonBorder] = QColor(0xD1, 0xD1, 0xD3);
    _darkThemeColorList[ElaThemeType::ToolButtonBorder] = QColor(0x3B, 0x3B, 0x40);
    _lightThemeColorList[ElaThemeType::ToolButtonBase] = QColor(0xFD, 0xFD, 0xFE);
    _darkThemeColorList[ElaThemeType::ToolButtonBase] = QColor(0x33, 0x33, 0x3B);
    _lightThemeColorList[ElaThemeType::ToolButtonHover] = QColor(0xF7, 0xF7, 0xFA);
    _darkThemeColorList[ElaThemeType::ToolButtonHover] = QColor(0x3C, 0x3C, 0x42);
    _lightThemeColorList[ElaThemeType::ToolButtonTransparentHover] = QColor(0xE5, 0xE4, 0xEB);
    _darkThemeColorList[ElaThemeType::ToolButtonTransparentHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[ElaThemeType::ToolButtonPress] = QColor(0xF3, 0xF2, 0xF5);
    _darkThemeColorList[ElaThemeType::ToolButtonPress] = QColor(0x2D, 0x2D, 0x34);
    _lightThemeColorList[ElaThemeType::ToolButtonTransparentPress] = QColor(0xEA, 0xE9, 0xF0);
    _darkThemeColorList[ElaThemeType::ToolButtonTransparentPress] = QColor(0x33, 0x33, 0x33);
    _lightThemeColorList[ElaThemeType::ToolButtonIndicator] = QColor(0xE0, 0xE0, 0xE0);
    _darkThemeColorList[ElaThemeType::ToolButtonIndicator] = QColor(0x40, 0x40, 0x40);

    //ElaWindow
    _lightThemeColorList[ElaThemeType::WindowText] = Qt::black;
    _darkThemeColorList[ElaThemeType::WindowText] = Qt::white;
    _lightThemeColorList[ElaThemeType::WindowTextDisable] = Qt::gray;
    _darkThemeColorList[ElaThemeType::WindowTextDisable] = Qt::gray;
    _lightThemeColorList[ElaThemeType::WindowBaseStart] = QColor(0xF2, 0xF2, 0xF9);
    _darkThemeColorList[ElaThemeType::WindowBaseStart] = QColor(0x1F, 0x1F, 0x2B);
    _lightThemeColorList[ElaThemeType::WindowBaseEnd] = QColor(0xF4, 0xF1, 0xF9);
    _darkThemeColorList[ElaThemeType::WindowBaseEnd] = QColor(0x1F, 0x1F, 0x29);
    _lightThemeColorList[ElaThemeType::WindowBaseLine] = QColor(0xE4, 0xE4, 0xEA);
    _darkThemeColorList[ElaThemeType::WindowBaseLine] = QColor(0x26, 0x2B, 0x2F);
    _lightThemeColorList[ElaThemeType::WindowDockWidgetResizeHandle] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::WindowDockWidgetResizeHandle] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::WindowDockWidgetResizeHandleHover] = QColor(0x6F, 0xAA, 0xDE);
    _darkThemeColorList[ElaThemeType::WindowDockWidgetResizeHandleHover] = QColor(0x6F, 0xAA, 0xDE);
    _lightThemeColorList[ElaThemeType::WindowRubberBand] = QColor(0xFF, 0xFF, 0xFF, 128);
    _darkThemeColorList[ElaThemeType::WindowRubberBand] = QColor(0x85, 0x85, 0x85, 128);
    _lightThemeColorList[ElaThemeType::WindowTabBarBase] = QColor(0xF5, 0xF1, 0xF8);
    _darkThemeColorList[ElaThemeType::WindowTabBarBase] = QColor(0x1A, 0x1A, 0x1A);
    _lightThemeColorList[ElaThemeType::WindowTabBarHover] = QColor(0xFC, 0xF8, 0xFF);
    _darkThemeColorList[ElaThemeType::WindowTabBarHover] = QColor(0x27, 0x27, 0x27);
    _lightThemeColorList[ElaThemeType::WindowTabBarSelected] = Qt::white;
    _darkThemeColorList[ElaThemeType::WindowTabBarSelected] = QColor(0x2D, 0x2D, 0x2D);
    _lightThemeColorList[ElaThemeType::WindowTabBarSeparator] = QColor(0x0E, 0x6F, 0xC3);
    _darkThemeColorList[ElaThemeType::WindowTabBarSeparator] = QColor(0x4C, 0xA0, 0xE0);
    _lightThemeColorList[ElaThemeType::WindowCentralStackBaseStart] = QColor(0xF9, 0xF9, 0xFC);
    _darkThemeColorList[ElaThemeType::WindowCentralStackBaseStart] = QColor(0x24, 0x29, 0x2D);
    _lightThemeColorList[ElaThemeType::WindowCentralStackBaseEnd] = QColor(0xFA, 0xF8, 0xFC);
    _darkThemeColorList[ElaThemeType::WindowCentralStackBaseEnd] = QColor(0x2E, 0x26, 0x33);
}
