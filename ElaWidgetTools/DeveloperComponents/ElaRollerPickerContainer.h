#ifndef ELAFRAMEWORK_ELAROLLERPICKERCONTAINER_H
#define ELAFRAMEWORK_ELAROLLERPICKERCONTAINER_H

#include "ElaDef.h"
#include <QPropertyAnimation>
#include <QWidget>
class ElaRoller;
class ElaRollerPickerContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, AnimationPixOffsetY)
    Q_PRIVATE_CREATE(int, ButtonAreaHeight)
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

#endif //ELAFRAMEWORK_ELAROLLERPICKERCONTAINER_H
