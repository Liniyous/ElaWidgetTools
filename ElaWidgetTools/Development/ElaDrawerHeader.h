#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QVBoxLayout>
#include <QWidget>
class ElaDrawerHeader : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(int, BorderRadius)
    ELA_VARIABLE(bool, IsExpand)
    ELA_PROPERTY(qreal, ExpandIconRotate)
public:
    explicit ElaDrawerHeader(QWidget* parent = nullptr);
    ~ElaDrawerHeader() override;

    void setHeaderWidget(QWidget* widget);
    void doExpandOrCollapseAnimation();
Q_SIGNALS:
    Q_SIGNAL void drawerHeaderClicked(bool isExpand);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isPressed{false};
    QWidget* _headerWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_

