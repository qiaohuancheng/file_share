/*************************************************************************
> File Name: server_tsl.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分57秒

 ************************************************************************/

#include "server.h"
int server_tls(int server_socket)
{
    int suc_mark=0;
    xmlChar* xmlbuf;
    xmlDocPtr doc;
	int stream_on=0;
	if((xmlbuf=recv_xml(server_socket))!=NULL)
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
	free(xmlbuf);

 
	xmlbuf="<?xml version=\"1.0\"?>\n<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" from=\"im_server.com\" version=\"1.0\">";

    if(send_xml(xmlbuf,server_socket)==0)
    {
		if(modify_num==1)
		{
			printf("发送%s成功！\n",xmlbuf);
	  	}
		stream_on=1;
    }  
    else
    { 
		if(modify_num==1)
		{
			printf("发送%s失败！\n",xmlbuf);
	  	}
        return -1;
    } 

    xmlbuf="<stream:features>\n  <starttls xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\">\n    <required/>\n  </starttls>\n  <mechanisms xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n  <mechanism>DIGEST-MD5</mechanism>\n  <mechanism>PLAIN</mechanism>\n  <mechanism>EXTERNAL</mechanism>\n</mechanisms>\n</stream:features>";

    if(send_xml(xmlbuf,server_socket)==0)
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
			printf("发送%s失败！\n",xmlbuf);
	 	}
        return -1;
    }  

    while(stream_on)
    { 
        if((xmlbuf=recv_xml(server_socket))!=NULL)
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
        curNode = curNode->xmlChildrenNode; //子节点集是链表
        xmlNodePtr propNodePtr = curNode;

        if(NULL == curNode)
        {
            xmlFreeDoc(doc);
            return -1;
        }  
        while(curNode != NULL)
        { 
            //取出节点中的内容
            if(xmlStrcmp(curNode->name, (const xmlChar *)"text"))
                break;
            curNode = curNode->next; //子节点集是链表
        } 

        int num=0;

        if(!xmlStrcmp(curNode->name, (const xmlChar *)"starttls"))
        { 
            num=1;
        } 
		xmlFreeDoc(doc);
		if(num==1)
        {
            //初始化实体必须(MUST)验证接收实体出示的证书
            //如果失败则发送<failure xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>
            //</stream:stream>
         //   xmlbuf="<starttls xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";
        //    xmlbuf="<proceed xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";
			xmlbuf="<proceed xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";

            if(send_xml(xmlbuf,server_socket)==0)
             {
				if(modify_num==1)
			 	{
					printf("发送%s成功！\n",xmlbuf);
		 		}
				stream_on=0;
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

