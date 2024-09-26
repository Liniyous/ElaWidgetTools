#ifndef ELAMICABASEINITOBJECT_H
#define ELAMICABASEINITOBJECT_H

#include <QObject>
class ElaApplicationPrivate;
class ElaMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit ElaMicaBaseInitObject(ElaApplicationPrivate* appPrivate, QObject* parent = nullptr);
    ~ElaMicaBaseInitObject();
    Q_SLOT void onInitMicaBase(QImage img);
Q_SIGNALS:
    Q_SIGNAL void initFinished();

private:
    ElaApplicationPrivate* _appPrivate{nullptr};
};

#endif // ELAMICABASEINITOBJECT_H
