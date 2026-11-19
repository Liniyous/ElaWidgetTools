#include "ElaTeachingTip.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaPushButton.h"
#include "ElaScrollBar.h"
#include "ElaTeachingTipPrivate.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

#include <QApplication>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>

ElaTeachingTip::ElaTeachingTip(QWidget* parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint), d_ptr(new ElaTeachingTipPrivate())
{
    Q_D(ElaTeachingTip);
    d->q_ptr = this;
    d->_shadowMargins = eTheme->getShadowMargins();
    setObjectName("ElaTeachingTip");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAccessibleName("教学提示");

    d->_contentScrollArea = new QScrollArea(this);
    d->_contentScrollArea->setObjectName("ElaTeachingTipContent");
    d->_contentScrollArea->setStyleSheet("QScrollArea#ElaTeachingTipContent{background:transparent;border:none;}");
    d->_contentScrollArea->setFrameShape(QFrame::NoFrame);
    d->_contentScrollArea->setWidgetResizable(true);
    d->_contentScrollArea->setVerticalScrollBar(new ElaScrollBar(d->_contentScrollArea));
    d->_contentScrollArea->setHorizontalScrollBar(new ElaScrollBar(d->_contentScrollArea));
    d->_contentScrollArea->viewport()->setAutoFillBackground(false);
    d->_contentWidget = new QWidget;
    d->_contentLayout = new QVBoxLayout(d->_contentWidget);
    d->_contentLayout->setContentsMargins(12, 12, 12, 12);
    d->_contentLayout->setSpacing(12);
    d->_contentLayout->setAlignment(Qt::AlignTop);

    d->_headerWidget = new QWidget(d->_contentWidget);
    auto* headerLayout = new QHBoxLayout(d->_headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(12);
    d->_iconText = new ElaText(d->_headerWidget);
    d->_iconText->setFixedSize(20, 24);
    d->_iconText->setTextPixelSize(18);
    headerLayout->addWidget(d->_iconText, 0, Qt::AlignTop);
    auto* textLayout = new QVBoxLayout;
    textLayout->setSpacing(4);
    d->_titleText = new ElaText(d->_headerWidget);
    d->_titleText->setTextPixelSize(eApp->getFontPixelSize() + 1);
    d->_titleText->setIsBold(true);
    d->_subTitleText = new ElaText(d->_headerWidget);
    textLayout->addWidget(d->_titleText);
    textLayout->addWidget(d->_subTitleText);
    headerLayout->addLayout(textLayout, 1);
    d->_closeToolButton = new ElaToolButton(d->_headerWidget);
    d->_closeToolButton->setElaIcon(ElaIconType::Xmark);
    d->_closeToolButton->setFixedWidth(ElaWidgetToolsSize::DefaultControlHeight);
    d->_closeToolButton->setToolTip("关闭提示");
    d->_closeToolButton->setAccessibleName("关闭提示");
    headerLayout->addWidget(d->_closeToolButton, 0, Qt::AlignTop);
    connect(d->_closeToolButton, &ElaToolButton::clicked, d, &ElaTeachingTipPrivate::onCloseButtonClicked);
    d->_contentLayout->addWidget(d->_headerWidget);

    d->_buttonWidget = new QWidget(d->_contentWidget);
    d->_buttonWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    auto* buttonLayout = new QHBoxLayout(d->_buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(8);
    d->_actionPushButton = new ElaPushButton(d->_buttonWidget);
    d->_closePushButton = new ElaPushButton(d->_buttonWidget);
    for (auto* button: {d->_actionPushButton, d->_closePushButton})
    {
        button->setBorderRadius(4);
        buttonLayout->addWidget(button, 1);
    }
    d->_actionPushButton->setIsAccent(true);
    connect(d->_actionPushButton, &ElaPushButton::clicked, this, &ElaTeachingTip::actionButtonClicked);
    connect(d->_closePushButton, &ElaPushButton::clicked, d, &ElaTeachingTipPrivate::onCloseButtonClicked);
    d->_contentLayout->addWidget(d->_buttonWidget);
    d->_contentScrollArea->setWidget(d->_contentWidget);
    d->_contentWidget->setAutoFillBackground(false);

    d->_layoutTimer = new QTimer(d);
    d->_layoutTimer->setSingleShot(true);
    connect(d->_layoutTimer, &QTimer::timeout, d, &ElaTeachingTipPrivate::onLayoutTimeout);
    d->_animation = new QVariantAnimation(d);
    connect(d->_animation, &QVariantAnimation::valueChanged, this, [this, d](const QVariant& value) {
        d->_animationScale = value.toSizeF();
        update();
    });
    connect(d->_animation, &QVariantAnimation::finished, d, &ElaTeachingTipPrivate::onAnimationFinished);
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this, d](ElaThemeType::ThemeMode mode) {
        d->_themeMode = mode;
        if (!d->_animationPix.isNull())
        {
            d->onAnimationFinished();
        }
        else
        {
            update();
        }
    });
    d->_updateContent();
    hide();
}

ElaTeachingTip::~ElaTeachingTip()
{
    Q_D(ElaTeachingTip);
    qApp->removeEventFilter(d);
    disconnect(d->_targetDestroyedConnection);
    disconnect(d->_customDestroyedConnection);
}

void ElaTeachingTip::setTargetWidget(QWidget* targetWidget)
{
    Q_D(ElaTeachingTip);
    if (targetWidget == d->_targetWidget || d->_getIsWidgetWithin(targetWidget, this))
    {
        return;
    }
    disconnect(d->_targetDestroyedConnection);
    d->_targetWidget = targetWidget;
    if (targetWidget)
    {
        d->_targetDestroyedConnection = connect(targetWidget, &QObject::destroyed, this, [this, d]() {
            d->_targetWidget.clear();
            hide();
        });
    }
    d->_layoutTimer->start();
}

QWidget* ElaTeachingTip::getTargetWidget() const
{
    Q_D(const ElaTeachingTip);
    return d->_targetWidget;
}

void ElaTeachingTip::setTitle(const QString& title)
{
    Q_D(ElaTeachingTip);
    d->_titleText->setText(title);
    setAccessibleName(title.isEmpty() ? QString("教学提示") : title);
    d->_updateContent();
}

QString ElaTeachingTip::getTitle() const
{
    Q_D(const ElaTeachingTip);
    return d->_titleText->text();
}

void ElaTeachingTip::setSubTitle(const QString& subTitle)
{
    Q_D(ElaTeachingTip);
    d->_subTitleText->setText(subTitle);
    setAccessibleDescription(subTitle);
    d->_updateContent();
}

QString ElaTeachingTip::getSubTitle() const
{
    Q_D(const ElaTeachingTip);
    return d->_subTitleText->text();
}

void ElaTeachingTip::setElaIcon(ElaIconType::IconName icon)
{
    Q_D(ElaTeachingTip);
    d->_iconText->setElaIcon(icon);
    d->_updateContent();
}

ElaIconType::IconName ElaTeachingTip::getElaIcon() const
{
    Q_D(const ElaTeachingTip);
    return d->_iconText->getElaIcon();
}

void ElaTeachingTip::setCustomWidget(QWidget* customWidget)
{
    Q_D(ElaTeachingTip);
    if (customWidget == d->_customWidget || (customWidget && (d->_getIsWidgetWithin(this, customWidget) || d->_getIsWidgetWithin(customWidget, d->_contentScrollArea))))
    {
        return;
    }
    if (!d->_animationPix.isNull())
    {
        QPointer<ElaTeachingTip> guard(this);
        d->onAnimationFinished();
        if (!guard)
        {
            return;
        }
    }
    disconnect(d->_customDestroyedConnection);
    if (d->_customWidget)
    {
        d->_contentLayout->removeWidget(d->_customWidget);
        d->_customWidget->hide();
        d->_customWidget->deleteLater();
    }
    d->_customWidget = customWidget;
    if (customWidget)
    {
        d->_contentLayout->insertWidget(1, customWidget);
        customWidget->show();
        d->_customDestroyedConnection = connect(customWidget, &QObject::destroyed, this, [this, d]() {
            d->_customWidget.clear();
            d->_layoutTimer->start();
            Q_EMIT pCustomWidgetChanged();
        });
    }
    d->_updateContent();
    Q_EMIT pCustomWidgetChanged();
}

QWidget* ElaTeachingTip::getCustomWidget() const
{
    Q_D(const ElaTeachingTip);
    return d->_customWidget;
}

void ElaTeachingTip::setActionButtonText(const QString& text)
{
    Q_D(ElaTeachingTip);
    d->_actionPushButton->setText(text);
    d->_updateContent();
}

QString ElaTeachingTip::getActionButtonText() const
{
    Q_D(const ElaTeachingTip);
    return d->_actionPushButton->text();
}

void ElaTeachingTip::setCloseButtonText(const QString& text)
{
    Q_D(ElaTeachingTip);
    d->_closePushButton->setText(text);
    d->_updateContent();
}

QString ElaTeachingTip::getCloseButtonText() const
{
    Q_D(const ElaTeachingTip);
    return d->_closePushButton->text();
}

void ElaTeachingTip::setPreferredPlacement(ElaTeachingTipType::PlacementMode placement)
{
    Q_D(ElaTeachingTip);
    if (placement < ElaTeachingTipType::Auto || placement > ElaTeachingTipType::Center)
    {
        return;
    }
    d->_preferredPlacement = placement;
    d->_layoutTimer->start();
}

ElaTeachingTipType::PlacementMode ElaTeachingTip::getPreferredPlacement() const
{
    Q_D(const ElaTeachingTip);
    return d->_preferredPlacement;
}

ElaTeachingTipType::PlacementMode ElaTeachingTip::getEffectivePlacement() const
{
    Q_D(const ElaTeachingTip);
    return d->_effectivePlacement;
}

void ElaTeachingTip::setTipWidth(int width)
{
    Q_D(ElaTeachingTip);
    d->_tipWidth = qMax(160, width);
    d->_layoutTimer->start();
}

int ElaTeachingTip::getTipWidth() const
{
    Q_D(const ElaTeachingTip);
    return d->_tipWidth;
}

void ElaTeachingTip::setIsTailVisible(bool isVisible)
{
    Q_D(ElaTeachingTip);
    d->_isTailVisible = isVisible;
    d->_layoutTimer->start();
}

bool ElaTeachingTip::getIsTailVisible() const
{
    Q_D(const ElaTeachingTip);
    return d->_isTailVisible;
}

void ElaTeachingTip::setIsLightDismissEnabled(bool isEnabled)
{
    Q_D(ElaTeachingTip);
    d->_isLightDismissEnabled = isEnabled;
    d->_updateContent();
}

bool ElaTeachingTip::getIsLightDismissEnabled() const
{
    Q_D(const ElaTeachingTip);
    return d->_isLightDismissEnabled;
}

void ElaTeachingTip::setIsOpen(bool isOpen)
{
    Q_D(ElaTeachingTip);
    if (isOpen == d->_isOpen)
    {
        return;
    }
    if (isOpen && !isVisible())
    {
        if (d->_updateGeometry())
        {
            show();
        }
        return;
    }
    d->_isOpen = isOpen;
    QPointer<ElaTeachingTip> guard(this);
    Q_EMIT pIsOpenChanged();
    if (guard && d->_isOpen == isOpen)
    {
        d->_startAnimation(isOpen);
    }
}

bool ElaTeachingTip::getIsOpen() const
{
    Q_D(const ElaTeachingTip);
    return d->_isOpen;
}

QSize ElaTeachingTip::sizeHint() const
{
    Q_D(const ElaTeachingTip);
    return d->_getBodySize(d->_tipWidth) + QSize(d->_shadowMargins.left() + d->_shadowMargins.right(), d->_shadowMargins.top() + d->_shadowMargins.bottom() + 8);
}

void ElaTeachingTip::showEvent(QShowEvent* event)
{
    Q_D(ElaTeachingTip);
    QWidget::showEvent(event);
    d->_previousFocusWidget = QApplication::focusWidget();
    d->_isPresented = true;
    if (!d->_updateGeometry())
    {
        hide();
        return;
    }
    qApp->installEventFilter(d);
    d->_isOpen = true;
    QPointer<ElaTeachingTip> guard(this);
    Q_EMIT pIsOpenChanged();
    if (guard && d->_isOpen)
    {
        d->_startAnimation(true);
    }
}

void ElaTeachingTip::hideEvent(QHideEvent* event)
{
    Q_D(ElaTeachingTip);
    qApp->removeEventFilter(d);
    d->_layoutTimer->stop();
    const bool wasOpen = d->_isOpen;
    const bool wasPresented = d->_isPresented;
    d->_isOpen = false;
    d->_isPresented = false;
    d->_restoreContent();
    QWidget::hideEvent(event);
    if (d->_previousFocusWidget && d->_previousFocusWidget->isVisible() && (QApplication::focusWidget() == nullptr || d->_getIsWidgetWithin(QApplication::focusWidget(), this)))
    {
        d->_previousFocusWidget->setFocus(Qt::OtherFocusReason);
    }
    d->_previousFocusWidget.clear();
    d->_shadowPressWidget.clear();
    QPointer<ElaTeachingTip> guard(this);
    if (wasOpen)
    {
        Q_EMIT pIsOpenChanged();
    }
    if (guard && wasPresented)
    {
        Q_EMIT closed();
    }
}

void ElaTeachingTip::closeEvent(QCloseEvent* event)
{
    event->ignore();
    setIsOpen(false);
}

void ElaTeachingTip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    Q_D(ElaTeachingTip);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (!d->_animationPix.isNull())
    {
        painter.translate(d->_animationOrigin);
        painter.scale(d->_animationScale.width(), d->_animationScale.height());
        painter.translate(-d->_animationOrigin);
        painter.drawPixmap(QPoint(), d->_animationPix);
        return;
    }
    eTheme->drawEffectShadow(&painter, d->_bubblePath);
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, PopupBase)));
    painter.drawPath(d->_bubblePath);
}
