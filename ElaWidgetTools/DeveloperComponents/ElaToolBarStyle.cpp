#include "ElaToolBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QToolButton>
#include <QtMath>

#include "ElaTheme.h"
#include "ElaToolBar.h"
ElaToolBarStyle::ElaToolBarStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaToolBarStyle::~ElaToolBarStyle()
{
}

void ElaToolBarStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelButtonTool:
    {
        return;
    }
    case QStyle::PE_FrameMenu:
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(ElaThemeColor(_themeMode, PopupBorder));
        painter->setBrush(ElaThemeColor(_themeMode, DialogBase));
        painter->drawRect(option->rect);
        painter->restore();
        return;
    }
    case QStyle::PE_IndicatorToolBarHandle:
    {
        //拖动虚线
        QRect handleRect = option->rect;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen handlePen(Qt::DotLine);
        handlePen.setColor(ElaThemeColor(_themeMode, PrimaryNormal));
        handlePen.setWidthF(1.8);
        painter->setPen(handlePen);
        const ElaToolBar* toolBar = dynamic_cast<const ElaToolBar*>(widget);
        if (toolBar->orientation() == Qt::Vertical)
        {
            painter->drawLine(handleRect.x(), handleRect.center().y(), handleRect.right(), handleRect.center().y());
        }
        else
        {
            painter->drawLine(handleRect.center().x(), handleRect.y(), handleRect.center().x(), handleRect.bottom());
        }
        painter->restore();
        return;
    }
    case QStyle::PE_IndicatorToolBarSeparator:
    {
        //间隔符绘制
        QRect separatorRect = option->rect;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
        const ElaToolBar* toolBar = dynamic_cast<const ElaToolBar*>(widget);
        if (toolBar->orientation() == Qt::Vertical)
        {
            painter->drawRect(QRectF(separatorRect.x() + separatorRect.width() * 0.1, separatorRect.center().y() - 0.9, separatorRect.width() * 0.8, 1.8));
        }
        else
        {
            painter->drawRect(QRectF(separatorRect.center().x() - 0.9, separatorRect.y() + separatorRect.height() * 0.1, 1.8, separatorRect.height() * 0.8));
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaToolBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ToolButtonLabel:
    {
        //展开按钮
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            if (widget->objectName() == "qt_toolbar_ext_button")
            {
                if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(bopt->rect);
                }
                //展开图标
                painter->setPen(!bopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicTextDisable) : ElaThemeColor(_themeMode, BasicText));
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(18);
                painter->setFont(iconFont);
                painter->drawText(bopt->rect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleRight));
            }
            else
            {
                if (bopt->arrowType != Qt::NoArrow)
                {
                    break;
                }
                //背景绘制
                if (bopt->state.testFlag(QStyle::State_Enabled))
                {
                    if (bopt->state.testFlag(QStyle::State_Sunken))
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
                        painter->drawRoundedRect(bopt->rect, 4, 4);
                    }
                    else
                    {
                        if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On))
                        {
                            painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                            painter->drawRoundedRect(bopt->rect, 4, 4);
                        }
                    }
                }
                //指示器绘制
                _drawIndicator(painter, bopt, widget);

                //图标绘制
                QRect contentRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarAddLine, widget);
                QRect iconRect = contentRect;
                int heightOffset = iconRect.height() * 0.1;
                iconRect.adjust(3, heightOffset, -3, -heightOffset);
                _drawIcon(painter, iconRect, bopt, widget);

                //文字绘制
                contentRect.adjust(0, heightOffset, 0, -heightOffset);
                _drawText(painter, contentRect, bopt);
            }
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ToolBar:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

int ElaToolBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ToolBarExtensionExtent:
    {
        return 16;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

void ElaToolBarStyle::_drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
        //指示器区域
        if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine))
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicIndicator));
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
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
}

void ElaToolBarStyle::_drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        const QToolButton* toolButton = dynamic_cast<const QToolButton*>(widget);
        QAction* action = toolButton->defaultAction();
        if (!action)
        {
            return;
        }
        if (action->property("ElaIconType").toString().isEmpty())
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
                case Qt::ToolButtonIconOnly:
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
            painter->setPen(ElaThemeColor(_themeMode, BasicText));
            QFont iconFont = QFont("ElaAwesome");
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            case Qt::ToolButtonTextBesideIcon:
            case Qt::ToolButtonFollowStyle:
            {
                QRect adjustIconRect(iconRect.x(), iconRect.y(), iconSize.width(), iconRect.height());
                iconFont.setPixelSize(0.8 * std::min(adjustIconRect.width(), adjustIconRect.height()));
                painter->setFont(iconFont);
                painter->drawText(adjustIconRect, Qt::AlignCenter, action->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonTextUnderIcon:
            {
                QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2, iconRect.y(), iconSize.width(), iconSize.height());
                iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(adjustIconRect, Qt::AlignHCenter, action->property("ElaIconType").toString());
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

void ElaToolBarStyle::_drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const
{
    if (!bopt->text.isEmpty())
    {
        painter->setPen(ElaThemeColor(_themeMode, BasicText));
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
