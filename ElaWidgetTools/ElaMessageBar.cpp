#include "ElaMessageBar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>

#include "ElaIconButton.h"
#include "ElaTheme.h"
#include "private/ElaMessageBarPrivate.h"

ElaMessageBar::ElaMessageBar(ElaMessageBarType::PositionPolicy policy, ElaMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaMessageBarPrivate())
{
    Q_D(ElaMessageBar);
    d->q_ptr = this;
    d->_borderRadius = 6;
    d->_title = title;
    d->_text = text;
    d->_policy = policy;
    d->_messageMode = messageMode;
    d->_themeMode = eTheme->getThemeMode();
    setFixedHeight(60);
    setMouseTracking(true);
    d->_pOpacity = 1;
    setFont(QFont("微软雅黑"));
    parent->installEventFilter(this);
    d->_closeButton = new ElaIconButton(ElaIconType::Xmark, 17, d->_closeButtonWidth, 30, this);
    switch (d->_messageMode)
    {
    case ElaMessageBarType::Success:
    {
        d->_closeButton->setLightHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case ElaMessageBarType::Warning:
    {
        d->_closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setLightIconColor(Qt::white);
        d->_closeButton->setDarkIconColor(Qt::white);
        break;
    }
    case ElaMessageBarType::Information:
    {
        d->_closeButton->setLightHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case ElaMessageBarType::Error:
    {
        d->_closeButton->setLightHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    }
    d->_closeButton->setBorderRadius(5);
    connect(d->_closeButton, &ElaIconButton::clicked, d, &ElaMessageBarPrivate::onCloseButtonClicked);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(d->_closeButton);
    setObjectName("ElaMessageBar");
    setStyleSheet("#ElaMessageBar{background-color:transparent;}");
    d->_messageBarCreate(displayMsec);
}

ElaMessageBar::~ElaMessageBar()
{
}

void ElaMessageBar::success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    // qDebug() << QApplication::topLevelWidgets();
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }

    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Success, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void ElaMessageBar::warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Warning, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void ElaMessageBar::information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Information, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void ElaMessageBar::error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Error, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void ElaMessageBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMessageBar);
    QPainter painter(this);
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_borderRadius);

    // 背景和图标绘制
    painter.save();
    painter.setPen(d->_themeMode == ElaThemeType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
    switch (d->_messageMode)
    {
    case ElaMessageBarType::Success:
    {
        d->_drawSuccess(&painter);
        break;
    }
    case ElaMessageBarType::Warning:
    {
        d->_drawWarning(&painter);
        break;
    }
    case ElaMessageBarType::Information:
    {
        d->_drawInformation(&painter);
        break;
    }
    case ElaMessageBarType::Error:
    {
        d->_drawError(&painter);
        break;
    }
    }
    // 文字绘制
    // 标题
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(16);
    painter.setFont(font);
    int titleTextWidth = painter.fontMetrics().horizontalAdvance(d->_title) + 1;
    if (titleTextWidth > 100)
    {
        titleTextWidth = 100;
    }
    int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere;
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing, -1, titleTextWidth, height()), textFlags, d->_title);
    // 正文
    font.setWeight(QFont::Light);
    font.setPixelSize(15);
    painter.setFont(font);
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin / 2), height()), textFlags, d->_text);
    int textHeight = painter.fontMetrics().boundingRect(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin), height()), textFlags, d->_text).height();
    if (textHeight >= minimumHeight() - 20)
    {
        setMinimumHeight(textHeight + 20);
    }
    painter.restore();
}

bool ElaMessageBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaMessageBar);
    if (watched == parentWidget())
    {
        switch (event->type())
        {
        case QEvent::Resize:
        {
            QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(event);
            QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
            if (d->_isNormalDisplay)
            {
                switch (d->_policy)
                {
                case ElaMessageBarType::Top:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->y());
                    break;
                }
                case ElaMessageBarType::Bottom:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->pos().y() + offsetSize.height());
                    break;
                }
                case ElaMessageBarType::Left:
                case ElaMessageBarType::TopLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y());
                    break;
                }
                case ElaMessageBarType::BottomLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                case ElaMessageBarType::Right:
                case ElaMessageBarType::TopRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->y());
                    break;
                }
                case ElaMessageBarType::BottomRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QWidget::eventFilter(watched, event);
}
