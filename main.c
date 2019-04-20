#include <stdio.h>
#include <getopt.h>
#include "server.h"
#include "client.h"
char *l_opt_arg;
char* const short_options = "hsc:";

struct option long_options[] = {
  { "help",     0,   NULL,    'h'     },
  { "server",  0,   NULL,    's'     },
  { "client",     1,   NULL,    'c'     },
  {      0,     0,     0,     0},
};

int main(int argc, char **argv)
{
int c;
while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
  {
switch (c)
  {
case 'h':
printf("-s or --server : it start as a server \n");
printf("-c or --client : it start as a client and need ip address \n");

break;
case 's':
printf("server starting\n");
server(0, 0);
break;
case 'c':
l_opt_arg = optarg; 
printf("client starting connecting to  %s!\n", l_opt_arg); 
client(2, l_opt_arg);
break;
}
}

return 0;
}
