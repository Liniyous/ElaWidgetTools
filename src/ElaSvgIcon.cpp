#include "ElaSvgIcon.h"

#include "ElaTheme.h"
#include "private/ElaSvgIconPrivate.h"

Q_SINGLETON_CREATE_CPP(ElaSvgIcon)
ElaSvgIcon::ElaSvgIcon(QObject* parent)
    : QObject{parent}, d_ptr(new ElaSvgIconPrivate())
{
    Q_D(ElaSvgIcon);
    d->q_ptr = this;


    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaSvgIcon::~ElaSvgIcon()
{

}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome, QColor iconColor)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome, int pixelSize)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome, int pixelSize, QColor iconColor)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome, int fixedWidth, int fixedHeight)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    renderer.setViewBox(QRectF(0, 0, fixedWidth, fixedHeight));
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

QIcon ElaSvgIcon::getSvgIcon(SvgIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor)
{
    Q_D(ElaSvgIcon);
    QSvgRenderer renderer;
    if (!renderer.load(d->getSvgPath(awesome))) {
        return QIcon();
    }
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    renderer.render(&painter);
    painter.end();
    return QIcon(pix);
}

