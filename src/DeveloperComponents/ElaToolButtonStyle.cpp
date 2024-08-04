#include "ElaToolButtonStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaTheme.h"
ElaToolButtonStyle::ElaToolButtonStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
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
        //内容绘制
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            if (bopt->arrowType != Qt::NoArrow)
            {
                break;
            }
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            //背景绘制
            if (bopt->state.testFlag(QStyle::State_Enabled))
            {
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, ToolButtonPress));
                    painter->drawRoundedRect(bopt->rect, 4, 4);
                }
                else
                {
                    if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On))
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, ToolButtonHover));
                        painter->drawRoundedRect(bopt->rect, 4, 4);
                    }
                }
            }
            //指示器绘制
            _drawIndicator(painter, bopt, widget);

            //图标绘制
            QRect contentRect = subControlRect(control, bopt, QStyle::SC_ScrollBarAddLine, widget);
            QRect iconRect = contentRect;
            int heightOffset = iconRect.height() * 0.05;
            iconRect.adjust(3, heightOffset, -3, -heightOffset);
            _drawIcon(painter, iconRect, bopt, widget);
            //文字绘制
            contentRect.adjust(0, heightOffset, 0, -heightOffset);
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
            if (!bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup) && bopt->text.isEmpty())
            {
                QSize toolButtonSize = QProxyStyle::sizeFromContents(type, option, size, widget);
                return QSize(toolButtonSize.width() - 13, toolButtonSize.height());
            }
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void ElaToolButtonStyle::_drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
        //指示器区域
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
        //指示器
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowText) : ElaThemeColor(_themeMode, WindowTextDisable));
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
}

void ElaToolButtonStyle::_drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        if (widget->property("ElaIconType").toString().isEmpty())
        {
            //绘制QIcon
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
                case Qt::ToolButtonTextBesideIcon:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
                    break;
                }
                case Qt::ToolButtonTextUnderIcon:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.center().x() - iconSize.width() / 2, iconRect.y()), iconSize), iconPix);
                    break;
                }
                case Qt::ToolButtonFollowStyle:
                {
                    painter->drawPixmap(iconRect.x(), iconRect.y() - iconSize.height() / 2, iconPix);
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
            //绘制ElaIcon
            painter->save();
            painter->setPen(ElaThemeColor(_themeMode, WindowText));
            QFont iconFont = QFont("ElaAwesome");
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            case Qt::ToolButtonTextBesideIcon:
            case Qt::ToolButtonFollowStyle:
            {
                QRect adjustIconRect(iconRect.x(), iconRect.y(), iconSize.width(), iconRect.height());
                iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(adjustIconRect, Qt::AlignCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonTextUnderIcon:
            {
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
            painter->drawText(contentRect, Qt::AlignCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            painter->drawText(QRect(contentRect.x() + bopt->iconSize.width() + 12, contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextUnderIcon:
        {
            painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignHCenter, bopt->text);
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
