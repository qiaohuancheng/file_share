/*************************************************************************
> File Name: client.h
> Author: qxs
> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 11时16分27秒

 ************************************************************************/
#ifndef client
#define client
#include "stdio.h"
#include <stdio.h> 
#include <strings.h>  //for bzero
#include <string.h>  //for bzero
#include <stdlib.h>  
#include <sys/types.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  //for htonl
#include <libxml/parser.h>  
#include <libxml/xmlmemory.h> 
#include <arpa/inet.h> //for inet_addr
#include <iconv.h> 
#include <sys/times.h>
#include <sys/select.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h> 
#include "md5.h"
#include <sys/stat.h>
#include<sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4 //socket，固定长度接收buffer
#define BAD_CAST (xmlChar *)
#define modify_num 1
#define HASHSIZE       16 //for md5
int connect_to_client(int *client_socket);
int send_xml(xmlChar* xmlbuf,int client_socket);
xmlChar* recv_xml(int server_socket);
int client_tls(int client_socket);
xmlDocPtr xmlchar_to_doc(xmlChar *xmlbuf_tmp);
xmlChar* xmlchar_construct(xmlChar *xmlbuf_head,xmlChar *xmlbuf_tmp,xmlChar *xmlbuf_tail);
//void md5 (const char *message, long len, char *output);
int client_sasl(int client_socket,char *client_name);
int client_bind(int client_socket,char *client_name,char *jid);
int client_final(int client_socket);
#endif
