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
#include <QButtonGroup>
T_Setting::T_Setting(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    setWindowTitle("Setting");

    ElaText* themeText = new ElaText("主题设置", this);
    themeText->setWordWrap(false);
    themeText->setTextPixelSize(18);

    _themeComboBox = new ElaComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");
    ElaScrollPageArea* themeSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* themeSwitchLayout = new QHBoxLayout(themeSwitchArea);
    ElaText* themeSwitchText = new ElaText("主题切换", this);
    themeSwitchText->setWordWrap(false);
    themeSwitchText->setTextPixelSize(15);
    themeSwitchLayout->addWidget(themeSwitchText);
    themeSwitchLayout->addStretch();
    themeSwitchLayout->addWidget(_themeComboBox);
    connect(_themeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        if (index == 0)
        {
            eTheme->setThemeMode(ElaThemeType::Light);
        }
        else
        {
            eTheme->setThemeMode(ElaThemeType::Dark);
        }
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
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
    ElaText* windowPaintText = new ElaText("主窗口绘制设置", this);
    windowPaintText->setWordWrap(false);
    windowPaintText->setTextPixelSize(15);

    _windowNormalButton = new ElaRadioButton("Normal", this);
    _windowNormalButton->setChecked(true);
    _windowPixmapButton = new ElaRadioButton("Pixmap", this);
    _windowMovieButton = new ElaRadioButton("Movie", this);

    QButtonGroup* windowPaintButtonGroup = new QButtonGroup(this);
    windowPaintButtonGroup->addButton(_windowNormalButton, 0);
    windowPaintButtonGroup->addButton(_windowPixmapButton, 1);
    windowPaintButtonGroup->addButton(_windowMovieButton, 2);
    connect(windowPaintButtonGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            window->setWindowPaintMode((ElaWindowType::PaintMode)windowPaintButtonGroup->id(button));
        }
    });
    connect(window, &ElaWindow::pWindowPaintModeChanged, this, [=]() {
        auto button = windowPaintButtonGroup->button(window->getWindowPaintMode());
        ElaRadioButton* elaRadioButton = dynamic_cast<ElaRadioButton*>(button);
        if (elaRadioButton)
        {
            elaRadioButton->setChecked(true);
        }
    });
    ElaScrollPageArea* windowPaintModeArea = new ElaScrollPageArea(this);
    QHBoxLayout* windowPaintModeLayout = new QHBoxLayout(windowPaintModeArea);
    windowPaintModeLayout->addWidget(windowPaintText);
    windowPaintModeLayout->addStretch();
    windowPaintModeLayout->addWidget(_windowNormalButton);
    windowPaintModeLayout->addWidget(_windowPixmapButton);
    windowPaintModeLayout->addWidget(_windowMovieButton);

    ElaText* helperText = new ElaText("应用程序设置", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(18);

    ElaText* micaSwitchText = new ElaText("窗口效果", this);
    micaSwitchText->setWordWrap(false);
    micaSwitchText->setTextPixelSize(15);
    _normalButton = new ElaRadioButton("Normal", this);
    _elaMicaButton = new ElaRadioButton("ElaMica", this);
#ifdef Q_OS_WIN
    _micaButton = new ElaRadioButton("Mica", this);
    _micaAltButton = new ElaRadioButton("Mica-Alt", this);
    _acrylicButton = new ElaRadioButton("Acrylic", this);
    _dwmBlurnormalButton = new ElaRadioButton("Dwm-Blur", this);
#endif
    _normalButton->setChecked(true);
    QButtonGroup* displayButtonGroup = new QButtonGroup(this);
    displayButtonGroup->addButton(_normalButton, 0);
    displayButtonGroup->addButton(_elaMicaButton, 1);
#ifdef Q_OS_WIN
    displayButtonGroup->addButton(_micaButton, 2);
    displayButtonGroup->addButton(_micaAltButton, 3);
    displayButtonGroup->addButton(_acrylicButton, 4);
    displayButtonGroup->addButton(_dwmBlurnormalButton, 5);
#endif
    connect(displayButtonGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            eApp->setWindowDisplayMode((ElaApplicationType::WindowDisplayMode)displayButtonGroup->id(button));
        }
    });
    connect(eApp, &ElaApplication::pWindowDisplayModeChanged, this, [=]() {
        auto button = displayButtonGroup->button(eApp->getWindowDisplayMode());
        ElaRadioButton* elaRadioButton = dynamic_cast<ElaRadioButton*>(button);
        if (elaRadioButton)
        {
            elaRadioButton->setChecked(true);
        }
    });
    ElaScrollPageArea* micaSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* micaSwitchLayout = new QHBoxLayout(micaSwitchArea);
    micaSwitchLayout->addWidget(micaSwitchText);
    micaSwitchLayout->addStretch();
    micaSwitchLayout->addWidget(_normalButton);
    micaSwitchLayout->addWidget(_elaMicaButton);
#ifdef Q_OS_WIN
    micaSwitchLayout->addWidget(_micaButton);
    micaSwitchLayout->addWidget(_micaAltButton);
    micaSwitchLayout->addWidget(_acrylicButton);
    micaSwitchLayout->addWidget(_dwmBlurnormalButton);
#endif

    _logSwitchButton = new ElaToggleSwitch(this);
    ElaScrollPageArea* logSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* logSwitchLayout = new QHBoxLayout(logSwitchArea);
    ElaText* logSwitchText = new ElaText("启用日志功能", this);
    logSwitchText->setWordWrap(false);
    logSwitchText->setTextPixelSize(15);
    logSwitchLayout->addWidget(logSwitchText);
    logSwitchLayout->addStretch();
    logSwitchLayout->addWidget(_logSwitchButton);
    connect(_logSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        ElaLog::getInstance()->initMessageLog(checked);
        if (checked)
        {
            qDebug() << "日志已启用!";
        }
        else
        {
            qDebug() << "日志已关闭!";
        }
    });

    _minimumButton = new ElaRadioButton("Minimum", this);
    _compactButton = new ElaRadioButton("Compact", this);
    _maximumButton = new ElaRadioButton("Maximum", this);
    _autoButton = new ElaRadioButton("Auto", this);
    _autoButton->setChecked(true);
    ElaScrollPageArea* displayModeArea = new ElaScrollPageArea(this);
    QHBoxLayout* displayModeLayout = new QHBoxLayout(displayModeArea);
    ElaText* displayModeText = new ElaText("导航栏模式选择", this);
    displayModeText->setWordWrap(false);
    displayModeText->setTextPixelSize(15);
    displayModeLayout->addWidget(displayModeText);
    displayModeLayout->addStretch();
    displayModeLayout->addWidget(_minimumButton);
    displayModeLayout->addWidget(_compactButton);
    displayModeLayout->addWidget(_maximumButton);
    displayModeLayout->addWidget(_autoButton);

    QButtonGroup* navigationGroup = new QButtonGroup(this);
    navigationGroup->addButton(_autoButton, 0);
    navigationGroup->addButton(_minimumButton, 1);
    navigationGroup->addButton(_compactButton, 2);
    navigationGroup->addButton(_maximumButton, 3);
    connect(navigationGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            window->setNavigationBarDisplayMode((ElaNavigationType::NavigationDisplayMode)navigationGroup->id(button));
        }
    });

    _noneButton = new ElaRadioButton("None", this);
    _popupButton = new ElaRadioButton("Popup", this);
    _popupButton->setChecked(true);
    _scaleButton = new ElaRadioButton("Scale", this);
    _flipButton = new ElaRadioButton("Flip", this);
    _blurButton = new ElaRadioButton("Blur", this);
    ElaScrollPageArea* stackSwitchModeArea = new ElaScrollPageArea(this);
    QHBoxLayout* stackSwitchModeLayout = new QHBoxLayout(stackSwitchModeArea);
    ElaText* stackSwitchModeText = new ElaText("堆栈切换模式选择", this);
    stackSwitchModeText->setWordWrap(false);
    stackSwitchModeText->setTextPixelSize(15);
    stackSwitchModeLayout->addWidget(stackSwitchModeText);
    stackSwitchModeLayout->addStretch();
    stackSwitchModeLayout->addWidget(_noneButton);
    stackSwitchModeLayout->addWidget(_popupButton);
    stackSwitchModeLayout->addWidget(_scaleButton);
    stackSwitchModeLayout->addWidget(_flipButton);
    stackSwitchModeLayout->addWidget(_blurButton);

    QButtonGroup* stackSwitchGroup = new QButtonGroup(this);
    stackSwitchGroup->addButton(_noneButton, 0);
    stackSwitchGroup->addButton(_popupButton, 1);
    stackSwitchGroup->addButton(_scaleButton, 2);
    stackSwitchGroup->addButton(_flipButton, 3);
    stackSwitchGroup->addButton(_blurButton, 4);
    connect(stackSwitchGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            window->setStackSwitchMode((ElaWindowType::StackSwitchMode)stackSwitchGroup->id(button));
        }
    });
    connect(window, &ElaWindow::pStackSwitchModeChanged, this, [=]() {
        auto button = stackSwitchGroup->button(window->getStackSwitchMode());
        ElaRadioButton* elaRadioButton = dynamic_cast<ElaRadioButton*>(button);
        if (elaRadioButton)
        {
            elaRadioButton->setChecked(true);
        }
    });

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Setting");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(themeText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(themeSwitchArea);
    centerLayout->addSpacing(15);
    centerLayout->addWidget(helperText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(logSwitchArea);
    centerLayout->addWidget(windowPaintModeArea);
    centerLayout->addWidget(micaSwitchArea);
    centerLayout->addWidget(displayModeArea);
    centerLayout->addWidget(stackSwitchModeArea);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);
}

T_Setting::~T_Setting()
{
}
