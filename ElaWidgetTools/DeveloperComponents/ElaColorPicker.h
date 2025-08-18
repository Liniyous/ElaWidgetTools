#ifndef ELACOLORPICKER_H
#define ELACOLORPICKER_H

#include <QPixmap>
#include <QWidget>

#include "ElaDef.h"
class ElaColorPicker : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaThemeType::ThemeMode, ThemeMode)

public:
    explicit ElaColorPicker(QWidget* parent = nullptr);
    ~ElaColorPicker();

    void setSelectedColor(QColor color);
    QColor getSelectedColor() const;

Q_SIGNALS:
    Q_SIGNAL void selectedColorChanged(QColor selectedColor);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QImage _colorPickerImage;
    QPoint _selectedPoint;
    QColor _selectedColor;
    QPoint _adjustPointLimit(QPoint point);
};

#endif // ELACOLORPICKER_H
