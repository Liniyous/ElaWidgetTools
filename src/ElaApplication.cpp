#include "ElaApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

#include "private/ElaApplicationPrivate.h"
Q_SINGLETON_CREATE_CPP(ElaApplication)
Q_PROPERTY_CREATE_Q_CPP(ElaApplication, bool, IsApplicationClosed)
Q_PRIVATE_CREATE_Q_CPP(ElaApplication, QIcon, WindowIcon)
ElaApplication::ElaApplication(QObject* parent)
    : QObject{parent}, d_ptr(new ElaApplicationPrivate())
{
    Q_D(ElaApplication);
    d->q_ptr = this;
    d->_pIsApplicationClosed = false;
    d->_pWindowIcon = QIcon(":/include/Image/Cirno.jpg");
}

ElaApplication::~ElaApplication()
{
}

void ElaApplication::setThemeMode(ElaApplicationType::ThemeMode themeMode)
{
    Q_D(ElaApplication);
    d->_themeMode = themeMode;
    Q_EMIT themeModeChanged(d->_themeMode);
}

ElaApplicationType::ThemeMode ElaApplication::getThemeMode() const
{
    Q_D(const ElaApplication);
    return d->_themeMode;
}

void ElaApplication::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
    Q_D(ElaApplication);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = d->_themeMode == ElaApplicationType::Light ? QColor(0x60, 0x5F, 0x61) : QColor(0x9C, 0x9B, 0x9E);
    for (int i = 0; i < shadowBorderWidth; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(shadowBorderWidth - i, shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
        int alpha = 5 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

void ElaApplication::init()
{
    Q_D(ElaApplication);
    QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    QApplication::setWindowIcon(d->_pWindowIcon);
    //默认字体
    QFont font = qApp->font();
    font.setPointSize(10);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
}

bool ElaApplication::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
