#include "ElaDockWidget.h"

#include <QEvent>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaDockWidgetPrivate.h"
#include "ElaDockWidgetTitleBar.h"
#include "ElaTheme.h"
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
            widget()->setContentsMargins(6, 0, 6, 6);
        }
        else
        {
            d->_titleBar->setContentsMargins(0, 0, 0, 0);
            widget()->setContentsMargins(0, 0, 0, 0);
        }
    });

    // 主题变更动画
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaDockWidgetPrivate::onThemeModeChanged);

    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, ElaThemeColor(ElaThemeType::Light, DockWidgetBaseStart));
    d->_windowLinearGradient->setColorAt(1, ElaThemeColor(ElaThemeType::Light, DockWidgetBaseEnd));

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
        eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, 6);
        //背景
        painter.setPen(ElaThemeColor(d->_themeMode, DockWidgetFloatBorder));
        painter.setBrush(*d->_windowLinearGradient);
        QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
        painter.drawRoundedRect(foregroundRect, 5, 5);
    }
    painter.restore();
}
