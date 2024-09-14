#ifndef ELACUSTOMWIDGET_H
#define ELACUSTOMWIDGET_H

#include <QWidget>

#include "Def.h"
class ElaAppBar;
class QVBoxLayout;
class ElaCustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ElaCustomWidget(QWidget* parent = nullptr);
    ~ElaCustomWidget();

    void setCentralWidget(QWidget* widget);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    QVBoxLayout* _mainLayout{nullptr};
    ElaAppBar* _appBar{nullptr};
    QWidget* _centralWidget{nullptr};

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELACUSTOMWIDGET_H
