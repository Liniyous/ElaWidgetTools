#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMICABASEINITOBJECT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMICABASEINITOBJECT_H_

#include <QImage>
#include <QObject>
class ElaMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit ElaMicaBaseInitObject(QObject* parent = nullptr);
    ~ElaMicaBaseInitObject() override;
    Q_SLOT void onInitMicaBase(QImage image, quint64 revision);
Q_SIGNALS:
    Q_SIGNAL void initFinished(QImage lightImage, QImage darkImage, quint64 revision);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELAMICABASEINITOBJECT_H_

