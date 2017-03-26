#include <sys/types.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "socks.h"
#include "linkedList.h"
#define PORTNUM 8799


int main()
{
	int fd; //descriptor of server connection
	struct node_t *exp = malloc(sizeof(struct node_t));
	//exp = {0};
	fd = connectToServer("127.0.0.1", PORTNUM);
	if(fd == -1) 
	{
		perror("connect_to_server\n");
		printf("Make sure server is running..\n");
		exit(EXIT_FAILURE);
    	}
	else
	{
		printf("Connection Established?\n");
	}
	printf("Would you like to add or remove to the stack?\n enter 1 to kill a process enter 0 to add a process\n enter 2 to command the server to print the process stack\n");
	scanf("%d", &(exp->cmd_mode));
	printf("command mode: %d", exp->cmd_mode);
	if(exp->cmd_mode == 1)
	{
		printf("Please enter the value of the port to kill: \n");
		scanf("%d", &(exp->port));
		
		printf("sending\n");
		send(fd, exp, sizeof(struct node_t),0);
		printf("sent.\n");
		exp = recv_args(fd);
		printf("recieved response\n");
		printf("removed node: %d\n, PID value: %d",exp->port, exp->pid);
	}
	else if(exp->cmd_mode == 0)
	{
		printf("Please enter internal port value: ");
		scanf("%d", &(exp->int_port));
		
		printf("\nsending\n");
		send(fd, exp, sizeof(struct node_t),0);
		printf("sent.\n");
		exp = recv_args(fd);
		//printf("advanced past receving args\n");

		printf("Successfully Added Port value: %d\n PID Value: %d\n", exp->port, exp->pid);
	}
	else if(exp->cmd_mode == 2)
	{
		printf("Retrieving List...\n");
		send(fd,exp, sizeof(struct node_t),0);
		//printf("Port: %d Pid: %d",exp->port, exp->pid);
		//printLst(exp);
		//printf("Finished.. Exiting\n");
	}
	else if(exp->cmd_mode == 3)
	{
		printf("Killing server...\n");
		send(fd,exp, sizeof(struct node_t),0);
	}else
	{
		printf("Boo Bad\n Invalid Input");
	}
	//exp->int_port = 6;
	//exp->cmd_mode = 0;
	//exp->int_port = 6080;
	//exp->cmd_mode = 1;
	//if(exp)
	//{
	//	printf("responce recieved!\n");
	//}
	//int *temp;
	//printLstH(decode_q(exp, temp));
	close(fd);
	printf("exiting...");
}
