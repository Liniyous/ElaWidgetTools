#include "ElaToolButtonStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaApplication.h"
ElaToolButtonStyle::ElaToolButtonStyle(QStyle* style)
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) {
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
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF1, 0xF0, 0xF7) : QColor(0x35, 0x35, 0x35));
                    painter->drawRoundedRect(bopt->rect, 4, 4);
                }
                else
                {
                    if (bopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xEA, 0xE9, 0xF0) : QColor(0x2F, 0x2F, 0x2F));
                        painter->drawRoundedRect(bopt->rect, 4, 4);
                    }
                }
            }
            //指示器绘制
            _drawIndicator(painter, bopt, widget);

            //图标绘制
            QRect contentRect = subControlRect(control, bopt, QStyle::SC_ScrollBarAddLine, widget);
            QRect iconRect = contentRect;
            iconRect.adjust(3, 3, -3, -3);
            _drawIcon(painter, iconRect, bopt, widget);

            //文字绘制
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

int ElaToolButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ButtonIconSize:
    {
        //Icon大小 adjust 3
        return 22;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
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
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE0, 0xE0, 0xE0) : QColor(0x40, 0x40, 0x40));
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
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled) ? _themeMode == ElaApplicationType::Light ? Qt::black : Qt::white : Qt::gray);
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
}

void ElaToolButtonStyle::_drawIcon(QPainter* painter, QRect iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        if (widget->property("ElaIconType").toString().isEmpty())
        {
            //绘制QIcon
            QIcon icon = bopt->icon;
            if (!icon.isNull())
            {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QPixmap iconPix = icon.pixmap(bopt->iconSize, painter->device()->devicePixelRatio(),
                                              (bopt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (bopt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
#else
                QPixmap iconPix = icon.pixmap(bopt->iconSize,
                                              (bopt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (bopt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
#endif
                switch (bopt->toolButtonStyle)
                {
                case Qt::ToolButtonTextBesideIcon:
                {
                    painter->drawPixmap(iconRect.x(), iconRect.y() + bopt->iconSize.height() / 2, iconPix);
                    break;
                }
                case Qt::ToolButtonTextUnderIcon:
                {
                    painter->drawPixmap(iconRect.center().x() - bopt->iconSize.width() / 2, iconRect.y(), iconPix);
                    break;
                }
                case Qt::ToolButtonFollowStyle:
                {
                    painter->drawPixmap(iconRect.x(), iconRect.y() + bopt->iconSize.height() / 2, iconPix);
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
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(bopt->iconSize.width());
            painter->setFont(iconFont);
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            case Qt::ToolButtonTextBesideIcon:
            case Qt::ToolButtonFollowStyle:
            {
                painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, widget->property("ElaIconType").toString());
                break;
            }
            case Qt::ToolButtonTextUnderIcon:
            {
                painter->drawText(iconRect, Qt::AlignTop | Qt::AlignHCenter, widget->property("ElaIconType").toString());
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
        switch (bopt->toolButtonStyle)
        {
        case Qt::ToolButtonTextOnly:
        {
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            painter->drawText(contentRect, Qt::AlignCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            painter->drawText(QRect(contentRect.x() + bopt->iconSize.width() + 12, contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextUnderIcon:
        {
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
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
