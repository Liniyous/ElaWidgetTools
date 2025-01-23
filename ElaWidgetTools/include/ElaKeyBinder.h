#ifndef ELAWIDGETTOOLS_SRC_ELAKEYBINDER_H_
#define ELAWIDGETTOOLS_SRC_ELAKEYBINDER_H_

#include "Def.h"
#include <QLabel>

class ElaKeyBinderPrivate;
class ELA_EXPORT ElaKeyBinder : public QLabel
{
    Q_OBJECT
    Q_Q_CREATE(ElaKeyBinder)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, BinderKeyText)
    Q_PROPERTY_CREATE_Q_H(quint32, NativeVirtualBinderKey)
public:
    explicit ElaKeyBinder(QWidget* parent = nullptr);
    ~ElaKeyBinder();

Q_SIGNALS:
    Q_SIGNAL void binderKeyTextChanged(QString binderKeyText);
    Q_SIGNAL void nativeVirtualBinderKeyChanged(quint32 binderKey);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif//ELAWIDGETTOOLS_SRC_ELAKEYBINDER_H_
