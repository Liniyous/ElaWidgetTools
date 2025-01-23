#ifndef T_ABOUT_H
#define T_ABOUT_H

#include <ElaWidget.h>

class T_About : public ElaWidget
{
    Q_OBJECT
public:
    explicit T_About(QWidget* parent = nullptr);
    ~T_About();
};

#endif // T_ABOUT_H
