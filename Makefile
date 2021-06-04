CC = g++

CFLAGS = -std=c++17 -Wall -Wextra -g

LIBS = -lm

SRCS = main.cpp facade.h facade.cpp board.h board.cpp game.h game.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = main

.PHONY: depend clean

build: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

run:
	./$(MAIN)