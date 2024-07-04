#include "ElaComboBox.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>

#include "Def.h"
#include "DeveloperComponents/ElaComboBoxDelegate.h"
#include "DeveloperComponents/ElaComboBoxView.h"
#include "ElaApplication.h"
#include "private/ElaComboBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaComboBox, int, BorderRadius)
ElaComboBox::ElaComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new ElaComboBoxPrivate())
{
    Q_D(ElaComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_pExpandIconRotate = 0;
    d->_pExpandMarkWidth = 0;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    setObjectName("ElaComboBox");
    setStyleSheet("#ElaComboBox{background-color:transparent;}");
    setMaximumHeight(35);

    d->_comboView = new ElaComboBoxView(false, this);
    d->_comboView->setItemDelegate(new ElaComboBoxDelegate(this));
    d->_comboView->setAutoScroll(false);
    setView(d->_comboView);
    d->_comboView->setSelectionMode(QAbstractItemView::NoSelection);
    QWidget* container = this->findChild<QFrame*>();
    if (container)
    {
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
    }
    view()->setAutoScroll(false);
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
    //展开指示器绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 3, d->_pExpandMarkWidth * 2, 3), 2, 2);
    // 展开图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    painter.setPen(d->_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    QRectF expandIconRect(width() - 25, 0, 20, height());
    painter.translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2 - 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
    painter.rotate(d->_pExpandIconRotate);
    painter.translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2 + 2, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
    painter.drawText(expandIconRect, Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleDown));
    painter.restore();
}

void ElaComboBox::showPopup()
{
    Q_D(ElaComboBox);
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);

    QWidget* container = this->findChild<QFrame*>();
    if (container)
    {
        container->move(container->pos().x(), container->pos().y() + 3);
        QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(d->_comboView, "pos");
        QPoint viewPos = QPoint(0, 0);
        viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - d->_comboView->height()));
        viewPosAnimation->setEndValue(viewPos);
        viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
        viewPosAnimation->setDuration(400);
        viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(container, "windowOpacity");
        opacityAnimation->setStartValue(0);
        opacityAnimation->setEndValue(1);
        opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    //指示器动画
    QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
    connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    rotateAnimation->setDuration(300);
    rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
    rotateAnimation->setStartValue(d->_pExpandIconRotate);
    rotateAnimation->setEndValue(-180);
    rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(d->_pExpandMarkWidth);
    markAnimation->setEndValue(width() / 2 - d->_pBorderRadius);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaComboBox::hidePopup()
{
    Q_D(ElaComboBox);
    if (d->_isHidePopupAnimationFinished)
    {
        QWidget* container = this->findChild<QFrame*>();
        if (container)
        {
            QPropertyAnimation* opcaityAnimation = new QPropertyAnimation(container, "windowOpacity");
            connect(opcaityAnimation, &QPropertyAnimation::finished, this, [=]() {
                QComboBox::hidePopup();
                container->setWindowOpacity(1);
                d->_isHidePopupAnimationFinished = true;
            });
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
        //指示器动画
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->_pExpandIconRotate);
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
