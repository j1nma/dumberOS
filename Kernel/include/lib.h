#ifndef LIB_H
#define LIB_H

#define PCI_CONFIG_ADDRESS  0x0CF8
#define PCI_CONFIG_DATA     0x0CFC


#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void sysOutLong( uint16_t addr, uint32_t data);
void sysOutWord( uint16_t addr, uint16_t data);
void sysOutByte( uint16_t addr, uint8_t data);

uint32_t sysInLong(uint16_t addr);
uint16_t sysInWord(uint16_t addr);
uint8_t sysInByte(uint16_t addr);

int mystrlen(char *s);

void getTime(void *ptr);

#endif