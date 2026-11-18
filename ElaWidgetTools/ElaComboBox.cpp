#include "ElaComboBox.h"

#include "ElaComboBoxPopup.h"
#include "ElaComboBoxStyle.h"
#include "ElaControlAnimation.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "private/ElaComboBoxPrivate.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QLineEdit>
#include <QListView>
#include <QMouseEvent>
#include <QStyleOptionComboBox>

ELA_PROPERTY_Q_CPP(ElaComboBox, int, BorderRadius)
ElaComboBox::ElaComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new ElaComboBoxPrivate())
{
    Q_D(ElaComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 4;
    d->_themeMode = eTheme->getThemeMode();
    setObjectName("ElaComboBox");
    setMinimumHeight(32);
    d->_comboBoxStyle = new ElaComboBoxStyle(style());
    d->_comboBoxStyle->setParent(this);
    setStyle(d->_comboBoxStyle);
    d->_controlAnimation = new ElaControlAnimation(this);
    d->_comboBoxStyle->setControlAnimation(d->_controlAnimation);

    //调用view 让container初始化
    setView(new QListView(this));
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setFrameShape(QFrame::NoFrame);
    comboBoxView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ElaScrollBar* scrollBar = new ElaScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::SingleSelection);
    comboBoxView->setObjectName("ElaComboBoxView");
    comboBoxView->setStyleSheet("#ElaComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->_comboBoxStyle);
    d->_popup = new ElaComboBoxPopup(this);
    d->_comboBoxStyle->setPopup(d->_popup);
    connect(d->_popup, &ElaComboBoxPopup::closeFinished, this, [this]() {
        QComboBox::hidePopup();
    });
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        // 旧标记可能位于列表当前选区之外 确认新值时一并刷新
        view()->viewport()->update();
    });
    connect(this, &ElaComboBox::pBorderRadiusChanged, this, [this]() {
        update();
    });
    QComboBox::setMaxVisibleItems(15);
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaComboBoxPrivate::onThemeChanged);
}

ElaComboBox::~ElaComboBox() = default;

void ElaComboBox::setEditable(bool editable)
{
    Q_D(ElaComboBox);
    QComboBox::setEditable(editable);
    if (editable)
    {
        lineEdit()->setStyle(d->_comboBoxStyle);
        d->onThemeChanged(d->_themeMode);
    }
}

void ElaComboBox::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaComboBox);
    QStyleOptionComboBox option;
    initStyleOption(&option);
    const QRect arrowRect = style()->subControlRect(QStyle::CC_ComboBox, &option, QStyle::SC_ComboBoxArrow, this);
    d->_isPopupPressed = event->button() == Qt::LeftButton && isEnabled() && (!isEditable() || arrowRect.contains(event->pos()));
    if (d->_isPopupPressed)
    {
        setFocus(Qt::MouseFocusReason);
        event->accept();
        update();
        return;
    }
    QComboBox::mousePressEvent(event);
}

void ElaComboBox::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaComboBox);
    if (event->button() == Qt::LeftButton && d->_isPopupPressed)
    {
        d->_isPopupPressed = false;
        if (isEnabled() && rect().contains(event->pos()))
        {
            if (d->_popup->getIsVisible())
            {
                hidePopup();
            }
            else
            {
                showPopup();
            }
        }
        event->accept();
        return;
    }
    QComboBox::mouseReleaseEvent(event);
}

void ElaComboBox::showPopup()
{
    Q_D(ElaComboBox);
    if (!isEnabled() || count() == 0)
    {
        return;
    }
    d->_popup->prepare();
    const bool oldAnimationEffects = QApplication::isEffectEnabled(Qt::UI_AnimateCombo);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);
    d->_popup->open(currentIndex() < 0 ? count() / 2 : currentIndex());
    d->_controlAnimation->release();
}

void ElaComboBox::hidePopup()
{
    Q_D(ElaComboBox);
    d->_controlAnimation->release();
    if (!d->_popup->close())
    {
        QComboBox::hidePopup();
    }
}

void ElaComboBox::paintEvent(QPaintEvent* event)
{
    Q_D(ElaComboBox);
    if (lineEdit() && lineEdit()->palette().color(QPalette::Text) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QComboBox::paintEvent(event);
}
