#include "ElaColorPicker.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaColorPicker::ElaColorPicker(QWidget* parent)
    : QWidget{parent}
{
    setFixedSize(260, 260);
    _pThemeMode = eTheme->getThemeMode();
    // 初始化色相图
    QPixmap ColorPickerPix(QSize(257, 257));
    QImage ColorPickerImage = ColorPickerPix.toImage();
    for (int y = 0; y < ColorPickerImage.height(); y++)
    {
        for (int x = 0; x < ColorPickerImage.width(); x++)
        {
            QColor pixColor;
            pixColor.setHsvF((qreal)1 / ColorPickerPix.width() * x, 1 - (qreal)1 / ColorPickerPix.height() * y, 1);
            pixColor = pixColor.toRgb();
            ColorPickerImage.setPixelColor(x, y, pixColor);
        }
    }
    _colorPickerImage = ColorPickerImage;
}

ElaColorPicker::~ElaColorPicker()
{
}

void ElaColorPicker::setSelectedColor(QColor color)
{
    _selectedColor = color;
    QColor hsvColor = color.toHsv();
    _selectedPoint = QPoint(257 * hsvColor.hue() / (qreal)360, 255 - 257 * hsvColor.saturation() / (qreal)255);
    update();
}

QColor ElaColorPicker::getSelectedColor() const
{
    return _selectedColor;
}

void ElaColorPicker::mousePressEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    _selectedColor = _colorPickerImage.pixelColor(_selectedPoint);
    Q_EMIT selectedColorChanged(_selectedColor);
    QWidget::mousePressEvent(event);
    update();
}

void ElaColorPicker::mouseReleaseEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    _selectedColor = _colorPickerImage.pixelColor(_selectedPoint);
    Q_EMIT selectedColorChanged(_selectedColor);
    QWidget::mouseReleaseEvent(event);
    update();
}

void ElaColorPicker::mouseMoveEvent(QMouseEvent* event)
{
    _selectedPoint = _adjustPointLimit(event->pos());
    _selectedColor = _colorPickerImage.pixelColor(_selectedPoint);
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
    painter.setRenderHint(QPainter::Antialiasing);
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
    else if (point.x() > pickerRect.right() - 3)
    {
        point.setX(pickerRect.right() - 3);
    }
    if (point.y() < 3)
    {
        point.setY(3);
    }
    else if (point.y() > pickerRect.bottom() - 3)
    {
        point.setY(pickerRect.bottom() - 3);
    }
    return point;
}
