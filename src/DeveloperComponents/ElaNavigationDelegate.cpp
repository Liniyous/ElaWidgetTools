#include "ElaNavigationDelegate.h"

#include <QApplication>
#include <QMetaEnum>
#include <QPainter>
#include <QPropertyAnimation>

#include "ElaApplication.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationView.h"
#include "private/ElaNavigationDelegatePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationDelegate, qreal, Opacity);
Q_PROPERTY_CREATE_Q_CPP(ElaNavigationDelegate, qreal, Rotate);
Q_PRIVATE_CREATE_Q_CPP(ElaNavigationDelegate, ElaNavigationView*, NavigationView);
ElaNavigationDelegate::ElaNavigationDelegate(QObject* parent)
    : QStyledItemDelegate(parent), d_ptr(new ElaNavigationDelegatePrivate())
{
    Q_D(ElaNavigationDelegate);
    d->q_ptr = this;
    d->_pOpacity = 1;
    d->_pRotate = 0;
    d->_pNavigationView = nullptr;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { d->_themeMode = themeMode; });
    setProperty("lastSelectMarkTop", 10.0);
    setProperty("lastSelectMarkBottom", 10.0);
    setProperty("selectMarkTop", 10.0);
    setProperty("selectMarkBottom", 10.0);
    // Mark向上
    d->_lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
    connect(d->_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                d->_lastSelectMarkTop = value.toReal();
                d->_pNavigationView->viewport()->update(); });
    d->_lastSelectMarkTopAnimation->setDuration(300);
    d->_lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->_selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
    connect(d->_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                d->_selectMarkBottom = value.toReal();
                d->_pNavigationView->viewport()->update(); });
    d->_selectMarkBottomAnimation->setDuration(300);
    d->_selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isSelectMarkDisplay = true;
                d->_lastSelectedNode = nullptr;
                d->_selectMarkBottomAnimation->setStartValue(0);
                d->_selectMarkBottomAnimation->setEndValue(10);
                d->_selectMarkBottomAnimation->start(); });

    // Mark向下
    d->_lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
    connect(d->_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                d->_lastSelectMarkBottom =  value.toReal();
                d->_pNavigationView->viewport()->update(); });
    d->_lastSelectMarkBottomAnimation->setDuration(300);
    d->_lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->_selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
    connect(d->_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                d->_selectMarkTop =  value.toReal();
                d->_pNavigationView->viewport()->update(); });
    d->_selectMarkTopAnimation->setDuration(300);
    d->_selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_isSelectMarkDisplay = true;
        d->_lastSelectedNode = nullptr;
        d->_selectMarkTopAnimation->setStartValue(0);
        d->_selectMarkTopAnimation->setEndValue(10);
        d->_selectMarkTopAnimation->start(); });
}

ElaNavigationDelegate::~ElaNavigationDelegate()
{
}

void ElaNavigationDelegate::navigationNodeStateChange(QVariantMap data)
{
    Q_D(ElaNavigationDelegate);
    if (data.contains("Expand"))
    {
        ElaNavigationNode* lastExpandNode = d->_expandAnimationTargetNode;
        d->_opacityAnimationTargetNode = data.value("Expand").value<ElaNavigationNode*>();
        d->_expandAnimationTargetNode = d->_opacityAnimationTargetNode;
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { d->_opacityAnimationTargetNode = nullptr; });
        connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            d->_pNavigationView->viewport()->update();
        });
        nodeOpacityAnimation->setDuration(600);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.4, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() { d->_expandAnimationTargetNode = nullptr; });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            d->_pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == d->_expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(d->_pRotate);
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
        ElaNavigationNode* lastExpandNode = d->_expandAnimationTargetNode;
        d->_opacityAnimationTargetNode = data.value("Collapse").value<ElaNavigationNode*>();
        d->_expandAnimationTargetNode = d->_opacityAnimationTargetNode;
        d->_pOpacity = 0;

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() {
            d->_pOpacity = 1;
            d->_expandAnimationTargetNode = nullptr; });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            d->_pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == d->_expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(d->_pRotate);
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
        d->_lastSelectedNode = data.value("LastSelectedNode").value<ElaNavigationNode*>();
        ElaNavigationNode* selectedNode = data.value("SelectedNode").value<ElaNavigationNode*>();
        bool direction = d->_compareItemY(selectedNode, d->_lastSelectedNode);
        d->_lastSelectMarkTop = 10;
        d->_lastSelectMarkBottom = 10;
        d->_selectMarkTop = 10;
        d->_selectMarkBottom = 10;
        if (direction)
        {
            d->_lastSelectMarkTopAnimation->setStartValue(10);
            d->_lastSelectMarkTopAnimation->setEndValue(0);
            d->_lastSelectMarkTopAnimation->start();
            d->_lastSelectMarkBottomAnimation->stop();
            d->_selectMarkTopAnimation->stop();
            d->_isSelectMarkDisplay = false;
        }
        else
        {
            d->_lastSelectMarkBottomAnimation->setStartValue(10);
            d->_lastSelectMarkBottomAnimation->setEndValue(0);
            d->_lastSelectMarkBottomAnimation->start();
            d->_lastSelectMarkTopAnimation->stop();
            d->_selectMarkBottomAnimation->stop();
            d->_isSelectMarkDisplay = false;
        }
    }
}

void ElaNavigationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const ElaNavigationDelegate);
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
    if (d->_opacityAnimationTargetNode && d->_opacityAnimationTargetNode->getIsChildNode(node))
    {
        painter->setOpacity(d->_pOpacity);
    }
    // 图标绘制
    painter->setPen(d->_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
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
            if (node == d->_expandAnimationTargetNode)
            {
                painter->rotate(d->_pRotate);
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
            QFont font = QFont("Microsoft YaHei", 10);
            font.setHintingPreference(QFont::PreferNoHinting);
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
    titlefont.setHintingPreference(QFont::PreferNoHinting);
    painter->setFont(titlefont);
    painter->setPen(d->_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter->drawText(itemRect.x() + 37, itemRect.y() + 25, node->getNodeTitle());
    // 选中特效
    if (d->_isSelectMarkDisplay && (node == model->getSelectedNode() || node == model->getSelectedExpandedNode()))
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + d->_selectMarkTop, 3, itemRect.height() - d->_selectMarkTop - d->_selectMarkBottom), 3, 3);
    }
    if (node == d->_lastSelectedNode)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0x0E, 0x6F, 0xC3));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + d->_lastSelectMarkTop, 3, itemRect.height() - d->_lastSelectMarkTop - d->_lastSelectMarkBottom), 3, 3);
    }
    painter->restore();
}

QSize ElaNavigationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}
