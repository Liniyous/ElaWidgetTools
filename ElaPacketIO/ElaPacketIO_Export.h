#ifndef ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_
#define ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_

#ifdef ELAPACKETIO_LIBRARY
#if defined(WIN32)
#if defined(_MSC_VER)
#define ELA_PACKETIO_EXPORT __declspec(dllexport)
#else
#define ELA_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#else
#define ELA_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(WIN32)
#if defined(_MSC_VER)
#define ELA_PACKETIO_EXPORT __declspec(dllimport)
#else
#define ELA_PACKETIO_EXPORT
#endif
#else
#define ELA_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#endif

#endif //ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_
