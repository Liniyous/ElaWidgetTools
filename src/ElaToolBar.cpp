#include "ElaToolBar.h"

#include <QLayout>

#include "ElaApplication.h"
#include "ElaToolBarPrivate.h"
#include "ElaToolBarStyle.h"
ElaToolBar::ElaToolBar(QWidget* parent)
    : QToolBar(parent), d_ptr(new ElaToolBarPrivate())
{
    Q_D(ElaToolBar);
    d->q_ptr = this;
    setObjectName("ElaToolBar");
    setStyleSheet("#ElaToolBar{background-color:transparent;}");
    setStyle(new ElaToolBarStyle(style()));
    layout()->setSpacing(10);
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) {
        if (this->isFloating())
        {
            update();
        }
    });
}

ElaToolBar::ElaToolBar(const QString& title, QWidget* parent)
    : ElaToolBar(parent)
{
    setWindowTitle(title);
}

ElaToolBar::~ElaToolBar()
{
}

void ElaToolBar::setToolBarSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

int ElaToolBar::getToolBarSpacing() const
{
    return layout()->spacing();
}
