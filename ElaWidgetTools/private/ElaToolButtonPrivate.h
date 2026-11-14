#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_

#include <QObject>
#include <QPointer>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaToolButton;
class ElaToolButtonStyle;
class ElaControlAnimation;
class QMenu;
class QTimer;
class ElaToolButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToolButton)
public:
    explicit ElaToolButtonPrivate(QObject* parent = nullptr);
    ~ElaToolButtonPrivate();
    Q_SLOT void onMenuDelayTimeout();

private:
    ElaToolButtonStyle* _toolButtonStyle{nullptr};
    ElaControlAnimation* _controlAnimation{nullptr};
    QTimer* _menuDelayTimer{nullptr};
    QPointer<QMenu> _pendingMenu;
    bool _isMenuPress{false};
    int _menuPressKey{0};
    bool _getIsMenuButtonHit(const QPoint& position) const;
    void _queueMenu();
    void _cancelMenu();
    void _positionMenu();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBUTTONPRIVATE_H_
