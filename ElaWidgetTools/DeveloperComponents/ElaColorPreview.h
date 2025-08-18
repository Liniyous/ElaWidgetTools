#ifndef ELACOLORPREVIEW_H
#define ELACOLORPREVIEW_H

#include <QWidget>

#include "ElaDef.h"
class ElaColorPreview : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QColor, BaseColor)
    Q_PRIVATE_CREATE(ElaThemeType::ThemeMode, ThemeMode)
public:
    explicit ElaColorPreview(QWidget* parent = nullptr);
    ~ElaColorPreview();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELACOLORPREVIEW_H
