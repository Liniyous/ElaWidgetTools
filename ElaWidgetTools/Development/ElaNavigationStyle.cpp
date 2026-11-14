#include "ElaNavigationStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaNavigationMarkAnimation.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

ElaNavigationStyle::ElaNavigationStyle(ElaNavigationView* navigationView)
{
    _navigationView = navigationView;
    setParent(navigationView);
    _pRotate = 0;
    _pItemHeight = 38;
    _markAnimation = new ElaNavigationMarkAnimation(navigationView->viewport(), false, this);
    _rotateAnimation = new QPropertyAnimation(this, "pRotate", this);
    _rotateAnimation->setDuration(150);
    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(QPointF(0.167, 0.167), QPointF(0, 1), QPointF(1, 1));
    _rotateAnimation->setEasingCurve(easing);
    connect(_rotateAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        _navigationView->viewport()->update();
    });
    connect(_rotateAnimation, &QPropertyAnimation::finished, this, [this]() {
        _expandAnimationTargetNode.clear();
    });
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaNavigationStyle::~ElaNavigationStyle()
{
}

void ElaNavigationStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        // Item背景
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            QModelIndex index = vopt->index;
            ElaNavigationNode* node = static_cast<ElaNavigationNode*>(index.internalPointer());
            if (node->getIsCategoryNode())
            {
                return;
            }
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            itemRect.setTop(itemRect.top() + 2);
            itemRect.setBottom(itemRect.bottom() - 2);
            QPainterPath path;
            path.addRoundedRect(itemRect, 5, 5);
            const QColor background = ElaControlStyle::getItemBackgroundColor(_themeMode, vopt->state, index == _pPressIndex);
            painter->fillPath(path, ElaColorAnimation::getColor(widget, background, "ItemBackground", vopt->index));
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        // 行背景
        return;
    }
    case QStyle::PE_IndicatorBranch:
    {
        // Branch指示器
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaNavigationStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    int fontPixelSize = eApp->getFontPixelSize();
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 背景绘制
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

            // 内容绘制
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            ElaNavigationNode* node = static_cast<ElaNavigationNode*>(vopt->index.internalPointer());
            ElaNavigationModel* model = dynamic_cast<ElaNavigationModel*>(const_cast<QAbstractItemModel*>(vopt->index.model()));

            _markAnimation->paint(painter, itemRect, node, node == model->getSelectedNode() || node == model->getSelectedExpandedNode(), ElaThemeColor(_themeMode, PrimaryNormal));

            // 图标绘制
            painter->setPen(vopt->index == _pPressIndex ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText));
            if (node->getAwesome() != ElaIconType::None)
            {
                painter->save();
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(fontPixelSize + 4);
                painter->setFont(iconFont);
                painter->drawText(QRect(itemRect.x(), itemRect.y(), _iconAreaWidth, itemRect.height()), Qt::AlignCenter, QChar(node->getAwesome()));
                painter->restore();
            }

            if (itemRect.width() <= _iconAreaWidth + _indicatorIconAreaWidth)
            {
                painter->restore();
                return;
            }
            // 文字绘制
            if (node->getIsCategoryNode())
            {
                QFont categoryFont = painter->font();
                categoryFont.setBold(true);
                painter->setFont(categoryFont);
                painter->setPen(ElaThemeColor(_themeMode, BasicTextNoFocus));
            }
            else
            {
                painter->setPen((vopt->index == _pPressIndex) ? ElaThemeColor(_themeMode, BasicTextNoFocus) : ElaThemeColor(_themeMode, BasicText));
            }
            QRect textRect;
            if (node->getAwesome() != ElaIconType::None)
            {
                textRect = QRect(itemRect.x() + _iconAreaWidth, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _iconAreaWidth, itemRect.height());
            }
            else
            {
                textRect = QRect(itemRect.x() + _leftPadding, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _leftPadding, itemRect.height());
            }
            QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

            // 展开图标 KeyPoints
            if (node->getIsExpanderNode())
            {
                if (node->getIsHasChild())
                {
                    QRectF expandIconRect(itemRect.right() - _indicatorIconAreaWidth, itemRect.y(), 17, itemRect.height());
                    painter->save();
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(fontPixelSize + 4);
                    painter->setFont(iconFont);
                    painter->translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
                    if (node == _expandAnimationTargetNode)
                    {
                        painter->rotate(_pRotate);
                    }
                    else
                    {
                        if (node->getIsExpanded())
                        {
                            //展开
                            painter->rotate(-180);
                        }
                        else
                        {
                            // 未展开
                            painter->rotate(0);
                        }
                    }
                    painter->translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2 + 1, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
                    painter->drawText(expandIconRect, Qt::AlignVCenter, QChar(ElaIconType::AngleDown));
                    painter->restore();
                }
                if (node->getIsChildHasKeyPoints())
                {
                    painter->save();
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                    painter->drawEllipse(QPoint(itemRect.right() - 17, itemRect.y() + 12), 3, 3);
                    painter->restore();
                }
            }
            else
            {
                int keyPoints = node->getKeyPoints();
                if (keyPoints)
                {
                    // KeyPoints
                    painter->save();
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                    int keyPointRadius = 8;
                    painter->drawEllipse(QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), keyPointRadius, keyPointRadius);
                    painter->setPen(QPen(ElaThemeColor(_themeMode, BasicTextInvert), 2));
                    QFont font = painter->font();
                    font.setBold(true);
                    if (keyPoints > 99)
                    {
                        keyPoints = 99;
                    }
                    if (keyPoints > 9)
                    {
                        font.setPixelSize(fontPixelSize - 3);
                    }
                    else
                    {
                        font.setPixelSize(fontPixelSize - 2);
                    }
                    painter->setFont(font);
                    painter->drawText(QRect(QPoint(itemRect.right() - 26 - keyPointRadius, itemRect.y() + itemRect.height() / 2 - keyPointRadius), QSize(2 * keyPointRadius, 2 * keyPointRadius)), Qt::AlignCenter, QString::number(keyPoints));
                    painter->restore();
                }
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

QSize ElaNavigationStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(_pItemHeight);
        return itemSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void ElaNavigationStyle::startExpandAnimation(ElaNavigationNode* node, bool isExpanded)
{
    const qreal startAngle = node == _expandAnimationTargetNode ? _pRotate : isExpanded ? 0
                                                                                        : -180;
    _rotateAnimation->stop();
    if (!_navigationView->isAnimated() || !ElaColorAnimation::getIsAnimationEnabled(_navigationView->viewport()))
    {
        _expandAnimationTargetNode.clear();
        _navigationView->viewport()->update();
        return;
    }
    _expandAnimationTargetNode = node;
    _rotateAnimation->setStartValue(startAngle);
    _rotateAnimation->setEndValue(isExpanded ? -180 : 0);
    _rotateAnimation->start();
}

void ElaNavigationStyle::startSelectionAnimation(ElaNavigationNode* previousNode, ElaNavigationNode* selectedNode)
{
    _markAnimation->start(previousNode, selectedNode);
}
