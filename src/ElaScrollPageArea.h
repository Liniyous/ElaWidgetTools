#ifndef ELASCROLLPAGEAREA_H
#define ELASCROLLPAGEAREA_H

#include <QWidget>

#include "Def.h"
#include "stdafx.h"
class ElaScrollPageArea : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, BorderRadius)
public:
    explicit ElaScrollPageArea(QWidget* parent = nullptr);
    ~ElaScrollPageArea();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    ElaApplicationType::ThemeMode _themeMode{ElaApplicationType::Light};
};

#endif // ELASCROLLPAGEAREA_H
