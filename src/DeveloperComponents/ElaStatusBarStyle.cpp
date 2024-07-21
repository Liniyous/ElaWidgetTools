#include "ElaStatusBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaApplication.h"
ElaStatusBarStyle::ElaStatusBarStyle(QStyle* style)
{
    _themeMode = eApp->getThemeMode();
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaStatusBarStyle::~ElaStatusBarStyle()
{
}

void ElaStatusBarStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelStatusBar:
    {
        //背景绘制
        QRect statusBarRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF5, 0xF1, 0xFF) : QColor(0x40, 0x40, 0x40));
        painter->drawRect(statusBarRect);
        painter->restore();
        return;
    }
    case QStyle::PE_FrameStatusBarItem:
    {
        //间隔符绘制
        QRect statusBarItemRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
        painter->drawRoundedRect(QRectF(statusBarItemRect.right() - 3, statusBarItemRect.y() + statusBarItemRect.height() * 0.1, 3, statusBarItemRect.height() - statusBarItemRect.height() * 0.2), 2, 2);
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

void ElaStatusBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_SizeGrip:
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

QSize ElaStatusBarStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    //qDebug() << type << QProxyStyle::sizeFromContents(type, option, size, widget);
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

int ElaStatusBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    return QProxyStyle::pixelMetric(metric, option, widget);
}
