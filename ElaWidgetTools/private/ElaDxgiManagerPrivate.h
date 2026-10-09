#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADXGIMANAGERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADXGIMANAGERPRIVATE_H_
#include <QObject>
#ifdef Q_OS_WIN
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
#include <QImage>
class ElaDxgi;
class ElaDxgiManager;
class ElaDxgiManagerPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaDxgiManager)
public:
    explicit ElaDxgiManagerPrivate(QObject* parent = nullptr);
    ~ElaDxgiManagerPrivate() override;

private:
    Q_SIGNAL void grabScreen();
    bool _isAllowedGrabScreen{false};
    ElaDxgi* _dxgi{nullptr};
    QThread* _dxgiThread{nullptr};
};

#endif
#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADXGIMANAGERPRIVATE_H_

