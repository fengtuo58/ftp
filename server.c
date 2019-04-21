#include   "./lib/unp.h"
#include   "sum.h"
//#include   "server.h"

char Serverbuffer[MAXLINE] = {0}; 
char * dealCommand(int sockfd, char* command)
{
    if (NULL != strstr(command, "ls")) {
      FILE * fp;
      char buffer[MAXLINE] = {0};
      char SendBuffer[MAXLINE] = {0};
      bzero(SendBuffer, sizeof(SendBuffer));
      if ((fp=popen("ls","r")) == NULL) {
	return NULL;
      }
      int i =0;
      while(fgets(buffer,sizeof(buffer),fp) != NULL) {
	strcpy(&(SendBuffer[i]), buffer);
	i = i + strlen(buffer);
	if (i >=  MAXLINE)
	  return NULL;
      }
      Writen(sockfd, SendBuffer, sizeof(SendBuffer));
      pclose(fp);
  
    } else if (NULL != strstr(command, "get")){

    }
}

void
ServerDeal(int sockfd)
{
  ssize_t		n;
  struct args		args;
  struct result	result;
  long length = 0;

  for ( ; ; ) {
    bzero(Serverbuffer, sizeof(Serverbuffer));
    if ( (n = Readn(sockfd, Serverbuffer, sizeof(Serverbuffer))) == 0) //找个阻塞性套接字
	    return;		/* connection closed by other end */
    
	  char* command = (char*)calloc(MAXLINE-4, sizeof(char));
	  if (command != 0) {
	    length = *(unsigned*)Serverbuffer;
      
	    printf("command:%s", Serverbuffer+3);
	    //dealCommand
	    dealCommand(sockfd, Serverbuffer + 3);

	    free(command);
	  }
  }
}


int
server(int argc, char **argv)
{
  int				listenfd, connfd;
  pid_t				childpid;
  socklen_t			clilen;
  struct sockaddr_in	cliaddr, servaddr;
  void				sig_chld(int);

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(20021);

  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  Signal(SIGCHLD, sig_chld);

  for ( ; ; ) {
    clilen = sizeof(cliaddr);
    if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
      if (errno == EINTR)
	continue;		/* back to for() */
      else
	err_sys("accept error");
    }

    if ( (childpid = Fork()) == 0) {	/* child process */
      Close(listenfd);	/* close listening socket */
      ServerDeal(connfd);	/* process the request */
      exit(0);
    }
    Close(connfd);			/* parent closes connected socket */
  }
}
