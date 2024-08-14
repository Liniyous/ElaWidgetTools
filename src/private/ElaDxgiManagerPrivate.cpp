#include "ElaDxgiManagerPrivate.h"
#ifdef Q_OS_WIN
ElaDxgiManagerPrivate::ElaDxgiManagerPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDxgiManagerPrivate::~ElaDxgiManagerPrivate()
{
}

ElaDxgiScreenPrivate::ElaDxgiScreenPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDxgiScreenPrivate::~ElaDxgiScreenPrivate()
{
}
#endif
