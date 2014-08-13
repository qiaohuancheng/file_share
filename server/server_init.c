/*************************************************************************
> File Name: server_init.c
> Author: qxs
> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 10时50分56秒

 ************************************************************************/

#include"server.h"
int server_init(int *server_socket)
{
	
	struct sockaddr_in server_addr;    
	int address_size;  	
	bzero(&server_addr, sizeof(server_addr));  //把一段内存区的内容全部设置为0

	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = INADDR_ANY;  
	server_addr.sin_port = htons(port);  
	//创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
	*server_socket = socket(AF_INET, SOCK_STREAM, 0);  
	if (-1 == *server_socket)  
	{
		perror("call to socket");  
		return -1;  
	}   
    //把socket和socket地址结构联系起来
    if(bind(*server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))  
	{   
		perror("call to bind");  
		return -1;  
	}  
	//server_socket用于监听
	if (listen(*server_socket,LENGTH_OF_LISTEN_QUEUE))
	{
		printf("Server Listen Failed!");
		return -1;
	}
	return 0;
}
