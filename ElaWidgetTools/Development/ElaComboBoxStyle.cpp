#include "ElaComboBoxStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaComboBox.h"
#include "ElaComboBoxPopup.h"
#include "ElaControlAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QComboBox>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

ElaComboBoxStyle::ElaComboBoxStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaComboBoxStyle::~ElaComboBoxStyle()
{
}

void ElaComboBoxStyle::setControlAnimation(ElaControlAnimation* animation)
{
    _controlAnimation = animation;
}

void ElaComboBoxStyle::setPopup(const ElaComboBoxPopup* popup)
{
    _popup = popup;
}

int ElaComboBoxStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_ComboBox_PopupFrameStyle)
    {
        return QFrame::NoFrame;
    }
    if (hint == QStyle::SH_ComboBox_Popup || hint == QStyle::SH_ComboBox_UseNativePopup)
    {
        return false;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void ElaComboBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_Widget:
    case QStyle::PE_PanelLineEdit:
    {
        return;
    }
#ifndef Q_OS_WIN
    case PE_PanelMenu:
    {
        return;
    }
    case PE_IndicatorArrowDown:
    {
        return;
    }
#endif
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaComboBoxStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ComboBoxLabel:
    {
        return;
    }
    case QStyle::CE_ShapedFrame:
    {
        //container区域
        if (widget && widget->objectName() == "ElaComboBoxContainer")
        {
            const QMargins margins = eTheme->getShadowMargins();
            const bool isAnimating = _popup && _popup->getIsAnimating();
            const QRect foregroundRect = (isAnimating && _popup->getBodyRect().isValid()) ? _popup->getBodyRect() : option->rect.marginsRemoved(margins);
            const QRect shadowRect = foregroundRect.marginsAdded(margins);
            const qreal popupOpacity = isAnimating ? _popup->getPopupOpacity() : 1.0;
            const qreal shadowOpacity = isAnimating ? _popup->getShadowOpacity() : 1.0;
            const int borderRadius = parent() ? qMax(3, parent()->property("pBorderRadius").toInt()) : 4;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            if (shadowOpacity > 0.005)
            {
                eTheme->drawEffectShadow(painter, shadowRect, margins, borderRadius, shadowOpacity);
            }
            const qreal baseOpacity = painter->opacity();
            painter->setOpacity(baseOpacity * popupOpacity);
            painter->setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1));
            painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, PopupBase)));
            painter->drawRoundedRect(QRectF(foregroundRect).adjusted(0.5, 0.5, -0.5, -0.5), borderRadius, borderRadius);
            if (isAnimating && !_popup->getIsDropDown() && _popup->getIsClosing())
            {
                // 原地收拢末段淡入下拉箭头 使收回结束切换回本体时视觉完全连续且不与未退场行重叠
                const qreal closeProgress = 1.0 - _popup->getItemChromeOpacity();
                const qreal chevronAlpha = qBound(0.0, (closeProgress - 0.7) / 0.3, 1.0);
                if (chevronAlpha > 0.005)
                {
                    const QRect rawArrowRect(foregroundRect.right() - 29, foregroundRect.top(), 28, foregroundRect.height());
                    const QRect arrowRect = visualRect(option->direction, foregroundRect, rawArrowRect);
                    const QPointF center = QRectF(arrowRect).center();
                    QPainterPath chevron;
                    chevron.moveTo(center + QPointF(-4, -2));
                    chevron.lineTo(center + QPointF(0, 2));
                    chevron.lineTo(center + QPointF(4, -2));
                    painter->setOpacity(baseOpacity * popupOpacity * chevronAlpha);
                    painter->setPen(QPen(ElaThemeColor(_themeMode, BasicText), 1.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                    painter->setBrush(Qt::NoBrush);
                    painter->drawPath(chevron);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            QStyle::State itemState = vopt->state;
            if (const auto* comboBox = qobject_cast<const ElaComboBox*>(parent()))
            {
                // Qt 列表选区会随悬停和键盘预览移动 选中底色和标记跟随组合框已确认的索引
                const int selectedRow = comboBox->currentIndex();
                itemState.setFlag(QStyle::State_Selected, selectedRow >= 0 && vopt->index == comboBox->model()->index(selectedRow, comboBox->modelColumn(), comboBox->rootModelIndex()));
            }
            const bool isAnimating = _popup && _popup->getIsAnimating();
            const qreal popupOpacity = isAnimating ? _popup->getPopupOpacity() : 1.0;
            const qreal chromeOpacity = isAnimating ? _popup->getItemChromeOpacity() : 1.0;
            int margin = 2;
            painter->save();
            const qreal baseOpacity = painter->opacity();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setOpacity(baseOpacity * popupOpacity * chromeOpacity);
            painter->setPen(Qt::NoPen);
            QPainterPath path;
            QRect optionRect = option->rect;
            optionRect.adjust(margin, margin, -margin, -margin);
#ifndef Q_OS_WIN
            optionRect.adjust(6, 0, -6, 0);
#endif
            path.addRoundedRect(optionRect, 5, 5);
            const QColor background = ElaControlStyle::getItemBackgroundColor(_themeMode, itemState);
            painter->setBrush(ElaColorAnimation::getColor(widget, background, "ItemBackground", vopt->index));
            painter->drawPath(path);
            if (itemState.testFlag(QStyle::State_Selected))
            {
                painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(optionRect.x() + 3, optionRect.y() + optionRect.height() * 0.2, 3, optionRect.height() * 0.6), 1.5, 1.5);
            }
            // 文字绘制
            painter->setOpacity(baseOpacity * popupOpacity);
            painter->setPen(option->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
            QRect textRect = visualRect(vopt->direction, option->rect, option->rect.adjusted(12, 0, -12, 0));
            QString text = vopt->text;
            text.replace('\r', ' ').replace('\n', ' ');
            text = vopt->fontMetrics.elidedText(text, Qt::ElideRight, qMax(0, textRect.width()));
            painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeading, text);
            if (option->state.testFlag(QStyle::State_HasFocus) && option->state.testFlag(QStyle::State_KeyboardFocusChange))
            {
                painter->setOpacity(baseOpacity * popupOpacity * chromeOpacity);
                painter->setBrush(Qt::NoBrush);
                painter->setPen(ElaThemeColor(_themeMode, BasicText));
                painter->drawRoundedRect(optionRect.adjusted(1, 1, -1, -1), 3, 3);
            }
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

void ElaComboBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_ComboBox:
    {
        //主体显示绘制
        if (const QStyleOptionComboBox* copt = qstyleoption_cast<const QStyleOptionComboBox*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
            const bool isEnabled = copt->state.testFlag(QStyle::State_Enabled);
            const bool isPressed = isEnabled && _controlAnimation && _controlAnimation->getIsPressed();
            const int borderRadius = widget ? qMax(0, widget->property("pBorderRadius").toInt()) : 4;
            QColor background = !isEnabled                                       ? ElaThemeColor(_themeMode, BasicDisable)
                : isPressed                                                      ? ElaThemeColor(_themeMode, BasicPress)
                : copt->editable && copt->state.testFlag(QStyle::State_HasFocus) ? ElaThemeColor(_themeMode, PopupBase)
                : copt->state.testFlag(QStyle::State_MouseOver)                  ? ElaThemeColor(_themeMode, BasicHover)
                                                                                 : ElaThemeColor(_themeMode, BasicBase);
            const QRectF comboBoxRect = QRectF(copt->rect).adjusted(0.5, 0.5, -0.5, -0.5);
            if (copt->editable)
            {
                ElaControlStyle::drawInputFrame(painter, copt->rect, borderRadius, _themeMode, copt->state, widget);
            }
            else
            {
                painter->setPen(QPen(ElaControlStyle::getControlBorder(comboBoxRect, _themeMode, isEnabled && !isPressed), 1));
                painter->setBrush(_controlAnimation ? _controlAnimation->getBackgroundColor(background) : background);
                painter->drawRoundedRect(comboBoxRect, borderRadius, borderRadius);
                if (isEnabled && copt->state.testFlag(QStyle::State_HasFocus) && copt->state.testFlag(QStyle::State_KeyboardFocusChange))
                {
                    painter->setBrush(Qt::NoBrush);
                    painter->setPen(ElaThemeColor(_themeMode, BasicText));
                    painter->drawRoundedRect(comboBoxRect.adjusted(2, 2, -2, -2), borderRadius, borderRadius);
                }
            }
            const bool isCovering = !copt->editable && _popup && _popup->getIsCoveringComboBox();
            if (!copt->editable && !isCovering)
            {
                QRect textRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ComboBoxEditField, widget);
                painter->setPen(!isEnabled ? ElaThemeColor(_themeMode, BasicTextDisable) : isPressed ? ElaThemeColor(_themeMode, BasicTextNoFocus)
                                                                                                     : ElaThemeColor(_themeMode, BasicText));
                QString text = copt->currentText;
                text.replace('\r', ' ').replace('\n', ' ');
                if (!copt->currentIcon.isNull())
                {
                    QRect iconRect(textRect.left(), textRect.center().y() - copt->iconSize.height() / 2, copt->iconSize.width(), copt->iconSize.height());
                    iconRect = visualRect(copt->direction, textRect, iconRect);
                    copt->currentIcon.paint(painter, iconRect, Qt::AlignCenter, isEnabled ? QIcon::Normal : QIcon::Disabled);
                    if (copt->direction == Qt::RightToLeft)
                    {
                        textRect.adjust(0, 0, -copt->iconSize.width() - 6, 0);
                    }
                    else
                    {
                        textRect.adjust(copt->iconSize.width() + 6, 0, 0, 0);
                    }
                }
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeading, copt->fontMetrics.elidedText(text, Qt::ElideRight, qMax(0, textRect.width())));
            }
            if (!isCovering)
            {
                const QRect arrowRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ComboBoxArrow, widget);
                const QPointF center = QRectF(arrowRect).center() + QPointF(0, _controlAnimation ? _controlAnimation->getChevronOffset() : 0);
                QPainterPath chevron;
                chevron.moveTo(center + QPointF(-4, -2));
                chevron.lineTo(center + QPointF(0, 2));
                chevron.lineTo(center + QPointF(4, -2));
                painter->setPen(QPen(!isEnabled ? ElaThemeColor(_themeMode, BasicTextDisable) : isPressed ? ElaThemeColor(_themeMode, BasicTextNoFocus)
                                                                                                          : ElaThemeColor(_themeMode, BasicText),
                                     1.25, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter->setBrush(Qt::NoBrush);
                painter->drawPath(chevron);
            }
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect ElaComboBoxStyle::subControlRect(ComplexControl control, const QStyleOptionComplex* option, SubControl subControl, const QWidget* widget) const
{
    if (control == QStyle::CC_ComboBox)
    {
        if (subControl == QStyle::SC_ComboBoxEditField)
        {
            const QRect textRect = option->rect.adjusted(12, 2, -32, -2);
            return visualRect(option->direction, option->rect, textRect);
        }
        if (subControl == QStyle::SC_ComboBoxArrow)
        {
            const QRect arrowRect(option->rect.right() - 29, option->rect.top(), 28, option->rect.height());
            return visualRect(option->direction, option->rect, arrowRect);
        }
    }
    return QProxyStyle::subControlRect(control, option, subControl, widget);
}

QSize ElaComboBoxStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(qMax(ElaWidgetToolsSize::DefaultListItemHeight, option->fontMetrics.height() + 12));
        return itemSize;
    }
    case QStyle::CT_ComboBox:
    {
        QSize comboBoxSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        comboBoxSize.setWidth(comboBoxSize.width() + 26);
        comboBoxSize.setHeight(ElaWidgetToolsSize::DefaultControlHeight);
        return comboBoxSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
