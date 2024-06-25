#include "ElaNavigationStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "ElaApplication.h"
#include "ElaNavigationNode.h"
ElaNavigationStyle::ElaNavigationStyle(QStyle* style)
{
    _hovergradient = new QLinearGradient(0, 0, 290, 38);
    _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedgradient = new QLinearGradient(0, 0, 290, 38);
    _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
    _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
    _selectedHovergradient = new QLinearGradient(0, 0, 290, 38);
    _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
    _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    _pOpacity = 1;
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaNavigationStyle::onThemeChanged);
}

ElaNavigationStyle::~ElaNavigationStyle()
{
    delete this->_hovergradient;
    delete this->_selectedgradient;
    delete this->_selectedHovergradient;
}

void ElaNavigationStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    switch (pe)
    {
    case PE_PanelItemViewItem:
    {
        drawPanelItemViewItem(opt, p, w);
        break;
    }
    default:
    {
        QProxyStyle::drawPrimitive(pe, opt, p, w);
        break;
    }
    }
}

void ElaNavigationStyle::drawPanelItemViewItem(const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    p->save();
    if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt))
    {
        ElaNavigationNode* node = static_cast<ElaNavigationNode*>(vopt->index.internalPointer());
        if (this->_animationTargetNode && node->getParentNode() == this->_animationTargetNode)
        {
            p->setOpacity(_pOpacity);
        }
        p->setRenderHint(QPainter::Antialiasing);
        QRect itemRect = vopt->rect;
        itemRect.setTop(itemRect.top() + 2);
        itemRect.setBottom(itemRect.bottom() - 2);
        QPainterPath path;
        path.addRoundedRect(itemRect, 8, 8);
        if (vopt->showDecorationSelected && (vopt->state & QStyle::State_Selected))
        {
            if (vopt->state & QStyle::State_MouseOver)
            {
                // 选中时覆盖
                p->fillPath(path, *_selectedHovergradient);
            }
            else
            {
                // 选中
                p->fillPath(path, *_selectedgradient);
            }
        }
        else
        {
            if (vopt->state & QStyle::State_MouseOver)
            {
                // 覆盖时颜色
                p->fillPath(path, *_hovergradient);
            }
        }
    }
    p->restore();
}

void ElaNavigationStyle::navigationNodeStateChange(QVariantMap data)
{
    if (data.contains("Expand"))
    {
        _animationTargetNode = data.value("Expand").value<ElaNavigationNode*>();
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { this->_animationTargetNode = nullptr; });
        connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {});
        nodeOpacityAnimation->setDuration(600);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.4, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _animationTargetNode = data.value("Collapse").value<ElaNavigationNode*>();
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { this->_animationTargetNode = nullptr; });
        nodeOpacityAnimation->setDuration(600);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(0);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void ElaNavigationStyle::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (themeMode == ElaApplicationType::Light)
    {
        _hovergradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _hovergradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedgradient->setColorAt(0, QColor(0xE9, 0xE9, 0xF0));
        _selectedgradient->setColorAt(1, QColor(0xEA, 0xE9, 0xF0));
        _selectedHovergradient->setColorAt(0, QColor(0xEC, 0xEC, 0xF3));
        _selectedHovergradient->setColorAt(1, QColor(0xED, 0xEC, 0xF3));
    }
    else
    {
        _hovergradient->setColorAt(0, QColor(0x27, 0x27, 0x27));
        _hovergradient->setColorAt(1, QColor(0x27, 0x27, 0x27));
        _selectedgradient->setColorAt(0, QColor(0x35, 0x35, 0x35));
        _selectedgradient->setColorAt(1, QColor(0x35, 0x35, 0x35));
        _selectedHovergradient->setColorAt(0, QColor(0x2F, 0x2F, 0x2F));
        _selectedHovergradient->setColorAt(1, QColor(0x2F, 0x2F, 0x2F));
    }
}
