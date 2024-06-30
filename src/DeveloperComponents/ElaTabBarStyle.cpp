#include "ElaTabBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaApplication.h"
ElaTabBarStyle::ElaTabBarStyle(QStyle* style)
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaTabBarStyle::~ElaTabBarStyle()
{
}

void ElaTabBarStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    // qDebug() << pe << opt->rect;
    switch (pe)
    {
    case QStyle::PE_FrameTabBarBase:
    {
        //底边线
        return;
    }
    case QStyle::PE_IndicatorArrowLeft:
    {
        return;
    }
    case QStyle::PE_IndicatorArrowRight:
    {
        return;
    }
    case QStyle::PE_PanelButtonTool:
    {
        //左侧隐藏图标
        return;
    }
    case QStyle::PE_IndicatorTabTear:
    {
        return;
    }
    case QStyle::PE_IndicatorTabClose:
    {
        p->save();
        p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        if (opt->state.testFlag(QStyle::State_MouseOver))
        {
            p->setPen(Qt::NoPen);
            p->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xDC, 0xDC, 0xDD) : QColor(0x5C, 0x5A, 0x56));
            p->drawRoundedRect(opt->rect, 2, 2);
        }
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(16);
        p->setFont(iconFont);
        p->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
        p->drawText(opt->rect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Xmark));
        p->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void ElaTabBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    int margin = 10;
    switch (element)
    {
    case QStyle::CE_TabBarTabShape:
    {
        //背景绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect tabRect = topt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(Qt::NoPen);
            if (topt->state.testFlag(QStyle::State_Selected))
            {
                //选中背景绘制
                tabRect.setLeft(tabRect.left() - margin);
                tabRect.setRight(tabRect.right() + margin);
                painter->setBrush(_themeMode == ElaApplicationType::Light ? Qt::white : QColor(0x2D, 0x2D, 0x2D));
                QPainterPath path;
                path.moveTo(tabRect.x(), tabRect.bottom() + 1);
                path.arcTo(QRectF(tabRect.x() - margin, tabRect.bottom() - margin * 2 + 1, margin * 2, margin * 2), -90, 90);
                path.lineTo(tabRect.x() + margin, tabRect.y() + margin);
                path.arcTo(QRectF(tabRect.x() + margin, tabRect.y(), margin * 2, margin * 2), 180, -90);
                path.lineTo(tabRect.right() - 2 * margin, tabRect.y());
                path.arcTo(QRectF(tabRect.right() - 3 * margin, tabRect.y(), margin * 2, margin * 2), 90, -90);
                path.lineTo(tabRect.right() - margin, tabRect.bottom() - margin);
                path.arcTo(QRectF(tabRect.right() - margin, tabRect.bottom() - 2 * margin + 1, margin * 2, margin * 2), -180, 90);
                path.lineTo(tabRect.right(), tabRect.bottom() + 10);
                path.lineTo(tabRect.x(), tabRect.bottom() + 10);
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                if (topt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xFC, 0xF8, 0xFF) : QColor(0x27, 0x27, 0x27));
                }
                else
                {
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF5, 0xF1, 0xF8) : QColor(0x1A, 0x1A, 0x1A));
                }
                tabRect.setHeight(tabRect.height() + 10);
                painter->drawRoundedRect(tabRect, 0, 0);
                tabRect.setHeight(tabRect.height() - 10);
            }

            //间隔符绘制
            if (!topt->state.testFlag(QStyle::State_Selected) && topt->position != QStyleOptionTab::End && topt->selectedPosition != QStyleOptionTab::NextIsSelected)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
                painter->drawRoundedRect(QRectF(tabRect.right() - 3, tabRect.y() + 7, 3, tabRect.height() - 14), 2, 2);
            }
            painter->restore();
            return;
        }
    }
    case QStyle::CE_TabBarTabLabel:
    {
        //文字和图标绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect textRect = subElementRect(QStyle::SE_TabBarTabText, topt, widget);
            textRect.setLeft(textRect.left() + 10);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            //图标绘制
            QIcon icon = topt->icon;
            if (!icon.isNull())
            {
                QRectF iconRect(topt->rect.x() + 15, textRect.center().y() - (qreal)topt->iconSize.height() / 2 + 1, topt->iconSize.width(), topt->iconSize.height());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QPixmap iconPix = icon.pixmap(topt->iconSize, painter->device()->devicePixelRatio(),
                                              (topt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (topt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
#else
                QPixmap iconPix = icon.pixmap(topt->iconSize,
                                              (topt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (topt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
#endif
                painter->drawPixmap(iconRect.x(), iconRect.y(), iconPix);
            }
            //文字绘制
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
            QString text = painter->fontMetrics().elidedText(topt->text, Qt::ElideRight, textRect.width());
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextDontClip, text);
            painter->restore();
            return;
        }
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize ElaTabBarStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case CT_TabBarTab:
    {
        return QSize(220, 35);
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

QRect ElaTabBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::SE_TabBarScrollLeftButton:
    case QStyle::SE_TabBarScrollRightButton:
    {
        return QRect();
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}
