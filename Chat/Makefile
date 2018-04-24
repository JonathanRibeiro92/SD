CC=gcc 
#CFLAGS=-Wall -ansi

all :
	server client

#compila o servidor:
server :
	gcc server.c -o server -lpthread -Wall -ansi

#compila o cliente:
client :
	gcc client.c -o client -lpthread -Wall -ansi

#remove tudo
clean :
	rm  -f client server
