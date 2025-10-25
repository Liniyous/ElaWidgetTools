#ifndef ELAFLOWLAYOUT_H
#define ELAFLOWLAYOUT_H

#include <QLayout>
#include <QMap>
#include <QStyle>

#include "ElaProperty.h"
class ElaFlowLayoutPrivate;
class ELA_EXPORT ElaFlowLayout : public QLayout
{
    Q_OBJECT
    Q_Q_CREATE(ElaFlowLayout)
public:
    explicit ElaFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit ElaFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~ElaFlowLayout() override;

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
};

#endif // ELAFLOWLAYOUT_H
