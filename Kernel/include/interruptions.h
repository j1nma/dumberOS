#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

void iSetHandler(int index, uint64_t handler);
void irqDispatcher(int irq);

void sti();
void setPicMaster(uint16_t);
void setPicSlave(uint16_t);

void tickHandler();
void irq0Handler();
void irq1Handler();
void irq2Handler();
void sysCallHandler();
void rtlHandler();

#endif
