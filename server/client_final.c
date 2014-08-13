/*************************************************************************
> File Name: client_final.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年07月12日 星期六 19时09分41秒

 ************************************************************************/

#include"server.h"
int client_final(int server_socket,MYSQL mysql,char *client_name)
{
    xmlChar* xmlbuf;
    xmlDocPtr doc;

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

    doc=xmlchar_to_doc(xmlbuf);
    free(xmlbuf);
    xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
    curNode = xmlDocGetRootElement(doc); //确定文档根元素

    if(NULL == curNode)
    {
        xmlFreeDoc(doc);
        return -1;
    }

    curNode = curNode->xmlChildrenNode; //子节点集是链
    xmlNodePtr propNodePtr = curNode;

    while(curNode != NULL)
    {
        //取出节点中的内容
        if(xmlStrcmp(curNode->name, (const xmlChar *)"text"))
            break;

        curNode = curNode->next; //子节点集是链表
    }

    //取出节点中的内容
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *row_name=(char *)malloc(20*sizeof(char));

    if((!xmlStrcmp(curNode->name, (const xmlChar *)"close")))
    {
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

		char server_socket_c[10];
		memset(server_socket_c,'\0',10);
		sprintf(server_socket_c,"%d",server_socket);
        while(row = mysql_fetch_row(res))
        {
            int i;
            char socket[10];
            memset(row_name,'\0',20);
            sprintf(row_name,"%s",row[1]);

            if(modify_num==1)
                printf("databases%s\n",row_name);

            if(strcmp(row_name,client_name)==0)
            {
                for(i=0; i<3; i++)
                {
					memset(socket,'\0',10);
					sprintf(socket,"%s",row[i+3]);
					if(strcmp(socket,server_socket_c)==0)
                    {
						break;
				 	}
                } 

                char query[100];
                memset(query,'\0',100);
                char resource;
                sprintf(query,"update staff_login set socket%d = NULL where name='%s'",i+1,client_name);

                if(mysql_query(&mysql,query)!=0)
                {
                    //执行插入语句
                    if(modify_num==1)
                    {
                        printf("insert data error");
                     }
                } 

                else
                { 
                    if(modify_num==1)
                     {
                        printf("insert data success");
                    }
                }

                memset(query,'\0',100);
                sprintf(query,"update staff_login set resource%d = NULL where name='%s'",i+1,client_name);

                if(mysql_query(&mysql,query)!=0)
                {
                    //执行插入语句
                    if(modify_num==1)
                    {
                        printf("insert data error");
                    }
                }

                else
                {
                    if(modify_num==1)
                    {
                        printf("insert data success");
                    }
                }
				break;
            } 
        }
    }

    free(row_name);
    mysql_free_result(res);
    xmlFreeDoc(doc);
    close(server_socket);
    return 0;
}


