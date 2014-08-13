/*************************************************************************
> File Name: remote_login.c
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年07月16日 星期三 21时06分45秒

 ************************************************************************/

#include"server.h"
int remote_login(int local_socket,char *jid,MYSQL mysql)
{
/*	printf("jid%s\n",jid); 
	int response_socket=0;
	xmlChar *xmlbuf_head="<message>";
	xmlChar *xmlbuf_tail="</message>";
	char *client_name=(char *)malloc(20*sizeof(char));
	char *resource=(char *)malloc(20*sizeof(char));
	memset(client_name,'\0',20);
	memset(resource,'\0',20);
	int i=0;
	printf("对因月明天");
	while(jid[i]!='@')
	{
		client_name[i]=jid[i];
		i++;
	} 
	client_name[i]='\0';
	printf("client_name%s",client_name);
	while(jid[i]!='/')
	{ 
		i++;
	}
	i++;
	int j=0;
	while(jid[i]!='\0')
	{
		resource[j]=jid[i];
		i++;
		j++;
	} 
	resource[j]='\0';
	printf("qiaohuancheng%s",resource);
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_real_query(&mysql,"set names utf8",(unsigned long)strlen("set names utf8"));
	char *query=(char *)malloc(50*sizeof(char));
	memset(query,'\0',50);
	sprintf(query,"select * from staff_login where name='%s'",client_name);     
	if(mysql_real_query(&mysql,query,(unsigned long)strlen(query)))
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
		printf("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		char *socket_tmp=(char *)malloc(10*sizeof(char));
		memset(socket_tmp,'\0',10);
		for(i=0;i<MAX_CLIENT;i++)                                                         
	 	{   
			char *resource_tmp=(char *)malloc(20*sizeof(char));
			memset(resource_tmp,'\0',20);
			if(row[i+2*MAX_CLIENT]==NULL)
				continue;
    		sprintf(resource_tmp,"%s",row[i+2*MAX_CLIENT]);
	//     	sprintf(resource_tmp,"%s",row[i+6]);
			if(strcmp(resource,resource_tmp)==0)
	 		{   
				break;
			}   
			free(resource_tmp);
		}
		printf("\n%s谁啊%d\n",resource,i);
		sprintf(socket_tmp,"%s",row[i+MAX_CLIENT]);
		int i=0;
		while(socket_tmp[i]!='\0')
		{
			response_socket=response_socket*10+socket_tmp[i]-'0';
			i++;
	 	}
		free(socket_tmp);
		printf("qiaoixnsheg%saa%d",socket_tmp,response_socket);
	} 
	mysql_free_result(res);
	free(query);
	free(client_name);
	free(resource);
	xmlChar *xmlbuf;
	xmlbuf=xmlchar_construct(xmlbuf_head,"open success",xmlbuf_tail);
	if(send_xml(xmlbuf,initiated_socket)==0)
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
	}
	fd_set rdfds,wtfds;
	struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0
	char *buffer=(char *)malloc(256*sizeof(char)); //256字节的接收缓冲区		 
	memset(buffer,'\0',256);
	while(1)
	{
		int exit_num=0;
		FD_ZERO(&rdfds); //每次循环都要清空集合，否则不能检测描述符变化
		FD_SET(initiated_socket,&rdfds); //添加描述符
		FD_SET(response_socket,&rdfds); //添加描述符
		int maxfdp=initiated_socket>response_socket?response_socket+1:response_socket+1; //描述符最大值加1
		switch(select(maxfdp,&rdfds,NULL,NULL,&timeout)) //select使用
		{
			case -1: exit(-1);break; //select错误，退出程
			case 0:break; //再次轮询
			default:
				   if(FD_ISSET(initiated_socket,&rdfds)) //测试sock是否可读，即是否网络上有数据
				   {
					   recv(initiated_socket,buffer,256,0);//接受网络数据
					   printf("接收发起端数据%s",buffer);
					   if(strcmp(buffer,"close")==0)
						   exit_num=1;
					   FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
					   FD_SET(response_socket,&wtfds); //添加描述符
					   select(response_socket+1,NULL,&wtfds,NULL,&timeout); //select使用
					   if(FD_ISSET(response_socket,&wtfds)) //测试文件是否可写
					   {
						   send(response_socket,buffer,256,0);//写入文件
						   printf("发送响应端数据！");
					   }
					   memset(buffer,'\0',256);
				   }// end if break;
				   if(FD_ISSET(response_socket,&rdfds)) //测试sock是否可读，即是否网络上有数据
				   {
					   recv(response_socket,buffer,256,0);//接受网络数据
					   printf("接收响应端数据%s",buffer);
					   FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
					   FD_SET(initiated_socket,&wtfds); //添加描述符
					   select(initiated_socket+1,NULL,&wtfds,NULL,&timeout); //select使用
					   if(FD_ISSET(initiated_socket,&wtfds)) //测试文件是否可写
					   { 
						   send(initiated_socket,buffer,256,0);//写入文件
						   printf("发送接收端数据！");
					   }
					   memset(buffer,'\0',256);
				   }// end if break; 
		}
		if(exit_num==1)
			break;
	}
	free(buffer);*/
	xmlChar *xmlbuf_head="<message>";
	xmlChar *xmlbuf_tail="</message>";
	xmlChar *xmlbuf;
	/* create a socket */  
	int server_socket= socket(AF_UNIX, SOCK_STREAM, 0);  

	struct sockaddr_un address;  
	address.sun_family = AF_UNIX;  
	sprintf(address.sun_path,"@%s",jid);  
    address.sun_path[0]=0;  
	    //server_len = sizeof(server_addr);  
    socklen_t server_len = strlen(jid)+1+offsetof(struct sockaddr_un, sun_path);  
		//
	/* connect to the server */  
	int result = connect(server_socket, (struct sockaddr *)&address,server_len);  
	if(result == -1)  
	{  
		perror("connect failed: ");  
		xmlbuf=xmlchar_construct(xmlbuf_head,"connect refuse",xmlbuf_tail);
		if(send_xml(xmlbuf,local_socket)==0)
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
		}
		return -1;
	}
	xmlbuf=xmlchar_construct(xmlbuf_head,"open success",xmlbuf_tail);
	if(send_xml(xmlbuf,local_socket)==0)
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
	} 

	fd_set rdfds,wtfds;
	struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0
	while(1)
	{
		int exit_num=0;
		FD_ZERO(&rdfds); //每次循环都要清空集合，否则不能检测描述符变化
		FD_SET(local_socket,&rdfds); //添加描述符
		FD_SET(server_socket,&rdfds); //添加描述符
		int maxfdp=local_socket>server_socket?local_socket+1:server_socket+1; //描述符最大值加1
		switch(select(maxfdp,&rdfds,NULL,NULL,&timeout)) //select使用
		{
			case -1: exit(-1);break; //select错误，退出程
			case 0:break; //再次轮询
			default:
				   if(FD_ISSET(local_socket,&rdfds)) //测试sock是否可读，即是否网络上有数据
				   {
					   if((xmlbuf=recv_xml(local_socket))!=NULL)
					   {   
						   if(modify_num==1)
						   {   
							   printf("客户端到服务器！接收%s成功！\n",xmlbuf);
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
				//	   recv(local_socket,buffer,256,0);//接受网络数据
				       xmlDocPtr doc;
					   doc = xmlReadMemory(xmlbuf,strlen(xmlbuf)+1,NULL,"utf-8",XML_PARSE_RECOVER);
					   //doc = xmlParseMemory(xmlbuf,strlen(xmlbuf)+1);
//					   doc=xmlchar_to_doc(xmlbuf);
					   xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
					   curNode = xmlDocGetRootElement(doc); //确定文档根元素
					   if(NULL == curNode)
					   { 
						   xmlFreeDoc(doc);
						   return -1;
					   }
					   if(xmlStrcmp(curNode->name, (const xmlChar *)"command")==0)
					   {
						   xmlChar *key;
						   key = xmlNodeListGetString(doc,curNode->xmlChildrenNode, 1); //获取文本结点的>文本，需用其子结点
						   if(xmlStrcmp(key,(const xmlChar *)"close")==0)
						   {
							   exit_num=1;
						   } 
						   xmlFree(key);
					   }
					   xmlFreeDoc(doc);

					   FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
					   FD_SET(server_socket,&wtfds); //添加描述符
					   select(server_socket+1,NULL,&wtfds,NULL,&timeout); //select使用
					   if(FD_ISSET(server_socket,&wtfds)) //测试文件是否可写
					   {
						   if(write_xml(xmlbuf,server_socket)==0)
						   {   
							   if(modify_num==1)
							   { 
								   printf("本地进程到对端进程！发送%s成功！\n",xmlbuf);
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

						  // send(server_socket,buffer,256,0);//写入文件
						   printf("发送响应端数据！");
					   }  
				   }// end if break;
				   if(FD_ISSET(server_socket,&rdfds)) //测试sock是否可读，即是否网络上有数据
				   { 
					   if((xmlbuf=read_xml(server_socket))!=NULL)
					    {    
						   if(modify_num==1)
						   {    
							   printf("对端进程到本地进程!接收%s成功！\n",xmlbuf);
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
					   printf("接收响应端数据%s",xmlbuf);
					   FD_ZERO(&wtfds); //每次循环都要清空集合，否则不能检测描述符变化
					   FD_SET(local_socket,&wtfds); //添加描述符
					   select(local_socket+1,NULL,&wtfds,NULL,&timeout); //select使用
					   if(FD_ISSET(local_socket,&wtfds)) //测试文件是否可写
					   { 
						   if(send_xml(xmlbuf,local_socket)==0)
						   {   
							   if(modify_num==1)
							   { 
								   printf("本地进程到客户端发送%s成功！\n",xmlbuf);
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

						  // send(server_socket,buffer,256,0);//写入文件
						   printf("发送接收端数据！");
					   }  
				   }// end if  break;  
		}
		if(exit_num==1)
		{
			close(server_socket);
			break;
		}
	} 
	xmlbuf=xmlchar_construct(xmlbuf_head,"close",xmlbuf_tail);
	if(send_xml(xmlbuf,local_socket)==0)
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

	return 0;
}



