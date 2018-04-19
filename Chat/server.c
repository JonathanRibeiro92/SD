/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>


#define MAXCLIENTS 5


pthread_t* threads;
int* sockets;
int t_count = 0;
pthread_t main_thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//TODO
void *connection_handler(void*);


void *receiveMessage(void *socket){
    int sockfd, n, i;
    char buffer[256];

    sockfd = *(int *)socket;
    bzero(buffer, 256);

    while ((strcmp(buffer,"bye") != 0)||signal(15,tratasinal){
        bzero(buffer,256);
        //fgets(buffer,255,stdin);
		//le do client
		n = read(newsockfd2,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        broadcast(buffer);
        printf("%d sent: %s \n", sockfd, buffer);

    }
    //Passando pro próximo client do chat
    for (i = 0;i < MAXCLIENTS; i++) {
        if (sockets[i] == sockfd) {
    		position = i;
    		break;
    	}
    }
    pthread_mutex_lock(&mutex);
    sockets[position] = -1;
    pthread_mutex_unlock(&mutex);
    
    t_count--;
    //close(sockfd);
    
    printf("Killing client %d \n", sockfd);
	
	if (t_count == 0) pthread_cancel(main_thread);

    return NULL;
}


void broadcast(char *buffer){
    int i;
    int sockfd, n;
    
    for (i = 0; i < t_count; i++){
        sockfd = sockets[i];
        
		if (sockfd != -1){
			n = write(sockfd, buffer, strlen(buffer));
			if (n < 0) error("ERROR writing to socket");
		}
    }
}


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{	
     int sockfd, sockfd2, newsockfd, newsockfd2, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd2 < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");    
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     listen(sockfd,5);
	 newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	  if (newsockfd2 < 0) 
          error("ERROR on accept");
     while(1){
		//le do client1
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        // escreve no client2
        n = write(newsockfd2,buffer,strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
		
	    if (strcmp(buffer,"bye\n")==0){ 
			close(newsockfd);
			close(newsockfd2);
			return 0;
		}
		bzero(buffer,256);
        //fgets(buffer,255,stdin);
		//le do client2
		n = read(newsockfd2,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        // escreve no client1
        n = write(newsockfd,buffer,strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
		
	    /* if (strcmp(buffer,"bye\n")==0){ 
			close(newsockfd);
			close(newsockfd2);
			return 0;
		}*/
     }
}