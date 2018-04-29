#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>




#define h_addr h_addr_list[0] /* for backward compatibility */


pthread_t listener, writer;

void error(char *msg)
{
    perror(msg);
    exit(0);
}


void *receiveMessage(void *socket){
    int sockfd, n;
    char buffer[256];

    sockfd = *(int *)socket;
    bzero(buffer, 256);

    while (1){
        bzero(buffer,256);
		n = read(sockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        

        if(!strcmp(buffer, "server-close-connection")){
            printf("\nCommand kill from server\n");
            closeConn(SIGTERM,sockfd);
        }else{
            printf("\nMessage received: %s\n",buffer);
        }

    }

    return NULL;
}

void *sendMessage (void *socket){
    char buffer[256];
    int sockfd, n;
    
    sockfd = *(int *)socket;
    
    do{
        printf("\nPlease enter the message: ");
        
        bzero(buffer,256);
        fgets(buffer,255,stdin);

        n = write(sockfd,buffer, strlen(buffer)-1);
        if (n < 0) error("ERROR reading from socket");
    }while (strcmp(buffer, "bye\n") != 0);
    
    
    return NULL;
}


int initSocket(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    return sockfd;
}


void doConnect(int sockfd, int portno, struct hostent *server) {
    struct sockaddr_in serv_addr;
    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
}

/*função para fechar as conexões*/
void closeConn(int sinal, int sockfd){
    char* bye = "bye";

    if (sinal == SIGINT) {
        printf("\nSignal 2\nClosing connection...\n");

        if (write(sockfd, bye, strlen(bye)) < 0) {
            printf("ERROR closing connection with server.\n");
        }
    }

    /*Encerrando as threads*/
    pthread_cancel(listener);
    pthread_cancel(writer);
    /*fechando o socket*/
    close(sockfd);

    exit(0);

}

int main(int argc, char *argv[]){
    
    int sockfd, portno;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    /* inicializa socket*/
    sockfd = initSocket();
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    /*Realiza conexão com o server*/
    doConnect(sockfd, atoi(argv[2]), server);
    
    
    pthread_create(&listener, NULL, receiveMessage, &sockfd);
    pthread_create(&writer, NULL, sendMessage, &sockfd);
    
    /*Ctrl C*/
    signal(SIGINT, closeConn);
    /*KILL*/
    signal(SIGTERM, closeConn);

    pthread_join(listener, NULL);
    pthread_join(writer, NULL);
    
    
   
    return 0;

}
