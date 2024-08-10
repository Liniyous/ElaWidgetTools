#include "ElaToolButtonStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaTheme.h"
ElaToolButtonStyle::ElaToolButtonStyle(QStyle* style)
{
    _pIsTransparent = true;
    _pExpandIconRotate = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaToolButtonStyle::~ElaToolButtonStyle()
{
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
            QRect toolButtonRect = bopt->rect;
            if (!_pIsTransparent)
            {
                // 边框距离
                toolButtonRect.adjust(1, 1, -1, -1);
            }
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(_pIsTransparent ? Qt::transparent : ElaThemeColor(_themeMode, ToolButtonBorder));
            // 背景绘制
            if (bopt->state.testFlag(QStyle::State_Enabled))
            {
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(_pIsTransparent ? ElaThemeColor(_themeMode, ToolButtonTransparentPress) : ElaThemeColor(_themeMode, ToolButtonPress));
                    painter->drawRoundedRect(toolButtonRect, 4, 4);
                }
                else
                {
                    if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On))
                    {
                        painter->setBrush(_pIsTransparent ? ElaThemeColor(_themeMode, ToolButtonTransparentHover) : ElaThemeColor(_themeMode, ToolButtonHover));
                        painter->drawRoundedRect(toolButtonRect, 4, 4);
                    }
                    else
                    {
                        if (!_pIsTransparent)
                        {
                            painter->setBrush(ElaThemeColor(_themeMode, ToolButtonBase));
                            painter->drawRoundedRect(toolButtonRect, 4, 4);
                        }
                    }
                }
            }
            // 指示器绘制
            _drawIndicator(painter, bopt, widget);

            // 图标绘制
            QRect contentRect = subControlRect(control, bopt, QStyle::SC_ScrollBarAddLine, widget);
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
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                toolButtonSize.setWidth(toolButtonSize.width() + _contentMargin + 0.65 * std::min(bopt->iconSize.width(), bopt->iconSize.height()));
            }
            return toolButtonSize;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void ElaToolButtonStyle::_drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
        // 指示器区域
        if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine))
        {
            painter->setBrush(ElaThemeColor(_themeMode, ToolButtonIndicator));
            QPainterPath path;
            path.moveTo(indicatorRect.topLeft());
            path.lineTo(indicatorRect.right() - 4, indicatorRect.y());
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.y(), 8, 8), 90, -90);
            path.lineTo(indicatorRect.right(), indicatorRect.bottom() - 4);
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.bottom() - 8, 8, 8), 0, -90);
            path.lineTo(indicatorRect.bottomLeft());
            path.closeSubpath();
            painter->drawPath(path);
        }
        // 指示器
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowText) : ElaThemeColor(_themeMode, WindowTextDisable));
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
    else if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu))
    {
        // 展开指示器
        QSize iconSize = bopt->iconSize;
        painter->save();
        QRect toolButtonRect = bopt->rect;
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
        painter->setFont(iconFont);
        int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar((unsigned short)ElaIconType::AngleDown));
        QRect expandIconRect(toolButtonRect.right() - _contentMargin - indicatorWidth, toolButtonRect.y() + 1, indicatorWidth, toolButtonRect.height());
        painter->setPen(ElaThemeColor(_themeMode, WindowText));
        painter->translate(expandIconRect.center().x(), expandIconRect.y() + (qreal)expandIconRect.height() / 2);
        painter->rotate(_pExpandIconRotate);
        painter->translate(-expandIconRect.center().x() - 1, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
        painter->drawText(expandIconRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleDown));
        painter->restore();
    }
}

void ElaToolButtonStyle::_drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        if (widget->property("ElaIconType").toString().isEmpty())
        {
            // 绘制QIcon
            QIcon icon = bopt->icon;
            if (!icon.isNull())
            {
                QPixmap iconPix = icon.pixmap(iconSize,
                                              (bopt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (bopt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
                switch (bopt->toolButtonStyle)
                {
                case Qt::ToolButtonIconOnly:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
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
                    if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                    {
                        iconRect.setRight(iconRect.right() - _calculateExpandIndicatorWidth(bopt, painter));
                    }
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
            painter->setPen(ElaThemeColor(_themeMode, WindowText));
            QFont iconFont = QFont("ElaAwesome");
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            {
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(iconRect, Qt::AlignCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonFollowStyle:
            case Qt::ToolButtonTextBesideIcon:
            {
                QRect adjustIconRect(iconRect.x() + _contentMargin, iconRect.y(), iconSize.width(), iconRect.height());
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(adjustIconRect, Qt::AlignCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonTextUnderIcon:
            {
                if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                {
                    iconRect.setRight(iconRect.right() - _calculateExpandIndicatorWidth(bopt, painter));
                }
                QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2, iconRect.y() + 0.2 * std::min(iconSize.width(), iconSize.height()), iconSize.width(), iconSize.height());
                iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
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
        painter->setPen(ElaThemeColor(_themeMode, WindowText));
        switch (bopt->toolButtonStyle)
        {
        case Qt::ToolButtonTextOnly:
        {
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                contentRect.setLeft(contentRect.left() + _contentMargin);
                painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            }
            else
            {
                painter->drawText(contentRect, Qt::AlignCenter, bopt->text);
            }
            break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            painter->drawText(QRect(contentRect.x() + _contentMargin * 2 + bopt->iconSize.width(), contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
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

qreal ElaToolButtonStyle::_calculateExpandIndicatorWidth(const QStyleOptionToolButton* bopt, QPainter* painter) const
{
    // 展开指示器
    QSize iconSize = bopt->iconSize;
    painter->save();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
    painter->setFont(iconFont);
    int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar((unsigned short)ElaIconType::AngleDown));
    painter->restore();
    return indicatorWidth;
}
