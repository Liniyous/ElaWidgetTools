#ifndef ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_
#define ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_

#include "Def.h"
#include <QWidget>
class ElaKeyBinder;
class ElaKeyBinderContainer : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QString, BinderKeyText)
    Q_PRIVATE_CREATE(quint32, NativeVirtualBinderKey)
public:
    explicit ElaKeyBinderContainer(QWidget* parent = nullptr);
    ~ElaKeyBinderContainer();
    void logOrResetHistoryData(bool isLog);
    void saveBinderChanged();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QString _historyBinderKeyText;
    quint32 _historyNativeVirtualBinderKey{0};
    ElaThemeType::ThemeMode _themeMode;
    ElaKeyBinder* _keyBinder{nullptr};
};

#endif//ELAWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_ELAKEYBINDERCONTAINER_H_
