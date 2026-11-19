#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTBOXSEARCHVIEWCONTAINER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTBOXSEARCHVIEWCONTAINER_H_

#include <QWidget>

#include "ElaWidgetToolsDef.h"
class ElaSuggestBoxSearchViewContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ElaSuggestBoxSearchViewContainer(QWidget* parent = nullptr);
    ~ElaSuggestBoxSearchViewContainer();
    void setBodyRect(const QRect& bodyRect);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QRect _bodyRect; // 面板当前显示区域 使用容器坐标
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELASUGGESTBOXSEARCHVIEWCONTAINER_H_

