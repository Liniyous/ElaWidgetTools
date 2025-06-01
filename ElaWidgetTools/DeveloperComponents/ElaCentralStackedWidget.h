#ifndef ELACENTRALSTACKEDWIDGET_H
#define ELACENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "Def.h"
class ElaCentralStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, PopupAnimationYOffset)
    Q_PROPERTY_CREATE(qreal, ScaleAnimationRatio)
    Q_PROPERTY_CREATE(qreal, ScaleAnimationPixOpacity)
    Q_PROPERTY_CREATE(qreal, FlipAnimationRatio)
public:
    explicit ElaCentralStackedWidget(QWidget* parent = nullptr);
    ~ElaCentralStackedWidget() override;
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setIsHasRadius(bool isHasRadius);

    void doWindowStackSwitch(ElaWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ElaWindowType::StackSwitchMode _stackSwitchMode{ElaWindowType::StackSwitchMode::Popup};
    ElaThemeType::ThemeMode _themeMode;
    QPixmap _targetStackPix;
    QPixmap _currentStackPix;
    bool _isTransparent{false};
    bool _isHasRadius{true};
    bool _isDrawNewPix{false};
    void _getTargetStackPix();
    void _getCurrentStackPix();
};

#endif // ELACENTRALSTACKEDWIDGET_H
