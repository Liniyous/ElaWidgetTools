#ifndef ELAMICABASEINITOBJECT_H
#define ELAMICABASEINITOBJECT_H

#include <QObject>
class ElaWindowPrivate;
class ElaMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit ElaMicaBaseInitObject(ElaWindowPrivate* windowPrivate, QObject* parent = nullptr);
    ~ElaMicaBaseInitObject();
    Q_SLOT void onInitMicaBase(QImage img);
Q_SIGNALS:
    Q_SIGNAL void initFinished();

private:
    ElaWindowPrivate* _windowPrivate{nullptr};
};

#endif // ELAMICABASEINITOBJECT_H
