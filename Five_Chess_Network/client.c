#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#define SERVER_PORT		20001
#define BUFFER_SIZE		256

char buffer[BUFFER_SIZE] = {0};
extern int ready;
extern int who;
extern char board[23 * 30];
extern int turn;
extern void draw_piece(fb_info fb,int x,int y,int r,u32_t color);
extern int check_all(fb_info fb);
int udp_client(fb_info fb, char *argv)
{
	int n;
	int client_sock;
	socklen_t server_len;
	struct sockaddr_in server;
    int x, y;
    int whom;
	if(strlen(argv) < 8)
	{
		printf("\n");
		printf("Sorry, your type is wrong.\n");
		exit(EXIT_FAILURE);
	}
	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("create socket ok!\n");
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(argv);

	while(1)
	{
		while(!ready);
        ready = 0;
		server_len = sizeof(server);
        printf("send:%s\n",buffer);
		n = sendto(client_sock, buffer, 10, 0, (struct sockaddr *)&server, server_len);
		n = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
        printf("receive:%s\n",buffer);
        sscanf(buffer, "%1d %2d %2d", &whom, &x, &y);
        draw_piece(fb, x * 30 + 330, y * 30 + 15, 13,(whom - 1) ? 0x00000000 : 0xffffffff);
        board[x + y * 23] = whom;
        if(check_all(fb))
           exit(0);
        turn = 1;
	}
	close(client_sock);

	exit(EXIT_SUCCESS);
}
