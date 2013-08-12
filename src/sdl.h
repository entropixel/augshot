#ifndef SDL_WRAP__
#define SDL_WRAP__

// SDL, you're being fucking ridiculous
char *strdup(const char *s);
int setenv(const char *name, const char *value, int overwrite);
#include <SDL.h>

#endif // SDL_WRAP__
