#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_

#include <QWidget>

#include "Def.h"
#include <QVBoxLayout>

#include <QGraphicsOpacityEffect>
class ElaDrawerContainer : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, BorderRadius)
public:
    explicit ElaDrawerContainer(QWidget* parent = nullptr);
    ~ElaDrawerContainer() override;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);

    void doDrawerAnimation(bool isExpand);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QGraphicsOpacityEffect* _opacityEffect{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    QVBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _containerLayout{nullptr};
    QWidget* _containerWidget{nullptr};
    QList<QWidget*> _drawerWidgetList;
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
