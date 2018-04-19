CC=gcc 
#CFLAGS=-Wall -ansi

chat :
	server client

#compila o servidor:
make server :
	gcc server.c -o server -lpthread -Wall -ansi

#compila o cliente:
make client :
	gcc client.c -o client -lpthread -Wall -ansi


clean :
	rm server *.o
	rm client *.o
