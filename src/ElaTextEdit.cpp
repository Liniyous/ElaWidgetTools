#include "ElaTextEdit.h"

#include <QPainter>
#include "ElaTheme.h"
#include "private/ElaTextEditPrivate.h"
ElaTextEdit::ElaTextEdit(QWidget* parent)
    : QPlainTextEdit(parent), d_ptr(new ElaTextEditPrivate())
{
    Q_D(ElaTextEdit);
    d->q_ptr = this;
    setObjectName("ElaTextEdit");
    setStyleSheet("#ElaTextEdit{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    textFont.setPixelSize(28);
    setFont(textFont);
    d->_themeMode = eTheme->getThemeMode();
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaTextEditPrivate::onThemeChanged);
}

ElaTextEdit::ElaTextEdit(QString text, QWidget* parent)
    : ElaTextEdit(parent)
{
    setPlainText(text);
}

ElaTextEdit::ElaTextEdit(QString text, int pixelSize, QWidget* parent)
    : ElaTextEdit(text, parent)
{
    QFont font = this->font();
    font.setPixelSize(pixelSize);
    setFont(font);
}

ElaTextEdit::~ElaTextEdit()
{
}


void ElaTextEdit::setTextPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
}

int ElaTextEdit::getTextPixelSize() const
{
    return this->font().pixelSize();
}

void ElaTextEdit::setTextPointSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
}

int ElaTextEdit::getTextPointSize() const
{
    return this->font().pointSize();
}

void ElaTextEdit::setTextStyle(ElaTextType::TextStyle textStyle)
{
    Q_D(ElaTextEdit);
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
    }
    setFont(textFont);
}

ElaTextType::TextStyle ElaTextEdit::getTextStyle() const
{
    Q_D(const ElaTextEdit);
    return d->_textStyle;
}

void ElaTextEdit::paintEvent(QPaintEvent* event)
{
    Q_D(ElaTextEdit);
    QPlainTextEdit::paintEvent(event);
}
