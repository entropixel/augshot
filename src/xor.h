#ifndef XOR_H__
#define XOR_H__

typedef struct
{
	uint32 seed;
	uint32 rs [4];
} xorshift;

void xsrand (uint32 seed);
void xsrand_r (xorshift *x, uint32 seed);
uint32 xrand (void);
uint32 xrand_r (xorshift *x);
float fxrand (void);
float fxrand_r (xorshift *x);

#endif // XOR_H__
