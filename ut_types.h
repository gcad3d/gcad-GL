/* ./ut_types.h
*/


#include <stdint.h>              // ISO-C99 uint32_t - not yet VS-2008


#ifdef _MSC_VER
// Mircosoft - MS32
#define UINT_16   unsigned short //UINT16         // unsigned short WORD
#define UINT_32   uint32_t       // __u32  unsigned int

#else

// GCC - Linux 32,64
#define UINT_16   unsigned short // __u16  uint16_t unsigned short
#define UINT_32   uint32_t       // __u32  unsigned int

#endif


// EOF
