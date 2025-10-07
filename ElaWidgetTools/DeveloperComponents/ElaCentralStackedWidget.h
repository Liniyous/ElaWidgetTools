#ifndef ELACENTRALSTACKEDWIDGET_H
#define ELACENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "ElaDef.h"
#include <QVBoxLayout>
class QGraphicsBlurEffect;
class ElaCentralStackedWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, PopupAnimationYOffset)
    Q_PROPERTY_CREATE(qreal, ScaleAnimationRatio)
    Q_PROPERTY_CREATE(qreal, ScaleAnimationPixOpacity)
    Q_PROPERTY_CREATE(qreal, FlipAnimationRatio)
    Q_PROPERTY_CREATE(int, BlurAnimationRadius)
public:
    explicit ElaCentralStackedWidget(QWidget* parent = nullptr);
    ~ElaCentralStackedWidget() override;

    QStackedWidget* getContainerStackedWidget() const;

    void setCustomWidget(QWidget* widget);
    QWidget* getCustomWidget() const;

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
    QGraphicsBlurEffect* _blurEffect{nullptr};
    bool _isTransparent{false};
    QVBoxLayout* _mainLayout{nullptr};
    QWidget* _customWidget{nullptr};
    QStackedWidget* _containerStackedWidget{nullptr};
    bool _isHasRadius{true};
    bool _isDrawNewPix{false};
    void _getTargetStackPix();
    void _getCurrentStackPix();
};

#endif // ELACENTRALSTACKEDWIDGET_H
