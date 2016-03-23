#include"header.h"
/*arrange*/
void arrange(char* c)
{

	int i;
	/*removing spaces before the first letter of the command*/
	while(c[0]==' ')
	{
		strcpy(c,c+1);
	}
	/*removing extra-spaces*/
	for(i=0;c[i];i++)
	{
		if(c[i]==' ')
		{
			if(c[i+1]==' ')
			{
				strcpy(c+i+1,c+i+2);//if ps  -e-->ps -e
				i--;
			}
			else if(c[i+1]=='|' || c[i+1]==';' || c[i+1]=='>' || c[i+1]=='<')
			{
				strcpy(c+i,c+i+1);//if ps -e |grep pts-->ps -e|grep pts
				i--;
			}

		}
		else if(c[i]=='|' || c[i]==';' || c[i]=='>' || c[i]== '<')
		{
			if(c[i+1]==' ')
			{
				strcpy(c+i+1,c+i+2);//if ps -e| grep pts--> ps -e|grep pts
				i--;
			}

		}

	}

}

/*Function to check if command contains valid tokens(consisting not only of ' ') or not*/

int command_not_null(char *s)
{
	int i;
	for(i=0;s[i];i++)
	{
		if(s[i]!=' ')
			return 1;
	}
	return 0;

}
