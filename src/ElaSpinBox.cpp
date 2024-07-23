#include "ElaSpinBox.h"

#include <QLineEdit>

#include "DeveloperComponents/ElaSpinBoxStyle.h"
#include "ElaTheme.h"
ElaSpinBox::ElaSpinBox(QWidget* parent)
    : QSpinBox(parent)
{
    setFixedSize(120, 30);
    setStyle(new ElaSpinBoxStyle(style()));
    lineEdit()->setAlignment(Qt::AlignCenter);
    lineEdit()->setStyleSheet("background-color:transparent");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::transparent);
        palette.setColor(QPalette::Text, ElaThemeColor(themeMode, WindowText));
        lineEdit()->setPalette(palette);
    });
}

ElaSpinBox::~ElaSpinBox()
{
}
