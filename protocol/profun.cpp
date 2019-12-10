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
	pfunc(DEBUG_INFO,"get yx pos %d\n",pos);
	return p;
}
YC_TAB * get_yc_data(int pos){
	YC_TAB *p;
	p=&YcTable[pos];
	pfunc(DEBUG_INFO,"get yc pos %d\n",pos);
	return p;
}
YC_TAB * get_acc_yc_data(int pos){
	YC_TAB *p;
	pos+=config_scada_data.pos_acc;
	p=&YcTable[pos];
	pfunc(DEBUG_INFO,"get acc yc pos %d\n",pos);
	return p;
}
int get_event_data(int port,event *&e,int change){
	int ret;
	ret=0;
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
	pfunc(DEBUG_DEBUG,"get event\n");
	return ret;
}
int get_clock(CP56Time2a &time){
	time.year=1;
	time.month=2;
	time.day=3;
	time.hour=4;
	time.minute=5;
	time.millisecond=7<<8;
	time.millisecond|=8;
	pfunc(DEBUG_INFO,"get clock\n");
	return 0;
}
int set_clock(CP56Time2a &time){
	pfunc(DEBUG_INFO,"set clock\n");
	return 0;
}
//type:0 execute;1:check;
int do_yk(int id,int type,int cmd){
	pfunc(DEBUG_INFO,"do yk:id:%x\n",id);
	if(type == 0){
		pfunc(DEBUG_INFO,"do execute:%x\n",cmd);
	}else if(type == 1){
		pfunc(DEBUG_INFO,"do select:%x\n",cmd);
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
	pfunc(DEBUG_DEBUG,"get change yc data\n");
	return ret;
}
int get_dir_data(_rd_dir *dir){
	pfunc(DEBUG_INFO,"dir:\n");
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
		pfunc(DEBUG_INFO,"%s \n",node.name);
	}
	return n;
}
int get_file_data(_rd_file *file){
	int ret;
	ret=-1;
	dir_list::iterator it(g_dir_list.MaxQueue);
	it=g_dir_list.begin();
	while(it!=g_dir_list.end()){
		if(strcmp(it->name,file->req_file.name)==0){
			ret=1;
			file->res_file=*it;
			break;
		}
		it++;
	}
	pfunc(DEBUG_INFO,"get file %s\n",file->req_file.name);
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
	ret_len=fread(file->segment.data,1,200,f);
	if(ret_len==200){
		if(!feof(f)){
			file->con=1;
		}else
			file->con=0;
		ret=1;
	}else if(ret_len<200&&ret_len>0){
		file->con=0;
		ret=1;
	}
	file->segment.len=ret_len;
	fclose(f);
	pfunc(DEBUG_INFO,"get segment of %s\n",file->req_file.name);
	return ret;
}
void load_file_list(){
	dir_node node;
	FILE *f;
	char *ret;
	f=fopen("file_list.dat","r");
	if(f==NULL)
		return;
	g_dir_list.clear();
	char buff[500];
	char time[30];

	while(!feof(f)){
		ret=fgets(buff,500,f);
		if(ret==NULL)
			break;
		sscanf(buff,"%s %ld %ld %s",&node.name[0],&node.file_id,&node.file_size,&time[0]);
		node.name_len=strlen(node.name);
		g_dir_list.push(node);
		pfunc(DEBUG_INFO,"%ld %s %ld %ld %s\n",node.name_len,&node.name[0],node.file_id,node.file_size,&time[0]);
	}
}
void save_file_list(){
	dir_node node;
	dir_list::iterator it(g_dir_list.MaxQueue);
	FILE *f;
	char buff[500];
	char time[30]="19020200000000";
	f=fopen("file_list.dat","wt");
	if(f==NULL)
		return;
	it=g_dir_list.begin();
	while(it!=g_dir_list.end()){
		node=*it;
		sprintf(buff,"%s %ld %ld %s\n",&node.name[0],node.file_id,node.file_size,&time[0]);
		fputs(buff,f);
		it++;
	}
	fclose(f);
}
int save_file_data(_rd_file *file){
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
	pfunc(DEBUG_INFO,"save %s\n",file->req_file.name);
	return ret;
}
int save_file_segment(_rd_file*file){
	int ret=1;
	int j;
	if(file->result!=0)
		return ret;
	FILE *f;
	unsigned char sum;
	sum=0;
	for(j=0;j<file->segment.len;j++){
		sum+=file->segment.data[j];
	}
	if(sum!=file->sum){
		file->result=FILE_CHECK_ERROR;
		return ret;
	}
	f=fopen(file->req_file.name,"a+");
	if(f==NULL){
		file->result=FILE_UNKNOWN_ERROR;
		return ret;
	}
	file->file_size=file->cur_offset+file->segment.len;
	fseek(f,file->cur_offset,SEEK_SET);
	fwrite(file->segment.data,file->segment.len,1,f);
	fclose(f);
	if(file->con==0){
		if(file->file_size!=file->req_file.file_size){
			file->result=FILE_FILE_SIZE_ERROR;//file error
			return ret;
		}else{
			file->result=0;
			file->suc=1;
		}
	}
	pfunc(DEBUG_INFO,"save segment of %s\n",file->req_file.name);
	return ret;
}
int get_dz_unit(_para_list *data){
	data->unit=LimitBounds.cur_unit=2;
	data->min_unit=LimitBounds.min_unit=1;//to do
	data->max_unit=LimitBounds.max_unit=100;//to do
	pfunc(DEBUG_INFO,"get unit %d\n",data->unit);
	return 0;
}
int set_dz_unit(int unit){
	pfunc(DEBUG_INFO,"set unit %d\n",unit);
	LimitBounds.cur_unit=unit;
	return 0;
}
int get_dz_data(para_node *para){
	pfunc(DEBUG_INFO,"get dz %x\n",para->id);
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
	pfunc(DEBUG_INFO,"set dz %x\n",para->id);
	return 0;
}
int do_update(){
	return 0;
}
// vim:tw=72 
