#include "ElaCheckBox.h"

#include "ElaApplication.h"
#include "ElaCheckBoxStyle.h"

ElaCheckBox::ElaCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setObjectName("ElaCheckBox");
    auto* checkBoxStyle = new ElaCheckBoxStyle(style());
    checkBoxStyle->setParent(this);
    setStyle(checkBoxStyle);
    QFont font = this->font();
    font.setPixelSize(eApp->getFontPixelSize());
    setFont(font);
}

ElaCheckBox::ElaCheckBox(const QString& text, QWidget* parent)
    : ElaCheckBox(parent)
{
    setText(text);
}

ElaCheckBox::~ElaCheckBox() = default;
