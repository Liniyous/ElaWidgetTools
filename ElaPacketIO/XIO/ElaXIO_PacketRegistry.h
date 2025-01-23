#ifndef ELAXIO_PACKETREGISTRY_H
#define ELAXIO_PACKETREGISTRY_H

#include "ElaPacketIO_Export.h"

#include <climits>
#include <vector>

#include "GenUniqueId.h"
class PakProcessor;
#include "ElaXIO_Packet.h"
#include "PakSerializeFwd.h"

class ELA_PACKETIO_EXPORT ElaXIO_PacketRegistry
{
public:
    static int getPacketVersion() { return sPacketVersion; }
    static void registerPackets(PakProcessor& aProcessor);
    static void registerClasses();

private:
    static int sPacketVersion;
};

// 必要包 用于发送心跳数据
class ELA_PACKETIO_EXPORT ElaXIO_HeartbeatPkt : public ElaXIO_Packet
{
public:
    XIO_DEFINE_PACKET(ElaXIO_HeartbeatPkt, ElaXIO_Packet, 1)
    {
        using namespace PakSerialization;
        serializeBuf & _applicationName & _tcpPort;
    }
    std::string _applicationName;
    uint16_t _tcpPort;
};

// 必要包 用于进行三次握手
class ELA_PACKETIO_EXPORT ElaXIO_InitializePkt : public ElaXIO_Packet
{
public:
    XIO_DEFINE_PACKET(ElaXIO_InitializePkt, ElaXIO_Packet, 2)
    {
        using namespace PakSerialization;
        serializeBuf & _stage & _applicationName;
    }
    int32_t _stage;
    std::string _applicationName;
};

class ELA_PACKETIO_EXPORT ElaXIO_ExamplePkt : public ElaXIO_Packet
{
public:
    // 包定义及序列化
    XIO_DEFINE_PACKET(ElaXIO_ExamplePkt, ElaXIO_Packet, 3)
    {
        using namespace PakSerialization;
        serializeBuf & _int & _double & _string & _struct& Array(_charBuf, 1000);
    }
    // 自定义结构体
    struct ExampleStruct {
        std::vector<int> _vector;
        XIO_DEFINE_STRUCT
        {
            using namespace PakSerialization;
            serializeBuf & _vector;
        }
    };
    // 包处理完成后自动调用 用于清除可能的堆空间
    XIO_DEFINE_CLEANUP
    {
        if (_struct)
        {
            delete _struct;
            _struct = nullptr;
        }
    }
    int32_t _int;
    double _double;
    std::string _string;
    ExampleStruct* _struct;
    char _charBuf[1000];
};

// 用于实现视频流传输
class ELA_PACKETIO_EXPORT ElaXIO_ScreenPkt : public ElaXIO_Packet
{
public:
    // 包定义及序列化
    XIO_DEFINE_PACKET(ElaXIO_ScreenPkt, ElaXIO_Packet, 4)
    {
        using namespace PakSerialization;
        serializeBuf & _imageWidth & _imageHeight & _dataOffset & _dataTotalLen
            & _currentDataLen & _currentDataLen & _dataID& Array(_data, 1024);
    }
    int _imageWidth;     // 图像宽度
    int _imageHeight;    // 图像高度
    int _dataOffset;     // 数据偏移量 从0开始 第一包为0 第二包为1024 ...最后一包时 _dataOffset + _currentDataLen = _dataTotalLen
    int _dataTotalLen;   // 数据总大小
    int _currentDataLen; // 当前数据包大小
    int _dataID;         // 数据ID 每包从0开始 依次递增 _dataOffset / 1024 = _dataID
    char _data[1024];    // 数据包
};
#endif
