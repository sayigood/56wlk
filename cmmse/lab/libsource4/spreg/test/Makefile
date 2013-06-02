OUTPUT=$(patsubst %.cpp,%, $(wildcard *.cpp))

# workroot is CVSROOT
WORKROOT = ../../..



ULIB=$(WORKROOT)/lib2

HARDWARE_PLATFORM := $(shell uname -m)
ifeq ($(HARDWARE_PLATFORM),x86_64)	
	ULIB = $(WORKROOT)/lib2-64
endif

ULIBPATH=$(ULIB)/ullib

REGEXPATH=..
CC = g++

CPPFLAGS = -fPIC -g -W -Wall -Winline -pipe $(INCLUDES)

INCLUDES = -I$(REGEXPATH) -I$(ULIBPATH)/include 
LDFLAGS = -L$(REGEXPATH) -lspreg -L$(ULIBPATH)/lib -lullib -lm -lpthread


all	: $(OUTPUT) 
	if [ ! -d output ]; then mkdir output; fi
	cp $(OUTPUT) output
	rm -f *.o

$(OUTPUT): %: %.o
	$(CC)  -o $@ $< $(INCLUDES) $(LDFLAGS)  
%.o	: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f *.o $(OUTPUT)
	rm -rf output
