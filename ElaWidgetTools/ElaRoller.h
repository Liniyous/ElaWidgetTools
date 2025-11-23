#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAROLLER_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAROLLER_H_

#include "ElaProperty.h"
#include <QWidget>

class ElaRollerPrivate;
class ELA_EXPORT ElaRoller : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaRoller)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QStringList, ItemList)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(bool, IsContainer)
    Q_PROPERTY_CREATE_Q_H(bool, IsEnableLoop)
public:
    explicit ElaRoller(QWidget* parent = nullptr);
    ~ElaRoller() override;

    void setCurrentData(const QString& data);
    QString getCurrentData() const;
Q_SIGNALS:
    Q_SIGNAL void currentDataChanged(const QString& data);

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELAROLLER_H_
