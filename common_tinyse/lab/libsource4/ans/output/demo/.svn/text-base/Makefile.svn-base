#
# Description: Makefile of odict library
# 

GCC = /usr/bin/g++

LIBPATH=./lib4/
INCLUDE = -I../ -I../include/ -I$(LIBPATH)/ullib/include 

CPPFLAGS = -g -finline-functions -Wall -Winline -pipe -DHASH_THREAD_SAFE -D_XOPEN_SOURCE=500
TLDFLAGS = -L../ -L$(LIBPATH)/ullib/lib/ -lullib -lpthread

TEST1 = test1 
TEST4 = test4

all: output target

output: $(TEST1) $(TEST2) $(TEST3)

target:
	@echo ""

clean:
	rm -rf *.o $(TEST1) $(TEST2) $(TEST3)

$(TEST1) : hash_t.o
	$(GCC) -o $@ $^ $(TLDFLAGS)
	rm -rf $^


%.o	: %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE)
