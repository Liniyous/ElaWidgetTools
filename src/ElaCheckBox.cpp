#include "ElaCheckBox.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
ElaCheckBox::ElaCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(25, 25);
    setObjectName("ElaCheckBox");
    setStyleSheet("#ElaCheckBox{background-color:transparent;}");
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaCheckBox::ElaCheckBox(const QString& text, QWidget* parent)
    : QCheckBox(text, parent)
{
    _pBorderRadius = 3;
    setMouseTracking(true);
    setObjectName("ElaCheckBox");
    setStyleSheet("#ElaCheckBox{background-color:transparent;}");
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    int textWidth = fontMetrics().horizontalAdvance(text);
    setFixedSize(textWidth + 25 * 0.8 + _textLeftSpacing, 25);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaCheckBox::~ElaCheckBox()
{
}

bool ElaCheckBox::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Leave:
    {
        _isLeftButtonPress = false;
        _isHoverButton = false;
        break;
    }
    default:
    {
        break;
    }
    }
    return QCheckBox::event(event);
}

bool ElaCheckBox::hitButton(const QPoint& pos) const
{
    QRect buttonRect = QRect(1, height() / 2 - height() * 0.4, height() * 0.8, height() * 0.8);
    return buttonRect.contains(pos);
}

void ElaCheckBox::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (hitButton(event->pos()))
        {
            _isLeftButtonPress = true;
        }
    }
    QCheckBox::mousePressEvent(event);
}

void ElaCheckBox::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _isLeftButtonPress = false;
    }
    QCheckBox::mouseReleaseEvent(event);
}

void ElaCheckBox::mouseMoveEvent(QMouseEvent* event)
{
    if (hitButton(event->pos()))
    {
        _isHoverButton = true;
    }
    else
    {
        _isHoverButton = false;
    }
    QCheckBox::mouseMoveEvent(event);
}

void ElaCheckBox::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 框选绘制
    if (_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(isChecked() ? QColor(0x00, 0x66, 0xB4) : _isLeftButtonPress ? QColor(0xBF, 0xBF, 0xBF)
                                                                                   : QColor(0x87, 0x87, 0x87));
        painter.setBrush(isChecked() ? _isLeftButtonPress ? QColor(0x00, 0x85, 0xEA)
                                       : _isHoverButton   ? QColor(0x00, 0x75, 0xCF)
                                                          : QColor(0x00, 0x66, 0xB4)
                         : _isHoverButton
                             ? QColor(0xEC, 0xEC, 0xEC)
                             : QColor(0xF7, 0xF7, 0xF7));
    }
    else
    {
        painter.setPen(isChecked() ? _isLeftButtonPress ? QColor(0x4C, 0xA0, 0xE0)
                                                        : QColor(0x4C, 0xA0, 0xE0)
                       : _isLeftButtonPress
                           ? QColor(0x5A, 0x5A, 0x5A)
                           : QColor(0xA0, 0xA0, 0xA0));

        painter.setBrush(isChecked() ? _isLeftButtonPress ? QColor(0x3A, 0x7B, 0xAC)
                                       : _isHoverButton   ? QColor(0x42, 0x8B, 0xC3)
                                                          : QColor(0x4C, 0xA0, 0xE0)
                         : _isHoverButton
                             ? QColor(0x48, 0x48, 0x48)
                             : QColor(0x2D, 0x2D, 0x2D));
    }
    painter.drawRoundedRect(QRect(1, height() / 2 - height() * 0.4, height() * 0.8, height() * 0.8), 2, 2);
    painter.setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
    painter.drawText(QRect(height() * 0.8 + _textLeftSpacing, 0, width(), height()), Qt::AlignVCenter, text());

    //图标绘制
    if (isChecked())
    {
        painter.setPen(_themeMode == ElaApplicationType::Light ? Qt::white : Qt::black);
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(height() / 2);
        painter.setFont(iconFont);
        painter.drawText(height() / 5, height() / 2 - height() * 0.4, height() * 0.8, height() * 0.8, Qt::AlignVCenter, QChar((unsigned short)ElaIconType::Check));
    }
    painter.restore();
}
