/*************************************************************************
> File Name: main.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 16时50分32秒

 ************************************************************************/

#include "server.h"
static void sigchld_handler(int signo)
{
	int status;
	pid_t pid = waitpid(-1,&status,WNOHANG);
	if(WIFEXITED(status))
	{
		printf("The child exit with code %d",WEXITSTATUS(status));
	}
}
int main()
{
	int server_socket;
	printf("服务器初始化...\n");
	if(server_init(&server_socket)==0)
	{ 
		printf("服务器初始化完成！\n");
	}  
	else
	{    
		printf("服务器初始化失败，请检查相关参数！\n");
	}
	struct sockaddr_in client_addr;
	signal(SIGCHLD, sigchld_handler); //非阻塞回收子进程
	while(1)
	{ 
        socklen_t length = sizeof(client_addr);
		int new_server_socket;
		new_server_socket=accept(server_socket,(struct sockaddr*)&client_addr,&length);
		if (new_server_socket < 0)
        {
			printf("Server Accept Failed!是吗\n");
			break;
 		}
		else
		{
			 printf("Server Accept Succeed!\n");
	 	}
		char socket_buf[MAXLINE];
		memset(socket_buf,'\0',MAXLINE);
		sprintf(socket_buf,"%d",new_server_socket);
		int rtn; /*子进程的返回数值*/
		if ( fork() == 0 ) 
	 	{   
			/* 子进程执行此命令 */  

			if(execl("/home/qiao/文档/im/im_server/server_on","server_on",socket_buf,NULL)<0)
			{ 
				printf("Execlp error\n");
	 		}
			/* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/   
			return 0;
		}   
/*		else 
		{   
		//	 父进程， 等待子进程结束，并打印子进程的返回值 
			wait ( &rtn );   
			printf( " child process return %d/n",rtn);   
		} */ 
	}
	return 0;
}

