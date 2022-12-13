SRCDIR = src
INCLUDEDIR = include
OBJDIR = obj
BINDIR = bin
VALGRIND = tools/valgrind

INCLUDEFLAGS = -I${INCLUDEDIR}
CPPFLAGS = -ansi -c -g -std=c++14 -Wall -DDEBUG ${INCLUDEFLAGS}

client : client.o client_main.o
	g++ -o ${BINDIR}/client ${OBJDIR}/client.o ${OBJDIR}/client_main.o -lpthread

client_main.o : ${SRCDIR}/client_main.cpp
	g++ -o ${OBJDIR}/client_main.o ${CPPFLAGS} ${SRCDIR}/client_main.cpp

client.o : ${SRCDIR}/client.cpp
	g++ -o ${OBJDIR}/client.o ${CPPFLAGS} ${SRCDIR}/client.cpp

valgrind : client
	valgrind --leak-check=full  ${BINDIR}/client 2> ${VALGRIND}/clientValgrind.txt

clean :
	echo "cleaning up .o and exes"
	rm ${BINDIR}/client ${OBJDIR}/*.o
