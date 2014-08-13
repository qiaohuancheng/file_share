/*************************************************************************
> File Name: client_tsl.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分47秒

 ************************************************************************/

#include"client.h"
int client_sasl(int client_socket,char *client_name_tmp)
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
			printf("发送%s成功！\n",xmlbuf);
		}
		free(xmlbuf);
		stream_on=3;
    }

    else
    {
		if(modify_num==1)
		{
			printf("发送%s！失败\n",xmlbuf);
	 	}
        return -1;
    } 

    while(stream_on)
    {
        if((xmlbuf=recv_xml(client_socket))!=NULL)
         {
            printf("接收%s成功！\n",xmlbuf);
        }

        else
         {
			if(modify_num==1)
		 	{
				printf("接收%s失败！\n",xmlbuf);
 			}
            continue;
         }

        xmlDocPtr doc;
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
                    if((!xmlStrcmp(curNode->name, (const xmlChar *)"mechanisms")))
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
				if(!xmlStrcmp(curNode->name, (const xmlChar *)"success"))
				{
					stream_on=0;
					return 0;
		 		}
				else
				{
					if(!xmlStrcmp(curNode->name, (const xmlChar *)"challenge"))
					{
						stream_on--;
						num_t=2;
						printf("密码验证失败，请重试！\n");
						break;
					}
					else
					{
						if(!xmlStrcmp(curNode->name, (const xmlChar *)"failure"))
						{
							num_t=1;
							stream_on--;
							printf("认证失败，请重试！\n");
							break;
						}
						else
						{
							curNode=curNode->next; 
						}
					}

		 		}
		 	}
         } 
		xmlFreeDoc(doc);
		if(stream_on==0)
			return -1;
        if(num_t==1)
         {  
			char *client_name=(char*)malloc(20*sizeof(char));
			char *client_name_p=client_name;      
			printf("请输入用户名：\n");
			fgets(client_name_p,20,stdin);
			client_name[strlen(client_name)-1]='\0';
			printf("所输入的用户名为：%s\n",client_name);
			char *client_password=(char*)malloc(20*sizeof(char));
			char *client_password_p=client_password;          
			printf("请输入密码：\n");
			fgets(client_password_p,20,stdin);
			client_password[strlen(client_password)-1]='\0';
			char *md5_buf=MD5_sign(client_password,strlen(client_password));
			char *send_buf=(char *)malloc((strlen(client_name)+34)*sizeof(char));
			memset(send_buf,'\0',strlen(client_name)+34);
			strcat(send_buf,client_name);
			send_buf[strlen(client_name)]='\n';
			send_buf[strlen(client_name)+1]='\0';
			strcat(send_buf,md5_buf);
			xmlChar *xmlbuf_head="<auth xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\" mechanism=\"DIGEST-MD5\">";
			xmlChar *xmlbuf_tail="</auth>";
			xmlbuf=xmlchar_construct(xmlbuf_head,send_buf,xmlbuf_tail);
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
			char *row_name_p=client_name;
			int i=0;
			while(*row_name_p)
			{                                                               
				client_name_tmp[i]=*row_name_p;
				row_name_p++;
				i++;
		 	}   
			client_name_tmp[i]='\0';
			free(send_buf);
			free(client_name);
			free(client_password);
		 }  
        if(num_t==2)
        {
			char *client_password=(char*)malloc(20*sizeof(char));  
			printf("请输入密码：\n");
			fgets(client_password,20,stdin);
			client_password[strlen(client_password)-1]='\0';
			char *md5_buf=(char *)malloc(33*sizeof(char));
			md5_buf=MD5_sign(client_password,strlen(client_password));
			xmlChar *xmlbuf_head="<response xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\" mechanism=\"DIGEST-MD5\">";
			xmlChar *xmlbuf_tail="</response>";
			xmlbuf=xmlchar_construct(xmlbuf_head,md5_buf,xmlbuf_tail);
            if(send_xml(xmlbuf,client_socket)==0)
            {
				if(modify_num==1)
				{
					printf("发送%s成功！\n",xmlbuf);
		 		}
				xmlFree(xmlbuf);
                continue;
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
			free(client_password);
			free(md5_buf);
		}
    }   
    return 0;
}


