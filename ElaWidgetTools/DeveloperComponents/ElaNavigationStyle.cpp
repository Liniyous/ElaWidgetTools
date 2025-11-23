#include "ElaNavigationStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
#include "ElaTheme.h"
ElaNavigationStyle::ElaNavigationStyle(QStyle* style)
{
    _pOpacity = 1;
    _pItemHeight = 40;
    _pLastSelectMarkTop = 10.0;
    _pLastSelectMarkBottom = 10.0;
    _pSelectMarkTop = 10.0;
    _pSelectMarkBottom = 10.0;

    // Mark向上
    _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "pLastSelectMarkTop");
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _pNavigationView->viewport()->update();
    });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "pSelectMarkBottom");
    connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _pNavigationView->viewport()->update();
    });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkBottomAnimation->setStartValue(0);
        _selectMarkBottomAnimation->setEndValue(10);
        _selectMarkBottomAnimation->start();
    });

    // Mark向下
    _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "pLastSelectMarkBottom");
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _pNavigationView->viewport()->update();
    });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "pSelectMarkTop");
    connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _pNavigationView->viewport()->update();
    });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start();
    });
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
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
            painter->save();
            QModelIndex index = vopt->index;
            ElaNavigationNode* node = static_cast<ElaNavigationNode*>(index.internalPointer());
            if (this->_opacityAnimationTargetNode && node->getParentNode() == this->_opacityAnimationTargetNode)
            {
                painter->setOpacity(_pOpacity);
            }
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            itemRect.setTop(itemRect.top() + 2);
            itemRect.setBottom(itemRect.bottom() - 2);
            QPainterPath path;
            path.addRoundedRect(itemRect, 8, 8);
            if (vopt->state & QStyle::State_Selected)
            {
                if (index == _pPressIndex)
                {
                    // 选中时点击
                    painter->fillPath(path, ElaThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    if (vopt->state & QStyle::State_MouseOver)
                    {
                        // 选中时覆盖
                        painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedHoverAlpha));
                    }
                    else
                    {
                        // 选中
                        painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedAlpha));
                    }
                }
            }
            else
            {
                if (index == _pPressIndex)
                {
                    // 点击时颜色
                    painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedHoverAlpha));
                }
                else
                {
                    if (vopt->state & QStyle::State_MouseOver)
                    {
                        // 覆盖时颜色
                        painter->fillPath(path, ElaThemeColor(_themeMode, BasicHoverAlpha));
                    }
                }
            }
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

            if (this->_opacityAnimationTargetNode && node->getParentNode() == this->_opacityAnimationTargetNode)
            {
                painter->setOpacity(_pOpacity);
            }

            // 选中特效
            if (_isSelectMarkDisplay && (node == model->getSelectedNode() || node == model->getSelectedExpandedNode()))
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _pSelectMarkTop, 3, itemRect.height() - _pSelectMarkTop - _pSelectMarkBottom), 3, 3);
            }
            if (node == _lastSelectedNode)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _pLastSelectMarkTop, 3, itemRect.height() - _pLastSelectMarkTop - _pLastSelectMarkBottom), 3, 3);
            }

            // 图标绘制
            painter->setPen(vopt->index == _pPressIndex ? ElaThemeColor(_themeMode, BasicTextPress) : ElaThemeColor(_themeMode, BasicText));
            if (node->getAwesome() != ElaIconType::None)
            {
                painter->save();
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(17);
                painter->setFont(iconFont);
                painter->drawText(QRect(itemRect.x(), itemRect.y(), _iconAreaWidth, itemRect.height()), Qt::AlignCenter, QChar((unsigned short)node->getAwesome()));
                painter->restore();
            }

            int viewWidth = widget->width();
            // 文字绘制
            painter->setPen(vopt->index == _pPressIndex ? ElaThemeColor(_themeMode, BasicTextPress) : ElaThemeColor(_themeMode, BasicText));
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

            if (viewWidth > _pNavigationView->getNavigationBarPrivate()->_pNavigationBarWidth - _indicatorIconAreaWidth)
            {
                // 展开图标 KeyPoints
                if (node->getIsExpanderNode())
                {
                    if (node->getIsHasChild())
                    {
                        QRectF expandIconRect(itemRect.right() - _indicatorIconAreaWidth, itemRect.y(), 17, itemRect.height());
                        painter->save();
                        QFont iconFont = QFont("ElaAwesome");
                        iconFont.setPixelSize(17);
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
                        painter->drawText(expandIconRect, Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleDown));
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
                            font.setPixelSize(10);
                        }
                        else
                        {
                            font.setPixelSize(11);
                        }
                        painter->setFont(font);
                        painter->drawText(QRect(QPoint(itemRect.right() - 26 - keyPointRadius, itemRect.y() + itemRect.height() / 2 - keyPointRadius), QSize(2 * keyPointRadius, 2 * keyPointRadius)), Qt::AlignCenter, QString::number(keyPoints));
                        painter->restore();
                    }
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

void ElaNavigationStyle::navigationNodeStateChange(QVariantMap data)
{
    if (data.contains("Expand"))
    {
        ElaNavigationNode* lastExpandNode = _expandAnimationTargetNode;
        _opacityAnimationTargetNode = data.value("Expand").value<ElaNavigationNode*>();
        _expandAnimationTargetNode = _opacityAnimationTargetNode;
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
            _opacityAnimationTargetNode = nullptr;
        });
        connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        nodeOpacityAnimation->setDuration(480);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.5, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() {
            _expandAnimationTargetNode = nullptr;
        });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == _expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(_pRotate);
        }
        else
        {
            rotateAnimation->setStartValue(0);
        }
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (data.contains("Collapse"))
    {
        ElaNavigationNode* lastExpandNode = _expandAnimationTargetNode;
        _opacityAnimationTargetNode = data.value("Collapse").value<ElaNavigationNode*>();
        _expandAnimationTargetNode = _opacityAnimationTargetNode;
        _pOpacity = 0;

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() {
            _pOpacity = 1;
            _expandAnimationTargetNode = nullptr;
        });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == _expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(_pRotate);
        }
        else
        {
            rotateAnimation->setStartValue(-180);
        }
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (data.contains("SelectMarkChanged"))
    {
        _lastSelectedNode = data.value("LastSelectedNode").value<ElaNavigationNode*>();
        ElaNavigationNode* selectedNode = data.value("SelectedNode").value<ElaNavigationNode*>();
        bool direction = _compareItemY(selectedNode, _lastSelectedNode);
        _pLastSelectMarkTop = 10;
        _pLastSelectMarkBottom = 10;
        _pSelectMarkTop = 10;
        _pSelectMarkBottom = 10;
        if (direction)
        {
            _lastSelectMarkTopAnimation->setStartValue(10);
            _lastSelectMarkTopAnimation->setEndValue(0);
            _lastSelectMarkTopAnimation->start();
            _lastSelectMarkBottomAnimation->stop();
            _selectMarkTopAnimation->stop();
            _isSelectMarkDisplay = false;
        }
        else
        {
            _lastSelectMarkBottomAnimation->setStartValue(10);
            _lastSelectMarkBottomAnimation->setEndValue(0);
            _lastSelectMarkBottomAnimation->start();
            _lastSelectMarkTopAnimation->stop();
            _selectMarkBottomAnimation->stop();
            _isSelectMarkDisplay = false;
        }
    }
}

bool ElaNavigationStyle::_compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2)
{
    // 返回true 即node1 高于 node2
    if (!node1)
    {
        return false;
    }
    if (!node2)
    {
        return true;
    }
    // 同一父节点
    if (node1->getParentNode() == node2->getParentNode())
    {
        if (node1->getModelIndex().row() < node2->getModelIndex().row())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        ElaNavigationNode* node1OriginalNode = node1->getOriginalNode();
        ElaNavigationNode* node2OriginalNode = node2->getOriginalNode();
        // 不同父节点  相同起源节点
        if (node1OriginalNode == node2OriginalNode)
        {
            int node1Depth = node1->getDepth();
            int node2Depth = node2->getDepth();
            // 相同深度
            if (node1Depth == node2Depth)
            {
                ElaNavigationNode* node1ParentNode = node1->getParentNode();
                ElaNavigationNode* node2ParentNode = node2->getParentNode();
                if (node1ParentNode->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (node1Depth < node2Depth)
                {
                    ElaNavigationNode* node2ParentNode = node2->getParentNode();
                    while (node2ParentNode->getDepth() != node1Depth)
                    {
                        node2ParentNode = node2ParentNode->getParentNode();
                    }
                    // 父子节点关系
                    if (node1 == node2ParentNode)
                    {
                        return true;
                    }
                    if (node1->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    ElaNavigationNode* node1ParentNode = node1->getParentNode();
                    while (node1ParentNode->getDepth() != node2Depth)
                    {
                        node1ParentNode = node1ParentNode->getParentNode();
                    }
                    if (node2 == node1ParentNode)
                    {
                        return false;
                    }
                    if (node1ParentNode->getModelIndex().row() < node2->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            if (node1OriginalNode->getModelIndex().row() < node2OriginalNode->getModelIndex().row())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
