CC = g++
CFLAGS = -g -Wall -Werror -pedantic-errors -DNDEBUG  -std=c++11
CCLINK = $(CC)
OBJS = smash.o commands.o signals.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.hpp
	$(CC) $(CFLAGS) -c commands.cpp
smash.o: smash.cpp commands.hpp
	$(CC) $(CFLAGS) -c smash.cpp
signals.o: signals.cpp signals.hpp
	$(CC) $(CFLAGS) -c signals.cpp
# Cleaning old files before new make
clean:
	$(RM) smash *.o *~ "#"* core.*
