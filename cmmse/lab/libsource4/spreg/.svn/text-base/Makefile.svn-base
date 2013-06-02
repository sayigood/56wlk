
LIBS = libspreg.a
OUTPUT = ./output

SRCS = $(wildcard *.cpp)
HEADS = $(wildcard *.h)

OBJS = $(SRCS:.cpp=.o)

GCC = g++
AR = ar

WORKROOT=../../

ifeq ($(MAC), 64)
	LIBPATH = $(WORKROOT)lib2-64
  USERFLAGS = -fPIC
  THIRDPATH = ../../third-64
else
	LIBPATH = $(WORKROOT)lib2
  THIRDPATH = ../../third
endif

PCREPATH = $(THIRDPATH)/pcre


INCLUDE = -I$(PCREPATH)/include
CPPFLAGS = -g $(USERFLAGS) -W -Wall -Winline -pipe 

.cpp.o : 
	$(GCC) -c $(CPPFLAGS) $(INCLUDE) $(DEFINES) $<

all : deps $(LIBS) $(OUTPUT) 

$(LIBS) : $(OBJS)
	@$(AR) x $(PCREPATH)/lib/libpcre.a
	@$(AR) -r $@ *.o
  
$(OUTPUT) : $(LIBS)
	@if [ -d $@ ]; then rm -rf $@; fi
	@if [ -d $@/include ]; then rm -rf $@/include; fi
	@if [ -d $@/lib ]; then rm -rf $@/include; fi
	@mkdir $@;
	@mkdir $@/include;
	@mkdir $@/lib;
	cp $^ $@/lib;
	@cp $(HEADS) $@/include;
	@cp $(PCREPATH)/include/pcre.h $@/include;
	@cp $(PCREPATH)/include/pcre.h ./;
	@rm *.o
  
deps : $(SRCS) $(HEADS)
	@$(GCC) -MM -MG $(CPPFLAGS) $(INCLUDE) $(DEFINES) $(filter %.cpp, $^) > $@

clean :
	rm -rf $(LIBS) *.o 
	@rm -rf pcre.h
	rm -rf deps $(OUTPUT)

# Clean and make all
rebuild : clean all

-include deps

