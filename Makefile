UNAME := $(shell uname)

FLAGS=--std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare -Wunreachable-code -pedantic  -Wstrict-prototypes
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	FLAGS   += -I/Library/Frameworks/SDL.framework/Headers
	LDFLAGS +=  -lportmidi  -framework SDL -framework Cocoa SDLmain.m
endif

ifeq ($(UNAME), Linux)
	LDFLAGS +=  -lSDL
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/ 
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL
	BINEXT = .exe
endif

LIB_SOURCES=$(wildcard libs/*.c)
LIB_HEADERS=$(wildcard libs/*.h)

all: midiPlasma

clean:
	rm -f midiPlasma

midiPlasma: main.c midiPlasma.c main.h Makefile $(LIB_HEADERS) $(LIB_SOURCES)
	gcc $(FLAGS) main.c midiPlasma.c $(LDFLAGS) -o midiPlasma -I. $(LIB_SOURCES) -Ilibs

