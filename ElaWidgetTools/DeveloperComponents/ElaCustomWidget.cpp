#include "ElaCustomWidget.h"

#include <QPainter>
#include <QVBoxLayout>

#include "ElaApplication.h"
#include "ElaTheme.h"
Q_TAKEOVER_NATIVEEVENT_CPP(ElaCustomWidget, _appBar);
ElaCustomWidget::ElaCustomWidget(QWidget* parent)
    : QDialog{parent}
{
    resize(500, 500); // 默认宽高
    setObjectName("ElaCustomWidget");
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3) || QT_VERSION > QT_VERSION_CHECK(6, 6, 1))
    setStyleSheet("#ElaCustomWidget{background-color:transparent;}");
#endif
    // 自定义AppBar
    _appBar = new ElaAppBar(this);
    _appBar->setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint);
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });

    _windowDisplayMode = eApp->getWindowDisplayMode();
    connect(eApp, &ElaApplication::pWindowDisplayModeChanged, this, [=]() {
        _windowDisplayMode = eApp->getWindowDisplayMode();
        update();
    });
    eApp->syncWindowDisplayMode(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

ElaCustomWidget::~ElaCustomWidget()
{
    eApp->syncWindowDisplayMode(this, false);
}

void ElaCustomWidget::setCentralWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    _centralWidget = widget;
    _mainLayout->addWidget(widget);
    widget->setVisible(true);
}

void ElaCustomWidget::paintEvent(QPaintEvent* event)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    if (_windowDisplayMode != ElaApplicationType::WindowDisplayMode::ElaMica)
#else
    if (_windowDisplayMode == ElaApplicationType::WindowDisplayMode::Normal)
#endif
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaThemeColor(_themeMode, WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
}
