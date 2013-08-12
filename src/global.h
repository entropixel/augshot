#ifndef GLOBAL_H__
#define GLOBAL_H__

// wrapper for stdint, make nice type names
#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

// fixed point madness
typedef int32_t fixed;
#define FRAC 16 // fractional bits
#define tofixed(a) ((fixed)(a * (1 << FRAC)))
#define fromfixed(a) ((float) ((a >> FRAC) + ((float)(a & 0xffff) * (1.0/65536.0))))
fixed fmul (fixed a, fixed b);
fixed fdiv (fixed a, fixed b);
void fsin_gentable (void);
fixed fsin (fixed a);

// extern globals
extern uint32 curtick;

#define SWIDTH 256
#define SHEIGHT 144

#endif // GLOBAL_H__
