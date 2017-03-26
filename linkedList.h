#define IP_SIZE 128
#define USER_SIZE 256
#define DEFPORT 6080

typedef struct node_t
{
	pid_t pid;
	int port;
	int int_port;
	char ip[IP_SIZE];
	//char user[USER_SIZE];
	struct node_t * previous;
	struct node_t * next;
	int cmd_mode;
	int error;
} node_t;

int add(struct node_t *node, struct node_t *head);
node_t *remv(int port, struct node_t *head);
node_t *lookup(int port, struct node_t *head);
void printLst(struct node_t *head);
void print(struct node_t node);
