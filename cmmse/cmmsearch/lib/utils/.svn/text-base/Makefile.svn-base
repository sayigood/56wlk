
CC=g++

WORKROOT=../../../../../

ifeq ($(MAC),64)
	LIB2_PATH=$(WORKROOT)/lib2-64/
else
	LIB2_PATH=$(WORKROOT)/lib2/
endif
PUBLIC=$(WORKROOT)/public
ULLIB_PATH=$(LIB2_PATH)/ullib
DICT_PATH=$(LIB2_PATH)/dict
CACHE_PATH=$(LIB2_PATH)/cache
FILEBLOCK=$(WORKROOT)/public/fileblock

INCLUDES=-I$(ULLIB_PATH)/include -I$(FILEBLOCK) -I../../include -I$(PUBLIC)/ans/ -I$(DICT_PATH)/include -I$(CACHE_PATH)/include

CFLAGS = -Wall -W -pipe -Wno-unused-parameter -g  -fPIC 
LDFLAGS= -lm
OBJS=utils.o fblock_util.o stopword.o 
LIB=libutils.a
all : $(LIB)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(LIB) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	cp *.h ../
	cp *.a ../

tags : 
	 ctags -R *

clean:
	rm -f $(OBJS) $(LIB)


