#ifndef ELASCROLLAREA_H
#define ELASCROLLAREA_H

#include <QScrollArea>

class ElaScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ElaScrollArea(QWidget* parent = nullptr);
    ~ElaScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);
    void setIsGrabGesture(Qt::Orientation orientation, bool isEnable, qreal mousePressEventDelay = 0.5);
    bool getIsGrabGesture(Qt::Orientation orientation) const;

protected:
private:
};

#endif // ELASCROLLAREA_H
