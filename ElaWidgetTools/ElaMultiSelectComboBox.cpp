#include "ElaMultiSelectComboBox.h"

#include "ElaColorAnimation.h"
#include "ElaComboBoxPopup.h"
#include "ElaComboBoxStyle.h"
#include "ElaComboBoxView.h"
#include "ElaControlAnimation.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "private/ElaMultiSelectComboBoxPrivate.h"

#include <QApplication>
#include <QHelpEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionComboBox>
#include <QToolTip>

ELA_PROPERTY_Q_CPP(ElaMultiSelectComboBox, int, BorderRadius)
ElaMultiSelectComboBox::ElaMultiSelectComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new ElaMultiSelectComboBoxPrivate())
{
    Q_D(ElaMultiSelectComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 4;
    d->_themeMode = eTheme->getThemeMode();
    setMinimumHeight(32);
    setMouseTracking(true);

    d->_comboBoxStyle = new ElaComboBoxStyle(style());
    d->_comboBoxStyle->setParent(this);
    setStyle(d->_comboBoxStyle);
    d->_controlAnimation = new ElaControlAnimation(this);
    d->_comboBoxStyle->setControlAnimation(d->_controlAnimation);

    //调用view 让container初始化
    d->_comboView = new ElaComboBoxView(this);
    setView(d->_comboView);
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setFrameShape(QFrame::NoFrame);
    comboBoxView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ElaScrollBar* scrollBar = new ElaScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setSelectionBehavior(QAbstractItemView::SelectRows);
    comboBoxView->setObjectName("ElaComboBoxView");
    comboBoxView->setStyleSheet("#ElaComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->_comboBoxStyle);
    d->_popup = new ElaComboBoxPopup(this);
    d->_comboBoxStyle->setPopup(d->_popup);
    connect(d->_popup, &ElaComboBoxPopup::closeFinished, this, [this]() {
        QComboBox::hidePopup();
    });
    connect(d->_comboView, &ElaComboBoxView::itemPressed, d, &ElaMultiSelectComboBoxPrivate::onItemPressed);
    connect(d->_comboView, &ElaComboBoxView::modelChanged, d, &ElaMultiSelectComboBoxPrivate::onModelChanged);
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), d, &ElaMultiSelectComboBoxPrivate::onCurrentIndexChanged);
    connect(this, &ElaMultiSelectComboBox::pBorderRadiusChanged, this, [this]() {
        update();
    });
    d->onModelChanged();
    QComboBox::setMaxVisibleItems(15);
    connect(eTheme, &ElaTheme::themeModeChanged, d, [this, d](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaMultiSelectComboBox::~ElaMultiSelectComboBox() = default;

void ElaMultiSelectComboBox::setCurrentSelection(const QString& selection)
{
    setCurrentSelection(QStringList{selection});
}

void ElaMultiSelectComboBox::setCurrentSelection(const QStringList& selection)
{
    QList<int> rows;
    for (int row = 0; row < count(); ++row)
    {
        if (selection.contains(itemText(row)))
        {
            rows.append(row);
        }
    }
    setCurrentSelection(rows);
}

void ElaMultiSelectComboBox::setCurrentSelection(int index)
{
    if (index >= 0 && index < count())
    {
        setCurrentSelection(QList<int>{index});
    }
}

void ElaMultiSelectComboBox::setCurrentSelection(const QList<int>& selectionIndex)
{
    Q_D(ElaMultiSelectComboBox);
    d->_setSelection(selectionIndex);
}

const QStringList& ElaMultiSelectComboBox::getCurrentSelection() const
{
    Q_D(const ElaMultiSelectComboBox);
    return d->_selectedTextList;
}

QList<int> ElaMultiSelectComboBox::getCurrentSelectionIndex() const
{
    Q_D(const ElaMultiSelectComboBox);
    QList<int> rows;
    for (const QModelIndex& index: d->_getSelectedIndexes())
    {
        rows.append(index.row());
    }
    return rows;
}

bool ElaMultiSelectComboBox::event(QEvent* event)
{
    Q_D(ElaMultiSelectComboBox);
    if (event->type() == QEvent::ToolTip)
    {
        const QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        for (const auto& tag: d->_getSelectionTags())
        {
            if (tag.rect.contains(helpEvent->pos()))
            {
                QToolTip::showText(helpEvent->globalPos(), tag.toolTip, this, tag.rect);
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseMove || event->type() == QEvent::Leave)
    {
        update();
    }
    return QComboBox::event(event);
}

void ElaMultiSelectComboBox::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaMultiSelectComboBox);
    d->_pressedTagIndex = QModelIndex();
    d->_isPopupPressed = false;
    if (event->button() == Qt::LeftButton && isEnabled())
    {
        for (const auto& tag: d->_getSelectionTags())
        {
            if (tag.index.isValid() && tag.closeRect.contains(event->pos()))
            {
                d->_pressedTagIndex = tag.index;
                setFocus(Qt::MouseFocusReason);
                event->accept();
                update();
                return;
            }
        }
    }
    if (event->button() == Qt::LeftButton && isEnabled())
    {
        d->_isPopupPressed = true;
        setFocus(Qt::MouseFocusReason);
        event->accept();
        update();
        return;
    }
    QComboBox::mousePressEvent(event);
}

void ElaMultiSelectComboBox::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaMultiSelectComboBox);
    if (event->button() == Qt::LeftButton && d->_pressedTagIndex.isValid())
    {
        const QPersistentModelIndex pressedIndex = d->_pressedTagIndex;
        d->_pressedTagIndex = QModelIndex();
        for (const auto& tag: d->_getSelectionTags())
        {
            if (tag.index == pressedIndex && tag.closeRect.contains(event->pos()))
            {
                d->onItemPressed(tag.index);
                break;
            }
        }
        event->accept();
        update();
        return;
    }
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

void ElaMultiSelectComboBox::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaMultiSelectComboBox);
    if (event->key() == Qt::Key_Backspace && !isEditable() && !d->_popup->getIsVisible())
    {
        const QModelIndexList indexes = d->_getSelectedIndexes();
        if (!indexes.isEmpty())
        {
            d->onItemPressed(indexes.last());
        }
        event->accept();
        return;
    }
    QComboBox::keyPressEvent(event);
}

void ElaMultiSelectComboBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    Q_D(ElaMultiSelectComboBox);
    QStyleOptionComboBox option;
    initStyleOption(&option);
    option.currentText.clear();
    option.currentIcon = QIcon();
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    style()->drawComplexControl(QStyle::CC_ComboBox, &option, &painter, this);
    const auto tags = d->_getSelectionTags();
    if (tags.isEmpty())
    {
        QString placeholder = "请选择";
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        if (!placeholderText().isEmpty())
        {
            placeholder = placeholderText();
        }
#endif
        const QRect textRect = style()->subControlRect(QStyle::CC_ComboBox, &option, QStyle::SC_ComboBoxEditField, this);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
        painter.drawText(textRect, Qt::AlignLeading | Qt::AlignVCenter, fontMetrics().elidedText(d->_getSingleLineText(placeholder), Qt::ElideRight, qMax(0, textRect.width())));
    }
    painter.setClipRect(QStyle::visualRect(layoutDirection(), rect(), rect().adjusted(6, 3, -30, -3)));
    const QPoint cursorPosition = mapFromGlobal(QCursor::pos());
    for (const auto& tag: tags)
    {
        const bool isCloseHovered = isEnabled() && underMouse() && tag.closeRect.contains(cursorPosition);
        painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
        painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(d->_themeMode, BasicHoverAlpha), "TagBackground", tag.index));
        painter.drawRoundedRect(QRectF(tag.rect).adjusted(0.5, 0.5, -0.5, -0.5), 4, 4);
        QRect textRect = tag.rect.adjusted(7, 0, -7, 0);
        if (tag.index.isValid())
        {
            if (layoutDirection() == Qt::RightToLeft)
            {
                textRect.setLeft(tag.closeRect.right() + 1);
            }
            else
            {
                textRect.setRight(tag.closeRect.left() - 1);
            }
            painter.setPen(Qt::NoPen);
            painter.setBrush(ElaColorAnimation::getColor(this, isCloseHovered ? ElaThemeColor(d->_themeMode, BasicHoverAlpha) : QColor(Qt::transparent), "TagClose", tag.index));
            painter.drawRoundedRect(tag.closeRect.adjusted(2, 2, -2, -2), 3, 3);
            const QPointF center = QRectF(tag.closeRect).center();
            painter.setPen(QPen((isEnabled() ? ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable)), 1.1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(center + QPointF(-2.5, -2.5), center + QPointF(2.5, 2.5));
            painter.drawLine(center + QPointF(-2.5, 2.5), center + QPointF(2.5, -2.5));
        }
        painter.setPen(isEnabled() ? ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable));
        painter.drawText(textRect, Qt::AlignVCenter | (tag.hiddenCount > 0 ? Qt::AlignHCenter : Qt::AlignLeading), tag.text);
    }
}

void ElaMultiSelectComboBox::showPopup()
{
    Q_D(ElaMultiSelectComboBox);
    if (!isEnabled() || count() == 0)
    {
        return;
    }
    const QItemSelection selection = view()->selectionModel()->selection();
    const QList<int> selectedRows = getCurrentSelectionIndex();
    d->_isUpdatingSelection = true;
    d->_popup->prepare();
    const bool oldAnimationEffects = QApplication::isEffectEnabled(Qt::UI_AnimateCombo);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);
    // 多选视图自行处理鼠标释放和键盘切换 原生单选过滤器不能提交或关闭它
    QWidget* container = view()->window();
    view()->removeEventFilter(container);
    view()->viewport()->removeEventFilter(container);
    view()->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
    const int anchorRow = selectedRows.isEmpty() ? 0 : selectedRows.first();
    view()->setCurrentIndex(model()->index(anchorRow, modelColumn(), rootModelIndex()));
    d->_isUpdatingSelection = false;
    d->_popup->open(anchorRow, true);
    d->_controlAnimation->release();
}

void ElaMultiSelectComboBox::hidePopup()
{
    Q_D(ElaMultiSelectComboBox);
    d->_controlAnimation->release();
    if (!d->_popup->close())
    {
        QComboBox::hidePopup();
    }
}
