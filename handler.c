#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "linkedList.h"

#define defPort 6080
#define MAXBUFSIZE 20
//int killConnection(struct node_t *node);
//int beginSockfiy(struct node_t *begin);
char **parseargs(char *buf, int *numargs);
/*
int main()
{
return 0;
}
*/
//int killConnection(int port, struct node_t *head)
int killConnection(struct node_t *node)
{
	//node_t *sig = remv(port, head);
	//printf("Killing Connection: %d .....\n",node->port);
	if(&node->pid == NULL || node->port < defPort)return -1;
	kill(node->pid, SIGTERM);
	//printf("KILLING PID: %d\n", node->pid);
	free(node);
	return 0;
}
int beginSockify(struct node_t *begin)
{
	//node_t *term = add(begin);
	printf("creating buffer\n");
	char buf[8196] = {0};
	//in the future I will be adding back in the support for sending script on start
	//to maintain
	sprintf(buf, "sh -c $PWD/testLaunch.sh 2>/dev/null &");
	//sprintf(buf, "sh -c '$PWD/noVNC-master/utils/novaLaunch.sh --vnc %s:%d --listen %d 2>/dev/null 2>/dev/null &'", begin->ip, begin->port, begin->port);
	//PROPER SYSTEM WAITING MUST BE ADDED
	int tempPid = fork();
	int numargs;

	if(tempPid == 0)
	{
		char **arglist = parseargs(buf,&numargs);
		printf("Forking Child Again\n");
		execvp(arglist[0],arglist);
		int j;
		for(j = 0; j < numargs; j++)
		{
			if(arglist[j]){free(arglist[j]);};
		}
		free(arglist);
		exit(0);
	}
	waitpid((pid_t)tempPid, NULL, 0);
	return 0;	
}
char **parseargs( char *buf, int *numargs )
{
	buf[strlen(buf)-1]= '\0';
	char *token;
        char **buffer = (char **) malloc((MAXBUFSIZE+1) * sizeof(char *));
	token = strtok(buf," ");
	int i=0;
	do
	{
		buffer[i] = (char *) malloc(strlen(token)+1);
		if(buffer[i]) strcpy(buffer[i],token);
		else
		{
			perror("no memory");
			exit(1);
		}
		token = strtok(NULL," ");
		i++;
	}while(token != NULL);
	*numargs = i;
	return buffer;
}
/*
void log()
{
}
*/
