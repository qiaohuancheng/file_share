/*************************************************************************
> File Name: recv_xml.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 11时25分04秒

 ************************************************************************/
#include"server.h"
xmlChar* recv_xml(int server_socket)
{
	char char_buf[MAXLINE];
	memset(char_buf,'\0',MAXLINE); 
	int length = recv(server_socket,char_buf,MAXLINE,0);
	if (length < 0)
	{   
		return NULL;
	} 
	int xml_char_size=atoi(char_buf)+1;
	char *xml_char_buf=(char *)malloc(xml_char_size*sizeof(char));
	memset(xml_char_buf,'\0',xml_char_size); 
	length = recv(server_socket,xml_char_buf,xml_char_size,0);
	if (length < 0)
	{   
		 return NULL;
	} 
	return (xmlChar *)xml_char_buf;
}


