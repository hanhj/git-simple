/*
 * File Name: a.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-05-07 17:43:23 week:4
*/
#include<stdio.h>
#include<libxml/parser.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <list>
using namespace std;
#define MAGIC_STR "123456789"
typedef unsigned long u32;
struct user_data{
	u32 base;
	int size;//1:8 bit;2:16 bit;4:32 bit
	int num;
	u32 *data;
};
typedef struct _emif_req{
	u32 base;
	int size;//1:8 bit;2:16 bit;4:32 bit
	int num;
	u32 data[10];
	int rw;//0 read,1 write
}emif_req;

typedef list<emif_req*> list_eq;
list_eq l_eq;
void free_list(void){
	emif_req *eq;
	list_eq::iterator it;
	it=l_eq.begin();
	while(it!=l_eq.end()){
		eq=*it;
		delete eq;
		it++;
	}
}
static int build_emif_req(xmlNodePtr node, emif_req *eq)
{
	if(eq!=NULL){
		eq->base=0;
		eq->size=1;
		eq->num=1;
		eq->rw=0;
	}
	while (node) {
		if (xmlStrcmp(node->name, BAD_CAST"base") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->base=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}

		if (xmlStrcmp(node->name, BAD_CAST"size") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->size=strtol((char*)content,0,10);//format is xxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"num") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->num=strtol((char*)content,0,10);//format is xxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"rw") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->rw=strtol((char*)content,0,10);//format is xxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value1") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[0]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value2") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[1]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value3") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[2]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value4") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[3]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value5") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[4]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value6") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[5]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value7") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[6]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value8") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[7]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value9") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[8]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"value10") == 0) {
			xmlChar *content = xmlNodeGetContent(node);
			eq->data[9]=strtol((char*)content,0,0);//format is 0xxxxx
			xmlFree(content);
		}
		if (xmlStrcmp(node->name, BAD_CAST"emif_req") == 0) {
			eq=new emif_req;
			l_eq.push_back(eq);
			build_emif_req(node->children,eq);
		}
		node = node->next;
	}
	return 0;
}
static int init_emif_req(const char *filename){
	xmlDocPtr doc;
	xmlNodePtr node;
	xmlNodePtr root;
	emif_req  *eq;
	struct user_data* user_data;
	eq=NULL;
	doc=xmlReadFile(filename,"UTF-8",XML_PARSE_RECOVER);
	if(doc==NULL)
		return -1;
	root=xmlDocGetRootElement(doc);
	if(doc==NULL){
		xmlFreeDoc(doc);
		return -1;
	}
	while(root){
		if(xmlStrcmp(root->name,BAD_CAST"emif_body")==0){
			build_emif_req(root->children,eq);
		}
		root=root->next;
	}
	list_eq::iterator it;
	it=l_eq.begin();
	while(it!=l_eq.end()){
		eq=*it;
		printf("eq:base:%lx,num:%d,rw:%d,size:%d\n",eq->base,eq->num,eq->rw,eq->size);
		it++;
	}
	return 0;
}

int main(int argc,char **argv){
	if(argc==1)
		init_emif_req("a.xml");
	else
		init_emif_req(argv[1]);
	free_list();
	return 0;
}
// vim:tw=72 
