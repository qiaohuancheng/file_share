/*************************************************************************
> File Name: server_on.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年06月12日 星期四 22时13分26秒

 ************************************************************************/

#include"server.h"
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        exit(0);
    }

	int tag_file=1;
	while(1)
	{   
		if (tag_file==0)
			continue;
		else
			break;
	}   


    int server_socket;
    server_socket=atoi(argv[1]);
    /*  struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        int server_socket;
        char socket_buf[MAXLINE];
        server_socket=accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if (server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            exit(0);
        }
        else
        {
            printf("Server Accept Succeed!\n");
        }*/

    if(server_tls(server_socket)==0)
    {
        printf("tls验证成功！");
    }

    else
    {
        printf("tls验证失败！");
        close(server_socket);
        return -1;
    }

    MYSQL mysql_conn;

//  mysql_options(&mysql_conn,MYSQL_SET_CHARSET_NAME,"utf8");//设置中文编码
    if(mysql_init(&mysql_conn)==NULL)
    {
        return -1;
    }

    if(mysql_real_connect(&mysql_conn, "localhost", "root", NULL, "enterprise_im_db", MYSQL_PORT, NULL, 0)==NULL)
    {
        printf("连接数据库失败！\n");
        return -1;
    }

    char *client_name=(char *)malloc(50*sizeof(char));

    if(server_sasl(server_socket,mysql_conn,client_name)==0)
    {
        printf("sasl验证成功！\n");
    }

    else
    {
        printf("sasl验证失败！\n");
        close(server_socket);
        return -1;
    }

    char *jid=(char *)malloc(50*sizeof(char));

    if(server_bind(server_socket,mysql_conn,client_name,jid)==0)
    {
        printf("bind成功！\n");
    }

    else
    {
        printf("bind失败！\n");
    }

    int lsn_fd;   //本地socket,进程间通信
    int *apt_fd=(int *)malloc(10*sizeof(int));
//    memset(apt_fd,-1,10);
	for(int i=0;i<10;i++)
		apt_fd[i]=-1;
    struct sockaddr_un srv_addr;
    struct sockaddr_un clt_addr;
    socklen_t clt_len;
    unlink(jid); /* delete the socket file */
    //create socket to bind local IP and PORT
    lsn_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(lsn_fd < 0)
    {
        perror("can't create communication socket!");
        return 1;
    }
    //create local IP and PORT
    srv_addr.sun_family = AF_UNIX;
    sprintf(srv_addr.sun_path,"@%s",jid);
    srv_addr.sun_path[0]=0;
    //server_len = sizeof(server_addr);
    socklen_t srv_len = strlen(jid)+1+offsetof(struct sockaddr_un, sun_path);
    //bind sockfd and sockaddr
    int ret = bind(lsn_fd,(struct sockaddr*)&srv_addr,srv_len);
    if(ret == -1)
    {
        perror("can't bind local sockaddr!");
        close(lsn_fd);
        return 1;
    }
    //listen lsn_fd, try listen 1
    ret = listen(lsn_fd,10);
    if(ret == -1)
    {
        perror("can't listen client connect request");
        close(lsn_fd);
        return 1;
    } 

    clt_len = sizeof(clt_addr);
    /*  while(1)
        {
            apt_fd = accept(lsn_fd, (struct sockaddr*)&clt_addr, &clt_len);
            if(apt_fd < 0)
            {
                perror("can't listen client connect request");
                close(lsn_fd);
                return 1;
            }
        }*/ 
    char *buffer=(char*)malloc(256*sizeof(char)); //256字节的接收缓冲区
    memset(buffer,'\0',256);
    fd_set rdfds,wtfds,allset;
    struct timeval timeout= {3,0}; //select等待3秒，3秒轮询，要非阻塞就置
    char command[5][10]= {"ll","exit","vim","open","close"};
    xmlChar *xmlbuf;
    FD_ZERO(&allset); //每次循环都要清空集合，否则不能检测描述符变化
    FD_SET(server_socket,&allset); //添加描述符
    FD_SET(lsn_fd,&allset); //添加描述符
    int maxfd=server_socket>lsn_fd?server_socket+1:lsn_fd+1;
	int max_apt=0;

    while(1)
    {
        rdfds=allset;
        FD_ZERO(&wtfds); //每次循环都要清空 集合，否则不能检测描述符变化
        FD_SET(server_socket,&wtfds); //添加描述
        int exit_num=0;
        int nready=select(maxfd,&rdfds,&wtfds,NULL,&timeout); //select使用

        if(FD_ISSET(server_socket,&rdfds)) //测试sock是否可读，即是否网络上有数据
        {
			message_parsing(server_socket,&exit_num,mysql_conn);
            if(--nready<=0)
                continue;
        }
        if(FD_ISSET(lsn_fd,&rdfds)) //测试sock是否可读，即是否网络上有数据
//                   if(FD_ISSET(server_socket,&wtfds)) //测试文件是否可写
        { 
            int apt_fd_tmp = accept(lsn_fd, (struct sockaddr*)&clt_addr, &clt_len);
            int i;
            for(i=0; i<10;i++)
             {
                if(apt_fd[i]<0)
                 {
                    apt_fd[i]=apt_fd_tmp;
                    break;
                }
            }

            if(i==10)
            {
                perror("本地通信进程数太多！");
             }
			if(i>max_apt)
				max_apt=i;

            FD_SET(apt_fd_tmp,&allset);

            if(apt_fd_tmp>=maxfd)
                maxfd=apt_fd_tmp+1;

            if(--nready<=0)
                continue;

            /*       if(send(server_socket,buffer,256,0) == -1)
                     {
                         return -1;
                     }
                      memset(buffer,'\0',256);*/
        }

        for(int i=0; i<=max_apt; i++)
        {
            int apt_fd_tmp;
            if((apt_fd_tmp=apt_fd[i])<0)
                continue;
            if(FD_ISSET(apt_fd_tmp,&rdfds))
            { 
                if((xmlbuf=read_xml(apt_fd_tmp))!=NULL)
                { 
                    if(modify_num==1)
                     {
                        printf("接收%s成功！\n",xmlbuf);
					}
                }
				else
				{ 
                    if(modify_num==1)
                     {
                        printf("接收%s失败！\n",xmlbuf);
                     }

                    return -1;
				}
				
				xmlDocPtr doc;
				doc = xmlReadMemory(xmlbuf,strlen(xmlbuf)+1,NULL,"utf-8",XML_PARSE_RECOVER    );

//				doc = xmlParseMemory(xmlbuf,strlen(xmlbuf)+1);
//				doc=xmlchar_to_doc(xmlbuf);
				xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)          
				curNode = xmlDocGetRootElement(doc); //确定文档根元素
				if(NULL == curNode)
				{   
					xmlFreeDoc(doc);
					return -1; 
				}   
/*				while(curNode != NULL)
				{   
					//取出节点中的内
					if(xmlStrcmp(curNode->name, (const xmlChar *)"text"))
						break;
					curNode = curNode->next; //子节点集是链表    
				} */
				int close_num=0;
				if(xmlStrcmp(curNode->name, (const xmlChar *)"command")==0)
				{   
					xmlChar *key;
					key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的>文本，需用其子结点
					if(xmlStrcmp(key,(const xmlChar *)"close")==0)
					{ 
						close_num=1;
					}
					xmlFree(key);
				}
				xmlFree(xmlbuf);
				if(close_num==1)
				{
					xmlFreeDoc(doc);
					close(apt_fd_tmp);
					apt_fd[i]=-1;
					FD_CLR(apt_fd_tmp,&allset);
					continue;
				} 
				xmlChar *local_socket=(xmlChar *)malloc(10*sizeof(xmlChar));
				sprintf(local_socket,"%d",apt_fd_tmp);
				xmlNewProp(curNode,BAD_CAST"local_socket",local_socket);
				int buffersize;
				xmlDocDumpFormatMemory(doc, &xmlbuf, &buffersize, 1);
				int num=0;
				char *ptr=xmlbuf;
				char *ptr_tmp=xmlbuf;
				while(*ptr)
				{
					if(*ptr=='<')
						num++;
					if(num==2)
						break;
					ptr++;
				}
				ptr[strlen(ptr)-1]='\0';
				xmlFree(local_socket);
                FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
                FD_SET(server_socket,&wtfds); //添加描述符
                select(server_socket+1,NULL,&wtfds,NULL,&timeout); //select使用

                if(FD_ISSET(server_socket,&wtfds)) //测试文件是否可写
                { 
                    if(send_xml(ptr,server_socket)==0)
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

				} 
            }
        }//end  of for

        if(exit_num==1)
            break;
    } 

    free(jid);
    free(buffer);

    if(client_final(server_socket,mysql_conn,client_name)==0)
    {
        printf("close成功！\n");
    }

    else
    {
        printf("close失败！\n");
    }

	
    free(client_name);
    return 0;
}

