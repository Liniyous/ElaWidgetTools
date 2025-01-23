#ifndef T_ELASCREEN_H
#define T_ELASCREEN_H
#include <QWidget>

#ifdef Q_OS_WIN
#include "T_BasePage.h"

class ElaDxgiScreen;
class ElaComboBox;
class T_ElaPacketIO;
class T_RecvScreen;
class T_ElaScreen : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ElaScreen(QWidget* parent = nullptr);
    ~T_ElaScreen() override;

    void initGrabImageHandleThread();

private:
    T_ElaPacketIO* _packetSendIO{nullptr};
    T_RecvScreen* _recvScreen{nullptr};
    QThread* _packetIOSendThread{nullptr};
    T_ElaPacketIO* _packetRecvIO{nullptr};
    QThread* _packetIORecvThread{nullptr};
    QPixmap _recvPix;
    ElaDxgiScreen* _dxgiScreen{nullptr};
    ElaComboBox* _dxComboBox{nullptr};
    ElaComboBox* _outputComboBox{nullptr};

    void _initSendThread(QString interfaceIP);
    void _initRecvThread(QString interfaceIP);
    void _unInitThread(bool isSend);
};
#endif
#endif // T_ELASCREEN_H
