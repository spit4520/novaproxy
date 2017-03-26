#define   HOSTLEN  256

int createSocket(int portnum, int maxClient, int mode);
int connectToServer(char *host, int portnum);
struct node_t *recv_args(int socket);
