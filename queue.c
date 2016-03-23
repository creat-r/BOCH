/*
 *
 *	description:
 *
 *	author: Chaitanya Borah
 *
 *	creation time: Sat Feb 27 19:56:35 2016
 *
 *
 */
#include"sh_header.h"

extern unsigned int count;
void enqueue(Q_NODE **hptr, char *cmd)
{
	Q_NODE *temp=malloc(sizeof(Q_NODE)),*q=*hptr;
	
	/*Filling up the members*/
	temp->serial_no=count;
	strcpy(temp->stored_command,cmd);
	
	/*If queue is empty*/
	if(*hptr==0)
	{
		*hptr=temp;
		temp->next=0;
	}
	/*moves towards the end and add node*/
	else
	{
		while(q->next)
		{
			q=q->next;
		}
		q->next=temp;
		temp->next=0;
	}

}


void dequeue(Q_NODE** hptr)
{
	Q_NODE* temp=*hptr;
	*hptr=(*hptr)->next;
	free(temp);

}

void show_command_history(Q_NODE* p)
{
	if(p==0)
	{
		printf("hist: No history\n");
	}
	while(p)
	{
		printf("%d: %s\n",p->serial_no,p->stored_command);
		p=p->next;
	}

}

int count_node(Q_NODE *p)
{
	unsigned int c=0;
	while(p)
	{
		p=p->next;
		c++;
	}
	return c;
}
