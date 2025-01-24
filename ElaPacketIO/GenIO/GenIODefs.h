// Define the following GENIO flags based on processor we are running on

// GENIO_LIT_ENDIAN
// GENIO_BIG_ENDIAN
// GENIO_VAX_D_FLOAT
// GENIO_VAX_G_FLOAT
// GENIO_LONG64
// GENIO_UINT64

#if defined(__GNUC__) && defined(__i386)
// This is for a 32 bit Intel/AMD processor with GCC
#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__GNUC__) && (defined(__ia64) || defined(__amd64) || defined(__x86_64)) && (defined(_LP64) || defined(__LP64) || defined(__LP64__))
// This is for a 64 bit Intel/AMD processor with GCC
#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if !defined(GENIO_LONG64)
#define GENIO_LONG64
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__GNUC__) && (defined(__PPC64__) || defined(__ppc64__))
// This is for 64 bit IBM Power Arch with GCC
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif
#else
#if !defined(GENIO_BIG_ENDIAN)
#define GENIO_BIG_ENDIAN
#endif
#endif

#if !defined(GENIO_LONG64)
#define GENIO_LONG64
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(_WIN32)
#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif
#else
#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned __int64
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 __int64
#endif
#endif
#elif defined(__alpha) && defined(__unix__)
#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if !defined(GENIO_LONG64)
#define GENIO_LONG64
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__vms)
#if defined(__alpha)
// printf("DEC Alpha/VMS");
#else
// printf("DEC VAX/VMS");
#endif

#if !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned __int64
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 __int64
#endif

#if __D_FLOAT != 0
#if !defined(GENIO_VAX_D_FLOAT)
#define GENIO_VAX_D_FLOAT
#endif
#endif

#if __G_FLOAT != 0
#if !defined(GENIO_VAX_G_FLOAT)
#define GENIO_VAX_G_FLOAT
#endif
#endif

#if __IEEE_FLOAT != 0
// Great this is the default
#endif

#elif defined(__hpux)
#if !defined(GENIO_BIG_ENDIAN)
#define GENIO_BIG_ENDIAN
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__host_mips)
#if !defined(GENIO_BIG_ENDIAN)
#define GENIO_BIG_ENDIAN
#endif

#if (_SIZE_LONG == 64)
#if !defined(GENIO_LONG64)
#define GENIO_LONG64
#endif
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__sparc)
#if !defined(GENIO_BIG_ENDIAN)
#define GENIO_BIG_ENDIAN
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#elif defined(__GNUC__) && defined(__arm__)
// This is for 32-bit ARM processor with GCC
#if defined(__ARMEL__) && !defined(GENIO_LIT_ENDIAN)
#define GENIO_LIT_ENDIAN
#endif

#if defined(__ARMEB__) && !defined(GENIO_BIG_ENDIAN)
#define GENIO_BIG_ENDIAN
#endif

#if !defined(GENIO_UINT64)
#define GENIO_UINT64 unsigned long long int
#endif

#if !defined(GENIO_INT64)
#define GENIO_INT64 long long int
#endif

#endif

// MSVC's way of declaring large integer constants
#if defined(_WIN32)
#if defined(__MINGW32__) || defined(__MINGW64__)
#define I64(x) x##LL
#define UI64(x) x##ULL
#else
#define I64(x) x##i64
#define UI64(x) x##ui64
#endif
// GCC's way of declaring large integer constants
#else
#define I64(x) x##LL
#define UI64(x) x##ULL
#endif
