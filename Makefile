C      =       gcc
CXX     =       g++
LINK = g++
ECHO := echo
CFLAGS := -x c -std=gnu99 -O3
CCFLAGS := -x c++ -std=gnu++0x -I. -I/usr/include -Wno-write-strings -O3
LDFLAGS := -L/usr/lib -lpthread -lstdc++
GTEST = ./gtest/src/gtest_main.a

.PHONY: all, test, force_look, clean

all: test

test: board_test
	./board_test

force_look :
	true

clean:
	rm -f *.o
	rm -f *.s
	rm -f *_test

$(GTEST): force_look
	$(ECHO) looking into gtest/src : $(MAKE) $(MFLAGS)
	cd gtest/src; $(MAKE) $(MFLAGS)

################################################################################
# board
################################################################################

board.o: board.cc board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

board_test.o: board_test.cc board.h
	$(CXX) $(CCFLAGS) -c $< -o $@

board_test: board_test.o board.o $(GTEST)
	$(LINK) -o $@ $^ $(LDFLAGS)

################################################################################
# generate
################################################################################

mask_generate_quartet.o: mask_generate_quartet.cc board.h
	$(CXX) $(CCFLAGS) -c $< -o $@
mask_generate_quartet: mask_generate_quartet.o board.o
	$(LINK) -o $@ $^ $(LDFLAGS)

mask_generate_under.o: mask_generate_under.cc board.h
	$(CXX) $(CCFLAGS) -c $< -o $@
mask_generate_under: mask_generate_under.o board.o
	$(LINK) -o $@ $^ $(LDFLAGS)

