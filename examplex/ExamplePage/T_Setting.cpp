#include "T_Setting.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaApplication.h"
#include "ElaComboBox.h"
#include "ElaLog.h"
#include "ElaRadioButton.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToggleSwitch.h"
#include "ElaWindow.h"
T_Setting::T_Setting(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    setWindowTitle("设置");

    ElaText* themeText = new ElaText("主题设置", this);
    themeText->setWordWrap(false);
    themeText->setTextPixelSize(18);

    _themeComboBox = new ElaComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");
    ElaScrollPageArea* themeSwitchArea = new ElaScrollPageArea(this);
    themeSwitchArea->setFixedHeight(75);
    QHBoxLayout* themeSwitchLayout = new QHBoxLayout(themeSwitchArea);
    themeSwitchLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* themeSwitchText = new ElaText("主题切换", this);
    themeSwitchText->setWordWrap(false);
    themeSwitchText->setTextPixelSize(15);
    themeSwitchLayout->addWidget(themeSwitchText);
    themeSwitchLayout->addStretch();
    themeSwitchLayout->addWidget(_themeComboBox);
    connect(_themeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        if (initing)
            return;
        if (index == 0)
        {
            eTheme->setThemeMode(ElaThemeType::Light);
        }
        else
        {
            eTheme->setThemeMode(ElaThemeType::Dark);
        }
        DataBase::getInstance()->setConf("ui", "theme", index);
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        if (initing)
            return;
        _themeComboBox->blockSignals(true);
        if (themeMode == ElaThemeType::Light)
        {
            _themeComboBox->setCurrentIndex(0);
        }
        else
        {
            _themeComboBox->setCurrentIndex(1);
        }
        _themeComboBox->blockSignals(false);
    });

    ElaText* helperText = new ElaText("应用程序设置", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(18);

    _logSwitchButton = new ElaToggleSwitch(this);
    ElaScrollPageArea* logSwitchArea = new ElaScrollPageArea(this);
    logSwitchArea->setFixedHeight(75);
    QHBoxLayout* logSwitchLayout = new QHBoxLayout(logSwitchArea);
    logSwitchLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* logSwitchText = new ElaText("启用日志功能", this);
    logSwitchText->setWordWrap(false);
    logSwitchText->setTextPixelSize(15);
    logSwitchLayout->addWidget(logSwitchText);
    logSwitchLayout->addStretch();
    logSwitchLayout->addWidget(_logSwitchButton);
    connect(_logSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        if (initing)
            return;
        ElaLog::getInstance()->initMessageLog(checked);
        if (checked) {
            qDebug() << "日志已启用!";
        } else {
            qDebug() << "日志已关闭!";
        }
        DataBase::getInstance()->setConf("ui", "log", checked);
    });

    _minimumButton = new ElaRadioButton("Minimum", this);
    _compactButton = new ElaRadioButton("Compact", this);
    _maximumButton = new ElaRadioButton("Maximum", this);
    _autoButton = new ElaRadioButton("Auto", this);
    _autoButton->setChecked(true);
    ElaScrollPageArea* displayModeArea = new ElaScrollPageArea(this);
    displayModeArea->setFixedHeight(75);
    QHBoxLayout* displayModeLayout = new QHBoxLayout(displayModeArea);
    displayModeLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* displayModeText = new ElaText("导航栏模式选择", this);
    displayModeText->setWordWrap(false);
    displayModeText->setTextPixelSize(15);
    displayModeLayout->addWidget(displayModeText);
    displayModeLayout->addStretch();
    displayModeLayout->addWidget(_minimumButton);
    displayModeLayout->addWidget(_compactButton);
    displayModeLayout->addWidget(_maximumButton);
    displayModeLayout->addWidget(_autoButton);
    connect(_minimumButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            window->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
            DataBase::getInstance()->setConf("ui", "navBar", ElaNavigationType::Minimal);
        }
    });
    connect(_compactButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            window->setNavigationBarDisplayMode(ElaNavigationType::Compact);
            DataBase::getInstance()->setConf("ui", "navBar", ElaNavigationType::Compact);
        }
    });
    connect(_maximumButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            window->setNavigationBarDisplayMode(ElaNavigationType::Maximal);
            DataBase::getInstance()->setConf("ui", "navBar", ElaNavigationType::Maximal);
        }
    });
    connect(_autoButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            window->setNavigationBarDisplayMode(ElaNavigationType::Auto);
            DataBase::getInstance()->setConf("ui", "navBar", ElaNavigationType::Auto);
        }
    });

    //// ----------------------------------
    _dwmblueButton = new ElaRadioButton("dwm-blue", this);
    _acrylicButton = new ElaRadioButton("acrylic-material", this);
    _micaButton = new ElaRadioButton("mica", this);
    _altButton = new ElaRadioButton("mica-alt", this);
    _noneButton = new ElaRadioButton("none", this);
    _noneButton->setChecked(true);
    ElaScrollPageArea* windowModeArea = new ElaScrollPageArea(this);
    windowModeArea->setFixedHeight(75);
    QHBoxLayout* windowModeLayout = new QHBoxLayout(windowModeArea);
    windowModeLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* windowModeText = new ElaText("窗口模式", this);
    windowModeText->setWordWrap(false);
    windowModeText->setTextPixelSize(15);
    windowModeLayout->addWidget(windowModeText);
    windowModeLayout->addStretch();
    windowModeLayout->addWidget(_dwmblueButton);
    windowModeLayout->addWidget(_acrylicButton);
    windowModeLayout->addWidget(_micaButton);
    windowModeLayout->addWidget(_altButton);
    windowModeLayout->addWidget(_noneButton);
    connect(_dwmblueButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            eApp->setWindowAttribute("dwm-blur");
        }
    });
    connect(_acrylicButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            eApp->setWindowAttribute("acrylic-material");
        }
    });
    connect(_micaButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            eApp->setWindowAttribute("mica");
        }
    });
    connect(_altButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            eApp->setWindowAttribute("mica-alt");
        }
    });
    connect(_noneButton, &ElaRadioButton::toggled, this, [=](bool checked) {
        if (initing)
            return;
        if (checked) {
            eApp->setWindowAttribute("none");
        }
    });
    //// -----------------------
    _transSwitchButton = new ElaToggleSwitch(this);
    ElaScrollPageArea* transSwitchArea = new ElaScrollPageArea(this);
    transSwitchArea->setFixedHeight(75);
    QHBoxLayout* transSwitchLayout = new QHBoxLayout(transSwitchArea);
    transSwitchLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* transSwitchText = new ElaText("中心窗体透明", this);
    transSwitchText->setWordWrap(false);
    transSwitchText->setTextPixelSize(15);
    transSwitchLayout->addWidget(transSwitchText);
    transSwitchLayout->addStretch();
    transSwitchLayout->addWidget(_transSwitchButton);
    connect(_transSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        if (initing)
            return;
        window->setIsCentralStackedWidgetTransparent(checked);
        DataBase::getInstance()->setConf("ui", "trans", checked);
        //eTheme->setThemeMode(_themeComboBox->currentIndex() == 0 ? ElaThemeType::Light : ElaThemeType::Dark);
    });

    _userSwitchButton = new ElaToggleSwitch(this);
    ElaScrollPageArea* userSwitchArea = new ElaScrollPageArea(this);
    userSwitchArea->setFixedHeight(75);
    QHBoxLayout* userSwitchLayout = new QHBoxLayout(userSwitchArea);
    userSwitchLayout->setContentsMargins(15, 6, 10, 6);
    ElaText* userSwitchText = new ElaText("隐藏用户卡片", this);
    userSwitchText->setWordWrap(false);
    userSwitchText->setTextPixelSize(15);
    userSwitchLayout->addWidget(userSwitchText);
    userSwitchLayout->addStretch();
    userSwitchLayout->addWidget(_userSwitchButton);
    connect(_userSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        if (initing)
            return;
        window->setUserInfoCardVisible(!checked);
        DataBase::getInstance()->setConf("ui", "hideUserCard", checked);
    });

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("设置");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(themeText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(themeSwitchArea);
    centerLayout->addSpacing(15);
    centerLayout->addWidget(helperText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(logSwitchArea);
    centerLayout->addWidget(displayModeArea);
    centerLayout->addWidget(windowModeArea);
    centerLayout->addWidget(transSwitchArea);
    centerLayout->addWidget(userSwitchArea);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 20, 20);
    addCentralWidget(centralWidget, true, true, 0);

    initConf();
}

T_Setting::~T_Setting()
{
}

void T_Setting::initConf()
{
    initing = true;

    QMap<QString, QString> uiConf = eBase->getConfsByGroup("ui");

    _themeComboBox->setCurrentIndex(uiConf["theme"].toInt());
    _logSwitchButton->setIsToggled(uiConf["log"].toInt());
    _transSwitchButton->setIsToggled(uiConf["trans"].toInt());
    _userSwitchButton->setIsToggled(uiConf["hideUserCard"].toInt());

    ElaNavigationType::NavigationDisplayMode navType = static_cast<ElaNavigationType::NavigationDisplayMode>(uiConf["navBar"].toInt());
    _minimumButton->setChecked(navType == ElaNavigationType::Minimal);
    _compactButton->setChecked(navType == ElaNavigationType::Compact);
    _maximumButton->setChecked(navType == ElaNavigationType::Maximal);
    _autoButton->setChecked(navType == ElaNavigationType::Auto);
    initing = false;
}
