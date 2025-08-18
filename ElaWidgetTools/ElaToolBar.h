#ifndef ELATOOLBAR_H
#define ELATOOLBAR_H

#include <QToolBar>

#include "ElaDef.h"
class ElaToolBarPrivate;
class ELA_EXPORT ElaToolBar : public QToolBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaToolBar)
public:
    explicit ElaToolBar(QWidget* parent = nullptr);
    explicit ElaToolBar(const QString& title, QWidget* parent = nullptr);
    ~ElaToolBar();

    void setToolBarSpacing(int spacing);
    int getToolBarSpacing() const;

    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text);
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATOOLBAR_H
