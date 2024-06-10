#include "ElaMultiSelectComboBox.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStandardItemModel>
#include <QTimer>

#include "Def.h"
#include "DeveloperComponents/ElaComboBoxDelegate.h"
#include "DeveloperComponents/ElaComboBoxListView.h"
#include "ElaApplication.h"
#include "private/ElaMultiSelectComboBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaMultiSelectComboBox, int, BorderRadius)
ElaMultiSelectComboBox::ElaMultiSelectComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new ElaMultiSelectComboBoxPrivate())
{
    Q_D(ElaMultiSelectComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    setMaximumHeight(35);
    setMouseTracking(true);
    setInsertPolicy(QComboBox::NoInsert);
    setModel(new QStandardItemModel(this));
    d->_comboView = new ElaComboBoxListView(this);
    d->_comboView->setItemDelegate(new ElaComboBoxDelegate(this));
    setView(d->_comboView);
    d->_comboView->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->_comboView->setSelectionMode(QAbstractItemView::NoSelection);
    connect(d->_comboView, &QAbstractItemView::pressed, d, &ElaMultiSelectComboBoxPrivate::onItemPressed);
    connect(this, QOverload<int>::of(&ElaMultiSelectComboBox::currentIndexChanged), d, &ElaMultiSelectComboBoxPrivate::_refreshCurrentIndexs);
    view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
    view()->setAutoScroll(false);
    d->_itemSelection.resize(32);
    d->_itemSelection.fill(false);
    d->_itemSelection[0] = true;

    QComboBox::setMaxVisibleItems(5);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaMultiSelectComboBox::~ElaMultiSelectComboBox()
{
}

void ElaMultiSelectComboBox::setCurrentSelection(QString selection)
{
    Q_D(ElaMultiSelectComboBox);
    d->_itemSelection.fill(false);
    d->_comboView->selectionModel()->clearSelection();
    for (int i = 0; i < this->count(); i++)
    {
        if (selection == itemText(i))
        {
            QModelIndex index = model()->index(i, 0);
            d->_comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->_itemSelection[index.row()] = true;
        }
    }
}

void ElaMultiSelectComboBox::setCurrentSelection(QStringList selection)
{
    Q_D(ElaMultiSelectComboBox);
    d->_comboView->selectionModel()->clearSelection();
    d->_itemSelection.fill(false);
    for (int i = 0; i < this->count(); i++)
    {
        if (selection.contains(itemText(i)))
        {
            QModelIndex index = model()->index(i, 0);
            d->_comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->_itemSelection[index.row()] = true;
        }
    }
}

void ElaMultiSelectComboBox::setCurrentSelection(int index)
{
    Q_D(ElaMultiSelectComboBox);
    if (index >= this->count() || index < 0)
    {
        return;
    }
    d->_comboView->selectionModel()->clearSelection();
    d->_itemSelection.fill(false);
    QModelIndex currentIndex = model()->index(index, 0);
    d->_comboView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    d->_itemSelection[index] = true;
}

void ElaMultiSelectComboBox::setCurrentSelection(QList<int> selectionIndex)
{
    Q_D(ElaMultiSelectComboBox);
    d->_itemSelection.fill(false);
    d->_comboView->selectionModel()->clearSelection();
    for (auto index : selectionIndex)
    {
        if (index >= this->count() || index < 0)
        {
            continue;
        }
        QModelIndex currentIndex = model()->index(index, 0);
        d->_comboView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        d->_itemSelection[index] = true;
    }
}

QStringList ElaMultiSelectComboBox::getCurrentSelection() const
{
    return d_ptr->_selectedTextList;
}

QList<int> ElaMultiSelectComboBox::getCurrentSelectionIndex() const
{
    QList<int> indexList;
    for (int i = 0; i < d_ptr->_itemSelection.count(); i++)
    {
        if (d_ptr->_itemSelection[i])
        {
            indexList.append(i);
        }
    }
    return indexList;
}

void ElaMultiSelectComboBox::paintEvent(QPaintEvent* e)
{
    Q_D(ElaMultiSelectComboBox);
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
    QString currentText = painter.fontMetrics().elidedText(d->_currentText, Qt::ElideRight, foregroundRect.width() - 27 - width() * 0.05);
    painter.drawText(10, height() / 2 + painter.fontMetrics().ascent() / 2 - 1, currentText);
    // 图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    if (view()->isVisible())
    {
        // 展开
        painter.drawText(width() * 0.95 - iconFont.pixelSize(), height() / 2 + iconFont.pixelSize() / 2 - 1, QChar((unsigned short)ElaIconType::AngleUp));
    }
    else
    {
        // 未展开
        painter.drawText(width() * 0.95 - iconFont.pixelSize(), height() / 2 + iconFont.pixelSize() / 2 - 1, QChar((unsigned short)ElaIconType::AngleDown));
    }
    painter.restore();
}

void ElaMultiSelectComboBox::showPopup()
{
    Q_D(ElaMultiSelectComboBox);
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

void ElaMultiSelectComboBox::hidePopup()
{
    Q_D(ElaMultiSelectComboBox);
    if (d->_isFirstPopup && !this->view()->underMouse())
    {
        d->_isFirstPopup = false;
        return;
    }
    if (this->view()->underMouse())
    {
        return;
    }
    else
    {
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
}
