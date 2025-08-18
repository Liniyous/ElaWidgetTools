#ifndef ELAMENUBAR_H
#define ELAMENUBAR_H

#include <QMenuBar>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMenu;
class ELA_EXPORT ElaMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit ElaMenuBar(QWidget* parent = nullptr);
    ~ElaMenuBar();

    QAction* addMenu(QMenu* menu);
    ElaMenu* addMenu(const QString& title);
    ElaMenu* addMenu(const QIcon& icon, const QString& title);
    ElaMenu* addMenu(ElaIconType::IconName, const QString& title);

    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text);
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut);
};

#endif // ELAMENUBAR_H
