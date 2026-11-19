#include "ElaToolButtonStyle.h"

#include "ElaControlAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

ElaToolButtonStyle::ElaToolButtonStyle(QStyle* style)
{
    _pIsSelected = false;
    _pIsTransparent = true;
    _pBorderRadius = 4;
    _pTextAlignment = Qt::AlignCenter;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaToolButtonStyle::~ElaToolButtonStyle()
{
}

void ElaToolButtonStyle::setControlAnimation(ElaControlAnimation* animation)
{
    _controlAnimation = animation;
}

void ElaToolButtonStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_ToolButton:
    {
        // 内容绘制
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            if (bopt->arrowType != Qt::NoArrow)
            {
                break;
            }
            QRectF toolButtonRect = QRectF(bopt->rect).adjusted(0.5, 0.5, -0.5, -0.5);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            const bool isEnabled = bopt->state.testFlag(QStyle::State_Enabled);
            const bool isPressed = isEnabled && (_controlAnimation ? _controlAnimation->getIsPressed() : bopt->state.testFlag(QStyle::State_Sunken));
            const bool isSelected = _pIsSelected || bopt->state.testFlag(QStyle::State_On);
            QColor background = _pIsTransparent ? QColor(Qt::transparent) : ElaThemeColor(_themeMode, BasicBase);
            if (!isEnabled)
            {
                background = _pIsTransparent ? QColor(Qt::transparent) : ElaThemeColor(_themeMode, BasicDisable);
            }
            else if (isPressed)
            {
                background = _pIsTransparent ? ElaThemeColor(_themeMode, BasicPressAlpha) : ElaThemeColor(_themeMode, BasicPress);
            }
            else if (isSelected || bopt->state.testFlag(QStyle::State_MouseOver))
            {
                background = _pIsTransparent ? ElaThemeColor(_themeMode, BasicHoverAlpha) : ElaThemeColor(_themeMode, BasicHover);
            }
            painter->setPen(_pIsTransparent ? QPen(Qt::NoPen) : QPen(ElaControlStyle::getControlBorder(toolButtonRect, _themeMode, isEnabled && !isPressed), 1));
            painter->setBrush(_controlAnimation ? _controlAnimation->getBackgroundColor(background) : background);
            painter->drawRoundedRect(toolButtonRect, _pBorderRadius, _pBorderRadius);
            // 指示器绘制
            _drawIndicator(painter, bopt, widget);

            // 图标绘制
            QRect contentRect = subControlRect(control, bopt, QStyle::SC_ToolButton, widget);
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                contentRect.adjust(0, 0, -_getExpandIndicatorWidth(bopt), 0);
                contentRect = visualRect(bopt->direction, bopt->rect, contentRect);
            }
            int heightOffset = contentRect.height() * 0.05;
            contentRect.adjust(0, heightOffset, 0, -heightOffset);
            _drawIcon(painter, contentRect, bopt, widget);

            // 文字绘制
            _drawText(painter, contentRect, bopt);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QSize ElaToolButtonStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    if (type == QStyle::CT_ToolButton)
    {
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            QSize toolButtonSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            if (bopt->toolButtonStyle != Qt::ToolButtonTextUnderIcon)
            {
                toolButtonSize.setHeight(ElaWidgetToolsSize::DefaultControlHeight);
            }
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                toolButtonSize.rwidth() += _getExpandIndicatorWidth(bopt);
            }
            return toolButtonSize;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void ElaToolButtonStyle::_drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (!bopt->features.testFlag(QStyleOptionToolButton::HasMenu))
    {
        return;
    }
    QRect indicatorRect;
    if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ToolButtonMenu, widget);
    }
    else
    {
        const int indicatorWidth = _getExpandIndicatorWidth(bopt);
        indicatorRect = QRect(bopt->rect.right() - indicatorWidth + 1, bopt->rect.y(), indicatorWidth - _contentMargin, bopt->rect.height());
        indicatorRect = visualRect(bopt->direction, bopt->rect, indicatorRect);
    }
    painter->save();
    const QPointF center = QRectF(indicatorRect).center() + QPointF(0, _controlAnimation ? _controlAnimation->getChevronOffset() : 0);
    QPainterPath path;
    path.moveTo(center + QPointF(-4, -2));
    path.lineTo(center + QPointF(0, 2));
    path.lineTo(center + QPointF(4, -2));
    const QColor color = !bopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicTextDisable)
        : _controlAnimation && _controlAnimation->getIsPressed()      ? ElaThemeColor(_themeMode, BasicTextNoFocus)
                                                                      : ElaThemeColor(_themeMode, BasicText);
    painter->setPen(QPen(color, 1.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
    painter->restore();
}

void ElaToolButtonStyle::_drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        if (!iconRect.isEmpty() && iconSize.height() > iconRect.height())
        {
            iconSize.scale(iconSize.width(), qFloor(iconRect.height()), Qt::KeepAspectRatio);
        }
        if (widget->property("ElaIconType").toString().isEmpty())
        {
            // 绘制QIcon
            QIcon icon = bopt->icon;
            if (!icon.isNull())
            {
                QPixmap iconPix = icon.pixmap(iconSize * (widget ? widget->devicePixelRatioF() : 1),
                                              (bopt->state & State_Enabled) ? (bopt->state & State_MouseOver ? QIcon::Active : QIcon::Normal)
                                                                            : QIcon::Disabled,
                                              (bopt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
                switch (bopt->toolButtonStyle)
                {
                case Qt::ToolButtonIconOnly:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.center().x() - iconSize.width() / 2.0, iconRect.center().y() - iconSize.height() / 2.0), iconSize), iconPix);
                    break;
                }
                case Qt::ToolButtonFollowStyle:
                case Qt::ToolButtonTextBesideIcon:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.x() + _contentMargin, iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
                    break;
                }
                case Qt::ToolButtonTextUnderIcon:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.center().x() - iconSize.width() / 2, iconRect.y()), iconSize), iconPix);
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
        else
        {
            // 绘制ElaIcon
            painter->save();
            if (bopt->state.testFlag(QStyle::State_Enabled))
            {
                painter->setPen((_pIsSelected || bopt->state.testFlag(QStyle::State_On))
                                    ? ElaThemeColor(_themeMode, PrimaryNormal)
                                    : (_controlAnimation && _controlAnimation->getIsPressed() ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText)));
            }
            else
            {
                painter->setPen(ElaThemeColor(_themeMode, BasicTextDisable));
            }
            QFont iconFont = QFont("ElaAwesome");
            int iconRotate = widget->property("ElaIconRotate").toInt();
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            {
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                if (iconRotate)
                {
                    painter->translate(iconRect.center());
                    painter->rotate(iconRotate);
                    painter->translate(-iconRect.center());
                }
                painter->drawText(iconRect, Qt::AlignCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonFollowStyle:
            case Qt::ToolButtonTextBesideIcon:
            {
                QRect adjustIconRect(iconRect.x() + _contentMargin, iconRect.y(), iconSize.width(), iconRect.height());
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                if (iconRotate)
                {
                    painter->translate(adjustIconRect.center());
                    painter->rotate(iconRotate);
                    painter->translate(-adjustIconRect.center());
                }
                painter->drawText(adjustIconRect, Qt::AlignCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonTextUnderIcon:
            {
                QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2, iconRect.y() + 0.2 * std::min(iconSize.width(), iconSize.height()), iconSize.width(), iconSize.height());
                iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                if (iconRotate)
                {
                    painter->translate(adjustIconRect.center());
                    painter->rotate(iconRotate);
                    painter->translate(-adjustIconRect.center());
                }
                painter->drawText(adjustIconRect, Qt::AlignHCenter, widget->property("ElaIconType").toString());
                break;
            }
            default:
            {
                break;
            }
            }
            painter->restore();
        }
    }
}

void ElaToolButtonStyle::_drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const
{
    if (!bopt->text.isEmpty())
    {
        if (bopt->state.testFlag(QStyle::State_Enabled))
        {
            painter->setPen((_pIsSelected || bopt->state.testFlag(QStyle::State_On))
                                ? ElaThemeColor(_themeMode, PrimaryNormal)
                                : (_controlAnimation && _controlAnimation->getIsPressed() ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText)));
        }
        else
        {
            painter->setPen(ElaThemeColor(_themeMode, BasicTextDisable));
        }
        switch (bopt->toolButtonStyle)
        {
        case Qt::ToolButtonTextOnly:
        {
            if (_pTextAlignment.testFlag(Qt::AlignLeft) || _pTextAlignment.testFlag(Qt::AlignRight))
            {
                contentRect.adjust(_contentMargin, 0, -_contentMargin, 0);
            }
            painter->drawText(contentRect, _pTextAlignment, bopt->text);
            break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            contentRect.adjust(_contentMargin * 2 + bopt->iconSize.width(), 0, 0, 0);
            painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextUnderIcon:
        {
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                contentRect.setLeft(contentRect.left() + _contentMargin);
                painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignLeft, bopt->text);
            }
            else
            {
                painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignHCenter, bopt->text);
            }
            break;
        }
        case Qt::ToolButtonFollowStyle:
        {
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

int ElaToolButtonStyle::_getExpandIndicatorWidth(const QStyleOptionToolButton* bopt) const
{
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(0.75 * std::min(bopt->iconSize.width(), bopt->iconSize.height()));
    return 12 + _contentMargin;
}
