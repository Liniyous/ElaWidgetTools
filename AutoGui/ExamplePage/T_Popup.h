#ifndef T_POPUP_H
#define T_POPUP_H

#include <ElaScrollPage.h>

class ElaToolButton;
class T_Popup : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_Popup(QWidget* parent = nullptr);
    ~T_Popup();

private:
    ElaToolButton* _toolButton{nullptr};
};

#endif // T_POPUP_H
