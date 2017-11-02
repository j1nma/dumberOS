#include "mutex.h"

long lock = UNLOCKED;

void down() {
  while (lock == LOCKED);
  lock = LOCKED;
}

void up() {
  lock = UNLOCKED;
}