#include <sys/socket.h> //socket系列函数头文件
#include <arpa/inet.h> //sockaddr_in结构体、inet_pton函数、htons函数头文件
#include <string.h> //strlen函数、memset函数头文件
#include <unistd.h> //close函数头文件
#include <stdlib.h> //exit函数头文件
#include <stdio.h> //perror函数头文件
#include <ctype.h> //toupper函数头文件
#include <sys/epoll.h> //epoll系列函数头文件

const char* server_ip = "127.0.0.1"; //指定服务端IP地址
const uint16_t SERVER_PORT = 2021;//指定监听端口号
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

    //端口复用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//将监听套接字描述符绑定到套接字地址结构
	if(bind(listen_fd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) < 0){
		perror("bind");
		exit(1);
	}

	//将listen_fd由主动套接字转换为被动套接字，并指定服务端同一时刻所能接受客户端连接请求的个数QUEUE
	if(listen(listen_fd, QUEUE) < 0){
		perror("listen");
		exit(1);
	}

    //创建一个epoll实例
    int epfd = epoll_create(100);
    if(epfd == -1){
        perror("epoll_create");
        exit(1);
    }

    //往epoll实例中添加需要检测的文件描述符, 现在只有listen_fd
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1){
        perror("epoll_ctl");
        exit(1);
    }

    //定义epoll_event结构体数组，作为epoll_wait的第二个参数，用于存储已就绪的文件描述符的信息
    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(struct epoll_event);

    while(1){
        int num = epoll_wait(epfd, evs, size, -1);
        for(int i = 0; i < num; ++i){
            int curr_fd = evs[i].data.fd;
            if(curr_fd == listen_fd){
                //当前就绪套接字是监听套接字，则为其创建连接
                //接受连接请求，创建 已连接套接字描述符
                int conn_fd = accept(curr_fd, NULL, NULL);
	            if(conn_fd < 0){
		            perror("accept");
		            exit(1);
	            }
                // 将conn_fd添加到epoll中
                ev.events = EPOLLIN;
                ev.data.fd = conn_fd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1){
                    perror("epoll_ctl-accept");
                    exit(1);
                }
            }
            else{
                //当前就绪套接字为已连接套接字，则收发数据
                char sendbuf[BUFFER_SIZE];
	            char recvbuf[BUFFER_SIZE];
        		memset(recvbuf, 0, sizeof(recvbuf));
		        memset(sendbuf, 0, sizeof(sendbuf));
		
		        recv(curr_fd, recvbuf, sizeof(recvbuf), 0); //从curr_fd套接字接收数据，保存至recvbuf
		        if(strcmp(recvbuf, "Q\n") == 0){ //如果收到的数据是Q，表示退出
                    printf("这个客户端断开了连接");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curr_fd, NULL);
			        close(curr_fd);
                }
		        fputs(recvbuf, stdout); //将收到的数据原样输出至stdout
		        //将recvbuf中的小写字母转为大写，新数据保存至sendbuf
		        for(int i = 0; i < strlen(recvbuf); ++i){
			        if(islower(recvbuf[i]))
				        sendbuf[i] = toupper(recvbuf[i]);
			        else
				        sendbuf[i] = recvbuf[i];
		        }
		        send(curr_fd, sendbuf, strlen(sendbuf), 0); //将sendbuf中的数据通过conn_fd套接字传输
            }
        }
    }
	//关闭连接及监听描述符
	close(listen_fd);
	return 0;
}