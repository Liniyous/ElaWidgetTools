#include "T_ElaScreen.h"

#ifdef Q_OS_WIN
#include "ElaComboBox.h"
#include "ElaDxgiManager.h"
#include "ElaLineEdit.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include "T_ElaPacketIO.h"
#include "T_RecvScreen.h"
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaScreen");

    // 顶部元素
    createCustomWidget("DXGI录制组件被放置于此，可在此界面预览录制效果");

    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);

    ElaScrollPageArea* dxgiScreenArea = new ElaScrollPageArea(this);
    dxgiScreenArea->setFixedHeight(700);
    QHBoxLayout* dxgiScreenLayout = new QHBoxLayout(dxgiScreenArea);
    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setFixedSize(1200, 678);
    dxgiScreenLayout->addWidget(_dxgiScreen);

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

    QHBoxLayout* packetLayout = new QHBoxLayout();
    ElaText* packetIOText = new ElaText("网络视图 (需要先进行屏幕捕获 若接口IP不正确或不可用 程序可能会崩溃)", this);
    packetIOText->setTextPixelSize(17);

    ElaText* interfaceIPText = new ElaText("接口IP", this);
    interfaceIPText->setTextPixelSize(15);
    ElaLineEdit* interfaceIPEdit = new ElaLineEdit(this);
    interfaceIPEdit->setMaximumWidth(140);
    interfaceIPEdit->setFixedHeight(33);
    interfaceIPEdit->setPlaceholderText("接口IP");
    interfaceIPEdit->setText("192.168.1");

    ElaToggleButton* sendButton = new ElaToggleButton("初始发送", this);
    connect(sendButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            _initSendThread(interfaceIPEdit->text().trimmed());
        }
        else
        {
            _unInitThread(true);
        }
    });
    ElaToggleButton* recvButton = new ElaToggleButton("初始接收", this);
    connect(recvButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            _initRecvThread(interfaceIPEdit->text().trimmed());
        }
        else
        {
            _unInitThread(false);
        }
    });
    packetLayout->addWidget(interfaceIPText);
    packetLayout->addWidget(interfaceIPEdit);
    packetLayout->addWidget(sendButton);
    packetLayout->addWidget(recvButton);
    packetLayout->addStretch();
    _recvScreen = new T_RecvScreen(this);
    ElaScrollPageArea* recvScreenArea = new ElaScrollPageArea(this);
    recvScreenArea->setFixedHeight(700);
    QHBoxLayout* recvScreenLayout = new QHBoxLayout(recvScreenArea);
    recvScreenLayout->addWidget(_recvScreen);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(dxgiScreenArea);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(packetIOText);
    centerLayout->addLayout(packetLayout);
    centerLayout->addWidget(recvScreenArea);
    addCentralWidget(centralWidget, false, true);
}

T_ElaScreen::~T_ElaScreen()
{
    _unInitThread(true);
    _unInitThread(false);
}

void T_ElaScreen::initGrabImageHandleThread()
{
}

void T_ElaScreen::_initSendThread(QString interfaceIP)
{
    _packetIOSendThread = new QThread(this);
    _packetSendIO = new T_ElaPacketIO();
    _packetSendIO->setInterfaceIP(interfaceIP);
    _packetSendIO->moveToThread(_packetIOSendThread);
    connect(_packetIOSendThread, &QThread::started, _packetSendIO, &T_ElaPacketIO::handleGrabImage);
    _packetIOSendThread->start();
}

void T_ElaScreen::_initRecvThread(QString interfaceIP)
{
    // 原则上一个程序最好只初始化一个XIO 但如果需要多个XIO 则控制初始化间隔 不能同时进行初始化
    QTimer::singleShot(1000, this, [=]() {
        _packetIORecvThread = new QThread(this);
        _packetRecvIO = new T_ElaPacketIO();
        _packetRecvIO->setInterfaceIP(interfaceIP);
        _packetRecvIO->moveToThread(_packetIORecvThread);
        connect(_packetIORecvThread, &QThread::started, _packetRecvIO, &T_ElaPacketIO::handleImagePacket);
        _packetIORecvThread->start();
        connect(_packetRecvIO, &T_ElaPacketIO::sendHandleResult, _recvScreen, &T_RecvScreen::onSendHandleResult);
    });
}

void T_ElaScreen::_unInitThread(bool isSend)
{
    if (isSend)
    {
        if (!_packetSendIO || !_packetIOSendThread)
        {
            return;
        }
        _packetSendIO->setIsActive(false);
        _packetIOSendThread->quit();
        _packetIOSendThread->wait();
        delete _packetSendIO;
        delete _packetIOSendThread;
        _packetSendIO = nullptr;
        _packetIOSendThread = nullptr;
    }
    else
    {
        if (!_packetRecvIO || !_packetIORecvThread)
        {
            return;
        }
        _packetRecvIO->setIsActive(false);
        _packetIORecvThread->quit();
        _packetIORecvThread->wait();
        delete _packetRecvIO;
        delete _packetIORecvThread;
        _packetRecvIO = nullptr;
        _packetIORecvThread = nullptr;
    }
}

#endif
