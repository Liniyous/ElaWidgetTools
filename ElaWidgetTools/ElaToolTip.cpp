#include "ElaToolTip.h"

#include <QPainter>
#include <QVBoxLayout>

#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolTipPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, DisplayMsec)
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, ShowDelayMsec)
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, HideDelayMsec)
ElaToolTip::ElaToolTip(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaToolTipPrivate())
{
    Q_D(ElaToolTip);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    d->_pDisplayMsec = -1;
    d->_pShowDelayMsec = 0;
    d->_pHideDelayMsec = 0;
    d->_pCustomWidget = nullptr;
    setObjectName("ElaToolTip");
    parent->installEventFilter(d);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    d->_toolTipText = new ElaText(this);
    d->_toolTipText->setWordWrap(false);
    d->_toolTipText->setTextPixelSize(17);
    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2);
    d->_mainLayout->addWidget(d->_toolTipText);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
    hide();
}

ElaToolTip::~ElaToolTip()
{
}

void ElaToolTip::setToolTip(QString toolTip)
{
    Q_D(ElaToolTip);
    resize(fontMetrics().horizontalAdvance(toolTip), height());
    d->_toolTipText->setText(toolTip);
    Q_EMIT pToolTipChanged();
}

QString ElaToolTip::getToolTip() const
{
    Q_D(const ElaToolTip);
    return d->_toolTipText->text();
}

void ElaToolTip::setCustomWidget(QWidget* customWidget)
{
    Q_D(ElaToolTip);
    if (!customWidget || customWidget == this)
    {
        return;
    }
    if (d->_pCustomWidget)
    {
        d->_mainLayout->removeWidget(d->_pCustomWidget);
        d->_pCustomWidget->deleteLater();
    }
    d->_toolTipText->hide();
    d->_mainLayout->addWidget(customWidget);
    d->_pCustomWidget = customWidget;
    Q_EMIT pCustomWidgetChanged();
}

QWidget* ElaToolTip::getCustomWidget() const
{
    Q_D(const ElaToolTip);
    return d->_pCustomWidget;
}

void ElaToolTip::updatePos()
{
    Q_D(ElaToolTip);
    d->_updatePos();
}

void ElaToolTip::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToolTip);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    //阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);
    QRect foregroundRect = rect();
    foregroundRect.adjust(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
    painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
    painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}
