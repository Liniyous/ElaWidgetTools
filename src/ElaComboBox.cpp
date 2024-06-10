#include "ElaComboBox.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>

#include "Def.h"
#include "DeveloperComponents/ElaComboBoxDelegate.h"
#include "DeveloperComponents/ElaComboBoxListView.h"
#include "ElaApplication.h"
#include "private/ElaComboBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaComboBox, int, BorderRadius)
ElaComboBox::ElaComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new ElaComboBoxPrivate())
{
    Q_D(ElaComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    setObjectName("ElaComboBox");
    setStyleSheet("#ElaComboBox{background-color:transparent;}");
    setMaximumHeight(35);

    d->_comboView = new ElaComboBoxListView(this);
    d->_comboView->setItemDelegate(new ElaComboBoxDelegate(this));
    setView(d->_comboView);
    d->_comboView->setSelectionMode(QAbstractItemView::NoSelection);
    view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
    QComboBox::setMaxVisibleItems(5);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaComboBox::~ElaComboBox()
{
}

void ElaComboBox::setMaxVisibleItems(int maxItems)
{
    Q_D(ElaComboBox);
    d->_comboView->setMaximumHeight(40 * maxItems);
    QComboBox::setMaxVisibleItems(maxItems);
}

void ElaComboBox::paintEvent(QPaintEvent* event)
{
    Q_D(ElaComboBox);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
        painter.setBrush(underMouse() ? QColor(0xF6, 0xF6, 0xF6) : QColor(0xFD, 0xFD, 0xFD));
    }
    else
    {
        painter.setPen(QPen(QColor(0x4B, 0x4B, 0x4D), 1));
        painter.setBrush(underMouse() ? QColor(0x44, 0x44, 0x44) : QColor(0x3B, 0x3B, 0x3B));
    }
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.setPen(d->_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter.drawText(10, height() / 2 + painter.fontMetrics().ascent() / 2 - 1, this->currentText());
    // 图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    if (view()->isVisible())
    {
        // 展开
        painter.drawText(width() * 0.95 - iconFont.pixelSize() - 1, height() / 2 + iconFont.pixelSize() / 2 - 1, QChar((unsigned short)ElaIconType::AngleUp));
    }
    else
    {
        // 未展开
        painter.drawText(width() * 0.95 - iconFont.pixelSize() - 1, height() / 2 + iconFont.pixelSize() / 2 - 1, QChar((unsigned short)ElaIconType::AngleDown));
    }
    painter.restore();
}

void ElaComboBox::showPopup()
{
    Q_D(ElaComboBox);
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);

    QWidget* container = this->view()->parentWidget();
    container->move(container->pos().x(), container->pos().y() + 3);
    QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(d->_comboView, "pos");
    QPoint viewPos = QPoint(0, 0);
    viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - d->_comboView->height()));
    viewPosAnimation->setEndValue(viewPos);
    viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
    viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(container, "windowOpacity");
    opacityAnimation->setStartValue(0);
    opacityAnimation->setEndValue(1);
    opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaComboBox::hidePopup()
{
    Q_D(ElaComboBox);
    if (d->_isHidePopupAnimationFinished)
    {
        QWidget* container = this->view()->parentWidget();
        QPropertyAnimation* opcaityAnimation = new QPropertyAnimation(container, "windowOpacity");
        connect(opcaityAnimation, &QPropertyAnimation::finished, this, [=]() {
            QComboBox::hidePopup();
            container->setWindowOpacity(1);
            d->_isHidePopupAnimationFinished = true; });
        opcaityAnimation->setStartValue(1);
        opcaityAnimation->setEndValue(0);
        opcaityAnimation->setEasingCurve(QEasingCurve::InCubic);
        opcaityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(d->_comboView, "pos");
        QPoint viewPos = QPoint(0, 0);
        connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() { d->_comboView->move(viewPos); });
        viewPosAnimation->setStartValue(viewPos);
        viewPosAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - d->_comboView->height()));
        viewPosAnimation->setEasingCurve(QEasingCurve::InCubic);
        viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        d->_isHidePopupAnimationFinished = false;
    }
}
