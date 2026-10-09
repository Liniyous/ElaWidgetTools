#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACENTRALSTACKEDWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACENTRALSTACKEDWIDGET_H_

#include "ElaPropertyMacro.h"
#include <QStackedWidget>

#include "ElaWidgetToolsDef.h"
#include <QVBoxLayout>
class QGraphicsBlurEffect;
class ELA_EXPORT ElaCentralStackedWidget : public QWidget
{
    Q_OBJECT
    ELA_PROPERTY(int, PopupAnimationYOffset)
    ELA_PROPERTY(qreal, ScaleAnimationRatio)
    ELA_PROPERTY(qreal, ScaleAnimationPixOpacity)
    ELA_PROPERTY(qreal, FlipAnimationRatio)
    ELA_PROPERTY(int, BlurAnimationRadius)
    ELA_PROPERTY(int, LastTargetIndex)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACENTRALSTACKEDWIDGET_H_
