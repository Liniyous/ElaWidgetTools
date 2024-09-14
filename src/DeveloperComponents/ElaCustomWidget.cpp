#include "ElaCustomWidget.h"

#include <QPainter>
#include <QVBoxLayout>

#include "ElaAppBar.h"
#include "ElaTheme.h"
ElaCustomWidget::ElaCustomWidget(QWidget* parent)
    : QWidget{parent}
{
    resize(500, 500); // 默认宽高
                      // 自定义AppBar
    setObjectName("ElaCustomWidget");
    _appBar = new ElaAppBar(this);
    _appBar->setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::CloseButtonHint);
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
    setAttribute(Qt::WA_DeleteOnClose);
}

ElaCustomWidget::~ElaCustomWidget()
{
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
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(_themeMode, WindowBaseStart));
    painter.drawRect(rect());
    painter.restore();
}
