#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEW_H
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEW_H

#include "ElaPropertyMacro.h"

#include <QTreeView>

class ElaActionCommanderViewStyle;
class ElaActionCommanderView : public QTreeView
{
    Q_OBJECT
    ELA_VARIABLE(QString, DomainName)
public:
    explicit ElaActionCommanderView(QWidget* parent = nullptr);
    ~ElaActionCommanderView() override;

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);

Q_SIGNALS:
    Q_SIGNAL void runToCommand(const QString& domainName, int commandIndex);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ElaActionCommanderViewStyle* _style{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAACTIONCOMMANDERVIEW_H
