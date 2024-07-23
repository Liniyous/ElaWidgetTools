#include "ElaNavigationStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "ElaNavigationNode.h"
#include "ElaTheme.h"
ElaNavigationStyle::ElaNavigationStyle(QStyle* style)
{
    _pOpacity = 1;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaNavigationStyle::~ElaNavigationStyle()
{
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
                p->fillPath(path, ElaThemeColor(_themeMode, NavigationSelectedHover));
            }
            else
            {
                // 选中
                p->fillPath(path, ElaThemeColor(_themeMode, NavigationSelected));
            }
        }
        else
        {
            if (vopt->state & QStyle::State_MouseOver)
            {
                // 覆盖时颜色
                p->fillPath(path, ElaThemeColor(_themeMode, NavigationHover));
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
