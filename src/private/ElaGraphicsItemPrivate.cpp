#include "ElaGraphicsItemPrivate.h"

ElaGraphicsItemPrivate::ElaGraphicsItemPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaGraphicsItemPrivate::~ElaGraphicsItemPrivate()
{
}

QDataStream& operator<<(QDataStream& stream, const ElaGraphicsItemPrivate* data)
{
    stream << data->_itemUID;
    stream << data->_pItemName;
    stream << data->_pMaxLinkPortCount;
    stream << data->_currentLinkPortState;
    stream << data->_pDataRoutes;
    stream << data->_pItemImage;
    stream << data->_pItemSelectedImage;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ElaGraphicsItemPrivate* data)
{
    stream >> data->_itemUID;
    stream >> data->_pItemName;
    stream >> data->_pMaxLinkPortCount;
    stream >> data->_currentLinkPortState;
    stream >> data->_pDataRoutes;
    stream >> data->_pItemImage;
    stream >> data->_pItemSelectedImage;
    return stream;
}
