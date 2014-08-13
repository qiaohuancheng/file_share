/*************************************************************************
> File Name: main.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年06月12日 星期四 16时50分26秒

 ************************************************************************/

#include "client.h"
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
    int client_socket;

    if(connect_to_client(&client_socket)==0)
    {
        printf("连接服务器成功！\n");
    }

    else
    {
        printf("连接服务器失败！\n");
        return -1;
    }

    if(client_tls(client_socket)==0)
    {
        printf("tls验证成功！\n");
    }

    else
    {
        printf("tls验证失败！\n");
        close(client_socket);
        return -1;
    }

    char *client_name=(char *)malloc(50*sizeof(char));

    if(client_sasl(client_socket,client_name)==0)
    {
        printf("sasl验证成功！\n");
    }

    else
    {
        printf("sasl验证失败！\n");
        close(client_socket);
        return -1;
    }

    free(client_name);
    char *jid=(char *)malloc(50*sizeof(char));

    if(client_bind(client_socket,client_name,jid)==0)
    {
        printf("bind成功！\n");
    }

    else
    {
        printf("bind失败！\n");
        close(client_socket);
        return -1;
    }

    free(jid);
    xmlChar *message_head="<message>";  //定义发送消息的头部
    xmlChar *message_tail="</message>";
    xmlChar *command_head="<command>";
    xmlChar *command_tail="</command>";
    char *buffer=(char *)malloc(256*sizeof(char));  //定义缓冲区
    memset(buffer,'\0',256);
    fd_set rdfds,wtfds,allset;
    struct timeval timeout= {3,0}; //select等待3秒，3秒轮询，要非阻塞就置0
    xmlChar *xmlbuf;
    int keyboard;
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK); //键盘输入
    FD_ZERO(&allset); //每次循环都要清空集合，否则不能检测描述符变化
    FD_SET(client_socket,&allset); //添加描述符
    FD_SET(keyboard,&allset); //添加描述符
    int maxfdp=keyboard>client_socket?keyboard+1:client_socket+1; //描述符>    最大值加1
    xmlChar *remote_dir=(char *)malloc(1024*sizeof(char));
    memset(remote_dir,'\0',1024);//存储受控制端的当前目录
    int open_on=0;//连接是否建立

    while(1)
    {
        int exit_num=0;//while循环结束标志
        memset(buffer,'\0',256);
        rdfds=allset;

        //      switch(select(client_socket+1,&rdfds,&wtfds,NULL,&timeout)) //select使用
        switch(select(maxfdp,&rdfds,NULL,NULL,&timeout)) //select使用
        {
            case -1:
                exit(-1);
                break; //select错误，退出程序

            case 0:
                break; //再次轮询

            default:
                if(FD_ISSET(client_socket,&rdfds)) //测试文件是否可写
                {
                    if((xmlbuf=recv_xml(client_socket))!=NULL)
                    {
                        /*  if(modify_num==1)
                        {
                            printf("接收%s成功！\n",xmlbuf);
                        }*/
                        xmlDocPtr doc;
                        doc = xmlReadMemory(xmlbuf,strlen(xmlbuf)+1,NULL,"utf-8",XML_PARSE_RECOVER);
                        //       doc = xmlParseMemory(xmlbuf,strlen(xmlbuf)+1);
                        //         doc=xmlchar_to_doc(xmlbuf);
                        xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
                        curNode = xmlDocGetRootElement(doc); //确定文档根元素

                        if(NULL == curNode)
                        {
                            xmlFreeDoc(doc);
                            return -1;
                        }

                        if(xmlStrcmp(curNode->name, (const xmlChar *)"message")==0)
                        {
                            xmlChar *key;
                            key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的文本，需用其子结点
                            printf("message%s\n",key);

                            if(xmlStrcmp(key, (const xmlChar *)"open success")==0)
                                open_on=1;

                            if(xmlStrcmp(key, (const xmlChar *)"close")==0)
                                open_on=0;

                            xmlFree(key);

                            if(xmlHasProp(curNode,(xmlChar *)"file_trans_ip"))  //判断结点curNode是否具有属性attribute
                            {
								xmlChar *remote_dir_tmp;
								remote_dir_tmp= xmlGetProp(curNode, "dir"); //获取属性值
                                xmlChar *server_ip= xmlGetProp(curNode, "file_trans_ip"); //获取属性值 文件传输过程中，建立监听套接字的对端的IP地址
                                xmlChar *server_port= xmlGetProp(curNode, "file_trans_port"); //获取属性值  文件传输过程中，建立监听套接字的对端的监听端口

                                if(fork() == 0)
                                {//建立文件传输的子进程，以独立运行文件传输进程，不影响主进程
                                    /* 子进程执行此命令 */
                                    if(execl("/home/qiao/文档/im/im_client/file_client","file_client",server_ip,server_port,remote_dir_tmp,NULL)<0)
                                    {
                                        printf("Execlp error\n");
                                    }

                                    /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
                                    return 0;
                                }
                                xmlFree(server_ip);
                                xmlFree(server_port);
                                xmlFree(remote_dir_tmp);
                                break;
                            }
                            if(xmlHasProp(curNode,(xmlChar *)"dir"))  //判断结点curNode是否具有属性attribute
                            {
                                xmlChar *remote_dir_tmp;
                                remote_dir_tmp= xmlGetProp(curNode, "dir"); //获取属性值
                                sprintf(remote_dir,"%s",remote_dir_tmp);
                                xmlFree(remote_dir_tmp);
                            } 
                        }

                        if(xmlStrcmp(curNode->name, (const xmlChar *)"command")==0)
                        {//接收到命令消息
                            xmlChar *key;
                            xmlChar *local_dir;
                            key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的文本，需用其子结点
                            if(xmlHasProp(curNode,(xmlChar *)"dir"))  //判断结点curNode是否具有属性attribute
                                local_dir= xmlGetProp(curNode, "dir"); //获取属性值
                            char file_com[5];//提取前五个字节，判断是否是与文件相关的命令
                            char *key_p=key;

                            for(int i=0; i<4; i++)
                            { 
                                file_com[i]=key[i];
                                key_p++;
                            }

                            file_com[4]='\0';

                            if(strcmp(file_com,"get ")==0)  //获取文件命令
                            {
								xmlChar *key;
								xmlChar *local_dir;
								xmlChar *local_socket_tmp;
								key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的文本，需用其子结点
								if(xmlHasProp(curNode,(xmlChar *)"dir"))  //判断结点curNode是否具有属性attribute啊对端主机的当前目录
									local_dir= xmlGetProp(curNode, "dir"); //获取属性值
								if(xmlHasProp(curNode,(xmlChar *)"local_socket"))  //判断结点curNode是否具有属性attribute
								{
									local_socket_tmp= xmlGetProp(curNode, "local_socket"); //获取属性值
								}
								char socket_buf[MAXLINE];
                                memset(socket_buf,'\0',MAXLINE);    //服务器进程间通信套接字
                                sprintf(socket_buf,"%d",client_socket);
								char *file_name_buff=(char *)malloc(256*sizeof(char));
								sprintf(file_name_buff,"%s/%s",local_dir,key_p); //欲获取文件的全路径
                                xmlFree(key);
                                xmlFree(local_dir);
								local_dir= xmlGetProp(curNode, "local_dir"); //获取属性值 欲获取文件所在的目录
                                if(fork() == 0)
                                {
                                    if(execl("/home/qiao/文档/im/im_client/file_server","file_server",socket_buf,file_name_buff,local_dir,local_socket_tmp,NULL)<0)
                                    {
                                        printf("Execlp error\n");
                                    }

                                    /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
                                    return 0;
                                }
								free(file_name_buff); //释放相关的资源
                                xmlFree(local_socket_tmp);
                                xmlFree(local_dir);
                                break;
                            }

                            int far_end_socket=0;
                            xmlChar *local_socket_tmp;

                            if(xmlHasProp(curNode,(xmlChar *)"local_socket"))  //判断结点curNode是否具有属性attribute
                            {
                                local_socket_tmp= xmlGetProp(curNode, "local_socket"); //获取属性值
                                int i=0;

                                while(local_socket_tmp[i]!='\0')
                                {
                                    far_end_socket=far_end_socket*10+local_socket_tmp[i]-'0';
                                    i++;
                                }
                            }

                            xmlFree(xmlbuf);
                            char *command_buf=(char *)malloc(2048*sizeof(char));
                            sprintf(command_buf,"cd %s && %s && pwd",local_dir,key);  //通过cd命令，获取当前目录，并将目录传到对端
                            FILE *fp=popen(command_buf,"r");  //执行shell命令
                            free(command_buf);
                            xmlFree(key);
                            xmlFree(local_dir);

                            if(!fp)
                            {
                                return -1;
                            }

                            memset(buffer,'\0',256);
                            xmlChar *comd_buf=(xmlChar*)malloc(256*sizeof(xmlChar));
                            xmlbuf=(xmlChar *)xmlMalloc(2048*sizeof(xmlChar));
                            memset(comd_buf,'\0',256);
                            memset(xmlbuf,'\0',2048);
                            int i=2;

                            while(fgets(comd_buf,256,fp)!=NULL)  //提取执行结果
                            {
                                if(strlen(xmlbuf)%2048+strlen(comd_buf)>=2048)
                                {
                                    xmlbuf=(xmlChar*)realloc(xmlbuf,i*2048*sizeof(xmlChar));
                                    i++;
                                }

                                sprintf((xmlbuf+strlen(xmlbuf)*sizeof(char)),"%s",comd_buf);
                            }

                            comd_buf[strlen(comd_buf)-1]='\0';
                            xmlbuf[strlen(xmlbuf)-1]='\0';
                            char *xmlbuf_p=xmlbuf+strlen(xmlbuf);

                            while(*xmlbuf_p!='\n')
                            {
                                xmlbuf_p--;
                            }

                            *xmlbuf_p='\0';
                            xmlDocPtr doc_tmp = xmlNewDoc(BAD_CAST"1.0");
                            xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"message");
                            //设置根节点
                            xmlDocSetRootElement(doc_tmp,root_node); //组织消息
                            //        //创建一个节点，设置其内容和属性，然后加入根结点
                            xmlNodePtr content = xmlNewText(xmlbuf);
                            xmlAddChild(root_node,content);
                            xmlNewProp(root_node,BAD_CAST"local_socket",local_socket_tmp); //服务器进程间通信套接字
                            xmlNewProp(root_node,BAD_CAST"dir",comd_buf);    //当前目录
                            xmlFree(comd_buf);
                            xmlFree(xmlbuf);
                            int buffersize;
                            xmlDocDumpFormatMemory(doc_tmp, &xmlbuf, &buffersize, 1);
                            xmlFreeDoc(doc_tmp);   //后处理舍弃doc文件的头部
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
                            FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
                            FD_SET(client_socket,&wtfds); //添加描述符
                            select(client_socket+1,NULL,&wtfds,NULL,&timeout); //select使用

                            if(FD_ISSET(client_socket,&wtfds)) //测试文件是否可写
                            {
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
                                }
                            }

                            xmlFree(local_socket_tmp); //释放内存
                            pclose(fp); //关闭管道
                        }

                        xmlFreeDoc(doc);
                    }

                    printf(">>");
                }

                /*              if(show_table==0)
                                {
                                    show_table=1;
                                    printf(">>");
                                 }*/

                if(FD_ISSET(keyboard,&rdfds)) //测试文件是否可写
                    //           if(1) //测试文件是否可写
                {
                    while(strlen(buffer)==0)
                    {
                        fgets(buffer,256,stdin);
                        buffer[strlen(buffer)-1]='\0';
                    }

                    if(open_on==0)  //未和远端主机建立逻辑上连接，命令传给服务器
                    { 
                        xmlbuf=xmlchar_construct(command_head,buffer,command_tail);

                        if(send_xml(xmlbuf,client_socket)==0)
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

                        if(strcmp(buffer,"exit")==0)
                         {
                            exit_num=1;
                            break;
                        }
                    }

                    else //和远端主机建立逻辑上联接，命令通过服务器，传给远端主机
                    {  
                        char file_com[5];
                        char *file_name=buffer;

                        for(int i=0; i<4; i++)
                        {
                            file_com[i]=buffer[i];
                            file_name++;
                        }

                        file_com[4]='\0';

                        if(strcmp(file_com,"put ")==0)  //若是与文件传输相关的命令
                        {
                            char socket_buf[MAXLINE];
                            memset(socket_buf,'\0',MAXLINE);
                            sprintf(socket_buf,"%d",client_socket);
                            int rtn; /*子进程的返回数值*/

                            if(fork() == 0)
                            {
                                /* 子进程执行此命令 */
                                if(execl("/home/qiao/文档/im/im_client/file_server","file_server",socket_buf,file_name,remote_dir,NULL)<0)
                                {
                                    printf("Execlp error\n");
                                }

                                /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
                                return 0;
                            }

                            break;
                        }

                        xmlDocPtr doc_tmp = xmlNewDoc(BAD_CAST"1.0");
                        xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"command");
                        //设置根节点
                        xmlDocSetRootElement(doc_tmp,root_node);
                        //创建一个节点，设置其内容和属性，然后加入根结点
                        xmlNodePtr content = xmlNewText((xmlChar *)buffer);
                        xmlAddChild(root_node,content);
                        if(strcmp(file_com,"get ")==0)
						{
							memset(buffer,'\0',256);
							char *dir_local=(char *)malloc(256*sizeof(char));
							memset(dir_local,'\0',256);
							FILE *fp=popen("cd && pwd","r");
							fgets(buffer,256,fp);
							buffer[strlen(buffer)-1]='\0';
							sprintf(dir_local,"%s/recv",buffer);
							xmlNewProp(root_node,BAD_CAST"local_dir",(xmlChar *)dir_local);
							free(dir_local);
							pclose(fp);
						}
						xmlNewProp(root_node,BAD_CAST"dir",remote_dir);
                        int buffersize;
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
                    }
                }

                memset(buffer,'\0',256);
         }// end if*/
 
        if(exit_num==1)
            break;
    } 

    xmlFree(remote_dir);
	free(buffer);

    if(client_final(client_socket)==0)
    {
        printf("close成功！\n");
    }

    else
    {
        printf("close失败！\n");
    }

    return 0;
}


