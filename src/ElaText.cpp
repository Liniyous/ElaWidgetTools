#include "ElaText.h"

#include <QPainter>

#include "ElaApplication.h"
ElaText::ElaText(QWidget* parent)
    : QLabel(parent)
{
    QFont textFont = QFont("微软雅黑");
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, this->_textSpacing);
    textFont.setPixelSize(_textSize);
    setWordWrap(true);
    setFont(textFont);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaText::onThemeChanged);
}

ElaText::ElaText(QString text, QWidget* parent)
    : QLabel(text, parent)
{
    QFont textFont = QFont("微软雅黑");
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, this->_textSpacing);
    textFont.setPixelSize(_textSize);
    setWordWrap(true);
    setFont(textFont);
    connect(ElaApplication::getInstance(), &::ElaApplication::themeModeChanged, this, &ElaText::onThemeChanged);
}

ElaText::~ElaText()
{
}

void ElaText::setTextSize(int size)
{
    this->_textSize = size;
    QFont font = this->font();
    font.setPixelSize(_textSize);
    setFont(font);
}

int ElaText::getTextSize() const
{
    return this->_textSize;
}

void ElaText::setTextStyle(ElaTextType::TextStyle textStyle)
{
    QFont textFont = font();
    _textStyle = textStyle;
    switch (textStyle)
    {
    case ElaTextType::NoStyle:
    {
        break;
    }
    case ElaTextType::Caption:
    {
        _textSize = 12;
        textFont.setPixelSize(_textSize);
        break;
    }
    case ElaTextType::Body:
    {
        _textSize = 13;
        textFont.setPixelSize(_textSize);
        break;
    }
    case ElaTextType::BodyStrong:
    {
        _textSize = 13;
        textFont.setPixelSize(_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Subtitle:
    {
        _textSize = 20;
        textFont.setPixelSize(_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Title:
    {
        _textSize = 28;
        textFont.setPixelSize(_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::TitleLarge:
    {
        _textSize = 40;
        textFont.setPixelSize(_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Display:
    {
        _textSize = 68;
        textFont.setPixelSize(_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    }
    setFont(textFont);
}

ElaTextType::TextStyle ElaText::getTextStyle() const
{
    return this->_textStyle;
}

void ElaText::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    QPalette palette = this->palette();
    if (themeMode == ElaApplicationType::Light)
    {
        palette.setColor(QPalette::WindowText, Qt::black);
        setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::WindowText, Qt::white);
        setPalette(palette);
    }
}

void ElaText::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::transparent);
    painter.drawRect(rect());
    painter.restore();
    QLabel::paintEvent(event);
}
