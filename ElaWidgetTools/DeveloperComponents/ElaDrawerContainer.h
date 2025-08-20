#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_

#include <QWidget>

#include "ElaDef.h"
#include <QVBoxLayout>

#include <QGraphicsOpacityEffect>
class ElaDrawerContainer : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, BorderRadius)
    Q_PRIVATE_CREATE(QPixmap, ContainerPix)
    Q_PROPERTY_CREATE(qreal, Opacity)
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
    int _calculateContainerMinimumHeight() const;
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
