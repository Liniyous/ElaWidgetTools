#ifndef ELAAPPLICATION_H
#define ELAAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "ElaDef.h"
#include "ElaSingleton.h"
#define eApp ElaApplication::getInstance()
class ElaApplicationPrivate;
class ELA_EXPORT ElaApplication : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaApplication)
    Q_SINGLETON_CREATE_H(ElaApplication)
    Q_PROPERTY_CREATE_Q_H(ElaApplicationType::WindowDisplayMode, WindowDisplayMode)
    Q_PROPERTY_CREATE_Q_H(QString, ElaMicaImagePath)
private:
    explicit ElaApplication(QObject* parent = nullptr);
    ~ElaApplication() override;

public:
    void init();
    void syncWindowDisplayMode(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);
};

#endif // ELAAPPLICATION_H
