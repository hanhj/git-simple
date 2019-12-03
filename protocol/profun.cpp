/*
 * File Name: profun.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-12-03 17:01:01 week:2
*/
#include<iostream>
using namespace std;
#include "protocol.h"
#include "configurable.h"
#include "para.h"
#include "profun.h"
int g_reset=0;
/****************************
 * realize extern interface functions for protocol
****************************/
SORT_YX_TAB * get_yx_data(int pos){
	SORT_YX_TAB *p;
	p=&SortYxTable[pos];
	pfunc(DEBUG_INFO,"\n");
	return p;
}
YC_TAB * get_yc_data(int pos){
	YC_TAB *p;
	p=&YcTable[pos];
	pfunc(DEBUG_INFO,"\n");
	return p;
}
YC_TAB * get_acc_yc_data(int pos){
	YC_TAB *p;
	pos+=config_scada_data.pos_acc;
	p=&YcTable[pos];
	pfunc(DEBUG_INFO,"\n");
	return p;
}
int get_event_data(int port,event *&e,int change){
	int ret;
	ret=0;
	pfunc(DEBUG_INFO,"\n");
	EventList::iterator end,it;
	end=event_list.end();
	it=event_list.begin();
	while(it!=end){
		if(it->readflag[port]==0){
			if(change)
				it->readflag[port]=1;
			e=&it;
			ret=1;
			break;
		}
		it++;
	}
	return ret;
}
int get_clock(CP56Time2a &time){
	pfunc(DEBUG_INFO,"\n");
	return 0;
}
//type:0 execute;1:check;
int do_yk(int id,int type,int cmd){
	pfunc(DEBUG_ERROR,"do yk:id:%x\n",id);
	if(type == 0){
		pfunc(DEBUG_ERROR,"do execute:%x\n",cmd);
	}else if(type == 1){
		pfunc(DEBUG_ERROR,"do select:%x\n",cmd);
	}
	return 0;
}
void do_reset(){
	g_reset=1;
}
int get_yc_cg_data(int port,event_yc *&e){
	int ret;
	ret=0;
	EventYcList::iterator it,end;
	it=event_yc_list.begin();
	end=event_yc_list.end();
	while(it!=end){
		if(it->readflag[port]==0){
			it->readflag[port]=1;
			e=&it;
			ret=1;
			break;
		}
		it++;
	}
	pfunc(DEBUG_INFO,"\n");
	return ret;
}
int get_dir_data(_rd_dir *dir){
	pfunc(DEBUG_INFO,"\n");
	dir_list *list;
	list=&dir->res_list;
	list->clear();
	list->init(10);
	dir_node node;
	int n;
	n=0;
	dir_list::iterator it(g_dir_list.MaxQueue);
	it=g_dir_list.begin();
	while(it!=g_dir_list.end()){
		node=*it;
		list->push(node);
		it++;
		n++;
	}
	//dir->res_list=g_dir_list;
	return n;
}
int get_file_data(_rd_file *file){
	pfunc(DEBUG_INFO,"\n");
	int ret;
	ret=-1;
	dir_list::iterator it(g_dir_list.MaxQueue);
	it=g_dir_list.begin();
	while(it!=g_dir_list.end()){
		if(it->file_id==file->req_file.file_id){
			ret=1;
			file->res_file=*it;
			break;
		}
	}
	return ret;
}
int get_file_segment(_rd_file*file){
	FILE *f;
	int ret;
	ret=-1;
	int ret_len;
	f=fopen(file->req_file.name,"r");
	if(!f)
	return ret;
	fseek(f,file->cur_offset,SEEK_SET);
	ret_len=fread(file->segment.data,200,1,f);
	if(ret_len<200){
		ret=0;
		file->con=0;
	}
	else{
		ret=1;
		file->con=1;
	}
	file->segment.len=ret_len;
	file->step=file->con;
	fclose(f);
	pfunc(DEBUG_INFO,"\n");
	return ret;
}
void load_file_list(){
	dir_node node;
	FILE *f;
	f=fopen("file_list.dat","r");
	if(f==NULL)
		return;
	g_dir_list.clear();
	char buff[1000];
	char time[30];

	while(!feof(f)){
		fgets(buff,1000,f);
		sscanf(buff,"%ld %s %ld %ld %s",&node.name_len,&node.name[0],&node.file_id,&node.file_size,&time[0]);
		g_dir_list.push(node);
		printf("%ld %s %ld %ld %s\n",node.name_len,&node.name[0],node.file_id,node.file_size,&time[0]);
	}
}
void save_file_list(){
	dir_node node;
	dir_list::iterator it(g_dir_list.MaxQueue);
	FILE *f;
	f=fopen("file_list.dat","wb");
	if(f==NULL)
		return;
	while(it!=g_dir_list.end()){
		node=*it;
		fwrite(&node,sizeof(node),1,f);
		it++;
	}
	fclose(f);
}
int save_file_data(_rd_file *file){
	pfunc(DEBUG_INFO,"\n");
	int ret;
	int find;
	dir_node node;
	ret=1;
	find=0;
	FILE *f;
	if(file->result==FILE_LONGNAME_ERROR)
		return ret;
	dir_list::iterator it(g_dir_list.MaxQueue);
	it=g_dir_list.begin();
	while(it!=g_dir_list.end()){
		if(it->file_id == file->req_file.file_id){
			node=*it;
			find=1;
			break;
		}
		it++;
	}
	f=fopen(file->req_file.name,"w");
	if(f==NULL){
		file->result=FILE_UNKNOWN_ERROR;//file error
		return ret;
	}
	fclose(f);
	file->file_size=0;
	if(find==0){
		strcpy(node.name,file->req_file.name);
		node.name_len=strlen(file->req_file.name);
		node.file_id=file->req_file.file_id;
		node.file_size=file->req_file.file_size;
		CP56Time2a time;
		get_clock(time);
		node.time=time;
		g_dir_list.push(node);
		save_file_list();
	}
	return ret;
}
int save_file_segment(_rd_file*file){
	int ret=0;
	int j;
	pfunc(DEBUG_INFO,"\n");
	if(file->result!=0)
		return ret;
	FILE *f;
	f=fopen(file->req_file.name,"w");
	if(f==NULL){
		file->result=FILE_UNKNOWN_ERROR;
		return ret;
	}
	unsigned char sum;
	sum=0;
	for(j=0;j<file->segment.len;j++){
		sum+=file->segment.data[j];
	}
	if(sum!=file->sum){
		file->result=FILE_CHECK_ERROR;
		return ret;
	}
	file->file_size+=file->segment.len;
	if(file->con==0){
		if(file->file_size!=file->req_file.file_size){
			file->result=FILE_FILE_SIZE_ERROR;//file error
			return ret;
		}else{
			file->result=0;
			file->suc=1;

		}
	}
	fseek(f,file->cur_offset,SEEK_SET);
	fwrite(file->segment.data,file->segment.len,1,f);
	fclose(f);
	return ret;
}
int get_dz_unit(_para_list *data){
	pfunc(DEBUG_INFO,"\n");
	data->unit=LimitBounds.cur_unit=2;
	data->min_unit=LimitBounds.min_unit=1;//to do
	data->max_unit=LimitBounds.max_unit=100;//to do
	return 0;
}
int set_dz_unit(int unit){
	pfunc(DEBUG_INFO,"\n");
	LimitBounds.cur_unit=unit;
	return 0;
}
int get_dz_data(para_node *para){
	pfunc(DEBUG_INFO,"\n");
	if(para->id==0x5001){
		para->tag=1;
		para->len=4;
		para->para[0]=1;
		para->para[1]=1;
		para->para[2]=1;
		para->para[3]=1;
	}else if(para->id==0x5002){
		para->tag=1;
		para->len=4;
		para->para[0]=2;
		para->para[1]=2;
		para->para[2]=2;
		para->para[3]=2;
	}else{
		para->tag=1;
		para->len=4;
		para->para[0]=3;
		para->para[1]=3;
		para->para[2]=3;
		para->para[3]=3;
	}

	return para->len;
}
int set_dz(int num,para_node*para){
	pfunc(DEBUG_WARNING,"set dz\n");
	return 0;
}
int do_update(){
	return 0;
}
// vim:tw=72 
