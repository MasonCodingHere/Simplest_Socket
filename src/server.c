#include <sys/socket.h> //socket系列函数头文件
#include <arpa/inet.h> //sockaddr_in结构体、inet_pton函数、htons函数头文件
#include <string.h> //strlen函数、memset函数头文件
#include <unistd.h> //close函数头文件
#include <stdlib.h> //exit函数头文件
#include <stdio.h> //perror函数头文件
#include <ctype.h> //toupper函数头文件

const char* server_ip = "192.168.0.4"; //指定服务端IP地址
const uint16_t SERVER_PORT = 2023;//指定监听端口号
const int QUEUE = 1024; //用于listen函数第二个参数，指定内核应为相应套接字排队的最大连接数
const int BUFFER_SIZE = 1024;//指定缓冲区大小

int main(){
	//定义服务端套接字地址结构并赋值
	struct sockaddr_in server_sockaddr;
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET; //指定IPv4
	server_sockaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_ip, &server_sockaddr.sin_addr);

	//创建一个监听套接字描述符
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0); //指定TCP协议
	if(listen_fd < 0){
		perror("socket");
		exit(1);
	}

	//将监听套接字描述符绑定到套接字地址结构
	if(bind(listen_fd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) < 0){
		perror("bind");
		exit(1);
	}

	//开始监听
	if(listen(listen_fd, QUEUE) < 0){
		perror("listen");
		exit(1);
	}

	//接受连接请求，创建 已连接套接字描述符
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
		
		recv(conn_fd, recvbuf, sizeof(recvbuf), 0); //从conn_fd套接字接收数据，保存至recvbuf
		if(strcmp(recvbuf, "Q\n") == 0) //如果收到的数据是Q，表示退出
			break;
		fputs(recvbuf, stdout); //将收到的数据原样输出至stdout
	
		//将recvbuf中的小写字母转为大写，新数据保存至sendbuf
		for(int i = 0; i < strlen(recvbuf); ++i){
			if(islower(recvbuf[i]))
				sendbuf[i] = toupper(recvbuf[i]);
			else
				sendbuf[i] = recvbuf[i];
		}

		send(conn_fd, sendbuf, strlen(sendbuf), 0); //将sendbuf中的数据通过conn_fd套接字传输
		
	}

	//关闭连接及监听描述符
	close(conn_fd);
	close(listen_fd);
	return 0;
}
