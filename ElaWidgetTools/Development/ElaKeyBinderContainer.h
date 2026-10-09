#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QWidget>
class ElaKeyBinder;
class ElaKeyBinderContainer : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(QString, BinderKeyText)
    ELA_VARIABLE(quint32, NativeVirtualBinderKey)
public:
    explicit ElaKeyBinderContainer(QWidget* parent = nullptr);
    ~ElaKeyBinderContainer() override;
    void logOrResetHistoryData(bool isLog);
    void saveBinderChanged();

protected:
    bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QString _historyBinderKeyText;
    quint32 _historyNativeVirtualBinderKey{0};
    ElaThemeType::ThemeMode _themeMode;
    ElaKeyBinder* _keyBinder{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_

