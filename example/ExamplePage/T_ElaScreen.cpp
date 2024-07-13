#include "T_ElaScreen.h"

#include <QVBoxLayout>

#include "ElaComboBox.h"
#include "ElaDxgiManager.h"
#include "ElaText.h"
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : ElaScrollPage(parent)
{
    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);
    dxgiManager->startGrabScreen();

    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setIsSyncGrabSize(true);

    ElaText* dxText = new ElaText("采集设备选择", this);
    dxText->setTextSize(15);
    _dxComboBox = new ElaComboBox(this);
    _dxComboBox->addItems(dxgiManager->getDxDeviceList());
    _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());

    ElaText* outputText = new ElaText("采集对象选择", this);
    outputText->setTextSize(15);
    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());

    connect(_dxComboBox, &ElaComboBox::currentIndexChanged, this, [=](int index) {
        dxgiManager->setDxDeviceID(index);
        _outputComboBox->blockSignals(true);
        _outputComboBox->clear();
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->blockSignals(false);
    });
    connect(_outputComboBox, &ElaComboBox::currentIndexChanged, this, [=](int index) {
        dxgiManager->setOutputDeviceID(index);
    });

    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(dxText);
    comboBoxLayout->addWidget(_dxComboBox);
    comboBoxLayout->addSpacing(10);
    comboBoxLayout->addWidget(outputText);
    comboBoxLayout->addWidget(_outputComboBox);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(_dxgiScreen);

    addCentralWidget(centralWidget, false, false);
}

T_ElaScreen::~T_ElaScreen()
{
}
