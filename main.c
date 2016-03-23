/*
 *
 *	description:
 *
 *	author: Chaitanya Borah
 *
 *	creation time: Sat Feb 27 18:35:11 2016
 *
 *
 */


#include"sh_header.h"

unsigned int count=0;
Q_NODE *hptr=0;/*Queue headpointer*/
FLAG f;/*flags*/


int main(int argc, char** argv)
{
//	signal(3,SIG_IGN);
	signal(2,SIG_IGN);
	char hostname[30];
	/*retrieving computer name*/
	if(gethostname(hostname,29)<0)		
	{
		perror("gethostname");
	}

	//if user doesn't provide appropriate input
	if(argc<3 && argc>1)
	{
		printf("Usage:boch -c <command> or boch\n");
		return 1;
	}

	/*single command mode */
	if(argc>=3)
	{
		/*check if appropriate option is selected*/
		if(strcmp(argv[1],"-c"))
		{
			printf("boch:illegal option %s\n",argv[1]);
			return 1;
		}
	//	execute(argv[2]);		
		execvp(argv[2],argv+2);
	}

	/*****************command prompt mode******************************/
	else
	{
		/*Declarations*/
		char command[130];/*character array to store command*/
		int i,j;
		f.exit_flag=1;
		while(f.exit_flag)
		{

			/**********Displaying cwd and taking in command*********/

			/*printing present working directory*/

			getcwd(command,sizeof(command));
			printf("\033[38;5;45mcwd:%s\033[39m\n",command);


			/*taking input from user*/
			printf("\033[38;5;196m<%d %s>\033[39m",++count,hostname);
			fgets(command,129,stdin);
			command[strlen(command)-1]=0;/*omitting \n from string*/
			/*******************Maintining the history queueu*****************************/

			/*if count>0 dequeue*/
			if(count_node(hptr)>10)
			{
				dequeue(&hptr);
			}

			/*enqueue new command*/
			if(command_not_null(command))
			{
				enqueue(&hptr,command);
			}
			else
			{
				continue;
			}


			/*************execution of executables*******************/			
			/*remove unnecessary characters from 'command'*/
			arrange(command);

			/*execute command*/
			char cmd[130];
			strcpy(cmd,command);
			for(i=0;command[i];i++)
			{
				if(command[i]==';')
					break;
			}
			cmd[i]=0;
			execute(cmd);
			for(i=0;command[i];i++)//executing the ; separated commads
			{
				if(command[i]==';')
				{	
					strcpy(cmd,command+i+1);
					for(j=0;cmd[j];j++)
					{
						if(cmd[j]==';')
						{
							cmd[j]=0;	
							break;
						}
					}
					execute(cmd);
				}
			}

		}

	}
	return 0;
}

