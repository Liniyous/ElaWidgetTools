#ifndef ELACENTRALSTACKEDWIDGET_H
#define ELACENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "Def.h"
class ElaCentralStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ElaCentralStackedWidget(QWidget* parent = nullptr);
    ~ElaCentralStackedWidget();
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QLinearGradient* _windowLinearGradient{nullptr};
};

#endif // ELACENTRALSTACKEDWIDGET_H
