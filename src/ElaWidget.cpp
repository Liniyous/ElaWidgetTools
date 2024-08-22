#include "ElaWidget.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QScreen>
#include <QVBoxLayout>

#include "ElaAppBar.h"
#include "ElaTheme.h"
#include "private/ElaWidgetPrivate.h"
ElaWidget::ElaWidget(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaWidgetPrivate())
{
    Q_D(ElaWidget);
    d->q_ptr = this;
    resize(500, 500); // 默认宽高
    setWindowTitle("ElaWidget");
    setObjectName("ElaWidget");
    setWindowModality(Qt::ApplicationModal);
    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, ElaThemeColor(ElaThemeType::Light, WindowBaseStart));
    d->_windowLinearGradient->setColorAt(1, ElaThemeColor(ElaThemeType::Light, WindowBaseEnd));

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    d->_appBar->setIsStayTop(true);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::StayTopButtonHint | ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint);
    d->_appBar->setIsDefaultClosed(false);
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, [=]() {
        hide();
    });
    setAttribute(Qt::WA_DeleteOnClose);
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaWidgetPrivate::onThemeModeChanged);
}

ElaWidget::~ElaWidget()
{
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

void ElaWidget::paintEvent(QPaintEvent* event)
{
    Q_D(ElaWidget);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    d->_windowLinearGradient->setFinalStop(width(), height());
    painter.setBrush(*d->_windowLinearGradient);
    painter.drawRect(rect());
    painter.restore();
    QWidget::paintEvent(event);
}
