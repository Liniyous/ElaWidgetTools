#include "T_BaseComponents.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaMessageButton.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaProgressBar.h"
#include "ElaRadioButton.h"
#include "ElaScrollPageArea.h"
#include "ElaSlider.h"
#include "ElaSpinBox.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include "ElaToggleSwitch.h"
T_BaseComponents::T_BaseComponents(QWidget* parent)
    : ElaScrollPage(parent)
{
    _toggleSwitch = new ElaToggleSwitch(this);

    ElaScrollPageArea* toggleSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);
    ElaText* toggleSwitchText = new ElaText("ElaToggleSwitch", this);
    toggleSwitchText->setTextSize(15);
    toggleSwitchLayout->addWidget(toggleSwitchText);
    toggleSwitchLayout->addWidget(_toggleSwitch);
    toggleSwitchLayout->addStretch();

    _toggleButton = new ElaToggleButton("Head", this);
    ElaScrollPageArea* toggleButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* toggleButtonLayout = new QHBoxLayout(toggleButtonArea);
    ElaText* toggleButtonText = new ElaText("ElaToggleButton", this);
    toggleButtonText->setTextSize(15);
    toggleButtonLayout->addWidget(toggleButtonText);
    toggleButtonLayout->addWidget(_toggleButton);
    toggleButtonLayout->addStretch();

    _comboBox = new ElaComboBox(this);
    QStringList comboList{"TEST1", "TSET2", "TEST3", "TEST4", "TEST5", "TSET6", "TSET7"};
    _comboBox->addItems(comboList);
    _comboBox->setFixedWidth(100);
    ElaScrollPageArea* comboBoxArea = new ElaScrollPageArea(this);
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
    ElaText* comboBoxText = new ElaText("ElaComboBox", this);
    comboBoxText->setTextSize(15);
    comboBoxLayout->addWidget(comboBoxText);
    comboBoxLayout->addWidget(_comboBox);
    comboBoxLayout->addStretch();

    _multiSelectComboBox = new ElaMultiSelectComboBox(this);
    _multiSelectComboBox->setFixedWidth(150);
    QStringList multiComboList{"TEST1", "TSET2", "TEST3", "TEST4", "TEST5", "TSET6", "TSET7"};
    QStringList multiSelectComboList{"TEST1", "TSET2", "TEST4", "TEST5"};
    _multiSelectComboBox->addItems(multiComboList);
    _multiSelectComboBox->setCurrentSelection(multiSelectComboList);
    ElaScrollPageArea* multiSelectComboBoxArea = new ElaScrollPageArea(this);
    QHBoxLayout* multiSelectComboBoxLayout = new QHBoxLayout(multiSelectComboBoxArea);
    ElaText* multiSelectComboBoxText = new ElaText("ElaMutilSelectComboBox", this);
    multiSelectComboBoxText->setTextSize(15);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxText);
    multiSelectComboBoxLayout->addWidget(_multiSelectComboBox);
    multiSelectComboBoxLayout->addStretch();

    _messageButton = new ElaMessageButton("Success", this);
    _messageButton->setBarTitle("Success");
    _messageButton->setBarText("今天 珍重 谁知道是真是梦，明天 过后 解答或解脱，如果 你在 幸存的平行宇宙，请记 得我 曾这么预言过");

    _infoMessageButton = new ElaMessageButton("Info", this);
    _infoMessageButton->setBarTitle("Information");
    _infoMessageButton->setBarText("今天 珍重 谁知道是真是梦，明天 过后 解答或解脱，如果 你在 幸存的平行宇宙，请记 得我 曾这么预言过");
    _infoMessageButton->setMessageMode(ElaMessageBarType::Information);
    _infoMessageButton->setPositionPolicy(ElaMessageBarType::TopLeft);

    _warningMessageButton = new ElaMessageButton("Warning", this);
    _warningMessageButton->setBarTitle("Warning");
    _warningMessageButton->setBarText("今天 珍重 谁知道是真是梦，明天 过后 解答或解脱，如果 你在 幸存的平行宇宙，请记 得我 曾这么预言过");
    _warningMessageButton->setMessageMode(ElaMessageBarType::Warning);
    _warningMessageButton->setPositionPolicy(ElaMessageBarType::BottomLeft);

    _errorMessageButton = new ElaMessageButton("Error", this);
    _errorMessageButton->setBarTitle("Error");
    _errorMessageButton->setBarText("今天 珍重 谁知道是真是梦，明天 过后 解答或解脱，如果 你在 幸存的平行宇宙，请记 得我 曾这么预言过");
    _errorMessageButton->setMessageMode(ElaMessageBarType::Error);
    _errorMessageButton->setPositionPolicy(ElaMessageBarType::BottomRight);

    ElaScrollPageArea* messageButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* messageButtonLayout = new QHBoxLayout(messageButtonArea);
    ElaText* messageButtonText = new ElaText("ElaMessageButton", this);
    messageButtonText->setTextSize(15);
    messageButtonLayout->addWidget(messageButtonText);
    messageButtonLayout->addWidget(_messageButton);
    messageButtonLayout->addWidget(_infoMessageButton);
    messageButtonLayout->addWidget(_warningMessageButton);
    messageButtonLayout->addWidget(_errorMessageButton);
    messageButtonLayout->addStretch();

    _checkBox = new ElaCheckBox("CheckBox", this);
    ElaScrollPageArea* checkBoxArea = new ElaScrollPageArea(this);
    QHBoxLayout* checkBoxLayout = new QHBoxLayout(checkBoxArea);
    ElaText* checkBoxText = new ElaText("ElacheckBox", this);
    checkBoxText->setTextSize(15);
    checkBoxLayout->addWidget(checkBoxText);
    checkBoxLayout->addWidget(_checkBox);
    checkBoxLayout->addStretch();

    _spinBox = new ElaSpinBox(this);
    ElaScrollPageArea* spinBoxArea = new ElaScrollPageArea(this);
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(spinBoxArea);
    ElaText* spinBoxText = new ElaText("ElaSpinBox", this);
    spinBoxText->setTextSize(15);
    spinBoxLayout->addWidget(spinBoxText);
    spinBoxLayout->addWidget(_spinBox);
    spinBoxLayout->addStretch();

    _slider = new ElaSlider(this);
    ElaScrollPageArea* sliderArea = new ElaScrollPageArea(this);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderArea);
    ElaText* sliderText = new ElaText("ElaSlider", this);
    sliderText->setTextSize(15);
    sliderLayout->addWidget(sliderText);
    sliderLayout->addWidget(_slider);
    sliderLayout->addStretch();

    _radioButton = new ElaRadioButton("RadioButton", this);
    ElaScrollPageArea* radioButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    ElaText* radioButtonText = new ElaText("ElaRadioButton", this);
    radioButtonText->setTextSize(15);
    radioButtonLayout->addWidget(radioButtonText);
    radioButtonLayout->addWidget(_radioButton);
    radioButtonLayout->addStretch();

    _progressBar = new ElaProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0);
    ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    ElaText* progressBarText = new ElaText("ElaProgressBar", this);
    progressBarText->setTextSize(15);
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);
    progressBarLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaBaseComponents");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(toggleSwitchArea);
    centerLayout->addWidget(toggleButtonArea);
    centerLayout->addWidget(comboBoxArea);
    centerLayout->addWidget(multiSelectComboBoxArea);
    centerLayout->addWidget(messageButtonArea);
    centerLayout->addWidget(checkBoxArea);
    centerLayout->addWidget(spinBoxArea);
    centerLayout->addWidget(sliderArea);
    centerLayout->addWidget(radioButtonArea);
    centerLayout->addWidget(progressBarArea);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);
}

T_BaseComponents::~T_BaseComponents()
{
}
