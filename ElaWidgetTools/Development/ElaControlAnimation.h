#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLANIMATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLANIMATION_H_

#include <QColor>
#include <QObject>
#include <QPointer>

class QVariantAnimation;
class QWidget;
class ElaControlAnimation : public QObject
{
    Q_OBJECT
public:
    explicit ElaControlAnimation(QWidget* widget);
    QColor getBackgroundColor(const QColor& target);
    qreal getChevronOffset() const;
    bool getIsPressed() const;
    void release();
    static bool getIsAnimationEnabled(const QWidget* widget);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QPointer<QWidget> _widget;
    QVariantAnimation* _chevronAnimation{nullptr};
    qreal _chevronOffset{0};
    qreal _releaseOffset{0};
    bool _isPressed{false};
    void _setPressed(bool isPressed);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLANIMATION_H_
