#ifndef T_ELASCREEN_H
#define T_ELASCREEN_H
#include <QWidget>

#include "ElaScrollPage.h"
#include "ElaDxgiManager.h"
#ifdef Q_OS_WIN

class ElaDxgiScreen;
class ElaComboBox;
class T_ElaScreen : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen();
    ElaDxgiManager* dxgiManager{nullptr};
    ElaDxgiScreen* _dxgiScreen{nullptr};
private:
    
    //ElaComboBox* _dxComboBox{nullptr};
    //ElaComboBox* _outputComboBox{nullptr};
};
#endif
#endif // T_ELASCREEN_H
