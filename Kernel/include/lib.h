#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include "scheduler.h"

#define PCI_CONFIG_ADDRESS  0x0CF8
#define PCI_CONFIG_DATA     0x0CFC


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char * cpuVendor(char *result);

void sysOutLong( uint16_t addr, uint32_t data);
void sysOutWord( uint16_t addr, uint16_t data);
void sysOutByte( uint16_t addr, uint8_t data);

uint32_t sysInLong(uint16_t addr);
uint16_t sysInWord(uint16_t addr);
uint8_t sysInByte(uint16_t addr);

void pushIPtoStack(void *s, void *ip);
void switchStackAndJump(void *s, void *j);
void * getRSP();
void endInter();

int mystrlen(char *s);

void getTime(char *ptr);

void int20();

void pushAQ();
void popAQ();

void sti();
void cli();

char * stateToString(int state);
char * itoaHelper(char * dest, int i);
char * itoa(char * dest, int i);

#endif