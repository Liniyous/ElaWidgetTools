#ifndef ELADXGIMANAGER_H
#define ELADXGIMANAGER_H

#include <QWidget>
#ifdef Q_OS_WIN
#include "ElaProperty.h"
#include "ElaSingleton.h"

class ElaDxgiManagerPrivate;
class ELA_EXPORT ElaDxgiManager : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaDxgiManager)
    Q_SINGLETON_CREATE_H(ElaDxgiManager);

private:
    explicit ElaDxgiManager(QObject* parent = nullptr);
    ~ElaDxgiManager() override;

public:
    QStringList getDxDeviceList() const;
    QStringList getOutputDeviceList() const;
    QImage grabScreenToImage() const;
    void startGrabScreen();
    void stopGrabScreen();
    bool getIsGrabScreen() const;
    bool setDxDeviceID(int dxID);
    int getDxDeviceID() const;
    bool setOutputDeviceID(int deviceID);
    int getOutputDeviceID() const;
    void setGrabArea(int width, int height); //从屏幕中心向外延伸
    void setGrabArea(int x, int y, int width, int height);
    QRect getGrabArea() const;
    void setGrabFrameRate(int frameRateValue);
    int getGrabFrameRate() const;
    void setTimeoutMsValue(int timeoutValue);
    int getTimeoutMsValue() const;
Q_SIGNALS:
    Q_SIGNAL void grabImageUpdate(QImage img);
};

class ElaDxgiScreenPrivate;
class ELA_EXPORT ElaDxgiScreen : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDxgiScreen)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaDxgiScreen(QWidget* parent = nullptr);
    ~ElaDxgiScreen();
    void setIsSyncGrabSize(bool isSyncGrabSize);
    bool getIsSyncGrabSize() const;

protected:
    void paintEvent(QPaintEvent* event) override;
};
#endif
#endif // ELADXGIMANAGER_H
