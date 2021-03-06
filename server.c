#include   "./lib/unp.h"
#include   "sum.h"
//#include   "server.h"

char Serverbuffer[MAXLINE] = {0};
char TempBUffer[MAXLINE] = {0};
int substr(char dst[],char src[],int start,int len)
{
  char* p = src + start;   //定义指针变量指向需要提取的字符的地址
  int n = strlen(p);       //求字符串长度
  int i = 0;
  if(n < len)
    {
      len = n;
    }
  while(len != 0)
    {
      dst[i] = src[i+start];
      len --;
      i++;
    }                        //复制字符串到dst中
  dst[i] = '\0';
  printf("dst:%s", dst);
  return 0;
}

char *trim(char *str)
{
  char *p = str;
  char *p1;
  if(p)
    {
      p1 = p + strlen(str) - 1;
      while(*p && isspace(*p)) p++;
      while(p1 > p && isspace(*p1)) *p1-- = '\0';
    }
  return p;
}

char * dealCommand(int sockfd, char* command)
{
  char * pfirstGet = NULL;
  char buffer[MAXLINE] = {0};
  char SendBuffer[MAXLINE] = {0};
  FILE * fp = NULL;
  bzero(SendBuffer, sizeof(SendBuffer));
   if (NULL != strstr(command, "ls")) {
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
  
   } else if (NULL != (pfirstGet = strstr(command, "get"))){
     //     char *pZeroGet = strstr(command, "0");
     char * pfile = trim(pfirstGet+4);
     FILE *fp1 = fopen(pfile, "r");
     
     if (fp1 == NULL) {
       printf("no such file:%s Can Not Open To Write!\n", pfile);  
       exit(1);  
     }
 
     int i =0;
     while(fgets(buffer,sizeof(buffer),fp1) != NULL) {
       strcpy(&(SendBuffer[i]), buffer);
       i = i + strlen(buffer);
       if (i >=  MAXLINE)
	 return NULL;
       bzero(buffer, sizeof(buffer));
     }

     Writen(sockfd, SendBuffer, sizeof(SendBuffer));
     printf("%s", Serverbuffer);
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
      printf("length:%d", ntohs(length));

      //     substr(TempBUffer, Serverbuffer + 4, 0, length);
      //      printf("command:%s", Serverbuffer+3);
      //  for (int i = 0; i < length; i++)
      //      printf('%c:', *(Serverbuffer+3+i));
      //dealCommand
      printf("%s", TempBUffer);
      dealCommand(sockfd, Serverbuffer+4);

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
