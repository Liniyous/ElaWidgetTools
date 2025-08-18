#ifndef ELAFRAMEWORK_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELAPACKETIO_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELAPACKETIO_H_

#include <QObject>
#if defined(Q_OS_WIN) && defined(BUILD_WITH_ELAPACKETIO)
#include "ElaProperty.h"
#include "ElaXIO_PacketRegistry.h"
#include "UtCallbackHolder.h"
#include <QImage>
#include <QMutex>
#include <QQueue>
class ElaXIO_Interface;
class PakPacket;
class ElaXIO_Connection;
class T_ElaPacketIO : public QObject
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsActive);
    Q_PRIVATE_CREATE(QString, InterfaceIP);

public:
    Q_INVOKABLE explicit T_ElaPacketIO(QObject* parent = nullptr);
    ~T_ElaPacketIO() override;

    Q_SLOT void handleGrabImage();
    Q_SLOT void handleImagePacket();

    Q_SIGNAL void sendHandleResult(QPixmap map);

private:
    QMutex _mutex;
    int _lastImageIndex{0};
    QByteArray _imageArray;
    std::unique_ptr<ElaXIO_Interface> _interface;
    UtCallbackHolder _callback;
    void _handleScreenPkt(ElaXIO_ScreenPkt& screenPkt);
    ElaXIO_Connection* _connection{nullptr};
    ElaXIO_Connection* _multicastConnection{nullptr};
    void _sendToXIO(ElaXIO_Packet& packet, bool isMulticast = false);
};
#endif
#endif //ELAFRAMEWORK_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_ELAPACKETIO_H_
