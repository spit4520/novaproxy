#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<time.h>
#include	<strings.h>
#include	"linkedList.h"

#define   HOSTLEN  256

int createSocket(int portnum, int maxClient, int mode);
int connectToServer(char *host, int portnum);
struct node_t *recv_args(int socket);
//for mode if 0=client if 1=server
int createSocket(int portnum, int maxClient, int mode)
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct  hostent     *hostpointer;   /* this is part of our    */
	char    hostname[HOSTLEN];     /* address            */
	int sock_id;           /* the socket which will be returned*/
	
	sock_id = socket(PF_INET, SOCK_STREAM, 0);//establish the socket
	if ( sock_id == -1 ) 
		return -1;
	
	bzero((void *)&saddr, sizeof(saddr));//null contensts of struct
	//you only need to create a empty socket if you are the server.
	//if you are the client you need to look up the server in the
	//file table by host name.
	if(mode != 1)
	{
		gethostname(hostname, HOSTLEN);//look up host name and store
		hostpointer = gethostbyname(hostname);
		bcopy( (void *)hostpointer->h_addr, (void *)&saddr.sin_addr, hostpointer->h_length);
        }
	else
	{
		saddr.sin_addr.s_addr = INADDR_ANY;
	}
	//give the socket good stuff
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AF_INET;
	
	//verify that everything worked and we are all good.
	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )return -1;
	if ( listen(sock_id, maxClient) != 0 ) 
		return -1;
	return sock_id;
}
/**
*used to create a new socket that is connected to the desired
*host and port number.
*Input: char *host  -> pointer to host
*Input: int portnum -> port number to connect to
*Output: int -> return socket opened and connected to server
*Error: return -1 on error.
**/
int connectToServer(char *host, int portnum)
{
	int sock;
	struct sockaddr_in  servadd;        /* the number to call */
	struct hostent      *hostpointer;            /* used to get number */

	/** Step 1: Get a socket **/
	sock = socket( AF_INET, SOCK_STREAM, 0 );    /* create client sock*/
	if ( sock == -1 )return -1;

	/** Step 2: connect to server **/

	bzero( &servadd, sizeof(servadd) );     /* zero the address     */
	hostpointer = gethostbyname( host );             /* lookup host's ip #   */
	if (hostpointer == NULL) 
		return -1;//invalid host

	bcopy(hostpointer->h_addr, (struct sockaddr *)&servadd.sin_addr, hostpointer->h_length);
	servadd.sin_port = htons(portnum);      /* fill in port number  */
	servadd.sin_family = AF_INET;          /* fill in socket type  */

 	if ( connect(sock,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
		return -1;
		//conection is established return connected pipe
	//free(hostpointer);
	return sock;
}
/*
*used for all communication over the socket
*/
struct node_t *recv_args(int socket)
{
	struct node_t *recvd_args = malloc(sizeof(struct node_t));
	bzero(recvd_args, sizeof(struct node_t));
	recv(socket, recvd_args, sizeof(struct node_t),0);
	printf("CMD: %d\n", recvd_args->cmd_mode);
	return recvd_args;
}
