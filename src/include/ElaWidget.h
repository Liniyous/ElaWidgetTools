#ifndef ELAWIDGET_H
#define ELAWIDGET_H

#include <QWidget>

#include "stdafx.h"

class ElaWidgetPrivate;
class ELA_EXPORT ElaWidget : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaWidget)
public:
    explicit ElaWidget(QWidget* parent = nullptr);
    ~ElaWidget();

    void setIsStayTop(bool isStayTop);
    bool getIsStayTop() const;
    void setIsFixedSize(bool isFixedSize);
    bool getIsFixedSize() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWIDGET_H
