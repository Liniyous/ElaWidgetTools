#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAFLOWLAYOUT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAFLOWLAYOUT_H_

#include <QLayout>
#include <QMap>
#include <QStyle>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaFlowLayoutPrivate;
class ELA_EXPORT ElaFlowLayout : public QLayout
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaFlowLayout)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAFLOWLAYOUT_H_
