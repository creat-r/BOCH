b:main.o queue.o string_op.o exec.o
	cc -g -Wall main.o queue.o string_op.o exec.o -o b

main.o:main.c
	cc -g -Wall -c main.c

queue.o:queue.c
	cc -g -Wall -c queue.c

string_op.o:string_op.c
	cc -g -Wall -c string_op.c

exec.o:exec.c
	cc -g -Wall -c exec.c
