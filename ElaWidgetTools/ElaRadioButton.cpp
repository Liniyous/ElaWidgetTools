#include "ElaRadioButton.h"

#include "ElaApplication.h"
#include "ElaRadioButtonStyle.h"
#include "ElaTheme.h"
#include "private/ElaRadioButtonPrivate.h"
ElaRadioButton::ElaRadioButton(QWidget* parent)
    : QRadioButton(parent), d_ptr(new ElaRadioButtonPrivate())
{
    Q_D(ElaRadioButton);
    d->q_ptr = this;
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    QFont font = this->font();
    font.setPixelSize(eApp->getFontPixelSize());
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

void ElaRadioButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaRadioButton);
    if (palette().color(QPalette::WindowText) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QRadioButton::paintEvent(event);
}
