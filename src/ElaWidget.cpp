#include "ElaWidget.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaAppBar.h"
#include "ElaApplication.h"
#include "private/ElaWidgetPrivate.h"
ElaWidget::ElaWidget(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaWidgetPrivate())
{
    Q_D(ElaWidget);
    d->q_ptr = this;
    resize(500, 500); // 默认宽高
    setObjectName("ElaWidget");

    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF2, 0xF2, 0xF9));
    d->_windowLinearGradient->setColorAt(1, QColor(0xF9, 0xEF, 0xF6));

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    d->_appBar->setIsStayTop(true);
    d->_appBar->setWindowButtonFlags(ElaAppBarType::StayTopButtonHint | ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint);
    d->_appBar->setWindowTitle("ElaWidget");
    d->_appBar->setIsDefaultClosed(false);
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, [=]() {
        hide();
    });
    setAttribute(Qt::WA_DeleteOnClose);
    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setSpacing(0);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_mainLayout->addWidget(d->_appBar);
    d->_mainLayout->addStretch();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaWidgetPrivate::onThemeModeChanged);
}

ElaWidget::~ElaWidget()
{
}

void ElaWidget::setCentralWidget(QWidget* centralWidget)
{
    Q_D(ElaWidget);
    d->_mainLayout->takeAt(0);
    d->_mainLayout->takeAt(0);
    if (d->_centralWidget)
    {
        delete d->_centralWidget;
    }
    d->_mainLayout->addWidget(d->_appBar);
    d->_mainLayout->addWidget(centralWidget);
}

void ElaWidget::setWindowTitle(QString title)
{
    Q_D(ElaWidget);
    d->_appBar->setWindowTitle(title);
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
