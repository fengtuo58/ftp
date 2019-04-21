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
    length = strlen(inputBuffer);
    char* Phead = sendline;
    *((unsigned*)Phead) = length;
    strcpy(sendline+length, inputBuffer);
    Writen(sockfd, sendline, sizeof(sendline));

    if (Readn(sockfd, ouputBuffer, sizeof(ouputBuffer)) == 0)
      err_quit("str_cli: server terminated prematurely");

    printf("%s\n", ouputBuffer);
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
