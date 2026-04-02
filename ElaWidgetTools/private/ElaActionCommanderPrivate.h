#ifndef ELAACTIONCOMMANDERPRIVATE_H
#define ELAACTIONCOMMANDERPRIVATE_H
#include <QMap>
#include <QObject>
#include <QVector>

#include "ElaProperty.h"
class ElaActionCommand;
class ElaActionCommander;
class ElaActionCommanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaActionCommander)
    Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
    struct ElaActionCommandData {
        int currentIndex{-1};
        QVector<ElaActionCommand*> commandList;
    };
    explicit ElaActionCommanderPrivate(QObject* parent = nullptr);
    ~ElaActionCommanderPrivate() override;

private:
    QMap<QString, ElaActionCommandData> _commandDataMap;
};

#endif // ELAACTIONCOMMANDERPRIVATE_H
