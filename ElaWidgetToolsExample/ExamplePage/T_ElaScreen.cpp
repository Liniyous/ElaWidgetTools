#include "T_ElaScreen.h"

#include "ElaComboBox.h"
#include "ElaFlowLayout.h"
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#ifdef Q_OS_WIN
#include "ElaDxgiManager.h"
#endif

#include <QSignalBlocker>
#include <QVBoxLayout>

T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("屏幕采集");
    createCustomWidget("选择显卡和显示器，使用 DXGI 预览本机屏幕画面。");
    auto* mainLayout = createPageLayout();
#ifdef Q_OS_WIN
    auto* captureLayout = addExampleSection(mainLayout, "屏幕捕获", "选择捕获源后开启采集，画面仅在本地预览。");
    auto* dxgiManager = ElaDxgiManager::getInstance();
    _graphicsComboBox = new ElaComboBox(this);
    _graphicsComboBox->setFixedWidth(240);
    _graphicsComboBox->addItems(dxgiManager->getDxDeviceList());
    _graphicsComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());
    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->setFixedWidth(200);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
    _captureToggleButton = new ElaToggleButton("捕获", this);
    _captureToggleButton->setEnabled(_outputComboBox->count() > 0);
    _captureImageCard = new ElaImageCard(this);
    setPreviewPlaceholder(_captureImageCard, "尚未开始捕获\n选择屏幕后点击捕获");

    connect(_graphicsComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [this, dxgiManager](int index) {
        dxgiManager->setDxDeviceID(index);
        const QSignalBlocker blocker(_outputComboBox);
        _outputComboBox->clear();
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
        _captureToggleButton->setEnabled(_outputComboBox->count() > 0);
        _captureImageCard->setCardImage(QImage());
    });
    connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [this, dxgiManager](int index) {
        dxgiManager->setOutputDeviceID(index);
        _captureImageCard->setCardImage(QImage());
    });
    connect(_captureToggleButton, &ElaToggleButton::toggled, this, [this, dxgiManager](bool isToggled) {
        if (isToggled)
        {
            dxgiManager->startGrabScreen();
        }
        else
        {
            dxgiManager->stopGrabScreen();
            _captureImageCard->setCardImage(QImage());
        }
    });
    connect(dxgiManager, &ElaDxgiManager::grabImageUpdate, this, [this, dxgiManager](const QImage& image) {
        if (dxgiManager->getIsGrabScreen())
        {
            _captureImageCard->setCardImage(image);
        }
    });

    auto* sourceLayout = new ElaFlowLayout(0, 16, 16);
    sourceLayout->addWidget(createLabeledControl("显卡", _graphicsComboBox));
    sourceLayout->addWidget(createLabeledControl("屏幕", _outputComboBox));
    sourceLayout->addWidget(createLabeledControl("操作", _captureToggleButton));
    captureLayout->addLayout(sourceLayout);
    captureLayout->addWidget(_captureImageCard);
#else
    auto* descriptionText = new ElaText("DXGI 屏幕采集仅支持 Windows，其他控件示例可正常使用。", 15, this);
    descriptionText->setWordWrap(true);
    mainLayout->addWidget(descriptionText);
#endif
}

T_ElaScreen::~T_ElaScreen()
{
#ifdef Q_OS_WIN
    if (_captureToggleButton && _captureToggleButton->getIsToggled())
    {
        ElaDxgiManager::getInstance()->stopGrabScreen();
    }
#endif
}
