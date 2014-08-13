/*************************************************************************
> File Name: client_tsl.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分47秒

 ************************************************************************/

#include"client.h"
int client_tls(int client_socket)
{
    xmlDocPtr doc;
    xmlChar *xmlbuf;
    int stream_on=0;
    xmlbuf="<?xml version=\"1.0\"?>\n<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" to=\"im_server.com\" version=\"1.0\">";

    if(send_xml(xmlbuf,client_socket)==0)
    {
		if(modify_num==1)
		{
			printf("发送%s成功！\n",xmlbuf);
		}
    }

    else
    {
		if(modify_num==1)
		{
			printf("发送%s成功！\n",xmlbuf);
		}
        return -1;
    }

    if((xmlbuf=recv_xml(client_socket))!=NULL)
    {
		if(modify_num==1)
		{
			printf("接收%s成功！\n",xmlbuf);
		}
		free(xmlbuf);
		stream_on=1;
    }

    else
    {
		if(modify_num==1)
		{
			printf("接收%s！失败\n",xmlbuf);
		}
        return -1;
    }

    while(stream_on)
    { 
        if((xmlbuf=recv_xml(client_socket))!=NULL)
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
            continue;
         }

        doc=xmlchar_to_doc(xmlbuf);
		free(xmlbuf);
        xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
        curNode = xmlDocGetRootElement(doc); //确定文档根元素

        if(NULL == curNode)
        { 
            xmlFreeDoc(doc);
            return -1;
         }

        curNode = curNode->xmlChildrenNode; //子节点集是链表
        xmlNodePtr propNodePtr = curNode;
        int num_t=0;

        while(curNode != NULL)
         { 
            //取出节点中的内容
            if((!xmlStrcmp(curNode->name, (const xmlChar *)"features")))
            {
                curNode = curNode->xmlChildrenNode; //子节点集是链表

                while(curNode != NULL)
                {
                    if((!xmlStrcmp(curNode->name, (const xmlChar *)"starttls")))
                    {
                        num_t=1;
                        break;
                     }

                    else
                     {
                        curNode=curNode->next;
                    }
                 }

                break;
             }

            else
            {
                if(!xmlStrcmp(curNode->name, (const xmlChar *)"proceed"))
                {
                    stream_on=0;
                    break;
                 }

                curNode=curNode->next;
            } 
        }
		xmlFreeDoc(doc);
        if(num_t==1)
        {
            xmlbuf="<starttls xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";

            if(send_xml(xmlbuf,client_socket)==0)
             {
				if(modify_num==1)
		 		{
					printf("发送%s成功！\n",xmlbuf);
				}
                continue;
            }

            else
            {
				if(modify_num==1)
				{
					printf("发送%s失败！\n",xmlbuf);
		 		}
                return -1;
             }
        } 
    } 

    return 0;
}


