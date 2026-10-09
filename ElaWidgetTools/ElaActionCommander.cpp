#include "ElaActionCommander.h"

#include "ElaActionCommanderModel.h"
#include "ElaActionCommanderPrivate.h"
#include "ElaActionCommanderView.h"
#include "ElaCheckBox.h"
#include "ElaMessageBar.h"
#include "ElaProgressRing.h"
#include "ElaSpinBox.h"
#include "ElaText.h"

#include <QApplication>
#include <QVBoxLayout>

ElaActionCommand::ElaActionCommand(QObject* parent)
    : QObject(parent)
{
    _pIsShowMessage = true;
    _pMessageMode = ElaMessageBarType::Success;
    _pMessageWidget = nullptr;
}

ElaActionCommand::~ElaActionCommand()
{
}

ELA_SINGLETON_CPP(ElaActionCommander)
ELA_PROPERTY_Q_CPP(ElaActionCommander, int, IsMessageDisplayEnable)
ELA_PROPERTY_Q_CPP(ElaActionCommander, int, MessageDisplayMesc)
ELA_PROPERTY_Q_CPP(ElaActionCommander, int, MaxCommandCount)
ELA_PROPERTY_Q_CPP(ElaActionCommander, bool, IsRunToCommandProcessEventEnable)
ElaActionCommander::ElaActionCommander(QObject* parent)
    : QObject{parent}, d_ptr(new ElaActionCommanderPrivate())
{
    Q_D(ElaActionCommander);
    d->q_ptr = this;
    d->_pIsMessageDisplayEnable = true;
    d->_pMessageDisplayMesc = 5000;
    d->_pMaxCommandCount = 500;
    d->_pIsRunToCommandProcessEventEnable = true;

    d->_commanderDesign = new QWidget();
    d->_commanderDesign->setMinimumWidth(400);
    d->_commanderDesign->setObjectName("ElaCommanderDesign");
    d->_commanderDesign->setStyleSheet("#ElaCommanderDesign{background-color:transparent;}");
    d->_commanderDesign->resize(400, 1000);

    d->_commanderView = new ElaActionCommanderView(d->_commanderDesign);
    connect(d->_commanderView, &ElaActionCommanderView::runToCommand, this, [=](const QString& domainName, int commandIndex) {
        runToCommand(domainName, commandIndex);
    });

    // 配置界面
    auto settingLayout = new QVBoxLayout();
    settingLayout->setSpacing(15);
    settingLayout->setContentsMargins(15, 0, 15, 15);
    auto settingTextLayout = new QHBoxLayout();
    auto settingIconText = new ElaText(d->_commanderDesign);
    settingIconText->setFixedSize(25, 25);
    settingIconText->setElaIcon(ElaIconType::GearComplex);
    settingIconText->setTextPixelSize(17);
    auto settingText = new ElaText("命令管理器配置", 17, d->_commanderDesign);
    settingText->setFixedWidth(130);
    d->_commanderBusyRing = new ElaProgressRing(d->_commanderDesign);
    d->_commanderBusyRing->setIsDisplayValue(false);
    d->_commanderBusyRing->setBusyingWidth(4);
    d->_commanderBusyRing->setFixedSize(25, 25);
    d->_commanderBusyRing->setIsBusying(true);

    d->_commanderBusyRingValue = new ElaText("100.0%", 17, d->_commanderDesign);
    d->_commanderBusyRingValue->setFixedWidth(65);
    settingTextLayout->addWidget(settingIconText);
    settingTextLayout->addWidget(settingText);
    settingTextLayout->addSpacing(80);
    settingTextLayout->addWidget(d->_commanderBusyRing);
    settingTextLayout->addWidget(d->_commanderBusyRingValue);
    settingTextLayout->addStretch();
    settingLayout->addLayout(settingTextLayout);

    int textWidth = 105;
    auto firstSettingLayout = new QHBoxLayout();
    settingLayout->addLayout(firstSettingLayout);
    auto messageDisplayText = new ElaText("启用命令提示", 17, d->_commanderDesign);
    messageDisplayText->setFixedWidth(textWidth);
    auto messageDisplayCheckBox = new ElaCheckBox(d->_commanderDesign);
    messageDisplayCheckBox->setChecked(true);
    connect(messageDisplayCheckBox, &ElaCheckBox::stateChanged, this, [=](int state) {
        setIsMessageDisplayEnable(state);
    });
    firstSettingLayout->addWidget(messageDisplayText);
    firstSettingLayout->addWidget(messageDisplayCheckBox);
    firstSettingLayout->addSpacing(70);

    auto processEventEnableText = new ElaText("启用界面同步", 17, d->_commanderDesign);
    processEventEnableText->setFixedWidth(textWidth);
    auto processEventEnableCheckBox = new ElaCheckBox(d->_commanderDesign);
    processEventEnableCheckBox->setChecked(true);
    connect(processEventEnableCheckBox, &ElaCheckBox::stateChanged, this, [=](int state) {
        setIsRunToCommandProcessEventEnable(state);
    });
    firstSettingLayout->addWidget(processEventEnableText);
    firstSettingLayout->addWidget(processEventEnableCheckBox);
    firstSettingLayout->addStretch();

    auto secondSettingLayout = new QHBoxLayout();
    settingLayout->addLayout(secondSettingLayout);
    auto messageDisplayTimeText = new ElaText("提示持续时间", 17, d->_commanderDesign);
    messageDisplayTimeText->setFixedWidth(textWidth);
    auto messageDisplayTimeSpinBox = new ElaSpinBox(d->_commanderDesign);
    messageDisplayTimeSpinBox->setMinimumWidth(200);
    messageDisplayTimeSpinBox->setRange(0, 100000);
    messageDisplayTimeSpinBox->setValue(d->_pMessageDisplayMesc);
    connect(messageDisplayTimeSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), this, [=](int value) {
        setMessageDisplayMesc(value);
    });
    auto messageDisplayTimeEndText = new ElaText("毫秒", 17, d->_commanderDesign);
    secondSettingLayout->addWidget(messageDisplayTimeText);
    secondSettingLayout->addWidget(messageDisplayTimeSpinBox);
    secondSettingLayout->addWidget(messageDisplayTimeEndText);
    secondSettingLayout->addStretch();

    auto thirdSettingLayout = new QHBoxLayout();
    settingLayout->addLayout(thirdSettingLayout);
    auto maxCommandCountText = new ElaText("最大命令容量", 17, d->_commanderDesign);
    maxCommandCountText->setFixedWidth(textWidth);
    auto maxCommandCountSpinBox = new ElaSpinBox(d->_commanderDesign);
    maxCommandCountSpinBox->setMinimumWidth(200);
    maxCommandCountSpinBox->setRange(0, 100000);
    maxCommandCountSpinBox->setValue(d->_pMaxCommandCount);
    connect(maxCommandCountSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), this, [=](int value) {
        setMaxCommandCount(value);
    });
    auto maxCommandCountEndText = new ElaText("条", 17, d->_commanderDesign);
    thirdSettingLayout->addWidget(maxCommandCountText);
    thirdSettingLayout->addWidget(maxCommandCountSpinBox);
    thirdSettingLayout->addWidget(maxCommandCountEndText);
    thirdSettingLayout->addStretch();

    auto designLayout = new QVBoxLayout(d->_commanderDesign);
    designLayout->setContentsMargins(0, 0, 0, 0);
    designLayout->addWidget(d->_commanderView);
    designLayout->addLayout(settingLayout);
    d->_commanderDesign->setVisible(false);
}

ElaActionCommander::~ElaActionCommander()
{
}

void ElaActionCommander::setCurrentDomain(const QString& domainName)
{
    Q_D(ElaActionCommander);
    auto& commandData = d->_commandDomainMap[domainName];
    d->_initDomain(commandData);
    if (d->_commanderView->getDomainName() != domainName)
    {
        d->_commanderView->setDomainName(domainName);
        d->_commanderView->setModel(commandData.commanderModel);
    }
}

const QString& ElaActionCommander::getCurrentDomain() const
{
    Q_D(const ElaActionCommander);
    return d->_commanderView->getDomainName();
}

bool ElaActionCommander::executeCommand(const QString& domainName, ElaActionCommand* command, bool isInvoke)
{
    Q_D(ElaActionCommander);
    if (isInvoke)
    {
        bool result = false;
        QMetaObject::invokeMethod(this, [&]() {
            result = d->_executeCommand(domainName, command);
        },
                                  Qt::BlockingQueuedConnection);
        return result;
    }
    return d->_executeCommand(domainName, command);
}

bool ElaActionCommander::recordCommand(const QString& domainName, ElaActionCommand* command, bool isRedo, bool isInvoke)
{
    Q_D(ElaActionCommander);
    if (isInvoke)
    {
        bool result = false;
        QMetaObject::invokeMethod(this, [&]() {
            result = d->_recordCommand(domainName, command, isRedo);
        },
                                  Qt::BlockingQueuedConnection);
        return result;
    }
    return d->_recordCommand(domainName, command, isRedo);
}

void ElaActionCommander::clearCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->_commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    commandData.currentIndex = -1;
    for (const auto command: commandList)
    {
        delete command;
    }
    commandList.clear();
    commandData.undoState = ElaActionCommanderType::UndoInvalid;
    commandData.redoState = ElaActionCommanderType::RedoInvalid;
    Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::UndoInvalid);
    Q_EMIT commanderStateChanged(domainName, ElaActionCommanderType::RedoInvalid);
}

void ElaActionCommander::undoCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    auto& commandData = d->_commandDomainMap[domainName];
    if (commandData.isBusyMode)
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", "任务管理器繁忙中!", d->_pMessageDisplayMesc, nullptr);
        return;
    }
    if (!d->_isCommanderValid)
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", "当前不处于可编辑模式，仅支持运行命令!", d->_pMessageDisplayMesc, nullptr);
        return;
    }
    setCurrentDomain(domainName);
    d->_undoCommand(domainName, true);
}

void ElaActionCommander::redoCommand(const QString& domainName)
{
    Q_D(ElaActionCommander);
    auto& commandData = d->_commandDomainMap[domainName];
    if (commandData.isBusyMode)
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", "任务管理器繁忙中!", d->_pMessageDisplayMesc, nullptr);
        return;
    }
    if (!d->_isCommanderValid)
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", "当前不处于可编辑模式，仅支持运行命令!", d->_pMessageDisplayMesc, nullptr);
        return;
    }
    setCurrentDomain(domainName);
    d->_redoCommand(domainName, true);
}

void ElaActionCommander::runToCommand(const QString& domainName, int commandIndex)
{
    Q_D(ElaActionCommander);
    auto& commandData = d->_commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    if (commandData.isBusyMode || commandIndex >= commandList.count())
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "警告", "任务管理器繁忙中!", d->_pMessageDisplayMesc, nullptr);
        return;
    }
    commandData.isBusyMode = true;
    bool isRedo = true;
    if (commandIndex > commandData.currentIndex)
    {
        // 重做到目标索引
        int runCount = commandIndex - commandData.currentIndex;
        for (int i = 0; i < runCount; i++)
        {
            d->_redoCommand(domainName, false);
            QString percent = QString::number(100 * (i + 1) / static_cast<qreal>(runCount), 'f', 1);
            d->_commanderBusyRingValue->setText(QString("%1%").arg(percent));
            if (d->_pIsRunToCommandProcessEventEnable)
            {
                QApplication::processEvents();
            }
        }
    }
    else
    {
        isRedo = false;
        // 撤销到目标索引 _currentIndex最小为-1
        int runCount = commandData.currentIndex - commandIndex + 1;
        for (int i = 0; i < runCount; i++)
        {
            d->_undoCommand(domainName, false);
            QString percent = QString::number(100 * (i + 1) / static_cast<qreal>(runCount), 'f', 1);
            d->_commanderBusyRingValue->setText(QString("%1%").arg(percent));
            if (d->_pIsRunToCommandProcessEventEnable)
            {
                QApplication::processEvents();
            }
        }
    }
    if (isRedo)
    {
        ElaMessageBar::information(ElaMessageBarType::TopRight, "重做", "重做到指定节点完成!", d->_pMessageDisplayMesc, nullptr);
    }
    else
    {
        ElaMessageBar::information(ElaMessageBarType::TopRight, "撤销", "撤销到指定节点完成!", d->_pMessageDisplayMesc, nullptr);
    }
    commandData.isBusyMode = false;
}

ElaActionCommanderType::CommanderState ElaActionCommander::getCommanderUndoState(const QString& domainName) const
{
    Q_D(const ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return ElaActionCommanderType::UndoInvalid;
    }
    return d->_commandDomainMap[domainName].undoState;
}

ElaActionCommanderType::CommanderState ElaActionCommander::getCommanderRedoState(const QString& domainName) const
{
    Q_D(const ElaActionCommander);
    if (!d->_commandDomainMap.contains(domainName))
    {
        return ElaActionCommanderType::RedoInvalid;
    }
    return d->_commandDomainMap[domainName].redoState;
}

QWidget* ElaActionCommander::getCommanderDesign() const
{
    Q_D(const ElaActionCommander);
    return d->_commanderDesign;
}
