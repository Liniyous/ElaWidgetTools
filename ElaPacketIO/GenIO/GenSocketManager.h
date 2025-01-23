#ifndef GENSOCKETMANAGER_H
#define GENSOCKETMANAGER_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

#include "GenIP.h"

namespace GenSockets
{
class ELA_PACKETIO_EXPORT GenSocketManager
{
public:
    static void Ready();
    static std::string GetLastError();
    static void PrintLastError(const std::string& aHeader);
    static int GetIP_Count();
    static GenIP GetIP(int aIndex);
    static GenIP GetInterfaceLocalIP(const std::string& aInterfaceIP);
    ~GenSocketManager();

protected:
    GenSocketManager();

    void InsertIP(const GenIP& aIP);

    void Startup();

    void Shutdown();

    static GenSocketManager& GetInstance();

    void CreateIP_List();

    bool mIsInitialized;
    std::vector<GenIP> mIP_List;
};
} // namespace GenSockets

#endif
