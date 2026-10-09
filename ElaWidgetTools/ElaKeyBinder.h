#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAKEYBINDER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAKEYBINDER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QLabel>

class ElaKeyBinderPrivate;
class ELA_EXPORT ElaKeyBinder : public QLabel
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaKeyBinder)
    ELA_PROPERTY_Q_H(int, BorderRadius)
public:
    explicit ElaKeyBinder(QWidget* parent = nullptr);
    ~ElaKeyBinder();

    void setBinderKeyText(const QString& binderKeyText);
    Q_REQUIRED_RESULT const QString& getBinderKeyText() const;
    void setNativeVirtualBinderKey(quint32 binderKey);
    Q_REQUIRED_RESULT quint32 getNativeVirtualBinderKey() const;

Q_SIGNALS:
    Q_SIGNAL void pBinderKeyTextChanged();
    Q_SIGNAL void pNativeVirtualBinderKeyChanged();
    Q_SIGNAL void binderKeyTextChanged(QString binderKeyText);
    Q_SIGNAL void nativeVirtualBinderKeyChanged(quint32 binderKey);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELAKEYBINDER_H_
