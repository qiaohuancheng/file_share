/*************************************************************************
> File Name: client_ll.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年07月15日 星期二 21时33分35秒

 ************************************************************************/

#include"server.h"
int client_ll(int server_socket,MYSQL mysql)
{
	char *client_on=(char *)malloc(256*sizeof(char));
	memset(client_on,'\0',256);
	xmlChar *xmlbuf_head="<message>";
	xmlChar *xmlbuf_tail="</message>";
	char *jid=(char *)malloc(50*sizeof(char));
	xmlChar* xmlbuf;
    MYSQL_RES *res;
    MYSQL_ROW row;
	mysql_real_query(&mysql,"set names utf8",(unsigned long)strlen("set names utf8"));
	if(mysql_real_query(&mysql,"select * from staff_login where socket1 is not null or socket2 is not null or socket3 is not null",(unsigned long)strlen("select * from staff_login where socket1 is not null or socket2 is not null or socket3 is not null")))   
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
		for(int i=0;i<MAX_CLIENT;i++)                                                                  
		{ 
			if(row[i+2*MAX_CLIENT]!=NULL)
			{ 
				memset(jid,'\0',50);
				sprintf(jid,"%s@im_server/%s\n",row[1],row[i+6]);
				if((strlen(client_on)%256+strlen(jid))>=256)
				{ 
					 client_on=(char*)realloc(client_on,(strlen(client_on)+strlen(jid)+256)*sizeof(char));
				}
				strcat(client_on,jid);
			}
		}
	}
	xmlbuf=xmlchar_construct(xmlbuf_head,client_on,xmlbuf_tail);
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
	free(client_on);
	mysql_free_result(res); 
	free(jid);
	return 0;
}

