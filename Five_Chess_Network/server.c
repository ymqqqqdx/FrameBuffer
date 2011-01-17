#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

char buffer[BUFFER_SIZE];
extern int who;
extern int ready;
extern char board[23 * 30];
extern int turn;
extern void draw_piece(fb_info fb,int x,int y,int r,u32_t color);
extern int check_all(fb_info fb);
int udp_server(fb_info fb)
{
	int len;
	int server_sock;
	socklen_t client_len;
	struct sockaddr_in server,client;
    //struct in_addr in;
    int x, y;
    int whom;

	if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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
	server.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("bind addr ok!\n");
		printf("\n");
	}

	while(1)
	{
		client_len = sizeof(client);
		len = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
        if(check_all(fb))
            exit(0);
		//if (len < 0)
		//{
			//close(server_sock);
			//fprintf(stderr, "%s\n", strerror(errno));
			//exit(EXIT_FAILURE);
		//}
		//else
		//{
			//printf("recvfrom client ok!\n");
			//in.s_addr = client.sin_addr.s_addr;
			//printf("client ip  : %s\n", inet_ntoa(in));
			//printf("client port: %d\n", ntohs(client.sin_port));
			//printf("\n");
		//}
		//if(buffer[0] == '.') break;
        printf("receive:%s\n",buffer);
        sscanf(buffer, "%1d %2d %2d", &whom, &x, &y);
        draw_piece(fb, x * 30 + OFFSET, y * 30 + 15, 13,(whom - 1) ? 0x00000000 : 0xffffffff);
        board[x + y * 23] = whom;
        turn = 1;
        if(check_all(fb))
            exit(0);
        while(!ready);
		sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client, client_len);
        printf("send:%s\n",buffer);
        ready = 0;
	}

	printf("Client close the socket\n");
	close(server_sock);
	exit(EXIT_SUCCESS);
}
