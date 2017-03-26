#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"
#include <string.h>
//#define IP_SIZE 128
//#define USER_SIZE 256
#define DEFPORT 6080

//node_t *head = NULL;//malloc(sizeof(struct node_t));
//node_t *tail = NULL;

int add(struct node_t *node, struct node_t *head)
{
	//head = ndhead;
	//should never reach here only for emergencies
	node_t * current = head;
	if(head == NULL)
	{
		head = node;
		node->port = DEFPORT;
		return 0;
	}
	
	while (current->next != NULL && current->next->port == current->port + 1) {
		current = current->next;
	}

	node->port = current->port + 1;

	if (current->next == NULL)  // end fo the list
	{
		//its safe to assume the node is already on the heap
		current->next=node;
		node->previous=current;
		node->next=NULL;
		node->pid = current->pid +1;	
	}
	
	// node goes in between two other nodes, so you have to insert it
	if (current->port + 1 != current->next->port) {
		// set up node
		
		node->previous = current;
		node->next = current->next;
		
		// set up node->next
		current->next->previous = node;
		current->next = node;
		node->port = current->port+1;
		return 0;
	}

	return 0;
}

node_t *remv(int port, struct node_t *head)
{
	//head = nhead;
	node_t *current = head;
	struct node_t *temp;
	while (current->next != NULL) {
		if(current->port == port)break;
		current = current->next;
	}
	if(current->next == NULL && current->port == port)
	{
		temp=malloc(sizeof(struct node_t));
		memcpy(temp,current, sizeof(struct node_t));
		current->previous->next = NULL;
		free(current);
		return temp;
	}
	
	else if (current->port == port) {
	
		current->next->previous = current->previous;	
		current->next->previous->next = current->next;
	}
	else 
	{
		printf("error\n");
		current->error = -1;
		return current;
	}
	return current;
}

node_t *lookup(int port, struct node_t *head)
{
	node_t * current = head;

	while(current->next != NULL && current->port != port)
	{
		current = current->next;
	}

	return (current != NULL && current->port == port) ? current : NULL;
}

void printLst(struct node_t *head)
{
	node_t * current = head;

	while(current != NULL)
	{
		printf("port: %d \n pid: %d \n next: %s \n", current->port, current->pid, current-> next);
		current = current->next;
	}
}
