#include "ElaKeyBinder.h"
#include "ElaContentDialog.h"
#include "ElaKeyBinderContainer.h"
#include "ElaKeyBinderPrivate.h"
#include "ElaTheme.h"
#include <QMouseEvent>
#include <QPainter>
#include <utility>
Q_PROPERTY_CREATE_Q_CPP(ElaKeyBinder, int, BorderRadius)
ElaKeyBinder::ElaKeyBinder(QWidget* parent)
    : QLabel(parent), d_ptr(new ElaKeyBinderPrivate())
{
    Q_D(ElaKeyBinder);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    setFixedHeight(35);
    setMouseTracking(true);
    setStyleSheet("#ElaKeyBinder{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(15);
    setFont(textFont);
    d->_binderContainer = new ElaKeyBinderContainer(this);
    setText(u8"  按键: " + QString(u8"未绑定") + "      ");
    d->_binderDialog = new ElaContentDialog(window());
    d->_binderDialog->setCentralWidget(d->_binderContainer);
    d->_binderDialog->setLeftButtonText(u8"取消");
    d->_binderDialog->setMiddleButtonText(u8"重置");
    d->_binderDialog->setRightButtonText(u8"确认");
    connect(d->_binderDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        d->_binderContainer->logOrResetHistoryData(false);
    });
    connect(d->_binderDialog, &ElaContentDialog::rightButtonClicked, this, [=]() {
        d->_binderContainer->saveBinderChanged();
    });
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaKeyBinderPrivate::onThemeChanged);
}

ElaKeyBinder::~ElaKeyBinder()
{
}

void ElaKeyBinder::setBinderKeyText(QString binderKeyText)
{
    Q_D(ElaKeyBinder);
    d->_binderContainer->setBinderKeyText(binderKeyText);
    setText(u8"  按键: " + binderKeyText + "      ");
}

QString ElaKeyBinder::getBinderKeyText() const
{
    Q_D(const ElaKeyBinder);
    return d->_binderContainer->getBinderKeyText();
}

void ElaKeyBinder::setNativeVirtualBinderKey(quint32 binderKey)
{
    Q_D(ElaKeyBinder);
    d->_binderContainer->setNativeVirtualBinderKey(binderKey);
}

quint32 ElaKeyBinder::getNativeVirtualBinderKey() const
{
    Q_D(const ElaKeyBinder);
    return d->_binderContainer->getNativeVirtualBinderKey();
}

bool ElaKeyBinder::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    case QEvent::Leave:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QLabel::event(event);
}

void ElaKeyBinder::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaKeyBinder);
    if (event->button() == Qt::LeftButton)
    {
        d->_binderDialog->show();
        d->_binderContainer->setFocus();
        d->_binderContainer->logOrResetHistoryData(true);
    }
    QLabel::mouseReleaseEvent(event);
}

void ElaKeyBinder::paintEvent(QPaintEvent* event)
{
    Q_D(ElaKeyBinder);
    if (palette().color(QPalette::WindowText) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase));
    QRect borderRect = rect();
    borderRect.adjust(1, 1, -1, -1);
    painter.drawRoundedRect(borderRect, d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(16);
    painter.setFont(iconFont);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    QRect iconRect = rect();
    iconRect.adjust(0, 0, -10, 0);
    painter.drawText(iconRect, Qt::AlignVCenter | Qt::AlignRight, QChar((unsigned short)ElaIconType::Pencil));
    painter.restore();
    QLabel::paintEvent(event);
}