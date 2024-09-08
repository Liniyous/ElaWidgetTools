#include "T_ElaScreen.h"

#include "ElaToggleButton.h"
#ifdef Q_OS_WIN
#include <QVBoxLayout>

#include "ElaComboBox.h"
#include "ElaDxgiManager.h"
#include "ElaText.h"
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaScreen");

    // 顶部元素
    createCustomWidget("DXGI录制组件被放置于此，可在此界面预览录制效果");

    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);

    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setIsSyncGrabSize(true);

    ElaText* dxText = new ElaText("显卡选择", this);
    dxText->setTextPixelSize(15);
    _dxComboBox = new ElaComboBox(this);
    _dxComboBox->addItems(dxgiManager->getDxDeviceList());
    _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());

    ElaText* outputText = new ElaText("屏幕选择", this);
    outputText->setTextPixelSize(15);
    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());

    connect(_dxComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        dxgiManager->setDxDeviceID(index);
        _outputComboBox->blockSignals(true);
        _outputComboBox->clear();
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
        _outputComboBox->blockSignals(false);
        _dxgiScreen->update();
    });
    connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        dxgiManager->setOutputDeviceID(index);
        _dxgiScreen->update();
    });

    ElaToggleButton* startButton = new ElaToggleButton("捕获", this);
    connect(startButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            dxgiManager->startGrabScreen();
        }
        else
        {
            dxgiManager->stopGrabScreen();
            _dxgiScreen->update();
        }
    });

    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(dxText);
    comboBoxLayout->addWidget(_dxComboBox);
    comboBoxLayout->addSpacing(10);
    comboBoxLayout->addWidget(outputText);
    comboBoxLayout->addWidget(_outputComboBox);
    comboBoxLayout->addWidget(startButton);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(_dxgiScreen);

    addCentralWidget(centralWidget, false, true);
}

T_ElaScreen::~T_ElaScreen()
{
}
#endif
