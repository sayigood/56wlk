WORKROOT=../../..
ifeq ($(shell uname -m),x86_64)
LIBPATH=$(WORKROOT)/lib2-64
else
LIBPATH=$(WORKROOT)/lib2
endif

CXXFLAGS=-g -Wall -W -fPIC -I../ -I$(WORKROOT)/public/mcpack/output/include -I$(LIBPATH)/bsl/include -I$(LIBPATH)/ullib/include
objects=libmcpack_idl.a
.PHONY: all clean
all: $(objects)
	mkdir -p ../output/lib
	cp libmcpack_idl.a ../output/lib
	mkdir -p ../output/include
	cp mcpack_idl.h ../output/include
clean:
	rm -f *.o $(objects)
mcpack_idl.o:mcpack_idl.cpp mcpack_idl.h
libmcpack_idl.a : mcpack_idl.o
	ar crs libmcpack_idl.a mcpack_idl.o 
