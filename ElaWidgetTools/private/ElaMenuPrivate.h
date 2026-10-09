#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaMenu;
class ElaMenuStyle;
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaMenu)
    ELA_PROPERTY(int, AnimationImagePosY)
public:
    explicit ElaMenuPrivate(QObject* parent = nullptr);
    ~ElaMenuPrivate() override;

private:
    QPixmap _animationPix;
    bool _isCloseAnimation{false};
    QPoint _mousePressPoint;
    ElaMenuStyle* _menuStyle{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_

