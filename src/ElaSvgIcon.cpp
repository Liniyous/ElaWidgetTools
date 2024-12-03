#include "ElaSvgIcon.h"

#include "ElaTheme.h"
#include "private/ElaSvgIconPrivate.h"

ElaSvgIcon::ElaSvgIcon(QWidget* parent)
    : QLabel(parent), d_ptr(new ElaSvgIconPrivate())
{
    Q_D(ElaSvgIcon);
    d->q_ptr = this;


    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->updateIcon();
        update();
    });
}

ElaSvgIcon::~ElaSvgIcon()
{

}

void ElaSvgIcon::setSvgIcon(SvgIconType::IconName icon, int iconW, int iconH)
{
    Q_D(ElaSvgIcon);
    iconW = iconW < width() ? iconW : width();
    iconH = iconH < height() ? iconH : height();
    d->updateIcon(icon, iconW, iconH);
    update();
}


void ElaSvgIcon::paintEvent(QPaintEvent* event)
{
    Q_D(ElaSvgIcon);
    if (d->_pixmap.isNull()) {
        return;
    }
    QSize pSize = d->_pixmap.size() / 2;
    int x = (width() - pSize.width()) / 2;
    int y = (height() - pSize.height()) / 2;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(QRect(x, y, pSize.width(), pSize.height()), d->_pixmap);
}
