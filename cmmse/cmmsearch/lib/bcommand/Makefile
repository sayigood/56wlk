##
## timeused module Makefile
##

CC=gcc
LIB=libbcommand.a
SHELL=/bin/sh
OBJS=bcommand.o
RANLIB=ranlib
WORKROOT=../../../../../

ifeq ($(MAC),64)
    ULIB=$(WORKROOT)/lib2-64
else
    ULIB=$(WORKROOT)/lib2
endif

INCLUDES= -I$(ULIB)/ullib/include

CPPFLAGS= -g -finline-functions -Wall -W -Winline -pipe

all: lib
	rm *.o

lib: $(LIB)

$(LIB): $(OBJS)
	rm -f $@ ||echo
	ar cr $@ $(OBJS)
	$(RANLIB) $@
	cp $(LIB) ../
	cp bcommand.h ../

%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

bcommand: bcommand.o 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(LIB) ||echo
