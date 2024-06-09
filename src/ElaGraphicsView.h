#ifndef ELAGRAPHICSVIEW_H
#define ELAGRAPHICSVIEW_H

#include <QGraphicsView>

class ElaGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ElaGraphicsView(QWidget* parent = nullptr);
    explicit ElaGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~ElaGraphicsView();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void _initStyle();
};

#endif // ELAGRAPHICSVIEW_H
