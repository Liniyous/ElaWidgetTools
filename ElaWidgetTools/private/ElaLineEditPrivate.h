#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALINEEDITPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALINEEDITPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
#include <QVariantMap>
class ElaEvent;
class ElaLineEdit;
class ElaLineEditPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaLineEdit)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(bool, IsClearButtonEnable)
    ELA_PROPERTY(qreal, ExpandMarkWidth)
public:
    explicit ElaLineEditPrivate(QObject* parent = nullptr);
    ~ElaLineEditPrivate() override;
    Q_INVOKABLE void onWMWindowClickedEvent(const QVariantMap& data);
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaEvent* _focusEvent{nullptr};
    qreal _textSpacing{0.5};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALINEEDITPRIVATE_H_

