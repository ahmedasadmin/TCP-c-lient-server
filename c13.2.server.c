/******* c13.2.server.c : TCP server.c file ********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAX   256
#define SERVER_HOST "localhost"
#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT  1234

struct sockaddr_in server_addr, client_addr;
int mysock, csock;         // socket descriptors
int r, len, n;             // help variabes 


int server_init(){
	
	printf("=================server init =================");
	printf("1: create a TCP stream scoket\n");
	mysock = socket(AF_INET, SOCK_STREAM, 0);
	if (mysock < 0){
		printf("socket call failed\n");
	}

	printf("2: fill server_addr with host IP and PORT# info\n");
	// initialize the server_addr structure 
	
	server_addr.sin_family = AF_INET; // for TCP/IP 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	
	printf("3: bind socket to server address\n");
	r = bind(mysock, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(r < 0){
	
		printf("bind failed\n"); return EXIT_FAILURE;

	}
	printf("  hostname = %s port = %d\n", SERVER_HOST, SERVER_PORT);
	
	printf("4: server is listening ....\n");
	listen(mysock, 5);     // queue LEN=5
	

	printf("=================== init done ===============");

}

int main(int argc, char **argv){
	
	char line[MAX];
	server_init();
	while(1){
		printf("server: accepting new connection ...\n");
		// Try to accept a client connection as descriptor newsock
		len = sizeof(client_addr);
		csock = accept(mysock, (struct sockaddr *)&client_addr, &len);
		if(csock < 0){
		
			printf("server: accept error\n");
			return EXIT_FAILURE;
		}
		printf("server: accept a client connection from\n");
		printf("----------------------------------------\n");
		printf("client: IP=%s  port=%d\n", 
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		
		printf("----------------------------------------\n");
		

		// processing loop: client_sock <== data ===> client
		while(1){
		
			n = read(csock, line, MAX);
			if (n == 0){
			
				printf("server: client died,s erver loops\n");
				close(csock);
				break;
			}
			// show the line string 
			printf("server: read n=%d bytes; line=%s\n", n, line);
			// echo line to client
			n = write(csock, line, MAX);
			printf("server: wrote n=%d bytes; ECHO= %s\n", n, line);
			printf("server: ready for next Quest\n");
		
		}
	}
}
