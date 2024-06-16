#ifndef ELADXGI_H
#define ELADXGI_H

#include <QObject>

#include "stdafx.h"
class ElaDxgiPrivate;
class ElaDxgi : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaDxgi)
    Q_PROPERTY_CREATE_Q_H(QStringList, DxDeviceList)
    Q_PROPERTY_CREATE_Q_H(QStringList, OutputDeviceList)
    Q_PROPERTY_CREATE_Q_H(int, DxDeviceID);
    Q_PROPERTY_CREATE_Q_H(int, OutputDeviceID);
    Q_PROPERTY_CREATE_Q_H(QString, LastError)
    Q_PROPERTY_CREATE_Q_H(bool, IsGrabActive)
    Q_PROPERTY_CREATE_Q_H(QRect, GrabArea);
    Q_PROPERTY_CREATE_Q_H(int, GrabFrameRate);  // 截图帧数
    Q_PROPERTY_CREATE_Q_H(int, TimeoutMsValue); // 超时等待

public:
    explicit ElaDxgi(QObject* parent = nullptr);
    ~ElaDxgi();
    bool initialize(int dxID, int outputID);
    QImage getGrabImage() const;
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver();

private:
    void releaseInterface();
    void cpuSleep(int usec);
};

#endif // ELADXGI_H
