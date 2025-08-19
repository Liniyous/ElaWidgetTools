#include "ElaColorPicker.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <utility>

#include "ElaTheme.h"
ElaColorPicker::ElaColorPicker(QWidget* parent)
    : QWidget{parent}
{
    setFixedSize(260, 260);
    _pThemeMode = eTheme->getThemeMode();
    // 初始化色相图
    QPixmap colorPickerPix(QSize(360, 360));
    QImage colorPickerImage = colorPickerPix.toImage();
    for (int y = 0; y < colorPickerImage.height(); y++)
    {
        for (int x = 0; x < colorPickerImage.width(); x++)
        {
            QColor pixColor;
            pixColor.setHsvF((qreal)1 / colorPickerPix.width() * x, 1 - (qreal)1 / (colorPickerPix.height() - 1) * y, 1);
            pixColor = pixColor.toRgb();
            colorPickerImage.setPixelColor(x, y, pixColor);
        }
    }
    _colorPickerImage = colorPickerImage;
}

ElaColorPicker::~ElaColorPicker()
{
}

void ElaColorPicker::setSelectedColor(QColor color)
{
    _selectedColor = std::move(color);
    _selectedPoint = QPoint(qRound(3 + 254 * _selectedColor.hueF() + 0.5f), qRound(257 - 254 * _selectedColor.saturationF() + 0.5f));
    update();
}

QColor ElaColorPicker::getSelectedColor() const
{
    return _selectedColor;
}

void ElaColorPicker::mousePressEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    QPointF colorPoint((_selectedPoint.x() - 3) / 254.0 * 359, (_selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal)1 / _colorPickerImage.width() * colorPoint.x(), 1 - (qreal)1 / (_colorPickerImage.height() - 1) * colorPoint.y(), 1);
    _selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(_selectedColor);
    QWidget::mousePressEvent(event);
    update();
}

void ElaColorPicker::mouseReleaseEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    QPointF colorPoint((_selectedPoint.x() - 3) / 254.0 * 359, (_selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal)1 / _colorPickerImage.width() * colorPoint.x(), 1 - (qreal)1 / (_colorPickerImage.height() - 1) * colorPoint.y(), 1);
    _selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(_selectedColor);
    QWidget::mouseReleaseEvent(event);
    update();
}

void ElaColorPicker::mouseMoveEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    QPointF colorPoint((_selectedPoint.x() - 3) / 254.0 * 359, (_selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal)1 / _colorPickerImage.width() * colorPoint.x(), 1 - (qreal)1 / (_colorPickerImage.height() - 1) * colorPoint.y(), 1);
    _selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(_selectedColor);
    QWidget::mouseMoveEvent(event);
    update();
}

void ElaColorPicker::paintEvent(QPaintEvent* event)
{
    int penWidth = 3;
    int borderRadius = 5;
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 色相图绘制
    QRect pickerRect = rect();
    pickerRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
    painter.setPen(QPen(ElaThemeColor(_pThemeMode, BasicBorder), penWidth));
    painter.drawRoundedRect(pickerRect, borderRadius, borderRadius);
    QPainterPath path;
    path.addRoundedRect(pickerRect, borderRadius, borderRadius);
    painter.setClipPath(path);
    painter.drawImage(pickerRect, _colorPickerImage);

    // 取色点绘制
    if (!_selectedPoint.isNull())
    {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(_selectedPoint, 8, 8);
    }
    painter.restore();
}

QPoint ElaColorPicker::_adjustPointLimit(QPoint point)
{
    QRect pickerRect = rect();
    if (point.x() < 3)
    {
        point.setX(3);
    }
    else if (point.x() > pickerRect.width() - 3)
    {
        point.setX(pickerRect.width() - 3);
    }
    if (point.y() < 3)
    {
        point.setY(3);
    }
    else if (point.y() > pickerRect.height() - 3)
    {
        point.setY(pickerRect.height() - 3);
    }
    return point;
}
