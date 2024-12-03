MAKEFLAGS += "-s -j 16"

##
## Project name
##
Project := OFFENSIVE_DEV

##
## Compilers
##
CC_X64	:= x86_64-w64-mingw32-gcc

##
## Compiler flags
##
CFLAGS  := -Os -Iinclude -static -nostdlib -Wl,-eEntry -nostartfiles -masm=intel
CFLAGS	+= -Wno-missing-declarations -Wno-missing-prototypes -Wno-inline

ifeq ($(PROFILE),debug)
	CFLAGS += -DDEBUG -g
else
	CFLAGS += -s
endif

##
## Loader source and object files
##
LOADER-SRC := $(wildcard src/*.c)
LOADER-OBJ := $(LOADER-SRC:%.c=%.o)

##
## x64 binaries
##
EXE-X64	:= bin/$(Project).x64.exe

##
## main target
##
all: x64

x64: clean asm-x64
	@ $(CC_X64) $(CFLAGS) bin/obj/hellhall.x64.o $(LOADER-SRC) -o $(EXE-X64)

asm-x64:
	@ nasm -f win64 asm/hellhall.x64.asm -o bin/obj/hellhall.x64.o

##
## Build source to object files
##
$(LOADER-OBJ):
	@ $(CC_X64) -o bin/obj/$(Project)_$(basename $(notdir $@)).x64.o -c $(basename $@).c $(CFLAGS)

##
## Clean object files and other binaries
##
clean:
	@ rm -rf .idea
	@ rm -rf bin/obj/*.o
	@ rm -rf bin/*.bin
	@ rm -rf bin/*.exe
	@ rm -rf cmake-build-debug