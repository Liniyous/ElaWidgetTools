#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAAPPLICATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAAPPLICATION_H_

#include "ElaPropertyMacro.h"
#include <QIcon>
#include <QObject>

#include "ElaSingletonMacro.h"
#include "ElaWidgetToolsDef.h"
#define eApp ElaApplication::getInstance()
class ElaApplicationPrivate;
class ELA_EXPORT ElaApplication : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaApplication)
    ELA_SINGLETON_H(ElaApplication)
private:
    explicit ElaApplication(QObject* parent = nullptr);
    ~ElaApplication() override;

public:
    void setWindowDisplayMode(ElaApplicationType::WindowDisplayMode windowDisplayType);
    Q_REQUIRED_RESULT ElaApplicationType::WindowDisplayMode getWindowDisplayMode() const;
    void setElaMicaImagePath(const QString& micaImagePath);
    Q_REQUIRED_RESULT const QString& getElaMicaImagePath() const;
    void setFontPixelSize(int fontPixelSize);
    Q_REQUIRED_RESULT int getFontPixelSize() const;

    void init();
    void syncWindowDisplayMode(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);

Q_SIGNALS:
    Q_SIGNAL void pWindowDisplayModeChanged();
    Q_SIGNAL void pElaMicaImagePathChanged();
    Q_SIGNAL void pFontPixelSizeChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAAPPLICATION_H_
