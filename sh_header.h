#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
typedef struct flag{
	uint8_t exit_flag:1;/*flag to check exit status of shell; 0:indicates exit*/
	uint8_t unused1:1;
	uint8_t unused2:1;
	uint8_t unused3:1;
	uint8_t unused4:1;
	uint8_t unused5:1;
	uint8_t unused6:1;
	uint8_t unused7:1;
}FLAG;

typedef struct q_node{
	unsigned int serial_no;
	char stored_command[130];
	struct q_node* next;
}Q_NODE;

/*Queue Operation <queue.c>*/
void enqueue(Q_NODE **,char* );
void dequeue(Q_NODE **);
void show_command_history(Q_NODE *);
int count_node(Q_NODE *);

/*string based operations<string_op.c>*/
int command_not_null(char*);
void arrange(char*);

/*execution*/
void execute(char*);
int execute_command(char*);
