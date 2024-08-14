#ifndef ELADOCKWIDGETTITLEBAR_H
#define ELADOCKWIDGETTITLEBAR_H

#include <QDockWidget>
#include <QLabel>
#include <QWidget>

#include "Def.h"
class ElaText;
class ElaIconButton;
class ElaDockWidget;
class ElaDockWidgetTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit ElaDockWidgetTitleBar(QWidget* parent = nullptr);
    ~ElaDockWidgetTitleBar();

    Q_SLOT void onFloatButtonClicked();
    Q_SLOT void onCloseButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaDockWidget* _dockWidget{nullptr};
    QLabel* _iconLabel{nullptr};
    ElaText* _titleLabel{nullptr};
    ElaIconButton* _floatButton{nullptr};
    ElaIconButton* _closeButton{nullptr};
    void _setVisibleFromFeatures(QDockWidget::DockWidgetFeatures features);
};

#endif // ELADOCKWIDGETTITLEBAR_H
