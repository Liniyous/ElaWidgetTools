#include "ElaText.h"

#include <QContextMenuEvent>
#include <QPainter>

#include "ElaTheme.h"
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
    textFont.setPixelSize(28);
    setFont(textFont);
    setWordWrap(true);
    d->_themeMode = eTheme->getThemeMode();
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaTextPrivate::onThemeChanged);
}

ElaText::ElaText(QString text, QWidget* parent)
    : ElaText(parent)
{
    setText(text);
}

ElaText::ElaText(QString text, int pixelSize, QWidget* parent)
    : ElaText(text, parent)
{
    QFont font = this->font();
    font.setPixelSize(pixelSize);
    setFont(font);
}

ElaText::~ElaText()
{
}

void ElaText::setIsWrapAnywhere(bool isWrapAnywhere)
{
    Q_D(ElaText);
    setWordWrap(isWrapAnywhere);
    d->_isWrapAnywhere = isWrapAnywhere;
}

bool ElaText::getIsWrapAnywhere() const
{
    Q_D(const ElaText);
    return d->_isWrapAnywhere;
}

void ElaText::setTextPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
}

int ElaText::getTextPixelSize() const
{
    return this->font().pixelSize();
}

void ElaText::setTextPointSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
}

int ElaText::getTextPointSize() const
{
    return this->font().pointSize();
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
        textFont.setPixelSize(12);
        break;
    }
    case ElaTextType::Body:
    {
        textFont.setPixelSize(13);
        break;
    }
    case ElaTextType::BodyStrong:
    {
        textFont.setPixelSize(13);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Subtitle:
    {
        textFont.setPixelSize(20);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Title:
    {
        textFont.setPixelSize(28);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::TitleLarge:
    {
        textFont.setPixelSize(40);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Display:
    {
        textFont.setPixelSize(48);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::CardTitle:
    {
        textFont.setPixelSize(15);
        break;
    }
    case ElaTextType::CardsubTitle:
    {
        textFont.setPixelSize(12);
        textFont.setWeight(QFont::Normal);
        break;
    }
    }
    setFont(textFont);
}

ElaTextType::TextStyle ElaText::getTextStyle() const
{
    Q_D(const ElaText);
    return d->_textStyle;
}

void ElaText::paintEvent(QPaintEvent* event)
{
    Q_D(ElaText);
    if (wordWrap() && d->_isWrapAnywhere)
    {
        Q_D(ElaText);
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, text());
        painter.restore();
    }
    else
    {
        QLabel::paintEvent(event);
    }
}
