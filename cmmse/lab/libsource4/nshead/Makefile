CXX		= g++
LIB		= libnshead.a
SHELL		= /bin/sh
OBJS		= nshead.o
RANLIB		= ranlib

ULIB=/lib64/lib4/
USERFLAGS=-fPIC

ULIB_LDFLAG     = -L$(ULIB)/ullib/lib/ -lullib
ULIB_INCFLAG    = -I$(ULIB)/ullib/include/

INCLUDES	= $(ULIB_INCFLAG) -I../../include/
LDFLAGS		= $(ULIB_LDFLAG)
CPPFLAGS	= $(USERFLAGS) -g -W -Wall -Winline -pipe -D_WRITELOG_FORMAT_CHECK 

all: outputdir lib
	rm *.o
outputdir:
	mkdir -p output 2>/dev/null || echo
	mkdir -p output/include 2>/dev/null || echo

lib: $(LIB)

$(LIB): $(OBJS)
	rm -f $@ || echo
	ar cr $@ $(OBJS)
	$(RANLIB) $@
	cp nshead.h output/include/
	cp $@ output/

%.o: %.cpp %.h
	$(CXX) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

nshead: nshead.o 
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(LIB)
	rm -rf output/ || echo

OUTPUT_PATH=output
INSTALL_PATH=/lib64/lib4/`svn info |grep URL | sed "s/^.*\///g"`
#-----------------------------------------------------------------#
# make install
#-----------------------------------------------------------------#
install: output
	if [ ! -d $(INSTALL_PATH) ]; then mkdir -p $(INSTALL_PATH); fi
	cp -rf $(OUTPUT_PATH) $(INSTALL_PATH)/
