#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPICKER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPICKER_H_

#include "ElaPropertyMacro.h"
#include <QPixmap>
#include <QWidget>

#include "ElaWidgetToolsDef.h"
class ElaColorPicker : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(ElaThemeType::ThemeMode, ThemeMode)

public:
    explicit ElaColorPicker(QWidget* parent = nullptr);
    ~ElaColorPicker();

    void setSelectedColor(const QColor& color);
    const QColor& getSelectedColor() const;

Q_SIGNALS:
    Q_SIGNAL void selectedColorChanged(const QColor& selectedColor);

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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPICKER_H_

