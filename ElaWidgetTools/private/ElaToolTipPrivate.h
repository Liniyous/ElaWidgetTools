#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLTIPPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLTIPPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaToolTip;
class QVBoxLayout;
class ElaText;
class ElaToolTipPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToolTip)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(int, DisplayMsec)
    ELA_PROPERTY_D(int, ShowDelayMsec)
    ELA_PROPERTY_D(int, HideDelayMsec)
    ELA_PROPERTY_D(QString, ToolTip)
    ELA_PROPERTY_D(QWidget*, CustomWidget)
    ELA_PROPERTY(qreal, Opacity)

public:
    explicit ElaToolTipPrivate(QObject* parent = nullptr);
    ~ElaToolTipPrivate() override;

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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLTIPPRIVATE_H_

