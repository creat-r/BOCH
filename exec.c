/*
 *
 *	description:
 *
 *	author: Chaitanya Borah
 *
 *	creation time: Sun Feb 28 12:04:50 2016
 *
 *
 */

#include"sh_header.h"

extern FLAG f;
extern Q_NODE* hptr;
void execute(char* command)
{

	//int i;
	/*if command is quit*/
	if(!strcmp(command,"quit"))
	{
		f.exit_flag=0;         
	}
	/*if command is hist*/
	else if(!strcmp(command,"hist"))
	{
		show_command_history(hptr);
	}
	/*if command is curPid*/
	else if(!strcmp(command,"curPid"))
	{
		printf("  %d\n",getpid());
	}
	/*if command is pPid*/
	else if(!strcmp(command,"pPid"))
	{
		printf("  %d\n",getppid());
	}
	/*if command is cd*/
	else if(strstr(command,"cd")==command)
	{	
		char *dir;/*pointer to directory name string*/
		//for(i=0;command[i]!=' ';i++);
		char path[130]="./";
		dir=command+3;
	
		if(command[3]=='.')
		{
			if(chdir(dir)==-1)
			{
				perror("chdir");
			}
		}
		else if(command[2]==0 || command[3]==0)
		{
			strcpy(path,"/home/");
			strcat(path,getlogin());
			if(chdir(path))//make generic
			{
				perror("chdir");
			}
		}
		else
		{
			strcat(path,dir);
			if(chdir(path)==-1)
			{
				perror("chdir");
			}
		}

	}
	else if(strchr(command,'|'))
	{
		char **pipe_end;
		int pipe_count=0,i,j,k=0;
		for(i=0;command[i];i++)
		{
			if(command[i]=='|')
			{
				pipe_count++;//counting the number of pipes
			}
		}
		pipe_end=malloc(sizeof(char*)*(pipe_count+1));//number of pipe ends will be twice the number of pipes

		for(i=0;i<pipe_count+1;i++)
		{
			pipe_end[i]=malloc(130);//assigning dynamic memory for each pipe end's command
		}

		strcpy(pipe_end[0],command);//first end of the first pipe
		for(i=0;pipe_end[0][i]!='|';i++);
		pipe_end[0][i]=0;

		for(i=0,j=1;command[i];i++)//filling rest of the pipe end commands
		{
			if(command[i]=='|')
			{
				strcpy(pipe_end[j],command+i+1);
				for(k=0;pipe_end[j][k];k++)
				{
					if(pipe_end[j][k]=='|')
					{
						pipe_end[j++][k]=0;
						break;
					}
				}
				i=i+k;

			}
		}
		pipe_end[j+1]=0;	
		/*
		   for(i=0;pipe_end[i];i++)
		   {
		   printf("%s\n",pipe_end[i]);
		   }
		 */	
		int pipe_fd[2];/*file descriptors for pipes*/
		if(fork()==0)/*create a child to execute the commands sequentially*/
		{
		
			for(i=0;i<pipe_count+1;i++)/*loop for creation of files and execution*/
			{
				if(i!=0)/*For all processes/commands except the first one*/
				{
					if(dup2(pipe_fd[0],0)<0)/*Connecting the read end to stdin*/
					{
						perror("dup2");
					}
					if(close(pipe_fd[1])<0)/*Closing the addition pipe fd*/
					{
						perror("close");
					}


				}
				if(i<pipe_count)/*For all processes/command except the last one*/
				{
					pipe(pipe_fd);/*create a new pipe to pass on its output to next child*/
					if(fork())
					{

						if(close(pipe_fd[0])<0)/*closing the additional pipe fd*/
						{
							perror("close");
						}
						if(dup2(pipe_fd[1],1)<0)/*connecting the write end to stdout fd*/
						{
							perror("dup2");
						}
								
					//	if(fork()==0)
					//	{
							execute_command(pipe_end[i]);
					//	}

					}
				}
				else/*Only for the last command*/
				{

					execute_command(pipe_end[i]);
				}
			}
		}
		else
		{
			while(waitpid(-1,0,0)!=-1);
		}
	}
	/*
	else if(strstr(command,">") || strstr(command,"<"))
	{
		if(fork()==0)
		{
			int i,input_fd,output_fd;
			char cmd[130];
			char inputfilename[130];
			char outputfilename[130];
			char tempbuf[130]="./";//buffer for swap if required
			inputfilename[0]=0;//initialising input file name buffer with null to recognise an unfilled buffer
			outputfilename[0]=0;
			strcpy(cmd,command);
			for(i=0;command[i];i++)//last occurrence
			{
				if(command[i]=='>')
				{
					strcpy(outputfilename,command+i+1);
					cmd[i]=0;
				}
				if(command[i]=='<')
				{
					strcpy(inputfilename,command+i+1);
					cmd[i]=0;
				}
			}
			refine the names
			for(i=0;outputfilename[i];i++)
			{
				if(outputfilename[i]=='<')
					outputfilename[i]=0;
			}
			for(i=0;inputfilename[i];i++)
			{
				if(inputfilename[i]=='>')
					inputfilename[i]=0;
			}
			if(inputfilename[0])
			{
				if(inputfilename[0]=='.' || inputfilename[0]=='/')//if path is given
				{
					if((input_fd=open(inputfilename,O_RDONLY))<0)
					{
						perror("open(redirection)");
					}
				}
				else
				{
					strcat(tempbuf,inputfilename);
					if((input_fd=open(tempbuf,O_RDONLY))<0)
					{
						perror("open(redirection)");
					}
				}
				dup2(input_fd,0);
			}
			strcpy(tempbuf,"./");//reinitialisation of temp_buf
			if(outputfilename[0])
			{
				if(outputfilename[0]=='.' || outputfilename[0]=='/')//if path is given
				{
					if((output_fd=open(outputfilename,O_WRONLY|O_TRUNC|O_CREAT,0700))<0)
					{
						perror("open(redirection)");
					}
				}
				else
				{
					strcat(tempbuf,outputfilename);
					if((output_fd=open(tempbuf,O_WRONLY|O_TRUNC|O_CREAT,0700))<0)
					{
						perror("open(redirection)");
					}
				}
				dup2(output_fd,1);


			}
			execute_command(cmd);
		}
		else
		{
			wait(0);
		}

	}*/
	else/*executable command*/
	{
		if(!fork())
		{
			exit(execute_command(command));
		}
		else
		{
			wait(0);
		}
	}	


}
int execute_command(char *command)
{
	signal(2,SIG_DFL);
                
        char cmd[130];
        strcpy(cmd,command);
        if(strstr(command,">") || strstr(command,"<"))
        {
                        int i,input_fd,output_fd;
                        char inputfilename[130];
                        char outputfilename[130];
                        char tempbuf[130]="./";//buffer for swap if required
                        inputfilename[0]=0;//initialising input file name buffer with null to recognise an unfilled buffer
                        outputfilename[0]=0;
                        for(i=0;command[i];i++)/*last occurrence*/
                        {
                                if(command[i]=='>')
                                {
                                        strcpy(outputfilename,command+i+1);
                                        cmd[i]=0;
                                }
                                if(command[i]=='<')
                                {
                                        strcpy(inputfilename,command+i+1);
                                        cmd[i]=0;
                                }
                        }
                        /*refine the names*/
                        for(i=0;outputfilename[i];i++)
                        {
                                if(outputfilename[i]=='<')
                                        outputfilename[i]=0;
                        }
                        for(i=0;inputfilename[i];i++)
                        {
                                if(inputfilename[i]=='>')
                                        inputfilename[i]=0;
                        }
                        //printf("%s |%s |%s\n",cmd,inputfilename,outputfilename);

                        if(inputfilename[0])
                        {
                                if(inputfilename[0]=='.' || inputfilename[0]=='/')//if path is given
                                {
                                        if((input_fd=open(inputfilename,O_RDONLY))<0)
                                        {
                                                perror("open(redirection)");
                                        }
                                }
                                else
                                {
                                        strcat(tempbuf,inputfilename);
                                        if((input_fd=open(tempbuf,O_RDONLY))<0)
                                        {
                                                perror("open(redirection)");
                                        }
                                }
                                dup2(input_fd,0);
                        }
                        strcpy(tempbuf,"./");//reinitialisation of temp_buf
                        if(outputfilename[0])
                        {
                                if(outputfilename[0]=='.' || outputfilename[0]=='/')//if path is given
                                {
                                        if((output_fd=open(outputfilename,O_WRONLY|O_TRUNC|O_CREAT,0666))<0)
                                        {
                                                perror("open(redirection)");
                                        }
                                }
                                else
                                {
                                        strcat(tempbuf,outputfilename);
                                        if((output_fd=open(tempbuf,O_WRONLY|O_TRUNC|O_CREAT,0666))<0)
                                        {
                                                perror("open(redirection)");
                                        }
                                }
                                dup2(output_fd,1);


                        }

                                        
        }
/*
        write(2,cmd,strlen(cmd));
        write(2,"\n",1);
*/
        unsigned int argcount=0;
        int i,j;
        char **args;
        //char cmd[130];
        //strcpy(cmd,command);

        for(i=0;cmd[i];i++)
        {
                if(cmd[i]==' ')
                        argcount++;     //Counting the number of arguments
        }
        args=malloc(sizeof(char*)*(argcount+5));

        //assigning address of first argument
        args[0]=cmd;

        //assigning rest of the arguments
        for(i=0,j=1;cmd[i];i++)
        {
                if(cmd[i]==' ')
                {
                        cmd[i]=0;
                        args[j++]=cmd+i+1;
                        
                }

        }
        //}
        args[j]=0;

        /*for(i=0;args[i];i++)
        {
                printf("%s\n",args[i]);
        }
        */
        if(execvp(args[0],args)==-1)
        {
                        perror("exec");
                        write(2,"Error:\033[0;38m",14);
                        write(2,args[0],strlen(args[0]));
                        write(2,"\033[0;39m:No such command\n",25);
                        return 1;
        }
        return 0;
}

/*
int execute_command(char *command)
{
	unsigned int argcount=0;
	int i,j;
	char **args;
	char cmd[130];
	strcpy(cmd,command);

	for(i=0;command[i];i++)
	{
		if(command[i]==' ')
			argcount++;	//Counting the number of arguments
	}
	args=malloc(sizeof(char*)*(argcount+5));

	//assigning address of first argument
	args[0]=command;

	//assigning rest of the arguments
	for(i=0,j=1;command[i];i++)
	{
		if(command[i]==' ')
		{
			command[i]=0;
			args[j++]=command+i+1;
			
		}

	}
	//}
	args[j]=0;

//	for(i=0;args[i];i++)
//	{
//		printf("%s\n",args[i]);
//	}
	
	if(execvp(args[0],args)==-1)
	{
			printf("Error:");
			printf("\033[38;5;196m%s\033[39m",args[0]);
			printf(":No such command\n");
			return 1;
	}
	return 0;
}*/
