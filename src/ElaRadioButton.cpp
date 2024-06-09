#include "ElaRadioButton.h"

#include "DeveloperComponents/ElaRadioButtonStyle.h"
#include "ElaApplication.h"
ElaRadioButton::ElaRadioButton(QWidget* parent)
    : QRadioButton(parent)
{
    setFixedHeight(20);
    setFont(QFont("Microsoft YaHei", 11));
    setStyle(new ElaRadioButtonStyle(style()));
    onThemeChanged(ElaApplication::getInstance()->getThemeMode());
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaRadioButton::onThemeChanged);
}

ElaRadioButton::ElaRadioButton(const QString& text, QWidget* parent)
    : QRadioButton(text, parent)
{
    setFixedHeight(20);
    setFont(QFont("Microsoft YaHei", 11));
    setStyle(new ElaRadioButtonStyle(style()));
    onThemeChanged(ElaApplication::getInstance()->getThemeMode());
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaRadioButton::onThemeChanged);
}

ElaRadioButton::~ElaRadioButton()
{
}

void ElaRadioButton::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (themeMode == ElaApplicationType::Light)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::WindowText, Qt::black);
        setPalette(palette);
    }
    else
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::WindowText, Qt::white);
        setPalette(palette);
    }
}
