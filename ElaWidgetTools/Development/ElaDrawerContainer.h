#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_

#include "ElaPropertyMacro.h"
#include <QWidget>

#include "ElaWidgetToolsDef.h"
#include <QVBoxLayout>

#include <QGraphicsOpacityEffect>
class ElaDrawerContainer : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(int, BorderRadius)
    ELA_VARIABLE(QPixmap, ContainerPix)
    ELA_PROPERTY(qreal, Opacity)
public:
    explicit ElaDrawerContainer(QWidget* parent = nullptr);
    ~ElaDrawerContainer() override;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);

    void doDrawerAnimation(bool isExpand);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    bool _isShowBorder{true};
    ElaThemeType::ThemeMode _themeMode;
    QVBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _containerLayout{nullptr};
    QWidget* _containerWidget{nullptr};
    QList<QWidget*> _drawerWidgetList;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_

