#include "ElaNavigationDelegate.h"

#include <QApplication>
#include <QMetaEnum>
#include <QPainter>
#include <QPropertyAnimation>

#include "ElaApplication.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
ElaNavigationDelegate::ElaNavigationDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    _pOpacity = 1;
    _pRotate = 0;
    _pNavigationView = nullptr;
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
    setProperty("lastSelectMarkTop", 10.0);
    setProperty("lastSelectMarkBottom", 10.0);
    setProperty("selectMarkTop", 10.0);
    setProperty("selectMarkBottom", 10.0);
    // Mark向上
    _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                _lastSelectMarkTop = value.toReal();
                _pNavigationView->viewport()->update(); });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
    connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                _selectMarkBottom = value.toReal();
                _pNavigationView->viewport()->update(); });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
                _isSelectMarkDisplay = true;
                this->_lastSelectedNode = nullptr;
                _selectMarkBottomAnimation->setStartValue(0);
                _selectMarkBottomAnimation->setEndValue(10);
                _selectMarkBottomAnimation->start(); });

    // Mark向下
    _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                _lastSelectMarkBottom =  value.toReal();
                _pNavigationView->viewport()->update(); });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
    connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                _selectMarkTop =  value.toReal();
                _pNavigationView->viewport()->update(); });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        this->_lastSelectedNode = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start(); });
}

ElaNavigationDelegate::~ElaNavigationDelegate()
{
}

void ElaNavigationDelegate::navigationNodeStateChange(QMap<QString, QVariant> data)
{
    if (data.contains("Expand"))
    {
        ElaNavigationNode* lastExpandNode = _expandAnimationTargetNode;
        _opacityAnimationTargetNode = data.value("Expand").value<ElaNavigationNode*>();
        _expandAnimationTargetNode = _opacityAnimationTargetNode;
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { this->_opacityAnimationTargetNode = nullptr; });
        connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        nodeOpacityAnimation->setDuration(600);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.4, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() { this->_expandAnimationTargetNode = nullptr; });
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
            this->_expandAnimationTargetNode = nullptr; });
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
        _lastSelectMarkTop = 10;
        _lastSelectMarkBottom = 10;
        _selectMarkTop = 10;
        _selectMarkBottom = 10;
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

void ElaNavigationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    ElaNavigationNode* node = static_cast<ElaNavigationNode*>(index.internalPointer());
    ElaNavigationModel* model = dynamic_cast<ElaNavigationModel*>(const_cast<QAbstractItemModel*>(index.model()));
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QRect itemRect = option.rect;
    if (this->_opacityAnimationTargetNode && _opacityAnimationTargetNode->getIsChildNode(node))
    {
        painter->setOpacity(_pOpacity);
    }
    // 图标绘制
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    if (node->getAwesome() != ElaIconType::None)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(itemRect.x() + 11, itemRect.y() + 26, QChar((unsigned short)node->getAwesome()));
    }
    // 展开图标 KeyPoints
    if (node->getIsExpanderNode())
    {
        if (node->getIsHasChild())
        {
            QRectF expandIconRect(247, itemRect.y(), 17, itemRect.height());
            painter->save();
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
            painter->setBrush(QColor(0xFF, 0x4D, 0x4F));
            painter->drawEllipse(QPoint(264, itemRect.y() + 12), 3, 3);
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
            painter->setBrush(Qt::white);
            painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
            painter->setBrush(QColor(0xFF, 0x4D, 0x4F));
            painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);
            painter->setPen(QPen(Qt::white, 2));
            QFont font = QFont("Microsoft YaHei");
            font.setBold(true);
            if (keyPoints > 99)
            {
                keyPoints = 99;
            }
            if (keyPoints > 9)
            {
                font.setPixelSize(11);
            }
            else
            {
                font.setPixelSize(12);
            }
            painter->setFont(font);
            painter->drawText(keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
            painter->restore();
        }
    }

    // 文字绘制
    QFont titlefont("Microsoft YaHei", 10);
    painter->setFont(titlefont);
    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter->drawText(itemRect.x() + 37, itemRect.y() + 25, node->getNodeTitle());
    // 选中特效
    if (_isSelectMarkDisplay && (node == model->getSelectedNode() || node == model->getSelectedExpandedNode()))
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _selectMarkTop, 3, itemRect.height() - _selectMarkTop - _selectMarkBottom), 3, 3);
    }
    if (node == _lastSelectedNode)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _lastSelectMarkTop, 3, itemRect.height() - _lastSelectMarkTop - _lastSelectMarkBottom), 3, 3);
    }
    painter->restore();
}

QSize ElaNavigationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}

bool ElaNavigationDelegate::_compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2)
{
    // 返回TRUE 即node1 高于 node2
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
