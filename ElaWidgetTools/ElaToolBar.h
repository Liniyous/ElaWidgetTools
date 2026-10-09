#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBAR_H_

#include "ElaPropertyMacro.h"
#include <QToolBar>

#include "ElaWidgetToolsDef.h"
class ElaToolBarPrivate;
class ELA_EXPORT ElaToolBar : public QToolBar
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaToolBar)
public:
    explicit ElaToolBar(QWidget* parent = nullptr);
    explicit ElaToolBar(const QString& title, QWidget* parent = nullptr);
    ~ElaToolBar() override;

    void setToolBarSpacing(int spacing);
    int getToolBarSpacing() const;

    void setToolButtonSize(const QSize& size);
    const QSize& getToolButtonSize() const;

    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text);
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBAR_H_
