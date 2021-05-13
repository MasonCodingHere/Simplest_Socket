#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const int BUFFER_SIZE = 1024;
const char* server_ip = "127.0.0.1";
const uint16_t SERVER_PORT = 2021;

int main(){
	//定义服务端套接字地址结构
	struct sockaddr_in server_sockaddr;
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_ip, &server_sockaddr.sin_addr);

	//创建客户端套接字描述符
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0){
		perror("socket");
		exit(1);
	}

	//发起连接
	if(connect(client_fd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) < 0){
		perror("connect");
		exit(1);
	}

	//开始数据传输
	char sendbuf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];
	while(fgets(sendbuf, sizeof(sendbuf), stdin)){
		if(strcmp(sendbuf, "Q\n") == 0)
			break;
		send(client_fd, sendbuf, strlen(sendbuf), 0);
		recv(client_fd, recvbuf, sizeof(recvbuf), 0);
		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}

	//关闭连接
	close(client_fd);
	return 0;
}
