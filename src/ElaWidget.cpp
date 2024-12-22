#include "ElaWidget.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QVBoxLayout>
#include <QTimer>
#include "ElaApplication.h"
#include "ElaTheme.h"
#include "private/ElaWidgetPrivate.h"
Q_TAKEOVER_NATIVEEVENT_CPP(ElaWidget, d_func()->_appBar);
ElaWidget::ElaWidget(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaWidgetPrivate())
{
    Q_D(ElaWidget);
    d->q_ptr = this;

    d->_windowAgent = new QWK::WidgetWindowAgent(this);
    d->_windowAgent->setup(this);

    resize(500, 500); // 默认宽高
    setWindowTitle("ElaWidget");
    setObjectName("ElaWidget");
    setStyleSheet("#ElaWidget{background-color:transparent;}");

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    d->_appBar->setIsStayTop(true);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::StayTopButtonHint | ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint);
    connect(d->_appBar, &ElaAppBar::routeBackButtonClicked, this, &ElaWidget::routeBackButtonClicked);
    connect(d->_appBar, &ElaAppBar::navigationButtonClicked, this, &ElaWidget::navigationButtonClicked);
    connect(d->_appBar, &ElaAppBar::themeChangeButtonClicked, this, &ElaWidget::themeChangeButtonClicked);
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, &ElaWidget::closeButtonClicked);

    // 主题
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaWidgetPrivate::onThemeModeChanged);

    //延时渲染
    QTimer::singleShot(1, this, [=] {
        d->onThemeModeChanged(d->_themeMode);
    });
}

ElaWidget::~ElaWidget()
{
}

void ElaWidget::setIsStayTop(bool isStayTop)
{
    Q_D(ElaWidget);
    d->_appBar->setIsStayTop(isStayTop);
}

bool ElaWidget::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void ElaWidget::setIsFixedSize(bool isFixedSize)
{
    Q_D(ElaWidget);
    d->_appBar->setIsFixedSize(isFixedSize);
}

bool ElaWidget::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void ElaWidget::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(ElaWidget);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

bool ElaWidget::getIsDefaultClosed() const
{
    Q_D(const ElaWidget);
    return d->_appBar->getIsDefaultClosed();
}

void ElaWidget::setAppBarHeight(int appBarHeight)
{
    Q_D(ElaWidget);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

int ElaWidget::getAppBarHeight() const
{
    Q_D(const ElaWidget);
    return d->_appBar->getAppBarHeight();
}

void ElaWidget::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void ElaWidget::setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(ElaWidget);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void ElaWidget::setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)
{
    Q_D(ElaWidget);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

ElaAppBarType::ButtonFlags ElaWidget::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

