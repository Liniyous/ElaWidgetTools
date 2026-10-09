#ifndef T_ELASCREEN_H
#define T_ELASCREEN_H
#include <QWidget>

#ifdef Q_OS_WIN
#include "T_BasePage.h"

class ElaComboBox;
class ElaImageCard;
class T_ElaScreen : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen() override;

private:
    ElaImageCard* _dxgiScreen{nullptr};
    ElaComboBox* _dxComboBox{nullptr};
    ElaComboBox* _outputComboBox{nullptr};
};
#endif
#endif // T_ELASCREEN_H
