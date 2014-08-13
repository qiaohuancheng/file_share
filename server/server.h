/*************************************************************************
> File Name: server.h
> Author: qxs> Mail: qxs2222@163.com 
> Created Time: 2014年06月12日 星期四 11时04分16秒

 ************************************************************************/
#ifndef server
#define server

#include <unistd.h>/*#包含<unistd.h>*/ //fork exec 
#include <sys/types.h>/*#包含<sys/types.h>*/
#include <stdio.h>
#include <mysql.h> 
#include <stdio.h>  //for printf
#include <stdlib.h>  //for exit
#include <strings.h>  //for bzero
#include <ctype.h>  
#include <sys/socket.h>  //for socket
#include <netinet/in.h>  //for sockaddr_in
#include <arpa/inet.h>  
#include <libxml/parser.h>  //for libxml2
#include <libxml/xmlmemory.h>  
#include<sys/wait.h>
#include <string.h> 
#include <sys/times.h>
#include <sys/select.h>
#include <sys/un.h> //进程间通信SOCKET
#include <stddef.h> //offsetof
#include"md5.h"
#define modify_num 1 //调试时，输出信息，设为0时，则正常输出
#define BAD_CAST (xmlChar *)
#define MAXLINE 4
#define LENGTH_OF_LISTEN_QUEUE 20
#define HASHSIZE       16 //for md5
#define port 6536 
#define MAX_CLIENT 3
xmlChar* recv_xml(int server_socket);
int send_xml(xmlChar* xmlbuf,int client_socket);
xmlChar* read_xml(int server_socket);
int write_xml(xmlChar* xmlbuf,int client_socket);
int server_init(int *server_socket);
int server_tls(int server_socket);
xmlDocPtr xmlchar_to_doc(xmlChar *xmlbuf);
//void md5 (const char *message, long len, char *output);
int server_sasl(int server_socket,MYSQL mysql,char *client_name);
int server_bind(int server_socket,MYSQL mysql,char *client_name,char *jid);
xmlChar* xmlchar_construct(xmlChar *xmlbuf_head,xmlChar *xmlbuf_tmp,xmlChar *xmlbuf_tail);
int client_final(int server_socket,MYSQL mysql,char *client_name);
int client_ll(int server_socket,MYSQL mysql);
int remote_login(int initiated_socket,char *jid,MYSQL mysql);
int message_parsing(int server_socket,int *exit_num,MYSQL mysql_conn);
#endif
