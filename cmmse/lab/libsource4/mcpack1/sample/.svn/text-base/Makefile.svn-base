#$Id: Makefile,v 1.1 2008/03/17 06:02:36 zhouzr Exp $
COMPILER?=g++
CC:=$(COMPILER)

WORKROOT=../../../

ifeq ($(MAC),64)
  LIBPATH=$(WORKROOT)lib2-64
else
  LIBPATH=$(WORKROOT)lib2
endif

ULLIB=$(LIBPATH)/ullib/
NSHEAD=$(WORKROOT)/public/nshead/
UBLIB=$(WORKROOT)/public/ub/
MCPACKLIB=$(WORKROOT)/public/mcpack/output
INCLUDE_PATH=-I$(ULLIB)/include/ -I$(NSHEAD) -I$(UBLIB)/include -I$(MCPACKLIB)/include
LIB_PATH=-L$(ULLIB)/lib -L$(UBLIB)/lib  -L$(NSHEAD) -L$(MCPACKLIB)/lib

OBJ:=test_pack test_pack2

CPPFLAGS = -g -Wall -W -Winline -Wshadow -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith -Werror -D_GNU_SOURCE

LIB = -lnshead -lullib -lpthread -lmcpack

.PHONY: all clean doc wc install

all: $(OBJ)

test_pack : test_pack.cpp
	$(CC) -o $@ $^ $(CPPFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

test_pack2 : test_pack2.c
	$(CC) -o $@ $^ $(CPPFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

clean:
	rm -f *.o $(OBJ)

doc:
	doxygen
wc:
	wc *.cpp *.c *.h
