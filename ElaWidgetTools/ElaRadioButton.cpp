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
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaRadioButtonPrivate::onThemeChanged);
}

ElaRadioButton::ElaRadioButton(const QString& text, QWidget* parent)
    : ElaRadioButton(parent)
{
    setText(text);
}

ElaRadioButton::~ElaRadioButton()
{
    delete this->style();
}
