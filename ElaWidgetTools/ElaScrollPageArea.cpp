#include "ElaScrollPageArea.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "ElaTheme.h"
#include "private/ElaScrollPageAreaPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaScrollPageArea, int, BorderRadius)
ElaScrollPageArea::ElaScrollPageArea(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaScrollPageAreaPrivate())
{
    Q_D(ElaScrollPageArea);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    setFixedHeight(75);
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaScrollPageArea::~ElaScrollPageArea()
{
}

void ElaScrollPageArea::paintEvent(QPaintEvent* event)
{
    Q_D(ElaScrollPageArea);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseAlpha));
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}
