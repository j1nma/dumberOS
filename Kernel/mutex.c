#include "mutex.h"
#include "drivers.h"

char * lock;

void initMutex() {
	lock = malloc(sizeof(char *));
	*lock = UNLOCKED;
}

void down() {
  while (*lock == LOCKED);
  *lock = LOCKED;
}

void up() {
  *lock = UNLOCKED;
}