#include "ElaControlStyle.h"

#include "ElaColorAnimation.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionTab>
#include <QTabBar>
#include <QWidget>

QBrush ElaControlStyle::getControlBorder(const QRectF& rect, ElaThemeType::ThemeMode themeMode, bool isElevated)
{
    const bool isLight = themeMode == ElaThemeType::Light;
    const QColor stroke = ElaThemeColor(themeMode, BasicBorder);
    if (!isElevated)
    {
        return stroke;
    }
    // WinUI 浅色主题在底部加深边框 深色主题在顶部保留柔和高光
    const QColor secondaryStroke = ElaThemeColor(themeMode, BasicBaseLine);
    QLinearGradient gradient;
    gradient.setStart(0, isLight ? rect.bottom() : rect.top());
    gradient.setFinalStop(0, isLight ? rect.bottom() - 3 : rect.top() + 3);
    gradient.setColorAt(0.33, secondaryStroke);
    gradient.setColorAt(1, stroke);
    return gradient;
}

QColor ElaControlStyle::getControlBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isAccent)
{
    if (!state.testFlag(QStyle::State_Enabled))
    {
        return eTheme->getThemeColor(themeMode, isAccent ? ElaThemeType::PrimaryDisabled : ElaThemeType::BasicDisable);
    }
    const auto role = isAccent ? (state.testFlag(QStyle::State_Sunken) ? ElaThemeType::PrimaryPress : state.testFlag(QStyle::State_MouseOver) ? ElaThemeType::PrimaryHover
                                                                                                                                              : ElaThemeType::PrimaryNormal)
                               : (state.testFlag(QStyle::State_Sunken) ? ElaThemeType::BasicPress : state.testFlag(QStyle::State_MouseOver) ? ElaThemeType::BasicHover
                                                                                                                                            : ElaThemeType::BasicBase);
    return eTheme->getThemeColor(themeMode, role);
}

QColor ElaControlStyle::getIndicatorBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isChecked)
{
    if (isChecked)
    {
        return getControlBackgroundColor(themeMode, state, true);
    }
    if (!state.testFlag(QStyle::State_Enabled))
    {
        return Qt::transparent;
    }
    const auto role = state.testFlag(QStyle::State_Sunken) ? ElaThemeType::BasicAltPress
        : state.testFlag(QStyle::State_MouseOver)          ? ElaThemeType::BasicAltHover
                                                           : ElaThemeType::BasicAltBase;
    return eTheme->getThemeColor(themeMode, role);
}

QColor ElaControlStyle::getItemBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isPressed, bool isAlternate)
{
    if (state.testFlag(QStyle::State_Selected))
    {
        return isPressed                              ? ElaThemeColor(themeMode, BasicHoverAlpha)
            : state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(themeMode, BasicPressAlpha)
                                                      : ElaThemeColor(themeMode, BasicHoverAlpha);
    }
    return isPressed                              ? ElaThemeColor(themeMode, BasicPressAlpha)
        : state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(themeMode, BasicHoverAlpha)
        : isAlternate                             ? ElaThemeColor(themeMode, BasicAltBase)
                                                  : QColor(Qt::transparent);
}

QColor ElaControlStyle::getTabBackgroundColor(const QWidget* widget, const QStyleOptionTab* option, const QColor& target)
{
    const auto* tabBar = qobject_cast<const QTabBar*>(widget);
    const int index = tabBar ? tabBar->tabAt(option->rect.center()) : -1;
    const QWidget* tabButton = tabBar ? tabBar->tabButton(index, QTabBar::RightSide) : nullptr;
    if (!tabButton && tabBar)
    {
        tabButton = tabBar->tabButton(index, QTabBar::LeftSide);
    }
    return ElaColorAnimation::getColor(widget, target, tabButton ? QByteArray("TabBackground") : "TabBackground" + QByteArray::number(index), {}, tabButton);
}

QColor ElaControlStyle::getStatusColor(ElaThemeType::ThemeMode themeMode, ElaMessageBarType::MessageMode mode, bool isBackground)
{
    switch (mode)
    {
    case ElaMessageBarType::Success:
    {
        return eTheme->getThemeColor(themeMode, isBackground ? ElaThemeType::StatusSuccessBackground : ElaThemeType::StatusSuccess);
    }
    case ElaMessageBarType::Warning:
    {
        return eTheme->getThemeColor(themeMode, isBackground ? ElaThemeType::StatusWarningBackground : ElaThemeType::StatusWarning);
    }
    case ElaMessageBarType::Error:
    {
        return eTheme->getThemeColor(themeMode, isBackground ? ElaThemeType::StatusDangerBackground : ElaThemeType::StatusDanger);
    }
    default:
    {
        return eTheme->getThemeColor(themeMode, isBackground ? ElaThemeType::PopupBase : ElaThemeType::PrimaryNormal);
    }
    }
}

void ElaControlStyle::drawStatusIcon(QPainter* painter, const QRectF& rect, ElaThemeType::ThemeMode themeMode, ElaMessageBarType::MessageMode mode)
{
    const ElaIconType::IconName icon = mode == ElaMessageBarType::Success ? ElaIconType::Check
        : mode == ElaMessageBarType::Warning                              ? ElaIconType::Exclamation
        : mode == ElaMessageBarType::Error                                ? ElaIconType::Xmark
                                                                          : ElaIconType::Info;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(getStatusColor(themeMode, mode));
    painter->drawEllipse(rect);
    QFont iconFont("ElaAwesome");
    iconFont.setPixelSize(qMax(1, qRound(rect.height() * 0.65)));
    painter->setFont(iconFont);
    painter->setPen(ElaThemeColor(themeMode, BasicTextInvert));
    painter->drawText(rect, Qt::AlignCenter, QChar(icon));
    painter->restore();
}

void ElaControlStyle::drawInputFrame(QPainter* painter, const QRectF& rect, qreal radius, ElaThemeType::ThemeMode themeMode, QStyle::State state, const QWidget* widget, qreal focusProgress)
{
    if (rect.isEmpty())
    {
        return;
    }
    const bool isEnabled = state.testFlag(QStyle::State_Enabled);
    const QWidget* popup = QApplication::activePopupWidget();
    const QWidget* focusWidget = QApplication::focusWidget();
    const bool isFocused = isEnabled && (state.testFlag(QStyle::State_HasFocus) || (widget && (widget->hasFocus() || (focusWidget && widget->isAncestorOf(focusWidget)) || (popup && widget->isAncestorOf(popup)))));
    const QColor background = !isEnabled          ? ElaThemeColor(themeMode, BasicDisable)
        : isFocused                               ? ElaThemeColor(themeMode, InputActive)
        : state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(themeMode, BasicHover)
                                                  : ElaThemeColor(themeMode, BasicBase);

    radius = qBound(0.0, radius, qMin(rect.width(), rect.height()) / 2);
    QPainterPath outerPath;
    outerPath.addRoundedRect(rect, radius, radius);
    QPainterPath borderPath = outerPath;
    const QRectF innerRect = rect.adjusted(1, 1, -1, -1);
    if (!innerRect.isEmpty())
    {
        borderPath.addRoundedRect(innerRect, qMax(0.0, radius - 1), qMax(0.0, radius - 1));
    }
    borderPath.setFillRule(Qt::OddEvenFill);
    const QColor stroke = ElaThemeColor(themeMode, BasicBorder);
    QBrush borderBrush(stroke);
    if (isEnabled)
    {
        // 输入框在明暗主题下都保留中性底线 与按钮的高光方向独立
        QLinearGradient gradient(0, rect.bottom(), 0, rect.bottom() - 2);
        gradient.setColorAt(0.5, ElaThemeColor(themeMode, BasicBorderDeep));
        gradient.setColorAt(1, stroke);
        borderBrush = gradient;
    }
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    // 获得焦点时直接进入激活底色 避免点击瞬间混入悬停或过渡中间色产生闪烁
    painter->fillPath(outerPath, ElaColorAnimation::getColor(widget, background, "InputBackground", {}, nullptr, isFocused));
    painter->fillPath(borderPath, borderBrush);
    const qreal progress = isEnabled ? (focusProgress < 0 ? (isFocused ? 1.0 : 0.0) : qBound(0.0, focusProgress, 1.0)) : 0.0;
    if (progress > 0)
    {
        // 按当前宽度计算展开区域 保证缩放与调整尺寸后仍然贴合圆角外轮廓
        const qreal markWidth = rect.width() * progress;
        const QRectF markRect(rect.center().x() - markWidth / 2, rect.bottom() - 2, markWidth, 2);
        painter->setClipPath(outerPath, Qt::IntersectClip);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(themeMode, PrimaryNormal));
        painter->drawRoundedRect(markRect, 1, 1);
    }
    painter->restore();
}
