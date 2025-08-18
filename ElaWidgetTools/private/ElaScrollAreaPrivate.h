#ifndef ELASCROLLAREAPRIVATE_H
#define ELASCROLLAREAPRIVATE_H

#include <QObject>
#include <QScrollBar>

#include "ElaProperty.h"
class ElaScrollBar;
class ElaScrollArea;
class ElaScrollAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollArea)
public:
    explicit ElaScrollAreaPrivate(QObject* parent = nullptr);
    ~ElaScrollAreaPrivate();
};

#endif // ELASCROLLAREAPRIVATE_H
