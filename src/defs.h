#ifndef POS_DEFS_HEADER
#define POS_DEFS_HEADER

#include <vector>
#include <string>
#include <stdint.h>
#include <cmath>
#include <cstring>

#ifndef TRUE
    #define TRUE    1
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#define ZERO_MEMORY(ptr, size)  memset((ptr), 0, (size));

#define TWO_DECIMAL(fn)  (float)(floorf((fn) * 100) / 100)

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#elif defined(__cplusplus)
# define UNUSED(x)
#else
# define UNUSED(x) x
#endif

#endif

