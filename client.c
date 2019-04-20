#include	"./lib/unp.h"
#include	"sum.h"
#include    "client.h"
void
cliDeal(FILE *fp, int sockfd)
{
  char			sendline[MAXLINE];
  struct args		args;
  struct result	result;

  while (Fgets(sendline, MAXLINE, fp) != NULL) {

    if (sscanf(sendline, "%ld%s", &args.arg1, &args.arg2) != 2) {
      printf("invalid input: %s", sendline);
	continue;
    }
    Writen(sockfd, &args, sizeof(args));

    if (Readn(sockfd, &result, sizeof(result)) == 0)
      err_quit("str_cli: server terminated prematurely");

    printf("%ld\n", result.sum);
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
