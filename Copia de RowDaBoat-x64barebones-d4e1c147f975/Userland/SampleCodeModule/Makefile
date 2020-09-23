include ../Makefile.inc
MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
ASMSOURCES=$(wildcard asm/*.asm)
ASMOBJECTS=$(ASMSOURCES:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(ASMOBJECTS)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -I ./include _loader.c $(SOURCES) $(ASMOBJECTS) -o ../$(MODULE)

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o *.o

.PHONY: all clean print