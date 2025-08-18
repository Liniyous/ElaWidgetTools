#ifndef ELASUGGESTBOXSEARCHVIEWCONTAINER_H
#define ELASUGGESTBOXSEARCHVIEWCONTAINER_H

#include <QWidget>

#include "ElaDef.h"
class ElaSuggestBoxSearchViewContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ElaSuggestBoxSearchViewContainer(QWidget* parent = nullptr);
    ~ElaSuggestBoxSearchViewContainer();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASUGGESTBOXSEARCHVIEWCONTAINER_H
