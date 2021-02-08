all : client
client : client.o
						gcc client.o -o client

client.o : client.c game.h
						gcc -c client.c

clean :
					rm *.o client
