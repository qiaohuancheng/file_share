/*************************************************************************
> File Name: client_final.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年07月12日 星期六 18时54分31秒

 ************************************************************************/

#include"client.h"
int client_final(int client_socket)
{
	char *xmlbuf;
	xmlbuf="<close xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" to=\"im_server.com\" />";
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

	close(client_socket);
	return 0;
}
