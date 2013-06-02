## $Id: Makefile,v 1.3 2008/08/28 08:19:32 yanghy2 Exp $
WORKROOT=../../../../../
include Makefile.env
CC= g++
SHELL= /bin/sh
DEBUG= -DMYDEBUG

OBJ= main.o myopt.o  talk.o
bin= tinyse-qtool

FELT_INCS = -I$(ULLIB_PATH)/include/\
			-I$(NSHEAD_PATH)/ \
			-I$(MYNET) \
			-I${MCPACK_PATH}\
			-I${MCPACK_PATH}/tools\
			-I../../include

FELT_LIBS = -L$(ULLIB_PATH)/lib\
			-L$(NSHEAD_PATH)\
			-L$(MYNET)\
			-L${MCPACK_PATH} \
			-lmcpack -lnshead -lmynet   



INCS=  $(FELT_INCS) \
	   -I$(ULLIB_PATH)/include/  \
	   -I$(DICT_PATH)/include 

LIBS= $(FELT_LIBS)\
      -L$(ULLIB_PATH)/lib \
	  -lullib -lpthread -lcrypt -lcrypto  -lm

VERINFO= -DPROJNAME="\"$(bin)\"" -DVERSION="\"$(bin) 1.0.0.0\"" -DCVSTAG="\"$(bin)_1-0-0-0_PD_BL\"" -DCVSPATH="\"app/search/space/$(bin)\"" -DAUTHOR="\"yanghy\""

CPPFLAGS= $(DEBUG) $(INCS) $(VERINFO) -g -pipe  -W -Wall -Wshadow -Wreturn-type \
		 -Wno-unused-parameter  -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith  -Werror

LDFLAGS=  -g  $(LIBS)

all: $(bin)
	rm *.o -rf
$(bin): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) $< -c -o $@ 

clean:
	rm *.o -rf
	rm $(bin) -rf
