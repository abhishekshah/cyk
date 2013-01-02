#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#define MAX_STR_LENGTH 20
#define MAX_FILE_LENGTH 10000
/*
******************************************************************************
Format of entering grammar and string and output:
-> A->B|C has to be entered as A-B|C
-> use small characters for terminals and capital letters for terminals
-> According to standard convention use S as sentence symbol
-> As the grammar rules are in CNF, S generally doesn't appear in the right 
   hand side of the code which should be taken care of.
-> Only terminals and hence small letters should be entered in the string.
-> I will present the output as whole parsing tree
-> If S is found to be appearing in the last line of the parsing tree then the 
   given set of characters or string can be formed with the particular grammar
   or in other words the sentence belongs to the grammar.
******************************************************************************
*/
int main(int argc,char **argv)
{
	int i=0,j=0,k=0,m=0,y=0,t=0,s=0,r=0,n=0,d=0,p=0;
	int fd;
	ssize_t nread,nwrite;
	char **grammar;//production rules
	char *lhs;
	int no_prod_rules=0;
	char *string;
	string=(char*)malloc( sizeof(char) ); //input string
	int input_str_length=0,flag=0;
	char ***matrix;
	char ***temp;
	temp=(char*)malloc( MAX_STR_LENGTH*sizeof(char) );
	char *buf;
	buf=(char*)malloc(MAX_FILE_LENGTH*sizeof(char) );
	char* str_output;
//get user input without file handling
	/*printf("enter the no. of production rules\n");
	scanf("%d",&no_prod_rules);
	*/
printf("%s",argv[1]);

// get user input of grammar in the file
	for(i=0;i<MAX_FILE_LENGTH;i++)
	{
		buf[i]=NULL;
	}
	fd=open(argv[1],O_RDONLY);
	if(fd==-1)
		exit(1);
	nread=read(fd,buf,MAX_FILE_LENGTH);
	//close(fd);
	//counting the no. of lines
	i=0;
	while(buf[i]!=NULL)
	{
		if(buf[i]=='\n')
			no_prod_rules++;
		i++;
	}

//initialization of the sizes of grammar,temp,lhs array
	lhs=(char*)malloc(no_prod_rules*sizeof(char));
	grammar=(char**)malloc(no_prod_rules*sizeof(char*));
	temp=(char***)malloc(no_prod_rules*sizeof(char**));
	for(i=0;i<no_prod_rules;i++)
	{
		grammar[i]=(char*)malloc(30*sizeof(char));
		temp[i]=(char**)malloc(no_prod_rules*sizeof(char*));
	}

	for(i=0;i<no_prod_rules;i++)
	{
		for(j=0;j<no_prod_rules;j++)
		{
			temp[i][j]=(char*)malloc(3*sizeof(char));
		}
	}
//creating grammar array without file 
/*	for(i=0;i<no_prod_rules;i++)
	{
		printf("enter the %d rule:\n",i+1);
		scanf("%s",grammar[i]);
		lhs[i]=grammar[i][0];
		k=0;
		
	}*/	
//creating grammar array with file
	i=0;
	j=0;
	k=0;
	while(i<no_prod_rules)
	{
		j=0;		
		while(buf[k]!='\n')
		{
			grammar[i][j]=buf[k];
			j++;
			k++;			
		}
		lhs[i]=grammar[i][0];
		i++;
		k++;
	}
	for(i=0;i<no_prod_rules;i++)
	{
		printf("%s\n",grammar[i]);
	}

	printf("enter the string to be tested:");
	scanf("%s",string);
	input_str_length=strlen(string);
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

//cyk table basis
	for(i=0;i<input_str_length;i++) 
	{		
		for(j=0;j<no_prod_rules;j++)
		{
			for(k=2;k<strlen(grammar[j]);k++)
		{	
				if(string[i]==grammar[j][k])
				{
					matrix[i][i][m]=lhs[j];
					m++;
				}
			}	
	
		}
		m=0;
	}


	for(d=1;d<input_str_length;d++)	//d indicates the difference between i and j to be kept
	{
		for(i=0;i<input_str_length-d;i++)
		{
			j=i+d;
			m=0;			
			for(p=i,r=i+1,s=0,t=0;r<=j;r++,p++)
			{
				for(s=0;s<strlen(matrix[i][p]);s++)
				{
					for(t=0;t<strlen(matrix[r][j]);t++)
					{
						printf("%s\t%s\n",matrix[i][p],matrix[r][j]);
						temp[s][t][0]=matrix[i][p][s];
						temp[s][t][1]=matrix[r][j][t];
						printf("%s\n",temp[s][t]);
						printf("%d,%d,%d,%d,%d,%d\n",i,p,s,r,j,t);

						for(n=0;n<no_prod_rules;n++)
						{
							if(strstr(grammar[n],temp[s][t]))
							{
								flag=0;								
								for(y=0;y<m;y++)// for avoiding repitition in the particular box
								{
									if(matrix[i][j][y]==grammar[n][0])
									{

										flag=1;
										break;
									}
								}
								if(flag==0)
								{										
									matrix[i][j][m]=grammar[n][0];
									printf("%c\n",matrix[i][j][m]);

									m++;
								}
							}
						}
					}
				}
			}
			printf("\nmatrix[%d][%d]=%s\n",i,j,matrix[i][j]);
		}		
		
	}
//presenting output
	printf("%d",fd);
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
	str_output=(char*)malloc(strlen(matrix[0][input_str_length-1])*sizeof(char));
	printf("%d",strlen(matrix[0][input_str_length-1]));
	str_output=strcpy(str_output,matrix[0][input_str_length-1]);
	printf("%s",str_output);
	
}
