set(CLIENT_SRC_LIST ./client.c)
set(SERVER_SRC_LIST ./server.c)
set(EPOLL_SERVER_SRC_LIST ./epoll_server.c)

add_executable(client ${CLIENT_SRC_LIST})
add_executable(server ${SERVER_SRC_LIST})
add_executable(epoll_server ${EPOLL_SERVER_SRC_LIST})
