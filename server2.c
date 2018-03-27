/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

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
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        
        n = write(newsockfd2,buffer,strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
		
	    if (strcmp(buffer,"bye\n")==0){ 
			close(newsockfd);
			close(newsockfd2);
			return 0;
		}
		bzero(buffer,256);
        fgets(buffer,255,stdin);
     }
}
