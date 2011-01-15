#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT		20000
#define BUFFER_SIZE		256

char buffer[BUFFER_SIZE];

int main(int argc, const char *argv[])
{
    int n,len;
    int client_sock;
    socklen_t server_len;
	struct sockaddr_in server;
	if(argc < 2)
	{
		printf("\n");
		printf("Sorry, your type is wrong.\n");
		printf("Usage: %s x.x.x.x(server IP)\n", argv[0]);
		printf("\n");
		exit(EXIT_FAILURE);
	}
    if((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        exit(1);
    }
    else
        printf("create socket ok!\n");
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server_len = sizeof(server);
	if(connect(client_sock, (struct sockaddr *)&server, server_len) < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
       printf("connect server ok!\n");
    while(1)
    {
        n = write(STDOUT_FILENO, "> ",3);//write to screen
        if((len = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
            n = send(client_sock, buffer, len, 0);
        if(buffer[0] == '.') break;
        if((len = recv(client_sock, buffer, len, 0)) > 0)
			n =	write(STDOUT_FILENO, buffer, len);
    }
    close(client_sock);
    return 0;
}
