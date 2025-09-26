#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELADRAWERAREA_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELADRAWERAREA_H_

#include "ElaProperty.h"
#include <QWidget>

class ElaDrawerAreaPrivate;
class ELA_EXPORT ElaDrawerArea : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDrawerArea)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, HeaderHeight)
public:
    explicit ElaDrawerArea(QWidget* parent = nullptr);
    ~ElaDrawerArea() override;

    void setDrawerHeader(QWidget* widget);

    void addDrawer(QWidget* widget);
    void removeDrawer(QWidget* widget);

    void expand();
    void collapse();

    bool getIsExpand() const;
Q_SIGNALS:
    Q_SIGNAL void expandStateChanged(bool isExpand);
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELADRAWERAREA_H_
