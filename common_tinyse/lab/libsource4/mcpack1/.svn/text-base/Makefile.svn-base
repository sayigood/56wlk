##
## $Id: Makefile,v 1.8 2008/09/08 12:15:56 zhouzr Exp $
## mc_pack Makefile
##

CC = gcc
LIB = libmcpack.a
SHELL = /bin/sh
OBJS = mc_pack_core.o tools/mc_pack_ex.o tools/mc_pack_text.o
RANLIB = ranlib

INCFLAGS = -I.
CFLAGS = -g -Wall -Winline -pipe -Wshadow -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith -Wsign-compare -Werror -fPIC \
		 -DVERSION="\"mcpack 1.0.0.7\""	-DCVSTAG="\"mcpack_1-0-0-7_PD_BL\"" -O2
LDFLAGS =

CPPUNIT_INC = -I$(HOME)/cppunit/include -Ioutput/include
CPPUNIT_LD = -L$(HOME)/cppunit/lib -lcppunit -ldl -Loutput/lib -lmcpack

all: outputdir lib
	rm -f $(OBJS)
outputdir:
	-mkdir output 2>/dev/null
	-mkdir output/include 2>/dev/null
	-mkdir output/lib 2>/dev/null

lib: $(LIB)

$(LIB): $(OBJS)
	-rm -f $@
	ar cr $@ $(OBJS)
	$(RANLIB) $@
	cp mc_pack.h output/include/
	cp mc_pack_core.h output/include/
	cp tools/mc_pack_ex.h output/include/
	cp tools/mc_pack_text.h output/include/
	cp libmcpack.a output/lib/

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

unitest: unitest.cpp libmcpack.a
	g++ -g -Wall -o $@ $^ $(CPPUNIT_INC) $(CPPUNIT_LD)

clean:
	rm -f unitest
	rm -f $(OBJS) $(LIB)
	-rm -rf output
