CC = g++
CFLAGS = -std=c++11 -g
LDFLAGS =

SOURCES = $(filter-out main.cpp, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)
HEADER_GCH = $(wildcard *.gch)

# Targets for each test
TESTS = test

all: $(TESTS)

test: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

clean:
	rm -f $(TESTS) $(HEADER_GCH)