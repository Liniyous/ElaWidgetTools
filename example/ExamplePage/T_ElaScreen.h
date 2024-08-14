#ifndef T_ELASCREEN_H
#define T_ELASCREEN_H
#include <QWidget>

#include "ElaScrollPage.h"
#ifdef Q_OS_WIN
class ElaDxgiScreen;
class ElaComboBox;
class T_ElaScreen : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen();

private:
    ElaDxgiScreen* _dxgiScreen{nullptr};
    ElaComboBox* _dxComboBox{nullptr};
    ElaComboBox* _outputComboBox{nullptr};
};
#endif
#endif // T_ELASCREEN_H
