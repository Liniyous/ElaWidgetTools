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

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setIsHasRadius(bool isHasRadius);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isTransparent{false};
    bool _isHasRadius{true};
};

#endif // ELACENTRALSTACKEDWIDGET_H
