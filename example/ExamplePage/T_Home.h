#ifndef T_HOME_H
#define T_HOME_H

#include "ElaScrollPage.h"
class ElaMenu;
class T_Home : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_Home(QWidget* parent = nullptr);
    ~T_Home();
Q_SIGNALS:
    Q_SIGNAL void elaScreenNavigation();
    Q_SIGNAL void elaBaseComponentNavigation();
    Q_SIGNAL void elaSceneNavigation();
    Q_SIGNAL void elaCardNavigation();
    Q_SIGNAL void elaIconNavigation();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _homeMenu{nullptr};
};

#endif // T_HOME_H
