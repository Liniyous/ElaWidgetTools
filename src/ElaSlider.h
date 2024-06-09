#ifndef ELASLIDER_H
#define ELASLIDER_H

#include <QSlider>

class ElaSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ElaSlider(QWidget* parent = nullptr);
    explicit ElaSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~ElaSlider();
};

#endif // ELASLIDER_H
