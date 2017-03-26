#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "linkedList.h"
#include "socks.h"
#include "handler.h"
#include <string.h>
//#include "transport.h"
#define PROXYPORT 8799

void process(int mode, struct node_t *current_node,int tsocket);
//int execute(int socket, struct node_t *head);
//struct node_t *decode(struct cnode_t *src, int *mode);
//struct cnode_t *encode(struct node_t *src, int mode);

int main()
{
        int sock_id;
        if((sock_id = createSocket(PROXYPORT,128,1)) == -1)
        {
                perror("Error opening socket");
                exit(1);
        }
	
	struct node_t *head = malloc(sizeof(struct node_t));
	head->port = 6079;
//	add(head, NULL);
//	printf("%d%s",head->port,head->next);
//	while(1);
        while(1)
        {
                int clientfd;

                clientfd = accept(sock_id, NULL, NULL);
                if (clientfd == -1)
                {
                        perror("client failed to connect?\n");
                        return -1;
                }
		struct node_t *args = recv_args(clientfd);
		//printf("COMMAND ENTERED: %d", args->cmd_mode);
		if(args->cmd_mode == 1)//kill connection
		{
			struct node_t *sig = remv(args->port, head);
			//if(killConnection(sig)==-1)
			//{printf("Failed to end connection\n"); args->error = -1;
			//printf("Port to be killed: %d\n",sig->port);
			process(0, sig, clientfd);
		}
		else if(args-> cmd_mode == 0)//create connection
		{
			//struct node_t *sig = remv(args->port, head);
			printf("Command Mode: 0 \nCommand Mode is: %d", args->cmd_mode);
			int success = add(args, head);
			if(success != 0)
			{printf("Faailed to begin connection\n"); args->error = -1;}
			process(1,args,clientfd);
			//wait(0);
			FILE *file;
			file = fopen("output.txt","r");
			fscanf(file, "%d", &args->pid);
			printf("PID OF NEW PROCESS: %d\n",args->pid);
			printf("Node After Head: %d\nNode After Head Pid: %d\n",head->next->port,head->next->pid);	
			//wait(NULL);
			printf("Value of pid from parent in server: %d \nPort: %d\n",args->pid,args->port);
		}
		else if(args-> cmd_mode == 2)
		{
			//return head for printing in client
			printf("Head: %d", head->port);
			//process(2,head,clientfd);
			//struct node_t *transmit = malloc(sizeof(struct node_t));
			//memcpy(transmit, head, sizeof(struct node_t));
			send(clientfd,head,sizeof(struct node_t),0);
			//free(transmit);
			printLst(head);
		}
		else if(args-> cmd_mode == 3)
		{
			printf("good bye my sweet love :'(\n dont forget me...");
			exit(0);
		}
		//printf("Forking and Entering...\n");
                //process(clientfd, head);
		printf("Returned from process\n");
		sleep(1);
		printf("awoken from sleen\n");
        }
}


void process(int mode, struct node_t *current_node, int tsocket)
{
	//if you are the child then execute
	//once the child completes execute
	//the child will exit
	//int *socket = malloc(sizeof(int));
	//memcpy(socket,tsocket,sizeof(int));
	pid_t pid= fork();
	if(pid == 0)
	{
		if(mode == 0)
		{
			killConnection(current_node);
			//printf("Completed kill Connection \n");
			int success = send(tsocket,current_node, sizeof(struct node_t),0);
			if(success != -1)
			{
				printf("Transmit successful\n");
				printf("transmit and kill successful freeing current node\n");
				//free(current_node);
			}	
		}
		else if(mode == 1)
		{
			beginSockify(current_node);
			int success = send(tsocket,current_node, sizeof(struct node_t),0);
			if(success != -1)
			{
				printf("Transmit successful\n");
			}
			else
			{
				printf("Failed to transmit freeing current node\n");
				free(current_node);
			}
		}
		else if(mode == 2)
		{
			int success = send(tsocket, current_node,sizeof(struct node_t),0);
			if(success != -1)
			{
				printf("Transmitted head exiting.\n");
			}
		}
		//int success = send(tsocket,args,sizeof(struct node_t),0);
		close(tsocket);
		exit(0);
	}
	else
	{
		printf("Parent should be waiting\n");
	//	waitpid(pid,NULL,0);
		wait(NULL);
		printf("returning from wait\n");
	}
}
