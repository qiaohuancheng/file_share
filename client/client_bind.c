/*************************************************************************
> File Name: client_tsl.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分47秒

 ************************************************************************/

#include"client.h"
int client_bind(int client_socket,char *client_name,char *jid)
{
    xmlDocPtr doc;
    xmlChar *xmlbuf;
    int stream_on=0;

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
		while(curNode != NULL)
		{ 
			//取出节点中的内容
	        if(xmlStrcmp(curNode->name, (const xmlChar *)"text")) //跳过text节点
				break;
			curNode = curNode->next; //子节点集是链表	    
		} 

        int num_t=0;
		//取出节点中的内容
        if((!xmlStrcmp(curNode->name, (const xmlChar *)"features")))
		{
			num_t=1;
        }
        if((!xmlStrcmp(curNode->name, (const xmlChar *)"iq")))
		{
			xmlAttrPtr attrPtr = curNode->properties; //属性集是链表
			while (attrPtr != NULL)
		    {
				if (!xmlStrcmp(attrPtr->name, BAD_CAST "type"))
				{
					xmlChar* xml_type = xmlGetProp(curNode,BAD_CAST "type");
					if (!xmlStrcmp(xml_type, BAD_CAST "result"))
					{
						free(xml_type);
						xmlChar* xml_jid = xmlNodeGetContent(curNode);
						sprintf(jid,"%s",xml_jid);
						xmlFree(xml_jid);
						num_t=2;
						break;
					}
					if (!xmlStrcmp(xml_type, BAD_CAST "error"))
					{
						printf("%s绑定资源数过多...\n",client_name);
						free(xml_type);
						num_t=3;
						break;
					}
					else
					{
						free(xml_type);
						num_t=3;
						break;
					}
				}
				attrPtr = attrPtr->next;
			}
		}
		xmlFreeDoc(doc);
        if(num_t==1)
        {
            xmlbuf="<iq type=\"set\" id=\"bind_1\">\n<bind xmlns=\"urn:ietf:params:xml:ns:xmpp-bind\"/>\n</iq>";
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
		if(num_t==3)
			return -1;
		if(num_t==2)
			break;
	}
    return 0;
}


