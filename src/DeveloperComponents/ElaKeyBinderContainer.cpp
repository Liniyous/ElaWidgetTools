#include "ElaKeyBinderContainer.h"
#include "ElaKeyBinder.h"
#include "ElaTheme.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
ElaKeyBinderContainer::ElaKeyBinderContainer(QWidget* parent)
    : QWidget(parent)
{
    _keyBinder = dynamic_cast<ElaKeyBinder*>(parent);
    setStyleSheet("#ElaKeyBinderContainer{background-color:transparent;}");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setFixedHeight(140);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(16);
    setFont(textFont);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaKeyBinderContainer::~ElaKeyBinderContainer()
{
}

void ElaKeyBinderContainer::logOrResetHistoryData(bool isLog)
{
    if (isLog)
    {
        _historyBinderKeyText = _pBinderKeyText;
        _historyNativeVirtualBinderKey = _pNativeVirtualBinderKey;
    }
    else
    {
        _pBinderKeyText = _historyBinderKeyText;
        _pNativeVirtualBinderKey = _historyNativeVirtualBinderKey;
        update();
    }
}

void ElaKeyBinderContainer::saveBinderChanged()
{
    Q_EMIT _keyBinder->binderKeyTextChanged(_pBinderKeyText);
    Q_EMIT _keyBinder->nativeVirtualBinderKeyChanged(_pNativeVirtualBinderKey);
    if (_pBinderKeyText.isEmpty())
    {
        _keyBinder->setText(u8"  按键: " + QString(u8"未绑定") + "      ");
    }
    else
    {
        _keyBinder->setText(u8"  按键: " + _pBinderKeyText + "      ");
    }
}

void ElaKeyBinderContainer::mousePressEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        _pBinderKeyText = "Mouse1";
        _pNativeVirtualBinderKey = 0x01;
        break;
    }
    case Qt::RightButton:
    {
        _pBinderKeyText = "Mouse2";
        _pNativeVirtualBinderKey = 0x02;
        break;
    }
    case Qt::MiddleButton:
    {
        _pBinderKeyText = "Middle";
        _pNativeVirtualBinderKey = 0x04;
        break;
    }
    case Qt::BackButton:
    {
        _pBinderKeyText = "Back";
        _pNativeVirtualBinderKey = 0x05;
        break;
    }
    case Qt::ForwardButton:
    {
        _pBinderKeyText = "Forward";
        _pNativeVirtualBinderKey = 0x06;
        break;
    }
    default:
    {
        break;
    }
    }
    QWidget::mousePressEvent(event);
    update();
}

void ElaKeyBinderContainer::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        _pBinderKeyText = QKeySequence(event->key()).toString();
        _pNativeVirtualBinderKey = event->nativeVirtualKey();
        update();
    }
    QWidget::keyPressEvent(event);
}

void ElaKeyBinderContainer::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    setFocus();
}

void ElaKeyBinderContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 顶部提示绘制
    painter.setPen(ElaThemeColor(_themeMode, BasicText));
    painter.drawText(QRect(20, 20, width(), 50), Qt::AlignLeft | Qt::AlignTop, u8"按下任意按键以进行绑定");
    if (_pBinderKeyText.isEmpty())
    {
        painter.restore();
        return;
    }
    // 中心文字背景绘制
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
    qreal textWidth = fontMetrics().horizontalAdvance(_pBinderKeyText) * 1.3;
    qreal textHeight = fontMetrics().height() * 1.2;
    textWidth = qMax(textWidth, 35.0);
    textHeight = qMax(textHeight, 35.0);
    painter.drawRoundedRect(QRectF((width() - textWidth) / 2.0, rect().center().y() + 10, textWidth, textHeight), 5, 5);
    // 文字绘制
    painter.setPen(ElaThemeColor(_themeMode, BasicTextInvert));
    painter.drawText(QRectF((width() - textWidth) / 2.0, rect().center().y() + 10, textWidth, textHeight), Qt::AlignCenter, _pBinderKeyText);
    painter.restore();
}
