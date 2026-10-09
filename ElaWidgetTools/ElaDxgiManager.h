#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELADXGIMANAGER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELADXGIMANAGER_H_

#include <QWidget>
#ifdef Q_OS_WIN
#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
#include "ElaWidgetToolsExport.h"

class ElaDxgiManagerPrivate;
class ELA_EXPORT ElaDxgiManager : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaDxgiManager)
    ELA_SINGLETON_H(ElaDxgiManager);

private:
    explicit ElaDxgiManager(QObject* parent = nullptr);
    ~ElaDxgiManager() override;

public:
    const QStringList& getDxDeviceList() const;
    const QStringList& getOutputDeviceList() const;
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
    Q_SIGNAL void grabImageUpdate(const QImage& img);
};

#endif
#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELADXGIMANAGER_H_

