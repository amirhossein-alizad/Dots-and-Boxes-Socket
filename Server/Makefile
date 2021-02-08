all : server
server : server.o
						gcc server.o -o server

server.o : server.c server.h
						gcc -c server.c

clean :
					rm *.o server
