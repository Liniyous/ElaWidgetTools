#ifndef ELATOOLTIPPRIVATE_H
#define ELATOOLTIPPRIVATE_H

#include <QObject>

#include "Def.h"
class ElaToolTip;
class QVBoxLayout;
class ElaText;
class ElaToolTipPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolTip)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DisplayMsec)
    Q_PROPERTY_CREATE_D(int, ShowDelayMsec)
    Q_PROPERTY_CREATE_D(int, HideDelayMsec)
    Q_PROPERTY_CREATE_D(QString, ToolTip)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE(qreal, Opacity)

public:
    explicit ElaToolTipPrivate(QObject* parent = nullptr);
    ~ElaToolTipPrivate();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

private:
    ElaText* _toolTipText{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{6};
    void _doShowAnimation();
    void _updatePos();
};

#endif // ELATOOLTIPPRIVATE_H
