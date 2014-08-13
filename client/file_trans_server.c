/*************************************************************************
> File Name: file_trans_server.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年07月23日 星期三 16时43分16秒

 ************************************************************************/
#include "client.h"
#define LISTENQ 1
#define BUFFSIZE 4096
int main(int argc, char *argv[])
{
    if(argc<3)
    {
        exit(0);
    }

    int client_socket=atoi(argv[1]);
    //Create socket
    int sockfd,connfd;
    struct sockaddr_in svraddr,clientaddr;
    bzero(&svraddr,sizeof(svraddr));
    svraddr.sin_family=AF_INET;
    svraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    svraddr.sin_port=htons(0);
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    if(sockfd<0)
    {
        perror("socket");
        exit(1);
    }

    //bind
    if(bind(sockfd,(struct sockaddr*)&svraddr,sizeof(svraddr))<0)
    {
        perror("bind");
        exit(1);
    }

    //listen
    if(listen(sockfd,LISTENQ)<0) //建立监听套接字
    {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in serv;

    char serv_ip[20];

    int serv_port;

    int serv_len = sizeof(serv);


    getsockname(client_socket, (struct sockaddr *)&serv, &serv_len); //获取本地ip及port

    inet_ntop(AF_INET, &serv.sin_addr, serv_ip, sizeof(serv_ip));

	getsockname(sockfd, (struct sockaddr *)&serv, &serv_len); //获取本地ip及port 通过xml消息传给，对端主机，以建立连接

    serv_port=ntohs(serv.sin_port);

    char serv_port_tmp[10];

    memset(serv_port_tmp,'\0',10);

    sprintf(serv_port_tmp,"%d",serv_port);

    xmlDocPtr doc_tmp = xmlNewDoc(BAD_CAST"1.0");//组织消息

    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"message");

    //设置根节点
    xmlDocSetRootElement(doc_tmp,root_node);

    xmlNewProp(root_node,BAD_CAST"file_trans_ip",(xmlChar*)serv_ip);

    xmlNewProp(root_node,BAD_CAST"file_trans_port",(xmlChar*)serv_port_tmp);

    xmlNewProp(root_node,BAD_CAST"dir",(xmlChar*)argv[3]);

	if(argc==5)
		xmlNewProp(root_node,BAD_CAST"local_socket",(xmlChar*)argv[4]);

    int buffersize;

    xmlChar *xmlbuf;

    xmlDocDumpFormatMemory(doc_tmp, &xmlbuf, &buffersize, 1);

    xmlFreeDoc(doc_tmp);

    xmlChar *ptr=xmlbuf;

    int num=0;

    while(*ptr)
    {
        if(*ptr=='<')
            num++;

        if(num==2)
            break;

        ptr++;
    } 

    ptr[strlen(ptr)-1]='\0';

    if(send_xml(ptr,client_socket)==0)
    {
        if(modify_num==1)
        {
            printf("发送%s成功！\n",xmlbuf);
        }

        xmlFree(xmlbuf);
    }

    else
    {
        if(modify_num==1)
        {
            printf("发送%s失败！\n",xmlbuf);
        }

        xmlFree(xmlbuf);
        return -1;
    }

    socklen_t length=sizeof(clientaddr);
    //accept
    connfd=accept(sockfd,(struct sockaddr*)&clientaddr,&length);//接收连接

    if(connfd<0)
    {
        perror("connect");
        exit(1);
    }

    //send file imformation
    char *filename=argv[2];
    char *filename_tmp=filename+strlen(filename);//不带路径

    while(*filename_tmp!='/')
    {
        filename_tmp--;
    }

    filename_tmp++;
    char *buff=(char*)malloc(BUFFSIZE*sizeof(char));
    int count;
    bzero(buff,BUFFSIZE);
    sprintf(buff,"%s",filename_tmp);
    count=send(connfd,buff,BUFFSIZE,0);

    if(count<0)
    {
        perror("Send file information");
        exit(1);
    }

    //read file
    FILE *fd=fopen(filename,"rb");

    if(fd==NULL)
    {
        printf("File :%s not found!\n",filename);
    }

    else
    {
        bzero(buff,BUFFSIZE);
        int file_block_length=0;

        while((file_block_length=fread(buff,sizeof(char),BUFFSIZE,fd))>0)
        {
            printf("file_block_length:%d\n",file_block_length);

            if(send(connfd,buff,file_block_length,0)<0)
            {
                perror("Send");
                exit(1);
            }

            bzero(buff,BUFFSIZE);
        }

        fclose(fd);
        printf("Transfer file finished !\n");
    }

    free(buff);
    close(connfd);
    close(sockfd);
    return 0;
}

