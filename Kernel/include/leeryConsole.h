#ifndef LEERY_CONSOLE_H
#define LEERY_CONSOLE_H

#include <stdint.h>

void lcPrintInt(int number);
void lcPrintChar(char c);
void lcPrint(char *str);
void lcNewLine();
void lcBackSpace();
void lcPrintHex(uint16_t value);

#endif