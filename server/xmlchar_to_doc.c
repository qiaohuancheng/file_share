/*************************************************************************
> File Name: xmlchar_to_doc.c
> Author: qxs> Mail: qxs2222@163.com
> Created Time: 2014年06月17日 星期二 18时37分40秒

 ************************************************************************/

#include"server.h"
xmlDocPtr xmlchar_to_doc(xmlChar *xmlbuf_tmp)
{
    xmlChar *xmlbuf_head="<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" from=\"im_server.com\" version=\"1.0\">";
    xmlChar *xmlbuf_tail="</stream:stream>";
    int xmlbuf_size=strlen(xmlbuf_tail)+strlen(xmlbuf_tmp)+strlen(xmlbuf_head)+4;

    xmlChar *xmlbuf;
	while(1)
	{
		xmlbuf=xmlMalloc(xmlbuf_size*sizeof(xmlChar));
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
	xmlbuf[num]='\n';
	num++;
	xmlbuf_len=strlen(xmlbuf_tmp);
	for(int i=0;i<xmlbuf_len;i++,num++)
	{
		xmlbuf[num]=xmlbuf_tmp[i];
	}
	xmlbuf[num]='\n';
	num++;
	xmlbuf_len=strlen(xmlbuf_tail);
	for(int i=0;i<xmlbuf_len;i++,num++)
	{
		xmlbuf[num]=xmlbuf_tail[i];
	} 
	xmlbuf[num]='\0';

/*    xmlStrcat(xmlbuf,xmlbuf_head);
    xmlStrcat(xmlbuf,BAD_CAST "\n");
    xmlStrcat(xmlbuf,xmlbuf_tmp);
    xmlStrcat(xmlbuf,BAD_CAST "\n");
    xmlStrcat(xmlbuf,xmlbuf_tail);*/
    xmlDocPtr  doc= xmlParseMemory(xmlbuf,strlen(xmlbuf)+1);
	if(xmlbuf!= NULL)
		xmlFree(xmlbuf);
    return doc;
}

