#include "T_ElaScreen.h"
#ifdef Q_OS_WIN
#include <QVBoxLayout>
#include "ElaComboBox.h"

#include "ElaText.h"
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : ElaScrollPage(parent)
{
    dxgiManager = ElaDxgiManager::getInstance();
    
    dxgiManager->setGrabArea(0, 0);
    dxgiManager->startGrabScreen();
    
    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setIsSyncGrabSize(true);

    // ElaText* dxText = new ElaText("采集设备选择", this);
    // dxText->setTextPixelSize(15);
    // _dxComboBox = new ElaComboBox(this);
    // _dxComboBox->addItems(dxgiManager->getDxDeviceList());
    // _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());

    // ElaText* outputText = new ElaText("采集对象选择", this);
    // outputText->setTextPixelSize(15);
    // _outputComboBox = new ElaComboBox(this);
    // _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    // _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());

    // connect(_dxComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
    //     dxgiManager->setDxDeviceID(index);
    //     _outputComboBox->blockSignals(true);
    //     _outputComboBox->clear();
    //     _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    //     _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
    //     _outputComboBox->blockSignals(false);
    //     _dxgiScreen->update();
    // });
    // connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
    //     dxgiManager->setOutputDeviceID(index);
    //     _dxgiScreen->update();
    // });

    // QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    // comboBoxLayout->addWidget(dxText);
    // comboBoxLayout->addWidget(_dxComboBox);
    // comboBoxLayout->addSpacing(10);
    // comboBoxLayout->addWidget(outputText);
    // comboBoxLayout->addWidget(_outputComboBox);
    // comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
     //centralWidget->setStyleSheet("border: 1px solid red;");
    //centralWidget->setWindowTitle("");
    
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    //centerLayout->addLayout(comboBoxLayout);
    //_dxgiScreen->setStyleSheet("border: 1px solid red;");

    centerLayout->addWidget(_dxgiScreen);
    centralWidget->setWindowTitle("录像");
    addCentralWidget(centralWidget, false, true);
    
}

T_ElaScreen::~T_ElaScreen()
{
}
#endif
