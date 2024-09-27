#ifndef ELAAPPLICATIONPRIVATE_H
#define ELAAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "Def.h"
class ElaApplication;
class ElaApplicationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaApplication)
    Q_PROPERTY_CREATE_D(bool, IsEnableMica)
    Q_PROPERTY_CREATE_D(QString, MicaImagePath)
public:
    explicit ElaApplicationPrivate(QObject* parent = nullptr);
    ~ElaApplicationPrivate();
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
};

#endif // ELAAPPLICATIONPRIVATE_H
