##############################################################################
#
# Module: Makefile
#
# Function:
#	Build and run the host tests: make -C test/host
#
#	Needs Catena-mcciadk; set MCCIADK to its src directory if it isn't
#	a sibling of this library.
#
# Copyright notice:
#	See accompanying LICENSE file.
#
# Author:
#	Terry Moore, MCCI Corporation	October 2026
#
##############################################################################

CXX ?= g++
MCCIADK ?= ../../../Catena-mcciadk/src
CXXFLAGS ?= -std=c++17 -Wall -Wextra -g
CPPFLAGS += -Istub -I../../src -I$(MCCIADK)

SOURCES = \
	test_fram.cpp \
	host_log.cpp \
	../../src/lib/Catena_Fram.cpp \
	../../src/lib/Catena_FramStorage.cpp

# the C parts of Catena-mcciadk that the FRAM code calls
MCCIADK_SOURCES = $(MCCIADK)/lib/mcciadklib_snprintf.c

BUILD = build

.PHONY: all check clean

all check: $(BUILD)/test_fram
	$(BUILD)/test_fram

$(BUILD)/mcciadk.o: $(MCCIADK_SOURCES)
	mkdir -p $(BUILD)
	$(CC) -c -g $(CPPFLAGS) -o $@ $(MCCIADK_SOURCES)

$(BUILD)/test_fram: $(SOURCES) $(BUILD)/mcciadk.o $(wildcard *.h stub/*.h ../../src/*.h)
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(SOURCES) $(BUILD)/mcciadk.o

clean:
	rm -rf $(BUILD)
