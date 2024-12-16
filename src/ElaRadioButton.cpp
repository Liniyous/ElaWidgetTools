#include "ElaRadioButton.h"

#include "DeveloperComponents/ElaRadioButtonStyle.h"
#include "ElaTheme.h"
#include "private/ElaRadioButtonPrivate.h"
ElaRadioButton::ElaRadioButton(QWidget* parent)
    : QRadioButton(parent), d_ptr(new ElaRadioButtonPrivate())
{
    Q_D(ElaRadioButton);
    d->q_ptr = this;
    setFixedHeight(20);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setStyle(new ElaRadioButtonStyle(style()));
    setStyleSheet(QString("background-color:transparent;color:%1;").arg(ElaThemeColor(eTheme->getThemeMode(), BasicText).name()));
    connect(eTheme, &ElaTheme::themeModeChanged, [=](ElaThemeType::ThemeMode themeMode){
        setStyleSheet(QString("background-color:transparent;color:%1;").arg(ElaThemeColor(themeMode, BasicText).name()));
    });
}

ElaRadioButton::ElaRadioButton(const QString& text, QWidget* parent)
    : ElaRadioButton(parent)
{
    setText(text);
}

ElaRadioButton::~ElaRadioButton()
{
}
