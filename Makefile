CC = g++
# CFLAGS = -g -Wall -I./hdr/
CFLAGS = -g -Wall -I./
LDFLAGS = -lm

src = $(wildcard *.cpp) \
		  $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

all: clean agents

agents: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) agents
