/*************************************************************************
> File Name: server_tsl.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年06月14日 星期六 09时44分57秒

 ************************************************************************/

#include "server.h"
int server_sasl(int server_socket,MYSQL mysql,char *client_name)
{
    int suc_mark=0;
    xmlChar* xmlbuf;
    xmlDocPtr doc;
	int stream_on=0;
	int num=0;
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
		stream_on=3;
    }  
    else
    { 
		if(modify_num==1)
		{
			printf("发送%s失败！\n",xmlbuf);
	  	}
        return -1;
    } 

    xmlbuf="<stream:features>\n  <mechanisms xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n  <mechanism>DIGEST-MD5</mechanism>\n  <mechanism>PLAIN</mechanism>\n  <mechanism>EXTERNAL</mechanism>\n</mechanisms>\n</stream:features>";

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

		doc= xmlParseMemory(xmlbuf,strlen(xmlbuf));
		free(xmlbuf);
        xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
        curNode = xmlDocGetRootElement(doc); //确定文档根元素
        if(NULL == curNode)
        {
            xmlFreeDoc(doc);
            return -1;
        }
		//取出节点中的内容
		if((!xmlStrcmp(curNode->name, (const xmlChar *)"auth")))
		{
			xmlChar *xml_buf = xmlNodeGetContent(curNode);
			xmlChar *xml_name_buf=(char *)malloc(20*sizeof(char));
			memset(xml_name_buf,'\0',20);
			char *xml_buf_p=xml_buf;
			int i=0;
			while(*xml_buf_p!='\n')
			{
				xml_name_buf[i]=*xml_buf_p;
				xml_buf_p++;
				i++;
			}
			xml_buf_p++;
			char *xml_md5_buf=(char*)malloc(33*sizeof(char));
			memset(xml_md5_buf,'\0',33);
			i=0;
			while(i!=33)
			{
				xml_md5_buf[i]=*xml_buf_p;
				xml_buf_p++;
				i++;
			} 
			xml_md5_buf[i]='\0';
			xmlFree(xml_buf);
			if(modify_num==1)
			{
				printf("%d\n",strlen(xml_name_buf));
				printf("用户名：%s\n",xml_name_buf);
		 	} 
			// 重复读取行，并输出第一个字段的值，直到row为NULL
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
				char *row_md5_buf=MD5_sign(row[2],strlen(row[2]));
				if(modify_num==1)
					printf("databases%s\n",row_name);
				if((strcmp(row_name,xml_name_buf)==0)&&(strcmp(row_md5_buf,xml_md5_buf)==0))
				{
					num=1;
					break;
			 	}
				else
				{
					if(strcmp(row_name,xml_name_buf)==0)
					{
						num=3;
						break;
		 			}
					else
						continue;
			 	}
			}  
			if(row==NULL)
				num=2;
			else
			{
				char *row_name_p=row_name;
				i=0;
				while(*row_name_p)
				{
					client_name[i]=*row_name_p;
					row_name_p++;
					i++;
				}
			}
			client_name[i]='\0';
			free(row_name);
			free(xml_md5_buf);
			free(xml_name_buf);
			mysql_free_result(res);
		}  
		if((!xmlStrcmp(curNode->name, (const xmlChar *)"response")))
		{  
			xmlChar *xml_md5_buf = xmlNodeGetContent(curNode);
			// 重复读取行，并输出第一个字段的值，直到row为NULL
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
				if((strcmp(row_name,client_name)==0))
				{
					char *row_md5_buf=MD5_sign(row[2],strlen(row[2])); 
					if(strcmp(xml_md5_buf,row_md5_buf)==0)
		 			{
						num=1;
						break;
		 		 	}
		 			else
		 			{
						num=3;
						break;
		 		 	}
		  	 	} 
		   	}  
			if(row==NULL)
				num=2;

			free(row_name);
			xmlFree(xml_md5_buf);
			mysql_free_result(res);
		} 
		xmlFreeDoc(doc);
		if(num==1)
          { 
            //初始化实体必须(MUST)验证接收实体出示的证书
            //如果失败则发送<failure xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>
            //</stream:stream>
         //   xmlbuf="<starttls xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";
        //    xmlbuf="<proceed xmlns=\"urn:ietf:params:xml:ns:xmpp-tls\"/>";
			xmlbuf="<success xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\"/>";

            if(send_xml(xmlbuf,server_socket)==0)
              {
				if(modify_num==1)
		 		{
					printf("发送%s成功！\n",xmlbuf);
	 	   	 	}
				stream_on=0;
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
		if(num==2)
		{  
			xmlbuf="<failure xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n<temporary-auth-failure/>\n</failure>";
			if(send_xml(xmlbuf,server_socket)==0)
	 	 	{
				if(modify_num==1)
	 	 		{
					printf("发送%s成功！\n",xmlbuf);
		 		}                                                                                
				stream_on--;
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
		if(num==3)
 	 	{   
			xmlbuf="<challenge xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n<temporary-auth-failure/>\n</challenge>";
		 	if(send_xml(xmlbuf,server_socket)==0)
 		 	{
				if(modify_num==1)
 		  		{
					printf("发送%s成功！\n",xmlbuf);
				}                                                                           
				stream_on--;
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
/*		if(stream_on>0)
		{
			xmlbuf="<stream:features>\n  <mechanisms xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n  <mechanism>DIGEST-MD5</mechanism>\n  <mechanism>PLAIN</mechanism>\n  <mechanism>EXTERNAL</mechanism>\n</mechanisms>\n</stream:features>";                                                                           
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
		}
		else
			return -1;*/

		if(stream_on==0)
			return -1;


/*		if(stream_on>0)
		{
			xmlbuf="<stream:features>\n  <mechanisms xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\">\n  <mechanism>DIGEST-MD5</mechanism>\n  <mechanism>PLAIN</mechanism>\n  <mechanism>EXTERNAL</mechanism>\n</mechanisms>\n</stream:features>";                                                                           
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
			} */

	}

	return 0;
}



