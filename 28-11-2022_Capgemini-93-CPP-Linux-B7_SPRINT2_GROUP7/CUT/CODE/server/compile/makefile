SRCDIR = src
INCLUDEDIR = include
OBJDIR = obj
BINDIR = bin
VALGRIND = tools/valgrind

INCLUDEFLAGS = -I${INCLUDEDIR}
CPPFLAGS = -ansi -c -g -std=c++14 -Wall -DDEBUG ${INCLUDEFLAGS}

server : server.o shared_mem.o server_main.o
	g++ -o ${BINDIR}/server ${OBJDIR}/server.o ${OBJDIR}/shared_mem.o ${OBJDIR}/server_main.o -lpthread

server_main.o : ${SRCDIR}/server_main.cpp
	g++ -o ${OBJDIR}/server_main.o ${CPPFLAGS} ${SRCDIR}/server_main.cpp

shared_mem.o : ${SRCDIR}/shared_mem.cpp
	g++ -o ${OBJDIR}/shared_mem.o ${CPPFLAGS} ${SRCDIR}/shared_mem.cpp

server.o : ${SRCDIR}/server.cpp
	g++ -o ${OBJDIR}/server.o ${CPPFLAGS} ${SRCDIR}/server.cpp

valgrind : server
	valgrind --leak-check=full --show-reachable=yes ${BINDIR}/server  2> ${VALGRIND}/serverValgrind.txt

clean :
	echo "cleaning up .o and exes"
	rm ${BINDIR}/server ${OBJDIR}/*.o
