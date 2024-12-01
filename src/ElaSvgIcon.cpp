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
        if (!d->getSvgPath().isEmpty()) {
            d->_renderer.load(d->getSvgPath());
            update();
        }
    });
}

ElaSvgIcon::~ElaSvgIcon()
{

}

void ElaSvgIcon::setSvgIcon(SvgIconType::IconName icon)
{
    Q_D(ElaSvgIcon);
    d->_iconName = icon;
    if (!d->getSvgPath().isEmpty()) {
        d->_renderer.load(d->getSvgPath());
        update();
    }
}


void ElaSvgIcon::paintEvent(QPaintEvent* event)
{
    Q_D(ElaSvgIcon);
    if (!d->_renderer.isValid()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    d->_renderer.render(&painter);
}
