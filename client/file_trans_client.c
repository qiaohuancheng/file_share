/*************************************************************************
> File Name: file_trans_client.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年07月23日 星期三 16时41分41秒

 ************************************************************************/
#include "client.h"
#define LISTENQ 1
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char *argv[])
{
    int clientfd;
	if(argc<2)                                                                                  
	{   
		exit(0);
	}   

    struct sockaddr_in clientaddr;
    bzero(&clientaddr,sizeof(clientaddr));
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_addr.s_addr=htons(INADDR_ANY);
    clientaddr.sin_port=htons(0);
    clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0)
    {
        perror("socket");
        exit(1);
    }

    if(bind(clientfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0)
    {
        perror("bind");
        exit(1);
    }


    struct sockaddr_in svraddr;
    bzero(&svraddr,sizeof(svraddr));

	int serv_port=0;
	char *serv_port_tmp=argv[2];
	int i=0;
	while(serv_port_tmp[i]!='\0')
	{
		serv_port=serv_port*10+serv_port_tmp[i]-'0';
		i++;
	}
	struct sockaddr_in server_addr;
	svraddr.sin_addr.s_addr=inet_addr(argv[1]);                                        
    svraddr.sin_family=AF_INET;
    svraddr.sin_port=htons(serv_port);
    socklen_t svraddrlen=sizeof(svraddr);

    if(connect(clientfd,(struct sockaddr*)&svraddr,svraddrlen)<0) //由参数获得对端主机的监听套接字IP和端口，建立连接
    {
        perror("connect");
        exit(1);
    }

    //recv file imformation
    char *buff=(char *)malloc(BUFFSIZE*sizeof(char));
    char *filename=(char*)malloc(FILE_NAME_MAX_SIZE*sizeof(char));
    int count;
    bzero(buff,BUFFSIZE);
    count=recv(clientfd,buff,BUFFSIZE,0);

    if(count<0)
    { 
        perror("recv");
        exit(1);
    }
    sprintf(filename,"%s/%s",argv[3],buff); //获取文件，所存储的本地目录，及名字
    printf("Preparing recv file : %s from %s \n",filename,argv[1]);
    //recv file
    FILE *fd=fopen(filename,"wb+");
    if(NULL==fd)
    { 
        perror("open");
        exit(1);
    }

    bzero(buff,BUFFSIZE);
    int length=0;

    while(length=recv(clientfd,buff,BUFFSIZE,0))
    { 
        if(length<0)
        {
            perror("recv");
            exit(1);
        } 

        int writelen=fwrite(buff,sizeof(char),length,fd);

        if(writelen<length)
        {
            perror("write");
            exit(1);
        }

        bzero(buff,BUFFSIZE);
    }

    printf("Receieved file:%s from %s finished!\n",filename,argv[1]);
	free(filename);
	free(buff);
    fclose(fd);
    close(clientfd);
    return 0;
}
