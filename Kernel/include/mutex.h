#ifndef _MUTEX_H
#define _MUTEX_H

#define UNLOCKED 0
#define LOCKED 1

void initMutex();

void down();

void up();

#endif