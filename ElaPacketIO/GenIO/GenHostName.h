#ifndef GENHOSTNAME_H
#define GENHOSTNAME_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

#include "GenIP.h"

namespace GenSockets
{
/** Represents a host name.
 */
class ELA_PACKETIO_EXPORT GenHostName
{
public:
    GenHostName(std::string aHostName) { mHostName = aHostName; }
    bool IsValid() const;
    //! User defined operator
    //! @return The hostname as a string.
    operator std::string() const { return mHostName; }
    operator GenIP() const;
    void GetIP_List(std::vector<GenIP>& aIP_List);
    static GenHostName LocalHostName();

protected:
    std::string mHostName;
};
} // namespace GenSockets
#endif
