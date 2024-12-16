#include "ElaScrollPageArea.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QDebug>

#include "ElaTheme.h"
#include "private/ElaScrollPageAreaPrivate.h"

ElaScrollPageArea::ElaScrollPageArea(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaScrollPageAreaPrivate())
{
    Q_D(ElaScrollPageArea);
    d->q_ptr = this;
    setObjectName("ElaScrollPageArea");
    d->_themeMode = eTheme->getThemeMode();
    setStyleSheet(d->getStyleStr());
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        setStyleSheet(d->getStyleStr());
    });
}

ElaScrollPageArea::~ElaScrollPageArea()
{
}

void ElaScrollPageArea::setBorderRadius(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
    Q_D(ElaScrollPageArea);
    d->_topLeftRadius = topLeft;
    d->_topRightRadius = topRight;
    d->_bottomLeftRadius = bottomLeft;
    d->_bottomRightRadius = bottomRight;
}

void ElaScrollPageArea::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
