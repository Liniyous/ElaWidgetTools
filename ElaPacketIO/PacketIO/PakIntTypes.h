#ifndef PAKINTTYPES_H

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define NO_64_BIT_INT
#endif

#define PAKINTTYPES_H
#ifndef _WIN32
#include <inttypes.h>
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#ifndef NO_64_BIT_INT
#ifdef _M_IA64 // microsoft's define for 64 bit machines
typedef signed long int64_t;
typedef unsigned long uint64_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif
#endif
#endif

#undef NO_64_BIT_INT
#endif
