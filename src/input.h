#ifndef INPUT_H__
#define INPUT_H__

uint8 *input_keys;

typedef struct
{
	const char *name;
	uint32 last;
	void (*func) (void);
	void (*sfunc) (void);
} keydef_t;

extern keydef_t keydefs [];

uint8 input_keydown (const char *key);
void input_dokeys (void);

#endif // INPUT_H__
