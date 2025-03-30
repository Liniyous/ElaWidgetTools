#ifndef T_ELASCREEN_H
#define T_ELASCREEN_H
#include <QWidget>

#ifdef Q_OS_WIN
#include "T_BasePage.h"

class ElaDxgiScreen;
class ElaComboBox;
#if defined(Q_OS_WIN) && defined(BUILD_WITH_ELAPACKETIO)
class T_ElaPacketIO;
#endif
class T_RecvScreen;
class T_ElaScreen : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen() override;

private:
    ElaDxgiScreen* _dxgiScreen{nullptr};
    ElaComboBox* _dxComboBox{nullptr};
    ElaComboBox* _outputComboBox{nullptr};
#if defined(Q_OS_WIN) && defined(BUILD_WITH_ELAPACKETIO)
    T_ElaPacketIO* _packetSendIO{nullptr};
    T_ElaPacketIO* _packetRecvIO{nullptr};
    T_RecvScreen* _recvScreen{nullptr};
    QThread* _packetIOSendThread{nullptr};
    QThread* _packetIORecvThread{nullptr};
    void _initSendThread(QString interfaceIP);
    void _initRecvThread(QString interfaceIP);
    void _unInitThread(bool isSend);
#endif
};
#endif
#endif // T_ELASCREEN_H
