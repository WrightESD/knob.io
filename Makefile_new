# try to do some autodetecting
UNAME := $(shell uname -s) 

ifeq "$(UNAME)" "Darwin"
	OS=macosx
endif
ifeq "$(OS)" "Windows_NT"
	OS=windows
endif
ifeq "$(UNAME)" "Linux"
	OS=linux
endif


#################  Mac OS X  ##################################################
ifeq "$(OS)" "macosx"

EXE_SUFFIX=

ARCHS=   -arch i386 -arch x86_64
CFLAGS+= $(ARCHS)
CFLAGS += -mmacosx-version-min=10.6
CFLAGS_MONGOOSE=  -I./mongoose -pthread -g 
LIBS+=	 $(ARCHS)

endif

#################  Windows  ##################################################
ifeq "$(OS)" "windows"

EXE_SUFFIX=.exe

CFLAGS_MONGOOSE = -I./mongoose -mthreads

endif


#################  Common  ##################################################

CFLAGS += $(INCLUDES) -O -Wall -std=gnu99 -lasound -lm


all: arduino-serial-modified 

arduino-serial-modified: arduino-serial-modified.o arduino-serial-modified-lib.o
	$(CC) $(CFLAGS) -o arduino-serial-modified$(EXE_SUFFIX) arduino-serial-modified.o arduino-serial-modified-lib.o $(LIBS)

arduino-serial-modified-server: arduino-serial-modified-lib.o
	$(CC) $(CFLAGS) $(CFLAGS_MONGOOSE) -o arduino-serial-modified-server$(EXE_SUFFIX) arduino-serial-modified-server.c  arduino-serial-modified-lib.o mongoose/mongoose.c $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o


clean:
	rm -f $(OBJ) arduino-serial-modified arduino-serial-modified.exe *.o *.a
	rm -f $(OBJ) arduino-serial-modified-server arduino-serial-modified-server.exe *.o *.a
	rm -f mongoose/mongoose.o