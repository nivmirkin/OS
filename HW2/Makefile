CC = g++
CFLAGS = -std=c++11 -Wall  -Werror -pedantic-errors -DNDEBUG -pthread
RM = rm -f

Bank: Bank.o ATM.o
	$(CC) $(CFLAGS) -o Bank Bank.o ATM.o

Bank.o: Bank.cpp Bank.hpp ATM.hpp
	$(CC) $(CFLAGS) -c Bank.cpp

ATM.o: ATM.cpp Bank.hpp ATM.hpp
	$(CC) $(CFLAGS) -c ATM.cpp

clean:
	$(RM) Bank *.o
