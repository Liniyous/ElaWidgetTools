#include "ElaDrawerArea.h"
#include "ElaDrawerAreaPrivate.h"
#include "ElaTheme.h"
ElaDrawerArea::ElaDrawerArea(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaDrawerAreaPrivate())
{
    Q_D(ElaDrawerArea);
    d->q_ptr = this;
    setObjectName("ElaDrawerArea");
    setStyleSheet("#ElaDrawerArea{background-color:transparent;}");

    d->_drawerHeader = new ElaDrawerHeader(this);
    d->_drawerContainer = new ElaDrawerContainer(this);
    connect(d->_drawerHeader, &ElaDrawerHeader::drawerHeaderClicked, d, &ElaDrawerAreaPrivate::onDrawerHeaderClicked);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(d->_drawerHeader);
    mainLayout->addWidget(d->_drawerContainer);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaDrawerArea::~ElaDrawerArea()
{
}

void ElaDrawerArea::setBorderRadius(int borderRadius)
{
    Q_D(ElaDrawerArea);
    d->_drawerHeader->setBorderRadius(borderRadius);
    d->_drawerContainer->setBorderRadius(borderRadius);
    Q_EMIT pBorderRadiusChanged();
}

int ElaDrawerArea::getBorderRadius() const
{
    Q_D(const ElaDrawerArea);
    return d->_drawerHeader->getBorderRadius();
}

void ElaDrawerArea::setHeaderHeight(int headerHeight)
{
    Q_D(ElaDrawerArea);
    d->_drawerHeader->setFixedHeight(headerHeight);
}

int ElaDrawerArea::getHeaderHeight() const
{
    Q_D(const ElaDrawerArea);
    return d->_drawerHeader->height();
}

void ElaDrawerArea::setDrawerHeader(QWidget* widget)
{
    Q_D(ElaDrawerArea);
    d->_drawerHeader->setHeaderWidget(widget);
}

void ElaDrawerArea::addDrawer(QWidget* widget)
{
    Q_D(ElaDrawerArea);
    d->_drawerContainer->addWidget(widget);
}

void ElaDrawerArea::removeDrawer(QWidget* widget)
{
    Q_D(ElaDrawerArea);
    d->_drawerContainer->removeWidget(widget);
}

void ElaDrawerArea::expand()
{
    Q_D(ElaDrawerArea);
    d->_drawerHeader->setIsExpand(true);
    d->_drawerHeader->doExpandOrCollapseAnimation();
    d->_drawerContainer->doDrawerAnimation(true);
    Q_EMIT expandStateChanged(true);
}

void ElaDrawerArea::collapse()
{
    Q_D(ElaDrawerArea);
    d->_drawerHeader->setIsExpand(false);
    d->_drawerHeader->doExpandOrCollapseAnimation();
    d->_drawerContainer->doDrawerAnimation(false);
    Q_EMIT expandStateChanged(false);
}

bool ElaDrawerArea::getIsExpand() const
{
    Q_D(const ElaDrawerArea);
    return d->_drawerHeader->getIsExpand();
}
