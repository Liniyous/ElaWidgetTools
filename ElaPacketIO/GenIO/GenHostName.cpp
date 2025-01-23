#include "GenHostName.h"

#include "GenSocketIncludes.h"
#include "GenSocketManager.h"

namespace GenSockets
{
//! Determine if the hostname can be found
//! @return 'true' if a host with this name is found.
bool GenHostName::IsValid() const
{
   GenIP ip = *this;
   return ip.IsValidForm();
}

//! User defined operator
//! @return IP address of the host with this name
GenHostName::operator GenIP() const
{
   GenSocketManager::Ready();
   GenIP            ipReturn;
   struct addrinfo* result = nullptr;
   int              error  = getaddrinfo(mHostName.c_str(), nullptr, nullptr, &result);
   if (error == 0 && result)
   {
      struct addrinfo*    ptr = nullptr;
      struct sockaddr_in* sockaddr_ipv4;
      for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
      {
         if (ptr->ai_family == AF_INET) // IPv4
         {
            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            ipReturn      = GenIP(sockaddr_ipv4->sin_addr);
            break; // Take first IPv4 address found
         }
      }
   }
   return ipReturn;
}

//! Returns a list of IP's this host name refers to
//! @param[out] aIP_List A list of GenIP's that refer to this host name
void GenHostName::GetIP_List(std::vector<GenIP>& aIP_List)
{
   GenSocketManager::Ready();

   struct addrinfo* result = nullptr;
   int              error  = getaddrinfo(mHostName.c_str(), nullptr, nullptr, &result);
   if (error == 0 && result)
   {
      struct addrinfo*    ptr = nullptr;
      struct sockaddr_in* sockaddr_ipv4;
      for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
      {
         if (ptr->ai_family == AF_INET) // IPv4
         {
            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            GenIP tmpIP(sockaddr_ipv4->sin_addr);
            aIP_List.push_back(tmpIP);
         }
      }
      freeaddrinfo(result);
   }
}

// static
//! Returns the local host name of this machine.
GenHostName GenHostName::LocalHostName()
{
   GenSocketManager::Ready();
   static char thisHostName[80] = "";
   if (!thisHostName[0])
   {
      gethostname(thisHostName, 80);
   }
   return GenHostName(thisHostName);
}
} // namespace GenSockets
