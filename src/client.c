#include <sys/socket.h> //socket系列函数头文件
#include <arpa/inet.h> //sockaddr_in结构体、inet_pton函数和htons函数头文件
#include <string.h> //strlen函数、memset函数头文件
#include <unistd.h> //close函数头文件
#include <stdlib.h> //exit函数头文件
#include <stdio.h> //fgets函数、fputs函数、perror函数头文件

const int BUFFER_SIZE = 1024; //定义缓冲区大小
const char* server_ip = "127.0.0.1"; //服务端IP地址
const uint16_t SERVER_PORT = 2021; //服务端监听端口号

int main(){
	//定义服务端套接字地址结构
	struct sockaddr_in server_sockaddr;
	memset(&server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET; //指定IPv4
	server_sockaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_ip, &server_sockaddr.sin_addr);

	//创建客户端套接字描述符
	int client_fd = socket(AF_INET, SOCK_STREAM, 0); //指定TCP协议
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
	while(fgets(sendbuf, sizeof(sendbuf), stdin)){ //从stdin读入待传输数据至sendbuf
		send(client_fd, sendbuf, strlen(sendbuf), 0); //将sendbuf中的数据通过client_fd套接字传输
		
		if(strcmp(sendbuf, "Q\n") == 0) //输入Q表示退出
			break;
		//send(client_fd, sendbuf, strlen(sendbuf), 0); //将sendbuf中的数据通过client_fd套接字传输
		recv(client_fd, recvbuf, sizeof(recvbuf), 0); //从client_fd套接字接收数据，保存至recvbuf
		fputs(recvbuf, stdout);//将recvbuf中的数据输出至stdout
		memset(sendbuf, 0, sizeof(sendbuf));//将sendbuf的内存值置0
		memset(recvbuf, 0, sizeof(recvbuf));//将recvbuf的内存值置0
	}

	//关闭连接
	close(client_fd);
	return 0;
}
