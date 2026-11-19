#include "ElaKeyBinderContainer.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaKeyBinder.h"
#include "ElaTheme.h"

#include <QFontMetrics>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

static constexpr auto keyBindingPrompt = "按下任意按键以进行绑定";
static constexpr int keyPreviewSpacing = 12;

ElaKeyBinderContainer::ElaKeyBinderContainer(QWidget* parent)
    : QWidget(parent)
{
    _keyBinder = dynamic_cast<ElaKeyBinder*>(parent);
    setStyleSheet("#ElaKeyBinderContainer{background-color:transparent;}");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setContentsMargins(20, 16, 20, 16);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(eApp->getFontPixelSize() + 3);
    setFont(textFont);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaKeyBinderContainer::~ElaKeyBinderContainer()
{
}

QSize ElaKeyBinderContainer::sizeHint() const
{
    const QMargins margins = contentsMargins();
    const QFontMetrics metrics = fontMetrics();
    const int keyPreviewHeight = qMax(ElaWidgetToolsSize::DefaultControlHeight, metrics.height() + 8);
    return {margins.left() + metrics.horizontalAdvance(keyBindingPrompt) + margins.right(),
            margins.top() + metrics.height() + keyPreviewSpacing + keyPreviewHeight + margins.bottom()};
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

bool ElaKeyBinderContainer::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (!keyEvent->isAutoRepeat())
        {
            switch (keyEvent->key())
            {
            case Qt::Key_Shift:
            {
                _pBinderKeyText = "Shift";
                break;
            }
            case Qt::Key_Control:
            {
                _pBinderKeyText = "Ctrl";
                break;
            }
            case Qt::Key_Alt:
            {
                _pBinderKeyText = "Alt";
                break;
            }
            case Qt::Key_Meta:
            {
                _pBinderKeyText = "Win";
                break;
            }
            default:
            {
                _pBinderKeyText = QKeySequence(keyEvent->key()).toString();
                break;
            }
            }
            _pNativeVirtualBinderKey = keyEvent->nativeVirtualKey();
            update();
        }
        event->accept();
        return true;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaKeyBinderContainer::mousePressEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        _pBinderKeyText = "鼠标左键";
        _pNativeVirtualBinderKey = 0x01;
        break;
    }
    case Qt::RightButton:
    {
        _pBinderKeyText = "鼠标右键";
        _pNativeVirtualBinderKey = 0x02;
        break;
    }
    case Qt::MiddleButton:
    {
        _pBinderKeyText = "鼠标中键";
        _pNativeVirtualBinderKey = 0x04;
        break;
    }
    case Qt::BackButton:
    {
        _pBinderKeyText = "鼠标后退键";
        _pNativeVirtualBinderKey = 0x05;
        break;
    }
    case Qt::ForwardButton:
    {
        _pBinderKeyText = "鼠标前进键";
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
    const QRect contentRect = contentsRect();
    painter.setPen(ElaThemeColor(_themeMode, BasicText));
    painter.drawText(contentRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, keyBindingPrompt);
    if (_pBinderKeyText.isEmpty())
    {
        painter.restore();
        return;
    }
    // 中心文字背景绘制
    const QFontMetrics metrics = fontMetrics();
    const QRect previewArea = contentRect.adjusted(0, metrics.height() + keyPreviewSpacing, 0, 0);
    const qreal previewWidth = qMin<qreal>(previewArea.width(), qMax(metrics.horizontalAdvance(_pBinderKeyText) * 1.3, 35.0));
    const QRectF previewRect(previewArea.left() + (previewArea.width() - previewWidth) / 2, previewArea.top(), previewWidth, previewArea.height());
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, PrimaryNormal), "Background"));
    painter.drawRoundedRect(previewRect, 5, 5);
    // 文字绘制
    painter.setPen(ElaThemeColor(_themeMode, BasicTextInvert));
    painter.drawText(previewRect, Qt::AlignCenter, _pBinderKeyText);
    painter.restore();
}
