#include "ElaDockWidget.h"

#include <QEvent>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#endif
#include "ElaApplication.h"
#include "ElaDockWidgetPrivate.h"
#include "ElaDockWidgetTitleBar.h"
ElaDockWidget::ElaDockWidget(QWidget* parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags), d_ptr(new ElaDockWidgetPrivate())
{
    Q_D(ElaDockWidget);
    d->q_ptr = this;
    setObjectName("ElaDockWidget");
    setStyleSheet("#ElaDockWidget{background-color:transparent;}");

    d->_titleBar = new ElaDockWidgetTitleBar(this);
    setTitleBarWidget(d->_titleBar);
    connect(this, &ElaDockWidget::topLevelChanged, this, [=](bool topLevel) {
        if (topLevel)
        {
            d->_titleBar->setContentsMargins(6, 6, 6, 0);
        }
        else
        {
            d->_titleBar->setContentsMargins(0, 0, 0, 0);
        }
    });

    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF3, 0xF2, 0xF9));
    d->_windowLinearGradient->setColorAt(1, QColor(0xF4, 0xF1, 0xF8));

    // 主题变更动画
    d->_themeMode = eApp->getThemeMode();
    connect(eApp, &ElaApplication::themeModeChanged, d, &ElaDockWidgetPrivate::onThemeModeChanged);
    setAttribute(Qt::WA_TranslucentBackground);
}

ElaDockWidget::ElaDockWidget(const QString& title, QWidget* parent, Qt::WindowFlags flags)
    : ElaDockWidget(parent, flags)
{
    this->setWindowTitle(title);
}

ElaDockWidget::~ElaDockWidget()
{
}

void ElaDockWidget::setWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    widget->setContentsMargins(6, 0, 6, 6);
    QDockWidget::setWidget(widget);
}

void ElaDockWidget::paintEvent(QPaintEvent* event)
{
    Q_D(ElaDockWidget);
    d->_windowLinearGradient->setFinalStop(width(), height());
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (isFloating())
    {
        // 高性能阴影
        eApp->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, 6);
        //背景
        painter.setPen(d->_themeMode == ElaApplicationType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
        painter.setBrush(*d->_windowLinearGradient);
        QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
        painter.drawRoundedRect(foregroundRect, 5, 5);
    }
    painter.restore();
}
