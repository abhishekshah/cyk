#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int MAX_STR_LENGTH=20;
int MAX_ANSWR_LENGTH=20;
int MAX_FILE_LENGTH=10000;
//for the same computer i could have used pipes to extract the value of maximum string length maximum answer length and max file length
//defined it individually in the client and server
/*
		INSTRUCTIONS
******************************************************
	argv1 is ip address
	argv2 is port no
	argv3 is file_name containing in grammar 
	 in this case use sample.txt
******************************************************
*/  
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	//variables of client program
    	int sockfd, portno; 
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
	// variables of cyk
	int i=0,j=0,k=0,m=0,y=0,t=0,s=0,r=0,n=0,d=0,p=0;
	int fd;
	char *string;
	string=(char*)malloc(MAX_STR_LENGTH*sizeof(char)); //input string
	int input_str_length=0,flag=0;
	char ***matrix;
	char *temp;
	temp=(char*)malloc( MAX_STR_LENGTH*sizeof(char));
	char *buf;
	char *buf_ans;
	buf=(char*)malloc(MAX_FILE_LENGTH*sizeof(char));
	buf_ans=(char*)malloc(MAX_ANSWR_LENGTH*sizeof(char));
	char* str_output;
     if (argc < 4) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
   

	//Working of the client 

   
    // Reading the file by the user and sending it to the server for further processing 
	for(i=0;i<MAX_FILE_LENGTH;i++)
		buf[i]=NULL;
	fd=open(argv[3],O_RDONLY);
	if(fd==-1)
	{
		printf("fatal file opening error");		
		exit(-1);
	}
	read(fd,buf,MAX_FILE_LENGTH);
    	send(sockfd,buf,strlen(buf),0);
    /* Read from the response from the server */
	if(recv(sockfd,temp,17,0))
	{
    /* send the input string to the server */ 
		printf("%s:\n",temp);
		scanf("%s",string);
		send(sockfd,string,strlen(string),0);
	}
	else
	{
		printf("server responded unexpectedly");
		exit(-1);
	}
    /* receiving the final answer*/
	if(recv(sockfd,buf_ans,MAX_ANSWR_LENGTH,0))
		printf("\n%s\n",buf_ans);
	else
	{
		printf("Server unable to answer");
		exit(-1);
	}
    close(fd);	
    close(sockfd);
    return 0;
}
