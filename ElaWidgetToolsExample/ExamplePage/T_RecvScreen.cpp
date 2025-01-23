#include "T_RecvScreen.h"
#include <QPainter>

T_RecvScreen::T_RecvScreen(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1200, 675);
}

T_RecvScreen::~T_RecvScreen()
{
}

void T_RecvScreen::onSendHandleResult(QPixmap pix)
{
    _pix = std::move(pix);
    update();
}

void T_RecvScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 图像绘制
    if (!_pix.isNull())
    {
        painter.drawPixmap(rect(), _pix);
    }
    painter.restore();
    QWidget::paintEvent(event);
}
