#include "T_ElaScreen.h"

#include <QVBoxLayout>

#include "ElaDxgiManager.h"
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : ElaScrollPage(parent)
{
    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);
    dxgiManager->startGrabScreen();

    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setIsSyncGrabSize(true);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(_dxgiScreen);
    addCentralWidget(centralWidget, false, false);
}

T_ElaScreen::~T_ElaScreen()
{
}
