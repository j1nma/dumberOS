#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <drivers_constants.h>
#include <ethernet_protocol.h>

// Divers Keyboard
void setKeyboardCallback(void (*f)(uint8_t, int));
void keyboardHandler();

// Divers Video
void write(char *str, int length);
int read(char* str);
void newLine();
void backspace();

// Divers Network

void net_start();
void net_send(char *msg);
int net_read(char *b);

//Drivers memoria

void *malloc(int size);
void *calloc(int size);
void free(void *ptr);