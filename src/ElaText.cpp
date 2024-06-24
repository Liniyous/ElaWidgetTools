#include "ElaText.h"

#include <QPainter>

#include "ElaApplication.h"
#include "private/ElaTextPrivate.h"
ElaText::ElaText(QWidget* parent)
    : QLabel(parent), d_ptr(new ElaTextPrivate())
{
    Q_D(ElaText);
    d->q_ptr = this;
    setObjectName("ElaText");
    setStyleSheet("#ElaText{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    textFont.setPixelSize(d->_textSize);
    setWordWrap(true);
    setFont(textFont);
    d->onThemeChanged(ElaApplication::getInstance()->getThemeMode());
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaTextPrivate::onThemeChanged);
}

ElaText::ElaText(QString text, QWidget* parent)
    : QLabel(text, parent), d_ptr(new ElaTextPrivate())
{
    Q_D(ElaText);
    d->q_ptr = this;
    setObjectName("ElaText");
    setStyleSheet("#ElaText{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    textFont.setPixelSize(d->_textSize);
    setWordWrap(true);
    setFont(textFont);
    d->onThemeChanged(ElaApplication::getInstance()->getThemeMode());
    connect(ElaApplication::getInstance(), &::ElaApplication::themeModeChanged, d, &ElaTextPrivate::onThemeChanged);
}

ElaText::~ElaText()
{
}

void ElaText::setTextSize(int size)
{
    Q_D(ElaText);
    d->_textSize = size;
    QFont font = this->font();
    font.setPixelSize(d->_textSize);
    setFont(font);
}

int ElaText::getTextSize() const
{
    return d_ptr->_textSize;
}

void ElaText::setTextStyle(ElaTextType::TextStyle textStyle)
{
    Q_D(ElaText);
    QFont textFont = font();
    d->_textStyle = textStyle;
    switch (textStyle)
    {
    case ElaTextType::NoStyle:
    {
        break;
    }
    case ElaTextType::Caption:
    {
        d->_textSize = 12;
        textFont.setPixelSize(d->_textSize);
        break;
    }
    case ElaTextType::Body:
    {
        d->_textSize = 13;
        textFont.setPixelSize(d->_textSize);
        break;
    }
    case ElaTextType::BodyStrong:
    {
        d->_textSize = 13;
        textFont.setPixelSize(d->_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Subtitle:
    {
        d->_textSize = 20;
        textFont.setPixelSize(d->_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Title:
    {
        d->_textSize = 28;
        textFont.setPixelSize(d->_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::TitleLarge:
    {
        d->_textSize = 40;
        textFont.setPixelSize(d->_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Display:
    {
        d->_textSize = 68;
        textFont.setPixelSize(d->_textSize);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    }
    setFont(textFont);
}

ElaTextType::TextStyle ElaText::getTextStyle() const
{
    return d_ptr->_textStyle;
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
