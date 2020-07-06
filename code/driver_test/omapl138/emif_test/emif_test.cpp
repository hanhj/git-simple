/*
 * File Name: test.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-04-26 23:29:45 week:0
*/
#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
#include<libxml/parser.h>
#include <list>
#include<iostream>
#define SET_OUTPUT_LOW 10
#define SET_OUTPUT_HIGH 11
#define GET_VALUE 12
#define SET_INPUT 13
#define SET_VALUE 14
using namespace std;
typedef unsigned long u32;
typedef struct _user_data{
	u32 base;
	int size;//1:8 bit;2:16 bit;4:32 bit
	int num;
	u32 *data;
}user_data;
typedef struct _emif_req{
	u32 base;
	int size;
	int num;
	int rw;//0 read,1 write
	u32 value[10];
}emif_req;
typedef list<emif_req*> list_eq;
user_data u_d;
u32 data_buf[10];
list_eq l_eq;

void free_list(void){
	emif_req * eq;
	list<emif_req*>::iterator it;
	it=l_eq.begin();
	while(it!=l_eq.end()){
		eq=*it;
		it++;
		delete eq;
	}
}
static int build_emif_req(xmlNodePtr node,emif_req *eq){
	if(eq!=NULL){
		eq->base=0;
		eq->size=1;
		eq->num=1;
		eq->rw=0;
	}
	while(node){
		if (xmlStrcmp(node->name,BAD_CAST"base")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->base=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"rw")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->rw=strtol((char*)content,0,10);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"size")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->size=strtol((char*)content,0,10);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"num")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->num=strtol((char*)content,0,10);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value1")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[0]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value2")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[1]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value3")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[2]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value4")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[3]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value5")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[4]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value6")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[5]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value7")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[6]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value8")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[7]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value9")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[8]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"value10")==0){
			xmlChar *content= xmlNodeGetContent(node);
			eq->value[9]=strtol((char*)content,0,0);
			xmlFree(content);
		}
		if (xmlStrcmp(node->name,BAD_CAST"emif_req")==0){
			eq=new emif_req;
			l_eq.push_back(eq);
			build_emif_req(node->children,eq);
		}
		node=node->next;
	}
	return 0;
}

int init_emif_req(const char *filename){
	xmlDocPtr doc;
	xmlNodePtr node;
	xmlNodePtr root;
	emif_req * eq;
	eq=NULL;
	doc=xmlReadFile(filename,"UTF-8",XML_PARSE_RECOVER);
	if(doc==NULL)
		return -1;
	root=xmlDocGetRootElement(doc);
	if(doc==NULL){
		xmlFreeDoc(doc);
		return -1;
	}
	eq=new emif_req;
	while(root){
		if(xmlStrcmp(root->name,BAD_CAST"emif_body")==0){
			build_emif_req(root->children,eq);
		}
		root=root->next;
	}
	list<emif_req*>::iterator it;
	it=l_eq.begin();
	while(it!=l_eq.end()){
		eq=*it;
		printf("eq:base:%lx,num:%d,rw:%d,size:%d\n",eq->base,eq->num,eq->rw,eq->size);
		it++;
	}
	return 0;
}
int main(int argc,char **argv){
	int fd;
	emif_req *eq;
	fd=open("/dev/test",O_RDWR);
	if(fd<0){
		perror("open  fail\n");
	}
	if(argc==1){
		u_d.base=0x68000000;
		u_d.num=1;
		u_d.size=4;
		u_d.data=&data_buf[0];
		ioctl(fd,GET_VALUE,&u_d);
		printf("get  addr:%lx,data:%lx\n",u_d.base,u_d.data[0]);
	}else if(argc>=2){
		int i;
		int count;
		count=100;
		init_emif_req(argv[1]);
		list<emif_req*>::iterator it;
		if(argc==3){
			count=atoi(argv[2]);
		}
		while(count){
			it=l_eq.begin();
			while(it!=l_eq.end()){
				eq=*it;
				u_d.base=eq->base;
				u_d.num=eq->num;
				u_d.size=eq->size;
				if(eq->rw==0){//GET_VALUE
					u_d.data=&data_buf[0];
					ioctl(fd,GET_VALUE,&u_d);
					for(i=0;i<u_d.num;i++){
						printf("get  addr:%lx,data:%lx\n",u_d.base+i*u_d.size,u_d.data[i]);
					}
				}else if(eq->rw==1){//SET_VALUE
					u_d.data=&data_buf[0];
					for(i=0;i<u_d.num;i++){
						data_buf[i]=eq->value[i];
						printf("set  addr:%lx,data:%lx\n",u_d.base+i*u_d.size,u_d.data[i]);
					}
					ioctl(fd,SET_VALUE,&u_d);
				
				}
				it++;
			}
			sleep(2);
			count--;
			printf("circle %d\n",count);
		}
	}
	free_list();
	close(fd);
	return 0;
}
// vim:tw=72 
