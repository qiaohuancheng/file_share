/*************************************************************************
> File Name: server_tsl.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分57秒

 ************************************************************************/

#include "server.h"
int server_bind(int server_socket,MYSQL mysql,char *client_name,char *jid)
{
    xmlChar* xmlbuf;
    xmlDocPtr doc;
	int stream_on=0;

	xmlbuf="<?xml version=\"1.0\"?>\n<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" from=\"im_server.com\" version=\"1.0\">";                                                                           
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
			printf("发送%s成功！\n",xmlbuf);
		}
		return -1;
	} 

	xmlbuf="<stream:features>\n<bind xmlns=\"urn:ietf:params:xml:ns:xmpp-bind\"/>\n</stream:features>";
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
	MYSQL_RES *res;
	MYSQL_ROW row;
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

        if(!xmlStrcmp(curNode->name, (const xmlChar *)"iq"))
        { 
            num=1;
        } 
		xmlFreeDoc(doc);
		int i;
		if(num==1)
        { 
			char *row_name=(char *)malloc(20*sizeof(char));
			mysql_real_query(&mysql,"set names utf8",(unsigned long)strlen("set names utf8"));
			if(mysql_real_query(&mysql,"select * from staff_login",(unsigned long)strlen("select * from staff_login")))   
			{
				if(modify_num==1)
					printf("mysql_real_query failure!");
				return -1;
		 	} 
			res = mysql_store_result(&mysql);
			if(NULL == res)
		 	{ 
				if(modify_num==1)
					printf("mysql_store_result failure!");
				return -1;
			}  
			while(row = mysql_fetch_row(res))
			{ 
				memset(row_name,'\0',20);
				sprintf(row_name,"%s",row[1]);

				if(modify_num==1)
					printf("databases%s\n",row_name);
				if((strcmp(row_name,client_name)==0))
				{
					for(i=0;i<MAX_CLIENT;i++)
		 			{ 
						if(row[i+2*MAX_CLIENT]==NULL)
			 			{ 
							num=2;
							break;
						}
					}
					if(i==MAX_CLIENT)
					{
						num=3;
					} 
					break;
		 		}
		 	}  
			if(row==NULL)
			{ 
				if(modify_num==1)
			   	{
					printf("用户名不存在！\n");
			  	}
				return -1;
			}  
			free(row_name);
			mysql_free_result(res); 
		}
		if(num==2)
		{   
			char query[50];
			memset(query,'\0',50);
			char resource;
			sprintf(query,"update staff_login set resource%d = 'resource%d' where name='%s'",i+1,i,client_name);
			if(mysql_query(&mysql,query)!=0)
		 	{  //执行插入语句 
				if(modify_num==1)
			 	{  
					printf("insert data error");
		  	 	}
		  	 }
			else
		  	{
				if(modify_num==1)
		 	 	{
					printf("insert data success");
		 	 	 }
			}    
			memset(query,'\0',50);                                                                  
			sprintf(query,"update staff_login set socket%d = %d where name='%s'",i+1,server_socket,client_name);
			if(mysql_query(&mysql,query)!=0)
			{  //执行插入语句 
				if(modify_num==1)
				{ 
					printf("insert data error");
		  	 	}
		  	} 
			else
		  	{ 
				if(modify_num==1)
		 		{
					printf("insert data success");
		 	 	 }
			}   
			xmlChar *xmlbuf_head="<iq type=\"result\" id=\"bind_2\">\n<bind xmlns=\"urn:ietf:params:xml:ns:xmpp-bind\">";
			xmlChar *xmlbuf_tail="</bind>\n</iq>";
			int xml_jid_size=strlen(client_name)+strlen("@im_server/")+strlen("resource")+2;
			char *xml_jid=(char *)malloc(xml_jid_size*sizeof(char));
			memset(xml_jid,'\0',xml_jid_size);
			sprintf(xml_jid,"%s@im_server/resource%d",client_name,i);
			sprintf(jid,"%s@im_server/resource%d",client_name,i);
			xmlbuf=xmlchar_construct(xmlbuf_head,xml_jid,xmlbuf_tail);
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
			free(xml_jid);
			return 0;
        } 
		if(num==3)
		{
			xmlbuf="<iq type=\"error\" id=\"bind_2\">\n<error type=\"cancel\">\n<not-allowed xmlns=\"urn:ietf:params:xml:ns:xmpp-stanzas\"/>\n</error>\n</iq>";
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
			return -1;
		}   

    } 

    return 0;
}

