#ifndef ELAAPPLICATION_H
#define ELAAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "singleton.h"
#include "stdafx.h"
#define eApp ElaApplication::getInstance()
class ElaApplicationPrivate;
class ELA_EXPORT ElaApplication : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaApplication)
    Q_SINGLETON_CREATE_H(ElaApplication)
    Q_PROPERTY_CREATE_Q_H(bool, IsEnableMica)
    Q_PROPERTY_CREATE_Q_H(QString, MicaImagePath)
private:
    explicit ElaApplication(QObject* parent = nullptr);
    ~ElaApplication();

public:
    void init();
    void syncMica(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);
};

#endif // ELAAPPLICATION_H
