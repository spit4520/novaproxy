/*
 * Prompting shell version 2
 *
 * Solves the 'one-shot' problem of version 1
 *    Uses execvp(), but fork()s first so that the
 *    shell waits around to perform another command
 * New problem: shell catches signals, Run vi, press ^C.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define  MAXARGS     20          /* cmdline args   */
#define  ARGLEN      100            /* token length   */
#define true 0
#define false 1
typedef int bool;
int pid,exitstatus;
bool run = true; 
int execute( char **arglist, int argnum, int background, int stdi, int stdo, int *pipe, int executeMode);
char **parseargs( char *buf, int *numargs );
void quithandler(int s);
int comparision(char *arglist, char comp);
int file_redir(char *argbuf, char redir);
int compare(char *arglist,char comp);
int *piping();
main()
{
	int beAPipe=0;
	int *pipe;
	char  *arglist[MAXARGS+1];    /* an array of ptrs  */
	int   numargs;                /* index into array  */
	char  argbuf[ARGLEN];         /* read stuff here   */
	char  *makestring();          /* malloc etc     */
	void quithandler(int);
	signal(SIGINT, quithandler);
	printf("Arg[%d]?", 0);
   while((fgets(argbuf, ARGLEN,stdin)!=NULL) && *argbuf != EOF && run == true)
   {
	//argbuf[strlen(argbuf) -1]='\0';
	int background = 0;
	int input_fd = 0;
	int output_fd = 0;
	printf("Arg[%d]? ", numargs);
      if ( *argbuf != '\n' )
     {
	if(compare(argbuf, '&'))
	{
		background = 1;
	}
	input_fd = file_redir(argbuf, '<');
	output_fd = file_redir(argbuf, '>');
	printf("%d",compare(argbuf, '|'));
	printf("%s",argbuf);
	if(strncmp(argbuf, "exit", 4 ) == 0)
	{
		printf("Should I Stay or Should I Go?\n");
		run = false;
	}
	if(beAPipe>0)
	{
		printf("debugging");
		*pipe = piping();
		char buffer2[ARGLEN];
		pid_t first = execute(parseargs(argbuf, &numargs),numargs,background,input_fd,output_fd,pipe,2);
		pid_t second = execute(parseargs(buffer2, &numargs), numargs, background,input_fd, output_fd, pipe,2);
		close(pipe[0]);
		close(pipe[1]);
		wait(0);
		wait(0);
	}
	else
	{
	execute(parseargs(argbuf,&numargs),numargs,background,input_fd,output_fd,pipe,3);
	}
      }
      else if ( numargs > 0 )
     {     /* any args?   */
	//printf("ENTERING ELSE IF STATEMENT\n");
         arglist[numargs]=NULL;    /* close list  */
         //execute( arglist );       /* do it */
         numargs = 0;              /* and reset   */
     }
   }
	printf("\n");
}
int file_redir(char *argbuf, char redir)
{
	int fd = 0;
	char *sec_str = NULL;
	char *tmp = NULL;
	char *val = NULL;

	sec_str = strchr(argbuf, (int)redir);

	if (sec_str)
	{
		argbuf[strlen(argbuf)-1]=0;
		val = malloc(1024);
		memset(val, 0, 1024);
		*sec_str = ' ';
		tmp = strtok(sec_str, " ");
		strcpy(val,tmp);

		memset(tmp, ' ', strlen(tmp)+1);
		
		if(redir == '<')
		{
			//read in
			fd = open(val, O_RDONLY);
		}
		if(redir == '>')
		{
			//printf("write out");
			fd = creat(val, 0644);
		}
		free(val);
	}
	return fd;
}
/**
char **pipe_parse(char *argbuf, char redir, int mode)
{
        int fd = 0;
        char *sec_str = NULL;
        char *tmp = NULL;
        char *val = NULL;
	char **buffer = (char **) malloc((MAXARGS+1) * sizeof(char *));
	
        sec_str = strchr(argbuf, (int)'|');

        if (sec_str)
        {
		int numargs =0;
		if(mode == 0)
		{
		argbuf[strlen(argbuf)-1]=0;
                val = malloc(1024);
                memset(val, 0, 1024);
		char *tempbuffer = (char *)malloc((sizeof(sec_str+1) * sizeof(char *)));
                *sec_str = 0;
		int index;
		index = (int) (strchr(argbuf, (int)'|') - *argbuf);
		memset(tempbuffer,sec_str,index);
                //tmp = strtok(sec_str, " ");
		buffer = parseargs(sec_str,&numargs);
                //strcpy(val,tmp);
                memset(tmp, ' ', strlen(tmp)+1);
                free(val);
		}
		if(mode == 1)
		{
		argbuf[strlen(argbuf)-1]=0;
                val = malloc(1024);
                memset(val, 0, 1024);
                *sec_str = ' ';
                //tmp = strtok(sec_str, " ");
		buffer = parseargs(sec_str,&numargs);
                //strcpy(val,tmp);
                memset(tmp, ' ', strlen(tmp)+1);
		//val is the command after the pipe
                free(val);
		}

        }
        return buffer;
}
**/
int execute( char **arglist,int argsize, int background, int stdi, int stdo, int *pipe, int executeMode)
{	
   pid = fork();              /* make new process */
   switch( pid )
   {
      case -1: 
         perror("fork failed");   
            exit(1);
      case 0:
	if(pipe)
	{
		if(executeMode == 1)
		{
			close(STDOUT_FILENO);
			dup(pipe[1]);
			close(pipe[0]);
			close(pipe[1]);
		}
		if(executeMode == 2)
		{
			close(STDIN_FILENO);
			dup(pipe[0]);
			close(pipe[1]);
			close(pipe[0]);
		}
	}
	if(stdi > 0)
	{
		close(0);
		dup2(stdi, 0);
	}
	if(stdo > 0)
	{
		close(1);
		dup2(stdo,1);
	}
	if(background)
	{
		setpgid(0,0);
	}
	execvp(arglist[0], arglist);
	perror("execvp failed"); 
            exit(1);
	if(stdi)close(stdi);
	if(stdo)close(stdo);
	int j;
	for( j = 0; j < argsize; j++)
	{
		if(arglist[j])
			free(arglist[j]);
	}
	free(arglist);
	arglist = NULL;

      default:
         while( wait(&exitstatus) != pid );
         //printf("child exited with status %d,%d\n",
           //    exitstatus >> 8, exitstatus & 0377);
   }
	return pid;
}
int compare(char *arglist, char comp)
{
	if(strchr(arglist, comp) == strrchr(arglist,comp) == strchrnul(arglist, comp))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}
char **parseargs( char *buf, int *numargs )
{
	buf[strlen(buf)-1]= '\0';
	char *token;
        char **buffer = (char **) malloc((MAXARGS+1) * sizeof(char *));
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
int *piping()
{
	int i;
	int *a = malloc(sizeof(int) *2);
	memset(a,0,sizeof(int)*2);
	i = pipe(a);
	return (i==0) ? a: NULL;
}
void quithandler(int s)
{
	if(pid == 0) return;
	kill(pid,SIGTERM);
	return;
}
