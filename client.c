#include	"./lib/unp.h"
#include	"sum.h"
#include    "client.h"



void
cliDeal(FILE *fp, int sockfd)
{
  char			sendline[MAXLINE];
  char			inputBuffer[MAXLINE - 4];
  struct args		args;
  struct result	result;
  char ouputBuffer[MAXLINE];
  bzero(ouputBuffer, sizeof(ouputBuffer));

  unsigned length = 0;
  while (Fgets(inputBuffer, MAXLINE, fp) != NULL) {
    bzero(sendline, sizeof(sendline));
    length = strlen(inputBuffer);
    char* Phead = sendline;
    char* Pfirst = NULL;
    *((unsigned*)Phead) = htons(length);
    strcpy(sendline+4, inputBuffer);
    Writen(sockfd, sendline, sizeof(sendline));
    //printf("send :%s", inputBuffer);
    if (Readn(sockfd, ouputBuffer, sizeof(ouputBuffer)) == 0)
	     err_quit("str_cli: server terminated prematurely");
    if (NULL != (strstr(sendline+4, "ls")))
      printf("%s\n", ouputBuffer);
    else
      {
	if (NULL != (Pfirst = strstr(sendline+4, "get"))) {
	  //printf("MMM%s", Pfirst+4);
	  //	  char * pfile1 = trim(Pfirst+4);
	  //	  printf("HHH%s", pfile1);
	  FILE *fp1 = NULL;
	  if ((fp1 = fopen(Pfirst+4, "w")) != NULL) {
	    int write_length = fwrite(ouputBuffer, sizeof(char), sizeof(ouputBuffer), fp1);
	    if (write_length < sizeof(ouputBuffer)) {  
	      printf("File:\t%s Write Failed!\n", Pfirst+4);  
	      break;  
	    }  
	  }
	}
      }
    bzero( ouputBuffer, sizeof(ouputBuffer));
  }
}

int
client(int argc, char *argv)
{
  int					sockfd;
  struct sockaddr_in	servaddr;

  if (argc != 2)
    err_quit("usage: tcpcli <IPaddress>");

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(20021);
  Inet_pton(AF_INET, argv, &servaddr.sin_addr);

  Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

  cliDeal(stdin, sockfd);		/* do it all */

  exit(0);
}
