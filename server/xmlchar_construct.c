/*************************************************************************
> File Name: xmlchar_to_doc.c
> Author: qxs
> Mail: qxs2222@163.com
> Created Time: 2014年06月17日 星期二 18时37分40秒

 ************************************************************************/

#include"server.h"
xmlChar *xmlchar_construct(xmlChar *xmlbuf_head,xmlChar *xmlbuf_tmp,xmlChar *xmlbuf_tail)
{
    int xmlbuf_size=strlen(xmlbuf_tail)+strlen(xmlbuf_tmp)+strlen(xmlbuf_head)+2;

    xmlChar *xmlbuf;
	while(1)
	{
		xmlbuf=xmlMalloc(xmlbuf_size*sizeof(char));
		if(xmlbuf_tmp == NULL)
		{
			continue;
		}    
		else
			break;
	}
	memset(xmlbuf,'\0',xmlbuf_size); 
	int num=0;
	int xmlbuf_len=strlen(xmlbuf_head);
	for(int i=0;i<xmlbuf_len;i++,num++)
	{
		xmlbuf[num]=xmlbuf_head[i];
	}
	xmlbuf_len=strlen(xmlbuf_tmp);
	for(int i=0;i<xmlbuf_len;i++,num++)
	{
		xmlbuf[num]=xmlbuf_tmp[i];
	} 
	xmlbuf_len=strlen(xmlbuf_tail);
	for(int i=0;i<xmlbuf_len;i++,num++)
	{
		xmlbuf[num]=xmlbuf_tail[i];
	}  
	xmlbuf[num]='\0';

/*	xmlStrcat(xmlbuf,xmlbuf_head);

	xmlStrcat(xmlbuf,BAD_CAST "\n");
    xmlStrcat(xmlbuf,xmlbuf_tmp);
    xmlStrcat(xmlbuf,BAD_CAST "\n");
    xmlStrcat(xmlbuf,xmlbuf_tail);*/
//    xmlDocPtr  doc= xmlParseMemory(xmlbuf,xmlbuf_size+1);
    return xmlbuf;
}

