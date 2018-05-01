/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>


#define MAXCLIENTS 5


pthread_t* vet_threads;
int* vet_sockets;
int t_count = 0;
pthread_t main_thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *receiveMessage(void *socket){
    int sockfd, i, position;
    char buffer[256];

    sockfd = *(int *)socket;
    

   do{
       int n;
       bzero(buffer, 256);
       n = read(sockfd,buffer,255);
       if (n < 0) error("ERROR reading from socket");
        /*Passando pro próximo client do chat*/
        for (i = 0;i < MAXCLIENTS; i++) {
            if ((vet_sockets[i] != -1) && vet_sockets[i]!= sockfd) {
                /*le do client*/
                n = write(vet_sockets[i], buffer, strlen(buffer));
			    if (n < 0){
                    error("ERROR writing to socket");
                }else {
                    printf("%d sent: %s \n", vet_sockets[i], buffer);
                }
                
            }
        }
        
        
    } while ((strcmp(buffer,"bye") != 0));
    /*trava da thread*/
    pthread_mutex_lock(&mutex);
    vet_sockets[sockfd-1] = -1;
    pthread_mutex_unlock(&mutex);
    
    t_count--;
    /*close(sockfd);*/
    
    printf("Killing client %d \n", sockfd);
	
	if (t_count == 0) pthread_cancel(main_thread);

    return NULL;
}

void *get_clients(void *socket){
    struct sockaddr_in cli_addr;
    int cli_len, newsockfd, i, sockfd, pos_vazio;

    sockfd = *(int *)socket;

    /*do while - executa ao menos uma vez*/
    do{
        listen(sockfd,5);
        pos_vazio = -1;

        /*navega no vetor de clientes pra pegar a primeira posição vazia*/
        pthread_mutex_lock(&mutex);
        for(i=0; i < MAXCLIENTS; i++){
            if(vet_sockets[i] == -1){
                pos_vazio = i;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);

        if(pos_vazio != -1){
            cli_len = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
            if(newsockfd<0){
                error("ERROR on accept");
            }
            pthread_create(&vet_threads[pos_vazio], NULL, receiveMessage, &newsockfd);
			vet_sockets[pos_vazio] = newsockfd;
            t_count++;
            printf("Connected: %d \n", t_count);
        }
        else{
            printf("Server is full, try again later\n");
        }
    } while (t_count > 0);

    return NULL;
}




void error(char *msg)
{
    perror(msg);
    exit(1);
}

int initSocket(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    return sockfd;
}   

void doBind(int sockfd, int portno){
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
}

void closeSocket(int sinal){
    int i;
    char* closeConn = "server-close-connection";

    for (i = 0; i < MAXCLIENTS; ++i)
    {
        if (vet_sockets[i] != -1) {
            printf("\nSending message to the client [%d] to close.\n", i);
            if (write(vet_sockets[i], closeConn, strlen(closeConn)) < 0) {
                printf("ERROR on close client sockfd: [%d].\n", vet_sockets[i]);
            }
        }
    }

    close(vet_sockets);
    exit(0);
}


int main(int argc, char *argv[])
{
    int sockfd, portno, i;
     
    vet_threads = malloc(5 * sizeof(pthread_t));
    vet_sockets = malloc(5 * sizeof(int));
     
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    /*inicia Socket do server*/
    sockfd = initSocket();
    /*executa bind*/
    doBind(sockfd, atoi(argv[1]));

    
	/*Inicializando vetor de sockets*/
    for (i = 0; i < MAXCLIENTS; i++){
		vet_sockets[i] = -1;
    }
    
    signal(SIGINT, closeSocket);

	pthread_create(&main_thread, NULL, get_clients, &sockfd);
	
	pthread_join(main_thread, NULL);
	
	free(vet_threads);
	free(vet_sockets);
    
     
    return 0; 
}