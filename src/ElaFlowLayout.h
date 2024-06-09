#ifndef ELAFLOWLAYOUT_H
#define ELAFLOWLAYOUT_H

#include <QLayout>
#include <QMap>
#include <QStyle>
class ElaEvent;
class ElaFlowLayout : public QLayout
{
public:
    explicit ElaFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit ElaFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~ElaFlowLayout();

    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect& rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;
    void setIsAnimation(bool isAnimation);

private:
    ElaEvent* _navigationAnimationEvent{nullptr};
    bool _isNavigationAnimation{false};
    Q_SLOT void onNavgationAnimationStateChanged(QMap<QString, QVariant> data);
    int doLayout(const QRect& rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;
    mutable QMap<ElaFlowLayout*, int> _lastHeightMap;
    mutable QMap<QLayoutItem*, QPoint> _lastGeometryMap;
    QList<QLayoutItem*> _itemList;
    bool _isAnimation{false};
    int _hSpacing;
    int _vSpacing;
};

#endif // ELAFLOWLAYOUT_H
