/*************************************************************************
> File Name: connect_to_server.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 11时10分26秒

 ************************************************************************/

#include"client.h"
int connect_to_client(int *client_socket)
{
    //设置一个socket地址结构client_addr,代表客户机internet地址, 端口
    struct sockaddr_in client_addr;  
	struct sockaddr_in server_addr;                  
	bzero(&client_addr,sizeof(client_addr)); //把一段内存区的内容全部设置为0
	client_addr.sin_family = AF_INET;    //internet协议族
	client_addr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY表示自动获取本机地址
	client_addr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口
	*client_socket=socket(AF_INET,SOCK_STREAM,0);  
	//创建用于internet的流协议(TCP)socket,用client_socket代表客户机socket
	if(*client_socket<0)
	{ 
		printf("Sock failed!\n");  
		return -1;  
	}    
	//把客户机的socket和客户机的socket地址结构联系起来
	if( bind(*client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
	{
		printf("Client Bind Port Failed!\n");
		return -1;
	}  
	//设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");  
	server_addr.sin_port=htons(6536);  
	socklen_t server_addr_length = sizeof(server_addr);
	//向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接
	if(connect(*client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
	{ 
		printf("Can Not Connect To Server\n");
		return -1;
	} 
	return 0;
}

