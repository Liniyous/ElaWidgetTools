#include "ElaCheckBox.h"

#include "ElaCheckBoxStyle.h"
ElaCheckBox::ElaCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setObjectName("ElaCheckBox");
    setStyle(new ElaCheckBoxStyle(style()));
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
}

ElaCheckBox::ElaCheckBox(const QString& text, QWidget* parent)
    : QCheckBox(text, parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setObjectName("ElaCheckBox");
    setStyle(new ElaCheckBoxStyle(style()));
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
}

ElaCheckBox::~ElaCheckBox()
{
}
