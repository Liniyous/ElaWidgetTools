#ifndef ELADXGIMANAGERPRIVATE_H
#define ELADXGIMANAGERPRIVATE_H
#include <QObject>
#ifdef Q_OS_WIN
#include "ElaProperty.h"
#include <QImage>
class ElaDxgi;
class ElaDxgiManager;
class ElaDxgiManagerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDxgiManager)
public:
    explicit ElaDxgiManagerPrivate(QObject* parent = nullptr);
    ~ElaDxgiManagerPrivate();

private:
    Q_SIGNAL void grabScreen();
    bool _isAllowedGrabScreen{false};
    ElaDxgi* _dxgi{nullptr};
    QThread* _dxgiThread{nullptr};
};

class ElaDxgiScreen;
class ElaDxgiScreenPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDxgiScreen)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaDxgiScreenPrivate(QObject* parent = nullptr);
    ~ElaDxgiScreenPrivate();

private:
    ElaDxgiManager* _dxgiManager{nullptr};
    bool _isSyncGrabSize{false};
    QImage _img;
};
#endif
#endif // ELADXGIMANAGERPRIVATE_H
