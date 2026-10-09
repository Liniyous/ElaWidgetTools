#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPLAINTEXTEDITPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPLAINTEXTEDITPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
#include <QVariantMap>
class ElaEvent;
class ElaPlainTextEdit;
class ElaPlainTextEditStyle;
class ElaPlainTextEditPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPlainTextEdit)

public:
    explicit ElaPlainTextEditPrivate(QObject* parent = nullptr);
    ~ElaPlainTextEditPrivate() override;
    Q_INVOKABLE void onWMWindowClickedEvent(const QVariantMap& data);
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaPlainTextEditStyle* _style{nullptr};
    ElaEvent* _focusEvent{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPLAINTEXTEDITPRIVATE_H_

