#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const char* server_ip = "127.0.0.1";
const uint16_t SERVER_PORT = 2021;
const int QUEUE = 1024;
const int BUFFER_SIZE = 1024;

char* string_to_upper(char* str_orig){
	char* str_new;
	str_new = (char*) malloc(sizeof(char)*BUFFER_SIZE);
	int i;
	for(i = 0; str_orig[i] != '\0'; ++i){
		if('a' <= str_orig[i] && str_orig[i] <= 'z')
			str_new[i] = str_orig[i] - 32;
		else
			str_new[i] = str_orig[i];
	}
	return str_new;
}

int main(){
	//定义服务端套接字地址结构并赋值
	struct sockaddr_in server_sockaddr;
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_ip, &server_sockaddr.sin_addr);

	//创建一个监听套接字描述符
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0){
		perror("socket");
		exit(1);
	}

	//将套接字描述符绑定到套接字地址结构
	if(bind(listen_fd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) < 0){
		perror("bind");
		exit(1);
	}

	//开始监听
	if(listen(listen_fd, QUEUE) < 0){
		perror("listen");
		exit(1);
	}

	//接受连接请求
	int conn_fd = accept(listen_fd, NULL, NULL);
	if(conn_fd < 0){
		perror("accept");
		exit(1);
	}

	//开始数据传输
	char sendbuf[BUFFER_SIZE];
	char recvbuf[BUFFER_SIZE];
	while(1){
		memset(recvbuf, 0, sizeof(recvbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		
		recv(conn_fd, recvbuf, sizeof(recvbuf), 0);
		if(strcmp(recvbuf, "Q\n") == 0)
			break;
		fputs(recvbuf, stdout);
	//	sendbuf = string_to_upper(recvbuf);
		int i;
		for(i = 0; i < strlen(recvbuf); ++i){
			if(islower(recvbuf[i]))
				sendbuf[i] = toupper(recvbuf[i]);
			else
				sendbuf[i] = recvbuf[i];
		}
		send(conn_fd, sendbuf, strlen(sendbuf), 0);
		
	}

	//关闭连接及监听描述符
	close(conn_fd);
	close(listen_fd);
	return 0;
}
