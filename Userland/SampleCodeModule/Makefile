include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
SOURCE_LOADER=_loader.c
OBJECT_LOADER=$(SOURCE_LOADER:.c=.o)


all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECT_LOADER)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld $(OBJECT_LOADER) $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)



%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@



clean:
	rm -rf *.o

.PHONY: all clean print
