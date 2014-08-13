/*************************************************************************
> File Name: send_xml.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 11时19分23秒

 n***********************************************************************/
#include"client.h"
int send_xml(xmlChar* xmlbuf,int client_socket)
{
	int xml_char_size;  
	xml_char_size=strlen((char*)xmlbuf);
	char *send_buf=(char*)malloc((xml_char_size+MAXLINE+1)*sizeof(char));
	memset(send_buf,'\0',xml_char_size+MAXLINE+1);
	sprintf(send_buf,"%d",xml_char_size);                               
	sprintf(send_buf+MAXLINE,"%s",xmlbuf);
	if(send(client_socket,send_buf,xml_char_size+MAXLINE+1,0) == -1)
	{
		return -1;                                                                              
	}
	free(send_buf);
	return 0;
}

