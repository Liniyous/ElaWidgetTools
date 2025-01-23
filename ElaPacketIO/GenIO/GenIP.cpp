#include "GenIP.h"

#include <array>
#include <cstdio>
#include <sstream>

#include "GenHostName.h"
#include "GenSocketIncludes.h"
#include "GenSocketManager.h"

namespace GenSockets
{
//! Constructor
//! Initializes to an IP represented as a string
//! @param aIP A string representing an IP Address
//!            Example: "127.0.0.1"
GenIP::GenIP(const std::string& aIP)
{
   if (!Parse(aIP))
   {
      mIPAddrDWord = 0;
   }
}

//! Constructor
//! Initializes to the IP 'a.b.c.d'
//! @param aA
//! @param aB
//! @param aC
//! @param aD
//!        The IP address in parts 'aA.aB.aC.aD'
GenIP::GenIP(int aA, int aB, int aC, int aD)
{
   mIPAddr[0] = aA;
   mIPAddr[1] = aB;
   mIPAddr[2] = aC;
   mIPAddr[3] = aD;
}

GenIP::GenIP(const in_addr& aSockAddr)
{
   mIPAddrDWord = aSockAddr.s_addr;
}

//! Parses an string for an IP address
//! @param aIP has the form "127.0.0.1"
bool GenIP::Parse(const std::string& aIP)
{
   std::string   ip = aIP;
   unsigned char a, b, c, d;

   auto count = sscanf(ip.c_str(), "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d);
   if (count == 4)
   {
      mIPAddr[0] = a;
      mIPAddr[1] = b;
      mIPAddr[2] = c;
      mIPAddr[3] = d;

      return true;
   }
   else
   {
      return false;
   }
}

//! User defined conversion
//! @return A string representing this IP Address
GenIP::operator std::string() const
{
   std::stringstream result;
   // Note:  need to cast here because it needs to print out integers, instead of characters.
   result << static_cast<int>(mIPAddr[0]) << '.' << static_cast<int>(mIPAddr[1]) << '.' << static_cast<int>(mIPAddr[2])
          << '.' << static_cast<int>(mIPAddr[3]);
   return result.str();
}

//! User defined conversion
//! @return The name of the host at this IP address
GenIP::operator GenHostName() const
{
   GenSocketManager::Ready();

   sockaddr_in tmpAddr;
   tmpAddr.sin_family      = AF_INET;
   tmpAddr.sin_addr.s_addr = mIPAddrDWord;
   tmpAddr.sin_port        = htons(3333);
   std::array<char, 255> hostName;
   // Attempt to look up the hostname, if the hostname is not found, attempt to
   // look up the IP address instead.
   if (getnameinfo(reinterpret_cast<sockaddr*>(&tmpAddr), sizeof(sockaddr_in), hostName.data(), 255, nullptr, 0, 0) == 0 ||
       getnameinfo(reinterpret_cast<sockaddr*>(&tmpAddr), sizeof(sockaddr_in), hostName.data(), 255, nullptr, 0, NI_NUMERICHOST) ==
          0)
   {
      return GenHostName(hostName.data());
   }
   return GenHostName("");
}
} // namespace GenSockets
