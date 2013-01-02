/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include<malloc.h>
#include<fcntl.h>
#include<pthread.h>
#define MAX_STR_LENGTH 20
#define MAX_FILE_LENGTH 10000    
char ***matrix;
typedef struct{
char gram[MAX_FILE_LENGTH];
int no_prod;
int id;
char string[MAX_STR_LENGTH];
int difference;
}var;
void *table_basis(void *);
void *cyk_main_routine(void *);

void cyk(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
    
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
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
     while (1) {
         newsockfd = accept(sockfd,NULL,NULL);
         if (newsockfd < 0) 
             error("ERROR on accept");
          pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         else if (pid == 0)  {
             cyk(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

void cyk(int sock)
{
    	char *input_str;//input string receiver
    	input_str=(char*)malloc(MAX_STR_LENGTH*sizeof(char));
    	var *variab; //set of structure variables
    	pthread_t *thread; // set of threads to be run 
    	int i=0,j=0,k=0,m=0,y=0,t=0,s=0,r=0,n=0,d=0,p=0; //my looping and timepass variables
    	int fd; //file descriptor pointers
    	int no_prod_rules=0;
    	int input_str_length=0;
    	char *arr;//array which receives the file
    	arr=(char*)malloc(MAX_STR_LENGTH*sizeof(char));
    	char *buf;
    	buf=(char*)malloc(MAX_FILE_LENGTH*sizeof(char));
    	char dis_str[]="enter the string";
     	bzero(buf,sizeof(buf));
    	recv(sock,arr,MAX_FILE_LENGTH,0);
    	i=0;
    	while(arr[i]!=NULL)
	{
		if(arr[i]=='\n')
			no_prod_rules++;
		i++;
	}
  
    	// Write the string so that client comes to know that he has to enter the string
    	send(sock,dis_str,strlen(dis_str),0);	
    	// Read the input string 
    	recv(sock,input_str,MAX_STR_LENGTH,0);
    	input_str_length=strlen(input_str);
	// declare the structure variables
	variab=(var*)malloc(input_str_length*sizeof(var));
	thread=(pthread_t*)malloc(input_str_length*sizeof(pthread_t));
	for(i=0;i<input_str_length;i++)
	{
		for(j=0;j<MAX_FILE_LENGTH;j++)
			variab[i].gram[j]=NULL;

		for(j=0;j<MAX_STR_LENGTH;j++)
			variab[i].string[j]=NULL;
			variab[i].no_prod=no_prod_rules;
	}
	
	for(i=0;i<input_str_length;i++)
	{
		strcpy(variab[i].string,input_str);
	}
	for(i=0;i<input_str_length;i++)
	{
		strcpy(variab[i].gram,arr);
	}
	
	//declaration of the structure variables over	
    	//declare size of cyk table[input_str_length][input_str_length]

	matrix=(char***)malloc(input_str_length*sizeof(char**));
	for(i=0;i<input_str_length;i++)
		matrix[i]=(char**)malloc(input_str_length*sizeof(char*));
	for(i=0;i<input_str_length;i++)
	{
		for(j=0;j<input_str_length;j++)
		{
			
			matrix[i][j]=(char*)malloc(no_prod_rules*sizeof(char));
			
		}
	}	
	//cyk table basis i.e preparing the first line of the cyk table 
	for(i=0;i<input_str_length;i++) 
	{				
		variab[i].id=i;		
		pthread_create(&thread[i],NULL,table_basis,(void *)&variab[i]);
	}
	
	for(i=0;i<input_str_length;i++)
	{
		pthread_join(thread[i],NULL);		
	}
	//preparing cyk table basis over
	//here starts the main code which fills the entire cyk table
	for(d=1;d<input_str_length;d++)	//d indicates the difference between i and j to be kept
	{
		for(i=0;i<input_str_length-d;i++)
		{
			j=i+d;
			m=0;	
			variab[i].id=i;
			variab[i].difference=d;		
			pthread_create(&thread[i],NULL,cyk_main_routine,(void *)&variab[i]);		
			
						
			
		}
		for(i=0;i<input_str_length-d;i++)
		{
			pthread_join(thread[i],NULL);		
		}		
		
	}
	//main algorithm over  
	//displaying cyk_table for clearance of the running
	//presenting output
	for(k=0;k<input_str_length;k++)
	{
		for(i=0;i<input_str_length-k;i++)
		{			
			if(matrix[i][i+k]!=NULL)
			{			
				printf("%s\t",matrix[i][i+k]);
			}
			else
			{
				printf("\t\t");
			}
		}
		printf("\n");
		
	}
    	//Write the answer 
    	send(sock,matrix[0][input_str_length-1],strlen(matrix[0][input_str_length-1]),0);
}
void *table_basis(void *argu)
{		
	var *arg=(var*)argu;		
	int j=0,k=0,m=0;
	int i=arg[0].id;
	//making grammar
	int u=0,v=0,w=0;
	char **rhs;
	char **grammar;
	grammar=(char**)malloc(arg[0].no_prod*sizeof(char*));
	rhs=(char**)malloc(arg[0].no_prod*sizeof(char*));
	for(u=0;u<arg[0].no_prod;u++)
	{
		grammar[u]=(char*)malloc(30*sizeof(char));
	}
	u=0;
	v=0;
	w=0;
	while(u<arg[0].no_prod)
	{
		v=0;		
		while(arg[0].gram[w]!='\n')
		{
			grammar[u][v]=arg[0].gram[w];
			v++;
			w++;			
		}
		u++;
		w++;
	}
	for(u=0;u<arg[0].no_prod;u++)
	{
		rhs[u]=(char*)malloc(strlen(grammar[u])*sizeof(char));		
		for(w=0,v=2;v<strlen(grammar[u]);v++,w++)
		{
			rhs[u][w]=grammar[u][v];
		}
	}
	//ending grammar
	for(j=0;j<arg[0].no_prod;j++)
	{
		for(k=2;k<strlen(grammar[j]);k++)
		{					
			if(arg[0].string[i]==(grammar[j][k]))
			{
				matrix[i][i][m]=grammar[j][0];
				m++;
			}
		}	
	
	}
}
void *cyk_main_routine(void *argu)
{			
	var *arg=(var*)argu;	
	int p=0,r=0,s=0,t=0,flag=0,m=0,y=0,n=0;	
	int i=arg[0].id;
	int d=arg[0].difference;
	int j=i+d;
	char **rhs;
	//making grammar
	int u,v,w;
	char **grammar;
	grammar=(char**)malloc(arg[0].no_prod*sizeof(char*));
	rhs=(char**)malloc(arg[0].no_prod*sizeof(char*));
	for(u=0;u<arg[0].no_prod;u++)
	{
		grammar[u]=(char*)malloc(30*sizeof(char));
	}
	u=0;
	v=0;
	w=0;
	while(u<arg[0].no_prod)
	{
		v=0;		
		while(arg[0].gram[w]!='\n')
		{
			grammar[u][v]=arg[0].gram[w];
			v++;
			w++;			
		}
		u++;
		w++;
	}
	for(u=0;u<arg[0].no_prod;u++)
	{
		rhs[u]=(char*)malloc(strlen(grammar[u])*sizeof(char));		
		for(w=0,v=2;v<strlen(grammar[u]);v++,w++)
		{
			rhs[u][w]=grammar[u][v];
		}
	}
	//ending grammar
	int input_str_length=strlen(arg[0].string);
	char ***temp;
	temp=(char***)malloc(arg[0].no_prod*sizeof(char**));
	for(s=0;s<arg[0].no_prod;s++)
	{
		temp[s]=(char**)malloc(arg[0].no_prod*sizeof(char*));
	}

	for(s=0;s<arg[0].no_prod;s++)
	{
		for(t=0;t<arg[0].no_prod;t++)
		{
			temp[s][t]=(char*)malloc(3*sizeof(char));
		}
	}	
			for(p=i,r=i+1,s=0,t=0;r<=j;r++,p++)
			{
				for(s=0;s<strlen(matrix[i][p]);s++)
				{
					for(t=0;t<strlen(matrix[r][j]);t++)
					{
						temp[s][t][0]=matrix[i][p][s];
						temp[s][t][1]=matrix[r][j][t];
						for(n=0;n<arg[0].no_prod;n++)
						{
							if(strstr(rhs[n],temp[s][t]))//comparing whether the particular set is in which rhs
							{
								flag=0;								
								for(y=0;y<m;y++)// for avoiding repitition of particular non terminal in the particular box
								{
									if(matrix[i][j][y]==grammar[n][0])
									{

										flag=1;
										break;
									}
								}
								if(flag==0)
								{										
									matrix[i][j][m]=grammar[n][0];//assigning the appropriate matrix the value which is appr.

									m++;
								}
							}
						}
						
					}
				}
			}
			//printf("\nmatrix[%d][%d]=%s\n",i,j,matrix[i][j]);
			//if you want to check the value of each matrix uncomment the above statement
}
