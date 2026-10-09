#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QColor>
#include <QIcon>
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaApplication;
class ElaApplicationPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaApplication)
    ELA_PROPERTY_D(ElaApplicationType::WindowDisplayMode, WindowDisplayMode)
    ELA_PROPERTY_D(QString, ElaMicaImagePath)
    ELA_PROPERTY_D(int, FontPixelSize)

public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate() override;
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);
Q_SIGNALS:
    Q_SIGNAL void initMicaBase(QImage img);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class ElaMicaBaseInitObject;
    ElaThemeType::ThemeMode _themeMode;
    QList<QWidget*> _micaWidgetList;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    void _initMicaBaseImage(QImage img);
    QRect _calculateWindowVirtualGeometry(QWidget* widget);
    void _updateMica(QWidget* widget, bool isProcessEvent = true);
    void _updateAllMicaWidget();
    void _resetAllMicaWidget();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAAPPLICATIONPRIVATE_H_
