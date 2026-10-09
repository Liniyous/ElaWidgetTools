#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAROLLERPICKERCONTAINER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAROLLERPICKERCONTAINER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QPropertyAnimation>
#include <QWidget>
class ElaRoller;
class ElaRollerPickerContainer : public QWidget
{
    Q_OBJECT
    ELA_PROPERTY(int, AnimationPixOffsetY)
    ELA_VARIABLE(int, ButtonAreaHeight)
public:
    explicit ElaRollerPickerContainer(QWidget* parent = nullptr);
    ~ElaRollerPickerContainer() override;

    void doPickerAnimation();

Q_SIGNALS:
    Q_SIGNAL void overButtonClicked();
    Q_SIGNAL void cancelButtonClicked();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    friend class ElaRollerPicker;
    friend class ElaRollerPickerPrivate;
    int _buttonMargin{5};
    int _buttonSpacing{5};
    QPixmap _animationPix;
    QRect _overButtonRect;
    QRect _cancelButtonRect;
    bool _isOverButtonHover{false};
    bool _isCancelButtonHover{false};
    bool _isOverButtonClicked{false};
    QList<ElaRoller*> _rollerList;
    QList<int> _historyIndexList;
    ElaThemeType::ThemeMode _themeMode;
    void _handleSaveOrReset(bool isSave);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAROLLERPICKERCONTAINER_H_

