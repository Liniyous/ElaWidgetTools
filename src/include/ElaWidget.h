#ifndef ELAWIDGET_H
#define ELAWIDGET_H

#include <QWidget>

#include "Def.h"

class ElaWidgetPrivate;
class ELA_EXPORT ElaWidget : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaWidget)
public:
    explicit ElaWidget(QWidget* parent = nullptr);
    ~ElaWidget();
    void moveToCenter();
    void setIsStayTop(bool isStayTop);
    bool getIsStayTop() const;
    void setIsFixedSize(bool isFixedSize);
    bool getIsFixedSize() const;

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWIDGET_H
