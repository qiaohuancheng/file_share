/*************************************************************************
> File Name: message_parsing.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年07月19日 星期六 15时24分46秒

 ************************************************************************/

#include"server.h"
int message_parsing(int server_socket,int *exit_num,MYSQL mysql_conn)
{
    char command[5][10]= {"ll","exit","vim","open","close"};
    xmlChar *xmlbuf;
    char *buffer=(char *)malloc(256*sizeof(char));
    memset(buffer,'\0',256);
    int response_type=-1; //响应类型
    xmlDocPtr doc;
    xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)

    if((xmlbuf=recv_xml(server_socket))!=NULL)
    {
        /*  if(modify_num==1)
         *  *                   {
         *  *                                    printf("接收%s成功！\n",xmlbuf);
         **                                                     }*/
//        doc=xmlchar_to_doc(xmlbuf);
 
		doc = xmlReadMemory(xmlbuf,strlen(xmlbuf)+1,NULL,"utf-8",XML_PARSE_RECOVER    );

     //   doc = xmlParseMemory(xmlbuf,strlen(xmlbuf)+1);
        free(xmlbuf);
        curNode = xmlDocGetRootElement(doc); //确定文档根元素

        if(NULL == curNode)
        {
            xmlFreeDoc(doc);
            return -1;
        }
        if(xmlStrcmp(curNode->name, (const xmlChar *)"message")==0)
            response_type=0;

        if(xmlStrcmp(curNode->name, (const xmlChar *)"command")==0)
            response_type=1;
    }

    if(response_type==0)
    {
        int local_socket;

        if(xmlHasProp(curNode,(xmlChar *)"local_socket"))  //判断结点curNode是否具有属性attribute
        {
            xmlChar *local_socket_tmp;
            local_socket_tmp= xmlGetProp(curNode, "local_socket"); //获取属性值
            int i=0;

            while(local_socket_tmp[i]!='\0')
            {
                local_socket=local_socket*10+local_socket_tmp[i]-'0';
                i++;
            }

            xmlFree(local_socket_tmp); //释放内存
        }

        xmlSetProp(curNode,(const xmlChar *)"local_socket", (xmlChar *)"no");
        int buffersize;
        xmlDocDumpFormatMemory(doc, &xmlbuf, &buffersize, 1);
        int num=0;
        char *ptr=xmlbuf;

        while(*ptr)
        {
            if(*ptr=='<')
                num++;

            if(num==2)
                break;

            ptr++;
        }

        ptr[strlen(ptr)-1]='\0';
        fd_set wtfds;
        struct timeval timeout= {3,0}; //select等待3秒，3秒轮询，要非阻塞就置
        FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
        FD_SET(local_socket,&wtfds); //添加描述符
        select(local_socket+1,NULL,&wtfds,NULL,&timeout); //select使用

        if(FD_ISSET(local_socket,&wtfds)) //测试文件是否可写
        {
            if(write_xml(ptr,local_socket)==0)
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
    }

    if(response_type==1)
    {
        xmlChar *key;
        key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的>文本，需用其子结点
        sprintf(buffer,"%s",key);
        xmlFree(key);
        char *jid=(char *)malloc(50*sizeof(char));
        memset(jid,'\0',50);
        char *buffer_tmp=(char *)malloc(256*sizeof(char));
        memset(buffer_tmp,'\0',256);
        int i=0;

        while(buffer[i]!=' '&&buffer[i]!='\0')
        {
            buffer_tmp[i]=buffer[i];
            i++;
        }

        buffer_tmp[i]='\0';
        int j=0;

        while(buffer[i]!='\0')
        {
            if(buffer[i]==' ')
            {
                i++;
                continue;
            }

            jid[j]=buffer[i];
            i++;
            j++;
        }

        jid[j]='\0';
        int num=-1;

        for(int i=0; i<5; i++)
            if(strcmp(buffer_tmp,command[i])==0)
            {
                num=i;
                break;
            }

        free(buffer_tmp);

        switch(num)
        {
            case 0:
                client_ll(server_socket,mysql_conn);
                break;

            case 1:
                *exit_num=1;
                break;

            case 3:
                if(remote_login(server_socket,jid,mysql_conn)==-1)
                    return -1;

                break;

            case 4:
                break;

            default:
                memset(buffer,'\0',256);
                sprintf(buffer,"%s","命令不存在！");
                xmlChar *xmlbuf_head="<message>";
                xmlChar *xmlbuf_tail="</message>";
                xmlChar *xmlbuf;
                xmlbuf=xmlchar_construct(xmlbuf_head,buffer,xmlbuf_tail);

                if(send_xml(xmlbuf,server_socket)==0)
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

        free(jid);
    }

    free(buffer);
    xmlFreeDoc(doc);
    return 0;
}


