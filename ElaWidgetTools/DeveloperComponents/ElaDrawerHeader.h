#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_

#include "ElaDef.h"
#include <QVBoxLayout>
#include <QWidget>
class ElaDrawerHeader : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, BorderRadius)
    Q_PRIVATE_CREATE(bool, IsExpand)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
public:
    explicit ElaDrawerHeader(QWidget* parent = nullptr);
    ~ElaDrawerHeader() override;

    void setHeaderWidget(QWidget* widget);
    void doExpandOrCollpaseAnimation();
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

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_
