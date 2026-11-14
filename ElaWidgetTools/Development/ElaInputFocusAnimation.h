#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAINPUTFOCUSANIMATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAINPUTFOCUSANIMATION_H_

#include <QPointer>
#include <QPropertyAnimation>

class QWidget;
class ElaInputFocusAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    ElaInputFocusAnimation(QObject* style, QWidget* widget);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QWidget* _widget{nullptr};
    QPointer<QWidget> _focusProxy;
    void _syncFocusProxy();
    void _animateFocus(bool isFocused);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELAINPUTFOCUSANIMATION_H_
