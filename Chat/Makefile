CC=gcc 
#CFLAGS=-Wall -ansi

chat :
	server client

#compila o servidor:
server :
	gcc server.c -o server -lpthread -Wall -ansi

#compila o cliente:
client :
	gcc client.c -o client -lpthread -Wall -ansi


clean :
	rm server *.o
	rm client *.o
