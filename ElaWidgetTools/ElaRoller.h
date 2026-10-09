#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAROLLER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAROLLER_H_

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include <QWidget>

class ElaRollerPrivate;
class ELA_EXPORT ElaRoller : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaRoller)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(bool, IsContainer)
    ELA_PROPERTY_Q_H(bool, IsEnableLoop)
public:
    explicit ElaRoller(QWidget* parent = nullptr);
    ~ElaRoller() override;

    void setItemList(const QStringList& itemList);
    Q_REQUIRED_RESULT const QStringList& getItemList() const;
    void setItemHeight(int itemHeight);
    Q_REQUIRED_RESULT int getItemHeight() const;
    void setMaxVisibleItems(int maxVisibleItems);
    Q_REQUIRED_RESULT int getMaxVisibleItems() const;
    void setCurrentIndex(int currentIndex);
    Q_REQUIRED_RESULT int getCurrentIndex() const;

    void setCurrentData(const QString& data);
    QString getCurrentData() const;
Q_SIGNALS:
    Q_SIGNAL void pItemListChanged();
    Q_SIGNAL void pItemHeightChanged();
    Q_SIGNAL void pMaxVisibleItemsChanged();
    Q_SIGNAL void pCurrentIndexChanged();
    Q_SIGNAL void currentDataChanged(const QString& data);

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELAROLLER_H_
