﻿/*************************************************************************
	> File Name: protocol.cpp
	> Author: hanhj
	> Mail: hanhj@zx-jy.com 
	> Created Time: 2019年02月19日 星期二 22时09分40秒
	> Relize 101 and 104 protocol
************************************************************************/
#include "gdw101_104.h"
#include "configurable.h"
#include "para.h"
#include "profun.h"
using namespace std;
/****************************
 * realize link_layer
****************************/
/**
***********************************************************************
*  @brief	link link_layer with physical com_port	
*  @param[in]  
*  @param[out]  
*  @return upon successful \n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int link_layer::set_link_com(com_port*c){
	if(c!=NULL){
		port=c->port_no;
		com=c;
		pfunc(DEBUG_DEBUG,"set link link_layer to com_port %d\n",c->port_no);
		return 0;
	}
	pfunc(DEBUG_ERROR,"invalid para\n");
	return -1;
}
int link_layer::set_app(app_layer*ap){
	app=ap;
	return 0;
}
int link_layer::send_frame(frame *f){
	int ret;
	ret=0;
	if(com==NULL||f==NULL){
		ret=ERR_INVALID_PARA;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid para\n");
		goto err;
	}
	pfunc(DEBUG_DEBUG,"send frame of link [%d]\n",port);
	ret=com->send(f->data,f->len);
	if(ret<0){
		goto err;
	}
err:
	return ret;
}
int link_layer::check_state(){
	int ret;
	ret=com->get_com_state();
	pfunc(DEBUG_DEBUG,"check state of link [%d] state=%d\n",port,ret);
	return ret;
}
int link_layer::on_link(frame *in,frame *out){
	return 0;
}
int link_layer::process_link(frame *out){
	return 0;
}
int link_layer::on_summon(frame *in,frame *out){
	summon_data.step=0;
	has_data=1;
	return 0;
}
int link_layer::process_summon(frame *out){
	int ret;
	ret=-1;
	if(summon_data.step==0){
		ret=app->build_summon_con(out,this);
	}else if(summon_data.step == 1){
		ret=app->build_yx_data(out,this);
	}else if(summon_data.step == 2){
		ret=app->build_dyx_data(out,this);
	}else if(summon_data.step == 3){
		ret=app->build_yc_data(out,this);
	}else if(summon_data.step == 4){
		ret=app->build_summon_term(out,this);
		process &=~PROCESS_SUMMON;
	}
	return ret;
}
int link_layer::on_summon_acc(frame *in,frame *out){
	summon_data.step=0;
	has_data=1;
	summon_data.sended_acc_num=0;
	return 0;
}
int link_layer::process_summon_acc(frame *out){
	int ret =0;
	if(summon_data.step==0){
		ret=app->build_summon_acc_con(out,this);
	}else if(summon_data.step==1){
		ret=app->build_summon_acc_resp(out,this);
	}else if(summon_data.step ==2){
		has_data=0;
		ret=app->build_summon_acc_term(out,this);
		process&=~PROCESS_SUMMON_ACC;
	}
	return ret;
}
int link_layer::on_clock(frame *in,frame *out){
	int ret;
	ret =0;
	int i=0;
	if(in->data[offset_cause]==CAUSE_Act){
		clock_data.clock_syn = 1;
		clock_data.time.millisecond=in->data[offset_data+i++];
		clock_data.time.millisecond|=in->data[offset_data+i++]<<8;
		clock_data.time.minute=in->data[offset_data+i++];
		clock_data.time.hour=in->data[offset_data+i++];
		clock_data.time.day=in->data[offset_data+i++];
		clock_data.time.month=in->data[offset_data+i++];
		clock_data.time.year=in->data[offset_data+i++];
	}else if(in->data[offset_cause] == CAUSE_Req){
		clock_data.clock_rd = 1;
	}
	return ret;
}
int link_layer::process_clock(frame *out){
	int ret;
	if(clock_data.clock_syn){
		set_clock(clock_data.time);
	}else if(clock_data.clock_rd){
		get_clock(clock_data.time);
	}
	ret=app->build_clock(out,this,&clock_data.time);
	process &=~PROCESS_CLOCK;
	return ret;
}
void link_layer::reset_yk_data(){
	yk_data.cur_state=0;
	yk_data.time.stop();
	process&=~PROCESS_RM_CTL;
}
int link_layer::on_yk(frame *in,frame *out){
	int ret =0;
	int ctrl;
	has_data=1;
	yk_data.cmd_id=in->data[offset_ti];
	yk_data.ctrl_id=in->data[offset_msg_id];
	yk_data.ctrl_id|=in->data[offset_msg_id+1]<<8;
	yk_data.sco.data=in->data[offset_data];
	yk_data.dco.data=yk_data.sco.data;
	
	if(yk_data.cmd_id == COMMAND_RM_CTL){
		ctrl=yk_data.sco.bit.scs;
	}else if(yk_data.cmd_id == COMMAND_RM_CTL_D){
		ctrl=yk_data.dco.bit.dcs==1?0:1;
	}
	if(in->data[offset_cause]==CAUSE_Act){
		if(yk_data.sco.bit.sel==1){//sel
			if(yk_data.cur_state==0){
				yk_data.cur_state=YK_SEL;
				ret=app->do_yk(yk_data.ctrl_id,1,ctrl);//do check
				if(ret<0)
					yk_data.fail=1;
				else {
					yk_data.time.start(60);//60s
				}
			}else//re select
				yk_data.fail=1;
		}else if(yk_data.sco.bit.sel==0){//act
			if(yk_data.cur_state==YK_SEL){
				yk_data.cur_state=YK_ACT;
				ret=app->do_yk(yk_data.ctrl_id,0,ctrl);//do act 
				if(ret<0)
					yk_data.fail=1;
			}else{
				yk_data.fail=1;
			}
		}
	}else if(in->data[offset_cause] == CAUSE_Deact){
		yk_data.cur_state=YK_DEACTIVE;
	}
	if(yk_data.fail){
		ret=-1;
		reset_yk_data();
	}
	if(yk_data.cur_state==YK_DEACTIVE){
		reset_yk_data();
	}
	return ret;
}
int link_layer::process_yk(frame *out){
	int ret;
	ret=app->build_yk(out,this);
	if(yk_data.cur_state==YK_SEL||yk_data.cur_state==YK_DEACTIVE||yk_data.cur_state==YK_ACTOVER){
		process&=~PROCESS_RM_CTL;
	}else if(yk_data.cur_state==YK_ACT){
		yk_data.cur_state=YK_ACTOVER;
	}
	return ret;
}
int link_layer::process_event(frame *out){
	int ret=-1;
	event *e;
	e=NULL;
	if(app->get_event_data(port,e,1)==1){
		ret=app->build_event_data(out,this,e);
	}else{
		process &=(~PROCESS_EVENT);
		event_data.need_ack[port] = 0;
		has_data=0;
	}
	return ret;
}
int link_layer::on_test_link(frame *in,frame *out){
	return 0;
}
int link_layer::process_test_link(frame *out){
	int ret;
	ret=app->build_link_test_con(out,this);
	process &=~PROCESS_TEST_LINK;
	return ret;
}
int link_layer::process_yc_change(frame *out){
	int ret=-1;
	event_yc *e;
	if(app->get_yc_cg_data(port,e)==1)
		ret=app->build_yc_cg_data(out,this,e);
	else{
		process &=~PROCESS_YC_CHANGE;
		event_data.need_yc_ack[port] = 0;
	}
	return ret;
}
int link_layer::on_reset_terminal(frame *in,frame *out){
	has_data =1;
	return 0;
}
int link_layer::process_reset_terminal(frame *out){
	int ret=0;
	ret=app->build_reset_con(out,this);
	process &=~PROCESS_RESET;
	app->need_reset=1;
	return ret;
}
int link_layer::on_file(frame *in,frame *out){
	int ret=0;
	int i,j;
	i=1;//skip addition type :02
	j=0;
	file_data.op=in->data[offset_data+i++];
	if(file_data.op == 1){//rd dir
		file_data.rd_dir.id=in->data[offset_data+i++];
		file_data.rd_dir.id=file_data.rd_dir.id<<8;
		file_data.rd_dir.id|=in->data[offset_data+i++];
		file_data.rd_dir.id=file_data.rd_dir.id<<8;
		file_data.rd_dir.id|=in->data[offset_data+i++];
		file_data.rd_dir.id=file_data.rd_dir.id<<8;
		file_data.rd_dir.id|=in->data[offset_data+i++];
		file_data.rd_dir.id=file_data.rd_dir.id<<8;
		file_data.rd_dir.len=in->data[offset_data+i++];
		for(j=0;j<file_data.rd_dir.len;j++){
			file_data.rd_dir.name[j]=in->data[offset_data+i++];
		}
		file_data.rd_dir.flag=in->data[offset_data+i++];
		for(j=0;j<7;j++)
			file_data.rd_dir.start_time.data[j]=in->data[offset_data+i++];
		for(j=0;j<7;j++)
			file_data.rd_dir.end_time.data[j]=in->data[offset_data+i++];
	}else if(file_data.op == 3){//rd file
		file_data.rd_file.req_file.name_len=in->data[offset_data+i++];
		for(j=0;j<file_data.rd_file.req_file.name_len;j++){
			file_data.rd_file.req_file.name[j]=in->data[offset_data+i++];
		}
		file_data.rd_file.req_file.name[j]=0;
		file_data.rd_file.step=0;
	}else if(file_data.op == 6){//rd file confirm
		file_data.rd_file.req_file.file_id=in->data[offset_data+i++];
		file_data.rd_file.req_file.file_id=file_data.rd_file.req_file.file_id<<8;
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.rd_file.req_file.file_id=file_data.rd_file.req_file.file_id<<8;
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.rd_file.req_file.file_id=file_data.rd_file.req_file.file_id<<8;
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.rd_file.req_file.file_id=file_data.rd_file.req_file.file_id<<8;
		
		file_data.rd_file.ack_offset=in->data[offset_data+i++];
		file_data.rd_file.ack_offset=file_data.rd_file.ack_offset<<8;
		file_data.rd_file.ack_offset|=in->data[offset_data+i++];
		file_data.rd_file.ack_offset=file_data.rd_file.ack_offset<<8;
		file_data.rd_file.ack_offset|=in->data[offset_data+i++];
		file_data.rd_file.ack_offset=file_data.rd_file.ack_offset<<8;
		file_data.rd_file.ack_offset|=in->data[offset_data+i++];
		file_data.rd_file.ack_offset=file_data.rd_file.ack_offset<<8;
		file_data.rd_file.con=in->data[offset_data+i++];
	}else if(file_data.op ==7){//write file act
		file_data.wt_file.req_file.name_len=in->data[offset_data+i++];
		file_data.wt_file.result=0;
		for(j=0;j<file_data.wt_file.req_file.name_len&&j<30;j++){
			file_data.wt_file.req_file.name[j]=in->data[offset_data+i++];
		}
		if(file_data.wt_file.req_file.name_len>30)
			file_data.wt_file.result=FILE_LONGNAME_ERROR;//long name file error
		file_data.wt_file.req_file.file_id=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_id=file_data.wt_file.req_file.file_id<<8;
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_id=file_data.wt_file.req_file.file_id<<8;
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_id=file_data.wt_file.req_file.file_id<<8;
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_id=file_data.wt_file.req_file.file_id<<8;

		file_data.wt_file.req_file.file_size=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_size=file_data.wt_file.req_file.file_size<<8;
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_size=file_data.wt_file.req_file.file_size<<8;
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_size=file_data.wt_file.req_file.file_size<<8;
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_size=file_data.wt_file.req_file.file_size<<8;
		file_data.wt_file.suc=0;
	}else if(file_data.op == 9){
		int data_len;
		long tmp_id;
		if(protocol==PRO_101)
			data_len=in->data[offset_len] -  1 - addr_size - 1 - 1 - cause_size - addr_size - msg_id_size - 1 - 1 - 4 - 4 - 1 - 1  ;
		else if(protocol==PRO_104)
			data_len=in->data[offset_len] - 4 - 1 - 1 - cause_size - addr_size - msg_id_size - 1 - 1 - 4 - 4 - 1 - 1;
		tmp_id=in->data[offset_data+i++];
		tmp_id=tmp_id<<8;
		tmp_id|=in->data[offset_data+i++];
		tmp_id=tmp_id<<8;
		tmp_id|=in->data[offset_data+i++];
		tmp_id=tmp_id<<8;
		tmp_id|=in->data[offset_data+i++];
		tmp_id=tmp_id<<8;
		file_data.wt_file.cur_offset=in->data[offset_data+i++];
		file_data.wt_file.cur_offset=file_data.wt_file.cur_offset<<8;
		file_data.wt_file.cur_offset|=in->data[offset_data+i++];
		file_data.wt_file.cur_offset=file_data.wt_file.cur_offset<<8;
		file_data.wt_file.cur_offset|=in->data[offset_data+i++];
		file_data.wt_file.cur_offset=file_data.wt_file.cur_offset<<8;
		file_data.wt_file.cur_offset|=in->data[offset_data+i++];
		file_data.wt_file.cur_offset=file_data.wt_file.cur_offset<<8;
		file_data.wt_file.con=in->data[offset_data+i++];
		file_data.wt_file.segment.len=data_len;
		for(j=0;j<data_len;j++){
			file_data.wt_file.segment.data[j]=in->data[offset_data+i++];
		}
		if(tmp_id!=file_data.wt_file.req_file.file_id)
			file_data.wt_file.result=FILE_FILE_ID_ERROR;//file error
		file_data.wt_file.sum=in->data[offset_data+i++];
	}
	return ret;
}
int link_layer::process_file(frame *out){
	int ret=0;
	if(file_data.op==1){//read dir
		ret=app->get_dir_data(&file_data.rd_dir);
		if(ret>=0){
			ret=app->build_rd_dir_resp(out,this,&file_data.rd_dir);
			if(file_data.rd_dir.con==0)
				file_data.op=0;
		}
	}else if(file_data.op==3){//read file
		if(file_data.rd_file.step==0){
			ret=app->get_file_data(&file_data.rd_file);
			if(ret>=0){
				ret=app->build_rd_file_con(out,this,&file_data.rd_file);
				file_data.rd_file.step=1;
			}
		}else if(file_data.rd_file.step==1){
			ret=app->get_file_segment(&file_data.rd_file);
			if(ret>0){
				ret=app->build_rd_file_resp(out,this,&file_data.rd_file);
				file_data.rd_file.step=2;
			}
		}
	}else if(file_data.op==6){//read file ack confirm
		//change file offset_asdu
		if(file_data.rd_file.ack_offset == file_data.rd_file.cur_offset){
			file_data.rd_file.cur_offset=file_data.rd_file.cur_offset+file_data.rd_file.segment.len;
			ret=app->get_file_segment(&file_data.rd_file);
			if(ret>0){
				ret=app->build_rd_file_resp(out,this,&file_data.rd_file);
			}else{
				process&=~PROCESS_FILE;
			}
		}
	}else if(file_data.op==7){//wr file
		ret=app->save_file_data(&file_data.wt_file);
		if(ret>0)
			ret=app->build_wr_file_con(out,this,&file_data.wt_file);
		if(strcmp(file_data.wt_file.req_file.name,"update.bin")==0){
			file_data.update_flag.save_ok=0;
		}
		file_data.op=0;
	}else if(file_data.op==9){//wr file segment
		ret=app->save_file_segment(&file_data.wt_file);
		if(ret>0)
			ret=app->build_wr_file_resp(out,this,&file_data.wt_file);
		
		if(strcmp(file_data.wt_file.req_file.name,"update.bin")==0 && file_data.wt_file.suc==1){
			file_data.update_flag.save_ok=1;
		}
		file_data.op=0;
	}
	return ret;
}
int link_layer::on_rd_dz(frame *in,frame *out){
	int ret=0;
	int i=0;
	int j;
	has_data=1;
	para_data.cur_read=0;
	para_data.req_num=in->data[offset_vsq]&0x7f;
	para_data.unit=in->data[offset_msg_id+i++];
	para_data.unit|=in->data[offset_msg_id+i++]<<8;
	if(para_data.req_num==0){
		para_data.op=2;//read all para 
		para_data.req_num=40;//to do
		for(j=0;j<para_data.req_num;j++){
			para_data.req_id[j]=j+0x5001;
		}
	}
	else{
		para_data.op=1;//read mul para 
		for(j=0;j<para_data.req_num;j++){
			para_data.req_id[j]=in->data[offset_msg_id+i++];
			para_data.req_id[j]|=in->data[offset_msg_id+i++]<<8;
		}
	}
	return ret;
}
int link_layer::process_rd_dz(frame *out){
	int ret=0;
	int len;
	int data_len;
	len=0;
	int i,j;
	j=0;
	data_len=6+1+addr_size+1+1+cause_size+addr_size+2+1;//6:len,1:c,addr,1:id,1:vsq,2:cause,2:pub addr,2:sn,1:pi 
	has_data=0;
	for(i=para_data.cur_read;i<para_data.req_num;i++){
		para_data.nodes[j].id=para_data.req_id[i];
		len=app->get_dz_data(&para_data.nodes[j]);
		data_len+=len+2+1+1;//2:msg id,1:tag,1:len
		para_data.cur_read++;
		j++;
		if(data_len>200&&data_len<256)
			break;
		else if(data_len>256){
			para_data.cur_read--;
			j--;
			break;
		}
	}
	if(para_data.cur_read==para_data.req_num){
		para_data.pi.bit.con=0;
		process&=~PROCESS_RD_DZ;
	}
	else
		para_data.pi.bit.con=1;
	para_data.res_num=j;
	ret=app->build_rd_dz_con(out,this,&para_data);
	return ret;
}
int link_layer::on_wr_dz(frame *in,frame *out){
	int ret=0;
	int i=0;
	int j;
	int m;
	has_data=1;

	para_data.req_num=in->data[offset_vsq]&0x7f;
	para_data.unit=in->data[offset_msg_id+i++];
	para_data.unit|=in->data[offset_msg_id+i++]<<8;
	para_data.pi.data=in->data[offset_msg_id+i++];
	if(para_data.pi.bit.se==1){
		para_data.op=3;//prepare set
	}else if(para_data.pi.bit.cr==1){
		para_data.op=5;//cancel set 
	}else if(para_data.pi.bit.se==0){
		para_data.op=4;//fix
	}
	for(j=0;j<para_data.req_num;j++){
		para_data.nodes[j].id=in->data[offset_msg_id+i++];
		para_data.nodes[j].id|=in->data[offset_msg_id+i++]<<8;
		para_data.nodes[j].tag=in->data[offset_msg_id+i++];
		para_data.nodes[j].len=in->data[offset_msg_id+i++];
		for(m=0;m<para_data.nodes[j].len;m++){
			para_data.nodes[j].para[m]=in->data[offset_msg_id+i++];
		}
	}
	return ret;
}
int link_layer::process_wr_dz(frame *out){
	int ret=0;
	has_data=0;
	if(para_data.op==4){//固化
		app->set_dz(para_data.req_num,&para_data.nodes[0]);
		para_data.op=0;	
	}else if(para_data.op==5){//撤销
		para_data.op=0;
		pfunc(DEBUG_INFO,"cancel set\n");
	}
	ret=app->build_wr_dz_con(out,this,&para_data);
	process&=~PROCESS_WR_DZ;
	return ret;
}
int link_layer::on_rd_unit(frame *in,frame *out){
	int ret=0;
	has_data=1;
	return ret;
}
int link_layer::process_rd_unit(frame *out){
	int ret=0;
	has_data=0;
	app->get_dz_unit(&para_data);
	ret=app->build_rd_unit_con(out,this,&para_data);
	process&=~PROCESS_RD_UNIT;
	return ret;
}
int link_layer::on_wr_unit(frame *in,frame *out){
	int ret=0;
	int i=0;
	has_data=1;
	para_data.unit=in->data[offset_data+i++];
	para_data.unit|=in->data[offset_data+i++]<<8;
	return ret;
}
int link_layer::process_wr_unit(frame *out){
	int ret=0;
	has_data=0;
	app->set_dz_unit(para_data.unit);
	ret=app->build_wr_unit_con(out,this,&para_data);
	process&=~PROCESS_WR_UNIT;
	return ret;
}

int link_layer::on_update(frame *in,frame *out){
	int ret=0;
	int ctype;
	ctype=in->data[offset_data];
	if(ctype&0x80){
		file_data.update_flag.start=1;
	}else{
		file_data.update_flag.start=0;
	}
	return ret;
}
int link_layer::process_update(frame *out){
	int ret=0;
	if(file_data.update_flag.start==0&& file_data.update_flag.save_ok){
		app->need_update=1;
	}
	ret=app->build_update_con(out,this,&file_data.update_flag);
	process&=~PROCESS_UPDATE;
	return ret;
}

/****************************
 *  realize link_layer_101
****************************/
int link_layer_101::save_frame(frame *f,int type){
	if(type==2)
		memcpy(&last_send_frame,f,sizeof(last_send_frame));
	else if(type ==1)
		memcpy(&last_fix_frame,f,sizeof(last_fix_frame));
	last_type=type;
	return 0;
}
int link_layer_101::send_last_frame(){
	int ret;
	ret=-1;
	if(last_type==2)
		ret=send_frame(&last_send_frame);
	else if(last_type==1){
		ret=send_frame(&last_fix_frame);
	}
	return ret;
}
int link_layer_101::build_ack(frame*out,int has_data){
	int ret;
	int i;
	ctrl_word ctl;
	i=0;
	ret=0;

	if(balance==UNBALANCE){
		ctl.sl.fc=0;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=has_data;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=0;
	}else if(balance==BALANCE){
		ctl.sl.fc=0;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=0;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}else if(addr_size>2){
		ret=ERR_BUILD_LINK_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size\n");
		goto err;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	ret=i;
err:
	return ret;
}
int link_layer_101::build_nak(frame*out){
	int ret;
	int i;
	ctrl_word ctl;
	ret=0;
	i=0;
	if(balance==UNBALANCE){
		ctl.sl.fc=1;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=0;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=0;
	}else if(balance==BALANCE){
		ctl.sl.fc=1;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=0;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}else if(addr_size>2){
		ret=ERR_BUILD_LINK_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size\n");
		goto err;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	ret=i;
err:
	return ret;

}
int link_layer_101::build_link_ack(frame*out){
	int ret;
	int i;
	ctrl_word ctl;
	ret=0;
	i=0;
	if(balance==UNBALANCE){
		ctl.sl.fc=11;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=0;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=0;
	}else if(balance==BALANCE){
		ctl.sl.fc=11;
		ctl.sl.dfc=0;
		ctl.sl.acd_rev=0;
		ctl.sl.prm=0;
		ctl.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}else if(addr_size>2){
		ret=ERR_BUILD_LINK_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size\n");
		goto err;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	ret=i;
err:
	return ret;
}
int link_layer_101::build_link_req(frame*out){
	int ret;
	int i;
	ctrl_word ctl;
	i=0;
	ret=0;
	if(balance==BALANCE){
		ctl.pm.fc=9;
		ctl.pm.fcv=0;
		ctl.pm.fcv=0;
		ctl.pm.prm=1;
		ctl.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl.data;
		out->data[i++]=addr&0x00ff;
		if(addr_size==2){
			out->data[i++]=addr>>8&0x00ff;
		}else if(addr_size>2){
			ret=ERR_BUILD_LINK_FR;
			errno=ret;
			pfunc(DEBUG_ERROR,"invalid addr_size\n");
			goto err;
		}
		out->data[i++]=sum(&out->data[1],addr_size+1);
		out->data[i++]=0x16;
		out->len=i;
	}	
	ret=i;
err:
	return ret;
}
int link_layer_101::build_reset_link(frame*out){
	int ret;
	int i;
	ctrl_word ctl;
	i=0;
	ret=0;
	if(balance==BALANCE){
		ctl.pm.fc=0;
		ctl.pm.fcv=0;
		ctl.pm.fcb=0;
		ctl.pm.prm=1;
		ctl.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl.data;
		out->data[i++]=addr&0x00ff;
		if(addr_size==2){
			out->data[i++]=addr>>8&0x00ff;
		}else if(addr_size>2){
			ret=ERR_BUILD_LINK_FR;
			errno=ret;
			pfunc(DEBUG_ERROR,"invalid addr_size\n");
			goto err;
		}
		out->data[i++]=sum(&out->data[1],addr_size+1);
		out->data[i++]=0x16;
		out->len=i;
	}	
	ret=i;
err:
	return ret;
}
int link_layer_101::build_link_layer(frame*out,int asdu_len){
	int ret;
	ret=0;
	int l;
	int len;
	switch(addr_size){
		case 0:
			l=asdu_len+1;
			break;
		case 1:
			l=asdu_len+2;
			break;
		case 2:
			l=asdu_len+3;
			break;
		default:
			ret=ERR_BUILD_APP_FR;
			errno=ret;
			pfunc(DEBUG_ERROR,"invalid addr_size:%d\n",addr_size);
			goto err;
	}
	len=l+6;
	out->data[0]=0x68;
	out->data[1]=l;
	out->data[2]=l;
	out->data[3]=0x68;
	out->data[4]=ctl_lo.data;
	out->data[5]=addr&0x00ff;
	if(addr_size==2){
		out->data[6]=addr>>8&0x00ff;
	}
	out->data[len-2]=sum(&out->data[offset_control],l);
	out->data[len-1]=0x16;
	out->len=len;
	ret=len;
err:
	return ret;
}
void link_layer_101::set_loc_ctl(){
	if(balance==UNBALANCE){
		ctl_lo.sl.fc=8;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=has_data;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==BALANCE){
		ctl_lo.pm.fc=3;
		ctl_lo.pm.fcv=1;
		ctl_lo.pm.fcb=!ctl_lo.pm.fcb;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
	}
}
//for balance
int link_layer_101::on_ack(frame *in,frame *out){
	int ret;
	ret=-1;
	pfunc(DEBUG_DEBUG,"on_ack\n");
	if(process & PROCESS_EVENT){
		ret = process_event(out);
	}else if(process & PROCESS_SUMMON){
		ret=process_summon(out);
	}else if(process & PROCESS_SUMMON_ACC){
		ret=process_summon_acc(out);
	}else if(process & PROCESS_CLOCK){
		ret = process_clock(out);
	}else if(process & PROCESS_RM_CTL){
		ret = process_yk(out);
	}else if(process & PROCESS_TEST_LINK){
		ret=process_test_link(out);
	}else if(process & PROCESS_YC_CHANGE){
		ret=process_yc_change(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_UPDATE){
		ret=process_update(out);
	}else if(process & PROCESS_RESET){
		ret=process_reset_terminal(out);
	}else if(process & PROCESS_RD_UNIT){
		ret=process_rd_unit(out);
	}else if(process & PROCESS_RD_DZ){
		ret=process_rd_dz(out);
	}else if(process & PROCESS_WR_UNIT){
		ret=process_wr_unit(out);
	}else if(process & PROCESS_WR_DZ){
		ret=process_wr_dz(out);
	}
	return ret ;
}
int link_layer_101::on_req(frame *in,frame *out){
	int ret;
	ret= -1;
	int ti;
	ti=in->data[offset_ti];
	switch(ti){
		case COMMAND_LINK_FINI://link finish
			ret=on_link(in,out);
			ret=process_link(out);
			break;
		case COMMAND_SUMMON://total sum
			process|=PROCESS_SUMMON;
			ret=on_summon(in,out);
			if(balance == BALANCE){
				ret=process_summon(out);
			}
			break;
		case COMMAND_CLOCK://clock 
			process|=PROCESS_CLOCK;
			ret=on_clock(in,out);
			if(balance ==BALANCE)
				ret=process_clock(out);
			break;
		case COMMAND_RM_CTL:
		case COMMAND_RM_CTL_D:
			process|=PROCESS_RM_CTL;
			ret=on_yk(in,out);
			if(balance == BALANCE)
				ret=process_yk(out);
			break;
		case COMMAND_TEST_LINK:
			process|=PROCESS_TEST_LINK;
			ret=on_test_link(in,out);
			if(balance == BALANCE)
				ret=process_test_link(out);
			break;
		case COMMAND_RESET:
			process|=PROCESS_RESET;
			ret=on_reset_terminal(in,out);
			if(balance==BALANCE)
				ret=process_reset_terminal(out);
			break;
		case COMMAND_FILE:
			process|=PROCESS_FILE;
			ret=on_file(in,out);
			if(balance ==BALANCE){
				ret=process_file(out);
			}
			break;
		case COMMAND_RD_UNIT:
			process|=PROCESS_RD_UNIT;
			ret=on_rd_unit(in,out);
			if(balance == BALANCE){
				ret = process_rd_unit(out);
			}
			break;
		case COMMAND_WR_UNIT:
			process|=PROCESS_WR_UNIT;
			ret=on_wr_unit(in,out);
			if(balance){
				ret=process_wr_unit(out);
			}
			break;
		case COMMAND_RD_DZ:
			process|=PROCESS_RD_DZ;
			ret=on_rd_dz(in,out);
			if(balance == BALANCE)
				ret=process_rd_dz(out);
			break;
		case COMMAND_WR_DZ:
			process|=PROCESS_WR_DZ;
			ret=on_wr_dz(in,out);
			if(balance == BALANCE){
				ret=process_wr_dz(out);
			}
			break;
		case COMMAND_SUMMON_ACC:
			process|=PROCESS_SUMMON_ACC;
			ret=on_summon_acc(in,out);
			if(balance == BALANCE){
				ret=process_summon_acc(out);
			}
			break;
		case COMMAND_UPDATE:
			process|=PROCESS_UPDATE;
			ret=on_update(in,out);
			if(balance==BALANCE)
				ret=process_update(out);
			break;
	}
	return ret;
}
//for unbalance
int link_layer_101::on_req_class_1(frame *in,frame *out){
	int ret ;
	pfunc(DEBUG_DEBUG,"on_req_class_1\n");
	ret=-1;
	if(process & PROCESS_EVENT){
		ret=process_event(out);
	}else if(process & PROCESS_SUMMON){
		ret = process_summon(out);
	}else if(process & PROCESS_SUMMON_ACC){
		ret=process_summon_acc(out);
	}else if(process & PROCESS_RM_CTL){
		ret=process_yk(out);
	}else if(process & PROCESS_RESET){
		ret=process_reset_terminal(out);
	}else if(process & PROCESS_RD_UNIT){
		ret=process_rd_unit(out);
	}else if(process & PROCESS_RD_DZ){
		ret=process_rd_dz(out);
	}else if(process & PROCESS_WR_UNIT){
		ret=process_wr_unit(out);
	}else if(process & PROCESS_WR_DZ){
		ret=process_wr_dz(out);
	}
	return ret ;
}
//for unbalance
int link_layer_101::on_req_class_2(frame *in,frame *out){
	int ret;
	ret=-1;
	pfunc(DEBUG_DEBUG,"on_req_class_2\n");
	if( process & PROCESS_EVENT){
		ret=process_event(out);
	}else if(process & PROCESS_CLOCK){
		ret=process_clock(out);
	}else if(process & PROCESS_TEST_LINK){
		ret=process_test_link(out);
	}else if(process & PROCESS_YC_CHANGE){
		ret=process_yc_change(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_UPDATE){
		ret=process_update(out);
	}
	return ret;
}


int link_layer_101::on_fc0(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc0\n");
	if(balance==UNBALANCE){
		if(link_step==2){
			//because remote reset link so reset ctl_lo and ctl_rm
			ctl_lo.pm.fcb=0;
			ctl_rm.pm.fcb=0;
			rep_timer.stop();

			link_step++;//3
			ret=build_ack(&s_fix_frame,1);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//4
		}
	}else if(balance==BALANCE){
		if(link_step==2){
			//because remote reset link so reset ctl_lo and ctl_rm
			ctl_lo.pm.fcb=0;
			ctl_rm.pm.fcb=0;
			rep_timer.stop();

			link_step++;//3
			ret=build_ack(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//4
			ret=build_link_req(&s_fix_frame);//build a s3 service
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//5
			save_frame(&s_fix_frame,2);//s3 service need resend 
			rep_timer.start(REP_TIME_S3);
		}else if(link_step==7){
			rep_timer.stop();
			link_step++;//8
			ret=app->build_link_fini(&s_var_frame,this);//build a s2 service
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_var_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//9
			save_frame(&s_var_frame,2);//save frame
			rep_timer.start(REP_TIME);
		}else if(link_step==9){
			rep_timer.stop();
			rep_times=0;
			link_step++;//10
			link_state=LINK_OPEN;
			process=0;
			pfunc(DEBUG_ERROR,"connect\n");
		}else if(link_state==LINK_OPEN){//receive ack frame
			rep_timer.stop();
			rep_times=0;
			ret=on_ack(f,&s_var_frame);
			if(ret<0){
				goto err;
			}
			if(ret>0){
				ret=send_frame(&s_var_frame);
				save_frame(&s_var_frame,2);//save frame
				rep_timer.start(REP_TIME);
				if(ret<0){
					goto err;
				}
			}
		}
	}
err:
	return ret;
}
int link_layer_101::on_fc1(frame*f){
	int ret;
	ret=0;
	app->do_reset();
	pfunc(DEBUG_DEBUG,"on_fc1\n");
//err:
	return ret;
}
int link_layer_101::on_fc2(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc2\n");
	ret=build_ack(&s_fix_frame,has_data);
	if(ret<0){
		goto err;
	}
	ret=send_frame(&s_fix_frame);
	if(ret<0){
		goto err;
	}//ack
err:
	return ret;
}
int link_layer_101::on_fc3(frame *f){
	int ret;
	int sret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc3\n");
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
	if(balance==UNBALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret>=0){
					sret=build_ack(&s_fix_frame,has_data);
					if(sret<0){
						goto err;
					}
					sret=send_frame(&s_fix_frame);
					if(sret<0){
						goto err;
					}//ack
				}else{
					sret=build_nak(&s_fix_frame);
					if(sret<0){
						goto err;
					}
					sret=send_frame(&s_fix_frame);
					if(sret<0){
						goto err;
					}
				}
				save_frame(&s_fix_frame,1);
			}else{//fcb not change ,resend last ack
				sret=send_frame(&last_fix_frame);
				if(sret<0){
					goto err;
				}
			}
		}else{//code=3 is s2 service fcv must valid,so now return nak
			sret=build_nak(&s_fix_frame);
			if(sret<0){
				goto err;
			}
			sret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
		}
	}else if(balance==BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret>=0){
					sret=build_ack(&s_fix_frame);
					if(sret<0){
						goto err;
					}
					sret=send_frame(&s_fix_frame);
					save_frame(&s_fix_frame,1);
					if(sret<0){
						goto err;
					}//ack
					if(ret>0){
						sret=send_frame(&s_var_frame);
						if(sret<0){
							goto err;
						}
						save_frame(&s_var_frame,2);//save frame
						rep_timer.start(REP_TIME);
					}
				}else{
					sret=build_nak(&s_fix_frame);
					if(sret<0){
						goto err;
					}
					sret=send_frame(&s_fix_frame);
					if(sret<0){
						goto err;
					}
					save_frame(&s_fix_frame,1);
				}
			}else{//fcb not change ,resend last ack
				sret=send_frame(&last_fix_frame);
				if(sret<0){
					goto err;
				}
			}
		}else{//code=3 is s2 service fcv must valid,so now return nak
			sret=build_nak(&s_fix_frame);
			if(sret<0){
				goto err;
			}
			sret=send_frame(&s_fix_frame);
			if(sret<0){
				goto err;
			}
		}
	}
err:
	return sret;
}
int link_layer_101::on_fc4(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc4\n");
//err:
	return ret;
}
int link_layer_101::on_fc5(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc5\n");
//err:
	return ret;
}
int link_layer_101::on_fc6(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc6\n");
//err:
	return ret;
}
int link_layer_101::on_fc7(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc7\n");
//err:
	return ret;
}
int link_layer_101::on_fc8(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc8\n");
	if(balance!=BALANCE){
		ret=build_link_ack(&s_fix_frame);
		if(ret<0){
			goto err;
		}
		ret=send_frame(&s_fix_frame);
		if(ret<0){
			goto err;
		}
	}
err:
	return ret;
}
int link_layer_101::on_fc9(frame*f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc9\n");
	if(link_state!=LINK_OPEN){
		link_step=1;
		link_state=LINK_DISCONNECT;
	}
	ret=build_link_ack(&s_fix_frame);
	if(ret<0){
		goto err;
	}
	ret=send_frame(&s_fix_frame);
	if(ret<0){
		goto err;
	}
	link_step++;//2
err:
	return ret;
}
int link_layer_101::on_fc10(frame*f){
	int ret;
	ret=0;
	if(balance==BALANCE)
		return ret;
	pfunc(DEBUG_DEBUG,"on_fc10\n");
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
		
	if(link_state!=LINK_OPEN){
		link_step++;//5
		ret=app->build_link_fini(&s_var_frame,this);
		if(ret<0){
			goto err;
		}
		ret=send_frame(&s_var_frame);
		if(ret<0){
			goto err;
		}
		link_step++;//6
		link_state=LINK_OPEN;
		pfunc(DEBUG_ERROR,"connect\n");
		process=0;
	}else{//link open
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req_class_1(f,&s_var_frame);
				if(ret>=0){
					ret=send_frame(&s_var_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_var_frame,2);//save frame
				}else{
					ret=build_nak(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_fix_frame,1);//save frame
				}
			}else{
				ret=send_last_frame();
				if(ret<0){
					goto err;
				}
			}
		}else{//code=10 is s3 service fcv must valid,so now return nak
			ret=build_nak(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
		}
	}
err:
	return ret;
}
int link_layer_101::on_fc11(frame *f){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"on_fc11\n");
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req_class_2(f,&s_var_frame);
				if(ret>=0){
					ret=send_frame(&s_var_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_var_frame,2);//save frame
				}else{
					ret=build_nak(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_fix_frame,2);
				}
			}else{
				ret=send_last_frame();
				if(ret<0){
					goto err;
				}
			}
		}else{//code=11 is s3 service fcv must valid,so now return nak
			ret=build_nak(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
		}
	}else if(balance==BALANCE){
		if(link_step==5){
			rep_timer.stop();
			link_step++;//6
			ret=build_reset_link(&s_fix_frame);//build a s2 service
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//7
			save_frame(&s_fix_frame,2);//s2 service need resend;
			rep_timer.start(REP_TIME);
		}
	}	
err:
	return ret;
}
#define BROADCASET_ADDR 0xffff
int link_layer_101::deal_frame(frame*f){
	int ret;
	int tmp;
	ctrl_word ctl;//tmp ctl  
	send_cause cause;//tmp cause.
	int req_addr;
	if(f->type==FIX_FRAME){
		ctl.data=f->data[1];
		req_addr=f->data[2];
		if(addr_size==2){
			tmp=f->data[3];
			req_addr|=(tmp<<8&0xff00);
		}
		if(req_addr!=addr&&req_addr!=BROADCASET_ADDR){
			pfunc(DEBUG_ERROR,"invalid address\n");
			return -1;
		}
	}else if(f->type==VAR_FRAME){
		ctl.data=f->data[offset_control];
		//next routine for get src addr 
		cause.data=f->data[offset_cause];
		if(cause_size==2){
			tmp=f->data[offset_cause+1];
			cause.data|=(tmp<<8&0xff00);
			rm_addr=cause.bit.src;
		}
		cause_rm.data=cause.data;

		req_addr=f->data[offset_addr];
		if(addr_size==2){
			tmp=f->data[offset_addr+1];
			req_addr|=(tmp<<8&0xff00);
		}
		if(req_addr!=addr&&req_addr!=BROADCASET_ADDR){
			pfunc(DEBUG_ERROR,"invalid address\n");
			return -1;
		}
	}else{
		pfunc(DEBUG_ERROR,"invalid frame type\n");
		return -1;
	}
	ret=0;
	if(balance==UNBALANCE){
		switch(ctl.pm.fc){
			case 0:
				ret=on_fc0(f);
				break;
			case 1:
				ret=on_fc1(f);
				break;
			case 3:
				if(link_state==LINK_OPEN)
					ret=on_fc3(f);
				break;
			case 4:
				ret=on_fc4(f);
				break;
			case 5:
				ret=on_fc5(f);
				break;
			case 8:
				ret=on_fc8(f);
				break;
			case 9:
				ret=on_fc9(f);
				break;
			case 10:
				if(link_state==LINK_OPEN)
					ret=on_fc10(f);
				break;
			case 11:
				if(link_state==LINK_OPEN)
					ret=on_fc11(f);
				break;
		}
	}else if(balance==BALANCE){
		switch(ctl.pm.fc){
			case 0:
				ret=on_fc0(f);
				break;
			case 1:
				ret=on_fc1(f);
				break;
			case 2:
				ret=on_fc2(f);
				break;
			case 3:
				if(link_state==LINK_OPEN)
					ret=on_fc3(f);
				break;
			case 4:
				ret=on_fc4(f);
				break;
			case 9:
				ret=on_fc9(f);
				break;
			case 11:
				ret=on_fc11(f);
				break;
		}
	}
	return ret;
}
int link_layer_101::get_frame(){
	int ret;
	unsigned char c;
	int fail=0;
	if(com==NULL)
		return -1;
	pfunc(DEBUG_DEBUG,"get frame of link [%d]\n",port);
	while(1){
		ret=com->get_byte(&c);
		if(ret==-1)
			break;
		if(c==0x10){
			if(!start_rcv_fix_flag){
				start_rcv_fix_flag=1;
				rcv_fix_timer.start(3);
				r_fix_pos=0;
			}
		}
		if(start_rcv_fix_flag){
			r_fix_frame.data[r_fix_pos]=c;
			r_fix_pos++;
			if(r_fix_pos==(addr_size+4)){
				if(r_fix_frame.data[r_fix_pos-1]==0x16){
					if(sum(&r_fix_frame.data[1],addr_size+1)==r_fix_frame.data[r_fix_pos-2]){
						r_fix_frame.len=r_fix_pos;
						rcv_fix_timer.stop();
						start_rcv_fix_flag=0;
						r_fix_pos=0;
						pdump(DEBUG_INFO,"get fix frame",&r_fix_frame.data[0],r_fix_frame.len);
						deal_frame(&r_fix_frame);
						break;
					}else{
						pfunc(DEBUG_ERROR,"get fix frame err1\n");
						fail=1;
					}
				}else{
					pfunc(DEBUG_ERROR,"get fix frame err2\n");
					fail=1;
				}
			}else if(r_fix_pos>(addr_size+4)||r_fix_pos>FIX_FRAME_BUFF){
				fail=1;
				pfunc(DEBUG_ERROR,"get fix frame err3\n");
			}
			if(rcv_fix_timer.is_reached()==1){//receive timeout
				start_rcv_fix_flag=0;
				r_fix_pos=0;
			}
		}
		if(fail){
				start_rcv_fix_flag=0;
				r_fix_pos=0;
				rcv_fix_timer.stop();
		}
		fail=0;
		if(c==0x68){
			if(!start_rcv_var_flag){
				start_rcv_var_flag=1;
				rcv_var_timer.start(3);
				r_var_pos=0;
				exp_len=3;
			}
		}
		if(start_rcv_var_flag){
			r_var_frame.data[r_var_pos]=c;
			r_var_pos++;
			if(r_var_pos==3){
				exp_len=(r_var_frame.data[r_var_pos-1]+6);
			}
			if(r_var_pos==exp_len){
				if(r_var_frame.data[r_var_pos-1]==0x16&&r_var_frame.data[offset_len]==r_var_frame.data[offset_len+1]){
					if(r_var_frame.data[r_var_pos-2]==sum(&r_var_frame.data[offset_control],exp_len-6)){
						r_var_frame.len=r_var_pos;
						r_var_pos=0;
						start_rcv_var_flag=0;
						rcv_var_timer.stop();
						pdump(DEBUG_INFO,"get var frame",&r_var_frame.data[0],r_var_frame.len);
						deal_frame(&r_var_frame);
						break;
					}else{
						fail=1;
						pfunc(DEBUG_ERROR,"get var frame err1\n");
					}
				}else{
					pfunc(DEBUG_ERROR,"get var frame err2\n");
					fail=1;
				}
			}else if(r_var_pos>exp_len||r_var_pos>FRAME_BUFF){
				fail=1;
				pfunc(DEBUG_ERROR,"get var frame err3\n");
			}
		}
		if(rcv_var_timer.is_reached()==1){
				start_rcv_var_flag=0;
				r_var_pos=0;
		}
		if(fail){
				start_rcv_var_flag=0;
				r_var_pos=0;
				rcv_var_timer.stop();
		}
	}
	return ret;
}
int link_layer_101::link_time(){
	int ret;
	ret=0;
	pfunc(DEBUG_DEBUG,"time of 101 frame of link [%d]\n",port);
	if(balance==UNBALANCE)
		return 0;
	if(rep_timer.is_reached()==1){
		pfunc(DEBUG_ERROR,"timeout\n");
		ret=send_frame(&last_send_frame);
		if(ret<0){
			goto err;
		}
		rep_times++;
		if(rep_times>=REP_TIMES){
			link_state=LINK_DISCONNECT;
		}else{
			rep_timer.restart();
		}
	}
err:		
	return ret;
}
void link_layer_101::deal_timeout(){
	int ret=0;
	event *e;
	e=NULL;

	link_time();
	if(yk_data.time.is_reached()==1){//60s
		yk_data.cur_state=0;
	}
	if(app->get_event_data(port,e,0)==1){
		process|=PROCESS_EVENT;
		has_data=1;
		if(balance == BALANCE){
			if(event_data.need_ack[port]==0){
				ret=app->build_event_data(&s_var_frame,this,e);
				event_data.need_ack[port]=1;
				goto end;
			}
		}
	}
	event_yc *e_yc;
	if(app->get_yc_cg_data(port,e_yc)){
		process|=PROCESS_YC_CHANGE;
		if(balance == BALANCE){
			if(event_data.need_yc_ack[port]==0){
				ret=app->build_yc_cg_data(&s_var_frame,this,e_yc);
				event_data.need_yc_ack[port]=1;
			}
		}
	}
end:if(ret){
		if(link_state==LINK_OPEN){
			if(!rep_timer.is_start()){
				send_frame(&s_var_frame);
				save_frame(&s_var_frame,2);//save frame
				rep_timer.start(REP_TIME);
			}
		}
	}
	if(app->need_reset)
		app->do_reset();
	if(app->need_update)
		app->do_update();
}
/****************************
 *	realize link_layer_104 
****************************/
void link_layer_104::set_loc_ctl(){

}
int link_layer_104::check_type(unsigned char c){
	if((c&3)==3)
		return TYPE_U;
	if((c&1)==0)
		return TYPE_I;
	if((c&1)==1)
		return TYPE_S;
	return 0;
}

int link_layer_104::build_sframe(frame *out,sfmt &sf){
	int ret=0;
	int i;
	i=0;
	out->data[i++]=0x68;
	out->data[i++]=4;
	out->data[i++]=sf.data.d1;
	out->data[i++]=sf.data.d2;
	out->data[i++]=sf.data.d3;
	out->data[i++]=sf.data.d4;
	ret=i;
	out->len=ret;
	return ret;
}
int link_layer_104::build_uframe(frame *out,ufmt &uf){
	int ret=0;
	int i;
	i=0;
	out->data[i++]=0x68;
	out->data[i++]=4;
	out->data[i++]=uf.data.d1;
	out->data[i++]=uf.data.d2;
	out->data[i++]=uf.data.d3;
	out->data[i++]=uf.data.d4;
	ret=i;
	out->len=ret;
	return ret;
}
int link_layer_104::build_test_link(){
	int ret;
	int i;
	ufmt uf;
	frame *out;
	i=0;
	uf.bit.testfr_cmd=1;
	out=&s_u_frame;
	out->data[i++]=0x68;
	out->data[i++]=0x4;
	out->data[i++]=uf.data.d1;
	out->data[i++]=uf.data.d2;
	out->data[i++]=uf.data.d3;
	out->data[i++]=uf.data.d4;
	out->len=i;
	ret=i;
	return ret;
}
int link_layer_104::build_link_layer(frame *out,int asdu_len){
	int ret=0;
	int i;
	i=0;
	ifmt tmpif;
	tmpif.bit.r_no=r_no;
	tmpif.bit.s_no=s_no;
	out->data[i++]=0x68;
	out->data[i++]=asdu_len+4;
	out->data[i++]=tmpif.data.d1;
	out->data[i++]=tmpif.data.d2;
	out->data[i++]=tmpif.data.d3;
	out->data[i++]=tmpif.data.d4;
	ret=i+asdu_len;
	out->len=ret;
	out->id=s_no;
	var_frame *o;
	o=(var_frame *)out;
	s_i_frames.push(*o);
	s_no=(s_no+1)%N;
	return ret;
}
int link_layer_104::clear_sq(){
	int ret;
	ret=0;
	var_frame fm;
	CircleQueue<var_frame>::iterator it(s_i_frames.MaxQueue);
	it=s_i_frames.begin();
	while(it!=s_i_frames.end()){
		fm=*it;
		if(fm.id<=ack_no){
			if(it==s_i_frames.begin())
				s_i_frames.pop();
		}
		it++;
	}
	return ret;
}
void link_layer_104::resend(){
	CircleQueue<var_frame>::iterator it;
	it=s_i_frames.begin();
	var_frame fm;
	while(it!=s_i_frames.end()){
		fm=*it;
		send_frame(&fm);
		it++;
	}
}
int link_layer_104::deal_frame(frame *in){
	int ret;
	int ti;
	ret=0;
	frame *out;
	out=&s_i_frame;
	if(in->type==TYPE_I){//for I frame
		ifmt tmpif;
		tmpif.data.d1=in->data[offset_control];
		tmpif.data.d2=in->data[offset_control+1];
		tmpif.data.d3=in->data[offset_control+2];
		tmpif.data.d4=in->data[offset_control];
		if(tmpif.bit.s_no!=r_no){//sequence error
			pfunc(DEBUG_ERROR,"sequence error\n");
			//not check send no sequence
			//link_state=LINK_ERROR;
			//return ret;
		}
		if(tmpif.bit.r_no==s_no){
			t1_timer.stop();
		}
		t2_timer.restart(T2_TIME);
		t3_timer.restart(T3_TIME);
		rcv_count++;
		ack_no=tmpif.bit.r_no;
		if(ack_no==s_no){
			t1_timer.stop();
		}
		clear_sq();
		r_no=(r_no+1)%N;
		ti=in->data[offset_ti];
		if(s_i_frames.full()||send_count>=send_num||link_state!=LINK_OPEN)//not deal frame if send queue is full or send count is reached limit or link is not open
			return ret;
		switch(ti){
			case COMMAND_LINK_FINI://link finish
				on_link(in,out);
				ret=process_link(out);
				break;
			case COMMAND_SUMMON://total sum
				process|=PROCESS_SUMMON;
				on_summon(in,out);
				ret=process_summon(out);
				break;
			case COMMAND_CLOCK://clock 
				process|=PROCESS_CLOCK;
				on_clock(in,out);
				ret=process_clock(out);
				break;
			case COMMAND_RM_CTL:
			case COMMAND_RM_CTL_D:
				process|=PROCESS_RM_CTL;
				on_yk(in,out);
				ret=process_yk(out);
				break;
			case COMMAND_TEST_LINK:
				process|=PROCESS_TEST_LINK;
				on_test_link(in,out);
				ret=process_test_link(out);
				break;
			case COMMAND_RESET:
				process|=PROCESS_RESET;
				on_reset_terminal(in,out);
				ret=process_reset_terminal(out);
				break;
			case COMMAND_FILE:
				process|=PROCESS_FILE;
				on_file(in,out);
				ret=process_file(out);
				break;
			case COMMAND_RD_UNIT:
				process|=PROCESS_RD_UNIT;
				on_rd_unit(in,out);
				ret = process_rd_unit(out);
				break;
			case COMMAND_WR_UNIT:
				process|=PROCESS_WR_UNIT;
				on_wr_unit(in,out);
				ret=process_wr_unit(out);
				break;
			case COMMAND_RD_DZ:
				process|=PROCESS_RD_DZ;
				on_rd_dz(in,out);
				ret=process_rd_dz(out);
				break;
			case COMMAND_WR_DZ:
				process|=PROCESS_WR_DZ;
				on_wr_dz(in,out);
				ret=process_wr_dz(out);
				break;
			case COMMAND_SUMMON_ACC:
				process|=PROCESS_SUMMON_ACC;
				on_summon_acc(in,out);
				ret=process_summon_acc(out);
				break;
			case COMMAND_UPDATE:
				process|=PROCESS_UPDATE;
				on_update(in,out);
				ret=process_update(out);
				break;
		}
		rcv_count --;
		if(ret>0){
			send_frame(&s_i_frame);
			t2_timer.stop();//no need send s frame
			t1_timer.start(T1_TIME);//send new i frame ,need ack
		}
	}else if(in->type==TYPE_S){//for S frame
		t3_timer.restart(T3_TIME);
		sfmt tmpsf;
		tmpsf.data.d1=in->data[offset_control];
		tmpsf.data.d2=in->data[offset_control+1];
		tmpsf.data.d3=in->data[offset_control+2];
		tmpsf.data.d4=in->data[offset_control+3];
		ack_no=tmpsf.bit.r_no;
		if(ack_no==s_no){
			t1_timer.stop();
		}
		clear_sq();
	}else if(in->type==TYPE_U){//for U frame
		t3_timer.restart(T3_TIME);
		ufmt tmpuf;
		tmpuf.data.d1=in->data[offset_control];
		tmpuf.data.d2=in->data[offset_control+1];
		tmpuf.data.d3=in->data[offset_control+2];
		tmpuf.data.d4=in->data[offset_control+3];
		if(tmpuf.bit.startdt_cmd){
			tmpuf.bit.startdt_cmd=0;
			tmpuf.bit.startdt_ack=1;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);
			link_state=LINK_OPEN;
			app->build_link_fini(&s_i_frame,this);
			send_frame(&s_i_frame);
			t1_timer.restart(T1_TIME);//send i frame ,need ack
			pfunc(DEBUG_ERROR,"connect\n");
		}else if(tmpuf.bit.startdt_ack==1){
			link_state=LINK_OPEN;
			t1_timer.stop();//rcv strartdt ack,stop timer
			pfunc(DEBUG_ERROR,"connect\n");
		}else if(tmpuf.bit.stopdt_cmd==1){
			tmpuf.bit.stopdt_cmd=0;
			tmpuf.bit.stopdt_ack=1;
			link_state=LINK_CLOSE;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);//rcv stopcmd send ack
		}else if(tmpuf.bit.stopdt_ack==1){
			link_state=LINK_CLOSE;
		}else if(tmpuf.bit.testfr_cmd==1){
			tmpuf.bit.testfr_cmd=0;
			tmpuf.bit.testfr_ack=1;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);//rcv testframe ,send ack
		}else if(tmpuf.bit.testfr_ack==1){
			t1_timer.stop();//rcv testframe ack,stop time1
		}
	}
	return ret;
}
int link_layer_104::deal_process(){
	int ret=0;
	frame *out;
	out=&s_i_frame;
	send_count=s_i_frames.size();
	if(s_i_frames.full()||send_count>=send_num||link_state!=LINK_OPEN)
		return ret;
	if(rcv_count>=rcv_num){
		sfmt sf;
		sf.bit.r_no=r_no;
		build_sframe(&s_s_frame,sf);
		send_frame(&s_s_frame);
		rcv_count=0;
	}
	if(process & PROCESS_EVENT){
		ret = process_event(out);
	}else if(process & PROCESS_SUMMON){
		ret=process_summon(out);
	}else if(process & PROCESS_SUMMON_ACC){
		ret=process_summon_acc(out);
	}else if(process & PROCESS_YC_CHANGE){
		ret=process_yc_change(out);
	}else if(process & PROCESS_RM_CTL){
		ret=process_yk(out);
	}else if(process & PROCESS_RD_DZ){
		ret=process_rd_dz(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_UPDATE){
		ret=process_update(out);
	}
	if(ret>0){
		send_frame(&s_i_frame);
		t2_timer.stop();
		t1_timer.restart();//send new i frame ,need ack
	}
	return ret;
}
int link_layer_104::get_frame(){
	int ret;
	unsigned char c;
	int flag;
	if(com==NULL)
		return -1;
	pfunc(DEBUG_DEBUG,"get frame of link [%d]\n",port);
	while(1){
		ret=com->get_byte(&c);
		if(ret==-1)
			break;
		if(c==0x68){
			if(!start_rcv_s_flag){
				start_rcv_s_flag=1;
			}
			if(!start_rcv_u_flag){
				start_rcv_u_flag=1;
			}
			if(!start_rcv_i_flag){
				start_rcv_i_flag=1;
			}
		}
		flag=start_rcv_s_flag+start_rcv_u_flag+start_rcv_i_flag;
		if(flag){
			r_tmp_frame.data[r_tmp_pos++]=c;
			if(r_tmp_pos==1){
				exp_len=4;
			}else if(r_tmp_pos==2){
				exp_len=c+2;
			}else if(r_tmp_pos==3){
				frame_type=check_type(c);
				if(frame_type==TYPE_I){
					start_rcv_u_flag=0;
					start_rcv_s_flag=0;
					r_s_pos=0;
					r_u_pos=0;
				}else if(frame_type==TYPE_S){
					start_rcv_i_flag=0;
					start_rcv_u_flag=0;
					r_i_pos=0;
					r_u_pos=0;
				}else if(frame_type==TYPE_U){
					start_rcv_i_flag=0;
					start_rcv_s_flag=0;
					r_s_pos=0;
					r_i_pos=0;
				}
			}
			if(r_tmp_pos>=exp_len||r_tmp_pos>=FRAME_BUFF){
				r_tmp_pos=0;
			}
		}
		if(start_rcv_s_flag){
			r_s_frame.data[r_s_pos++]=c;
			if(r_s_pos==exp_len){
				r_s_frame.len=r_s_pos;
				pdump(DEBUG_INFO,"get s frame:",&r_s_frame.data[0],r_s_frame.len);
				deal_frame(&r_s_frame);
				ret=1;
				r_s_pos=0;
				start_rcv_s_flag=0;
				r_s_frame.len=0;
				exp_len=0;
				break;
			}else if(r_s_pos>exp_len||r_s_pos>=FRAME_BUFF){
				start_rcv_s_flag=0;
				r_s_pos=0;
				r_s_frame.len=0;
			}
		}
		if(start_rcv_u_flag){
			r_u_frame.data[r_u_pos++]=c;
			if(r_u_pos==exp_len){
				r_u_frame.len=r_u_pos;
				pdump(DEBUG_INFO,"get u frame:",&r_u_frame.data[0],r_u_frame.len);
				deal_frame(&r_u_frame);
				ret=1;
				start_rcv_u_flag=0;
				r_u_pos=0;
				r_u_frame.len=0;
				exp_len=0;
				break;
			}else if(r_u_pos>exp_len||r_u_pos>=FRAME_BUFF){
				start_rcv_u_flag=0;
				r_u_pos=0;
				r_u_frame.len=0;
			}
		}
		if(start_rcv_i_flag){
			r_i_frame.data[r_i_pos++]=c;
			if(r_i_pos==exp_len){
				r_i_frame.len=r_i_pos;
				pdump(DEBUG_INFO,"get i frame:",&r_i_frame.data[0],r_i_frame.len);
				deal_frame(&r_i_frame);
				ret=1;
				start_rcv_i_flag=0;
				r_i_frame.len=0;
				r_i_pos=0;
				exp_len=0;
				break;
			}else if(r_i_pos>exp_len||r_i_pos>=FRAME_BUFF){
				start_rcv_i_flag=0;
				r_i_frame.len=0;
				r_i_pos=0;
			}
		}
	}
	deal_process();
	return ret;		
}
int link_layer_104::link_time(){
	int ret=0;
	pfunc(DEBUG_DEBUG,"time of 104 frame of link [%d]\n",port);
	if(t1_timer.is_reached()==1){//t1 timeout for no ack testcmd,no ack for i frame,no ack for startdtcmd
		pfunc(DEBUG_ERROR,"t1 timeout\n");
		t1_count++;
		if(t1_count>3){//this part is not mention by 104 
			link_state=LINK_ERROR;
			pfunc(DEBUG_ERROR,"link error\n");
		}else{
			resend();
			t1_timer.restart();
		}
	}
	if(t2_timer.is_reached()==1){//t2 timer for send s frame
		sfmt sf;
		sf.bit.r_no=r_no;
		pfunc(DEBUG_ERROR,"t2 timeout\n");
		ret=build_sframe(&s_s_frame,sf);
		send_frame(&s_s_frame);
	}
	if(t3_timer.is_reached()==1){//t3 timeout for send test cmd u frame
		pfunc(DEBUG_ERROR,"t3 timeout\n");
		build_test_link();
		send_frame(&s_u_frame);
		t3_timer.restart();
	}
	send_count=s_i_frames.size();
	int com_state;
	com_state=check_state();
	switch(link_state){
		case LINK_ERROR:
			com->close();
			com->connect();
			link_state=LINK_DISCONNECT;
			break;
		case LINK_DISCONNECT:
			t0_timer.start(T0_TIME);
			if(t0_timer.is_reached()==1){
				if(com_state!=LINK_CONNECT){
					link_state=LINK_ERROR;//do relink
					pfunc(DEBUG_ERROR,"re link\n");
				}else if(com_state==LINK_CONNECT){
					link_state=LINK_CLOSE;
					r_no=0;
					s_no=0;
					ack_no=0;
					s_i_frames.clear();
					ufmt tmpuf;
					tmpuf.bit.startdt_cmd=1;//send startdt cmd
					build_uframe(&s_u_frame,tmpuf);
					send_frame(&s_u_frame);
					t1_timer.start(T1_TIME);
					pfunc(DEBUG_ERROR,"re open\n");
				}
			}
			break;
	}
	return ret;
}
void link_layer_104::deal_timeout(){
	event *e;
	e=NULL;
	link_time();
	if(yk_data.time.is_reached()==1){//60s
		yk_data.cur_state=0;
	}
	if(app->get_event_data(port,e,0)==1){
		process|=PROCESS_EVENT;
	}
	event_yc *e_yc;
	if(app->get_yc_cg_data(port,e_yc)){
		process|=PROCESS_YC_CHANGE;
	}
	if(app->need_reset)
		app->do_reset();
	if(app->need_update)
		app->do_update();
}
/****************************
 *	realize app_layer 
****************************/
int app_layer::get_link_info(link_layer*link){
	int ret;
	ret=0;
	offset_asdu=link->offset_asdu;
	addr_size=link->addr_size;
	cause_size=link->cause_size;
	msg_id_size=link->msg_id_size;
	addr=link->addr;
	if(addr_size>2){
		ret=ERR_BUILD_APP_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size:%d\n",addr_size);
		goto err;
	}
	if(cause_size>2){
		ret=ERR_BUILD_APP_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size:%d\n",cause_size);
		goto err;
	}
	if(msg_id_size>3){
		ret=ERR_BUILD_APP_FR;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid addr_size:%d\n",msg_id_size);
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu header  
*  @param[out]  out point to out frame 
*  @return upon successful return header number of asdu size\n
*  @note use this function should prepare global data:cmd_id,vsq,cause 	
*  @see		
***********************************************************************
*/
int app_layer::build_asdu_header(frame *out){
	int ret;
	int i=0;
	out->data[offset_asdu+i++]=cmd_id;
	out->data[offset_asdu+i++]=vsq_lo.data;
	out->data[offset_asdu+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset_asdu+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset_asdu+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset_asdu+i++]=addr>>8&0x00ff;
	}
	ret=i;
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of link_fini command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see	cio	
***********************************************************************
*/
int app_layer::build_link_fini(frame*out,link_layer*link){
	int i;
	int ret;
	cio cio;

	cio.data=0;

	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Init;
	cmd_id=COMMAND_LINK_FINI;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0x0;
	out->data[offset_asdu+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0x0;
	out->data[offset_asdu+i++]=cio.data;//coi
	ret=i;
	ret=link->build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of summon confirm command
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_summon_con(frame*out,link_layer*link){
	int i;
	int ret;
	qoi qoi;

	qoi.data=20;
	link->has_data=1;
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;

	link->summon_data.step++;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;
	cmd_id=COMMAND_SUMMON;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0x0;
	out->data[offset_asdu+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=qoi.data;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of summon terminal command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_summon_term(frame *out,link_layer *link){
	int i;
	int ret;
	qoi qoi;

	qoi.data=20;

	link->has_data=0;
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actterm;
	cmd_id=COMMAND_SUMMON;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0x0;
	out->data[offset_asdu+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=qoi.data;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of yx data command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_yx_data(frame *out,link_layer *link){//cause 20
	int i,j;
	i=0;
	int ret= 0;
	int send_num;
	yxdata_t *pyx;
	
	link->has_data=1;
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	pyx=get_yx_data(link->summon_data.sended_yx_num);
	send_num = config_scada_data.SelectYxNum - link->summon_data.sended_yx_num;
	if(send_num > MAX_SEND_YX_PER_FRAME){
		send_num = MAX_SEND_YX_PER_FRAME;
	}else{
		link->summon_data.step++;//next for yc
	}

	link->summon_data.sended_yx_num+=send_num;
	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;
	cmd_id=COMMAND_SP;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=pyx->addr;
	out->data[offset_asdu+i++]=pyx->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset_asdu+i]=pyx[j].statu;
		i++;
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of double yx data command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_dyx_data(frame *out,link_layer *link){//cause 20
	int i,j;
	i=0;
	int ret= 0;
	int send_num;
	yxdata_t *pyx;
	
	link->has_data=1;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	send_num = config_scada_data.SelectDpYxNum - link->summon_data.sended_yx_num;
	pyx=get_yx_data(link->summon_data.sended_yx_num);
	if(send_num > MAX_SEND_YX_PER_FRAME){
		send_num = MAX_SEND_YX_PER_FRAME;
		link->summon_data.sended_yx_num+=send_num;
	}
	else{
		link->summon_data.step++;//next for yc
		link->summon_data.sended_yx_num = 0;
	}

	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;
	cmd_id=COMMAND_DP;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=pyx->addr;
	out->data[offset_asdu+i++]=pyx->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset_asdu+i]=pyx[j].statu;
		i++;
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of yc data command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/int app_layer::build_yc_data(frame *out,link_layer *link){//cause 20
	int i,j;
	i=0;
	int ret= 0;
	int send_num;
	ycdata_t *pyc;
	
	link->has_data=1;
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	send_num = config_scada_data.SelectYcNum - link->summon_data.sended_yc_num;
	pyc=get_yc_data(link->summon_data.sended_yc_num);
	if(send_num > MAX_SEND_YC_PER_FRAME){
		send_num = MAX_SEND_YC_PER_FRAME;
		link->summon_data.sended_yc_num+=send_num;
	}
	else{
		link->summon_data.step++;//next 
		link->summon_data.sended_yc_num = 0;
	}

	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;

	cmd_id=yc_data_type;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=pyc->addr;
	out->data[offset_asdu+i++]=pyc->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(yc_data_type == 9 || yc_data_type ==11){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i+3*j]=pyc[j].deadpass->intdata;
			out->data[offset_asdu+i+3*j+1]=pyc[j].deadpass->intdata>>8;
			out->data[offset_asdu+i+3*j+2]=0;//qds
		}
		ret=i+3*j;
	}else if(yc_data_type == 13){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i+5*j]=pyc[j].deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i+5*j+1]=pyc[j].deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i+5*j+2]=pyc[j].deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i+5*j+3]=pyc[j].deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu+i+5*j+4]=0;//qds
		}
		ret=i+5*j;
	}
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of event data command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_event_data(frame *out,link_layer *link,event* e){//cause 3
	int i;
	i=0;
	int ret;
	siq siq;
	diq diq;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Spont;

	if(e->soe.type==S_YX){
		cmd_id=COMMAND_SP_TIME;
		siq.bit.spi=e->soe.status;
	}else if(e->soe.type == D_YX){
		cmd_id=COMMAND_DP_TIME;
		diq.bit.dpi=e->soe.status==0?1:2;
	}
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=e->soe.datasign;
	out->data[offset_asdu+i++]=e->soe.datasign>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(e->soe.type==S_YX)
		out->data[offset_asdu + i++]=siq.data;
	else if(e->soe.type == D_YX)
		out->data[offset_asdu + i++] = diq.data;
	out->data[offset_asdu+i++]=e->soe.time.millisecond & 0x00ff;
	out->data[offset_asdu+i++]=e->soe.time.millisecond>>8;
	out->data[offset_asdu+i++]=e->soe.time.minute;
	out->data[offset_asdu+i++]=e->soe.time.hour;
	out->data[offset_asdu+i++]=e->soe.time.day;
	out->data[offset_asdu+i++]=e->soe.time.month;
	out->data[offset_asdu+i++]=e->soe.time.year;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of clock confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_clock(frame *out,link_layer *link,CP56Time2a* time){//cause 7
	int i;
	i=0;
	int ret= 0;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	if(link->clock_data.clock_syn){
		cause_lo.bit.cause=CAUSE_Actcon;
		link->clock_data.clock_syn = 0;
	}else if(link->clock_data.clock_rd){
		cause_lo.bit.cause=CAUSE_Req;
		link->clock_data.clock_rd = 0;
	}

	cmd_id=COMMAND_CLOCK;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=time->millisecond & 0x00ff;
	out->data[offset_asdu+i++]=time->millisecond>>8;
	out->data[offset_asdu+i++]=time->minute;
	out->data[offset_asdu+i++]=time->hour;
	out->data[offset_asdu+i++]=time->day;
	out->data[offset_asdu+i++]=time->month;
	out->data[offset_asdu+i++]=time->year;
	ret = i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of yk confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_yk(frame *out,link_layer *link){//cause=7,sel=0 or 1
	int i;
	i=0;
	int ret= 0;
	link->has_data=0;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	if(link->yk_data.cur_state==YK_SEL){
		cause_lo.data=CAUSE_Actcon;
	}else if(link->yk_data.cur_state==YK_DEACTIVE){
		cause_lo.data=CAUSE_Deactcon;
	}else if(link->yk_data.cur_state==YK_ACT){
		cause_lo.data=CAUSE_Actcon;
	}else if(link->yk_data.cur_state==YK_ACTOVER){
		cause_lo.data=CAUSE_Actterm;
		link->reset_yk_data();
	}
	cmd_id=link->yk_data.cmd_id;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=link->yk_data.ctrl_id;
	out->data[offset_asdu+i++]=link->yk_data.ctrl_id>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=link->yk_data.cmd_id == COMMAND_RM_CTL?link->yk_data.sco.data:link->yk_data.dco.data;
	ret = i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of link_test confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_link_test_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;

	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_TEST_LINK;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu + i++]=0xaa;
	out->data[offset_asdu + i++]=0x55;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of yc change command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/int app_layer::build_yc_cg_data(frame *out,link_layer *link,event_yc*e){//cause 3
	int i;
	i=0;
	int ret;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Spont;

	cmd_id=yc_data_type;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=e->data->addr;
	out->data[offset_asdu+i++]=e->data->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(yc_data_type==9||yc_data_type == 11){
		out->data[offset_asdu + i++]=e->data->deadpass->intdata;
		out->data[offset_asdu + i++]=e->data->deadpass->intdata>>8;
		out->data[offset_asdu + i++]=0;
	}else if(yc_data_type == 13){
			out->data[offset_asdu + i++ ]=e->data->deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu + i++ ]=e->data->deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu + i++ ]=e->data->deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu + i++ ]=e->data->deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu + i++ ]=0;//qds
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of reset terminal confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/int app_layer::build_reset_con(frame *out,link_layer *link){//reset terminal cause 7
	int i;
	i=0;
	int ret;
	
	link->has_data=0;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_RESET;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of read dirtionary command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_rd_dir_resp(frame *out,link_layer *link,_rd_dir *dir){//cause 5
	int i;
	int j;
	int ret;
	int bak_pos;
	int n;
	dir_list::iterator it(dir->res_list.MaxQueue);
	dir_node node;
	int data_len;
	i=0;
	n=0;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=0;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Req;

	cmd_id=COMMAND_FILE;
	ret=build_asdu_header(out);
	i=ret;
	data_len=6+1+addr_size;//6:asdu,1:id,1:vsq,2:cause,2:pub addr 
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=2;//addition data type: 2:file transfer
	out->data[offset_asdu+i++]=2;//act: 2:rd dir
	out->data[offset_asdu+i++]=0;//result: 0:successful 1 fail
	out->data[offset_asdu+i++]=dir->id;
	out->data[offset_asdu+i++]=dir->id>>8;
	out->data[offset_asdu+i++]=dir->id>>16;
	out->data[offset_asdu+i++]=dir->id>>24;
	bak_pos=i;
	out->data[offset_asdu+i++]=dir->con;//con
	out->data[offset_asdu+i++]=n;//number of files

	it=dir->res_list.begin();
	n=0;
	while(n<dir->cur_read){
		it++;
		n++;
	}
	int diff1,diff;
	data_len+=i;//
	n=0;
	while(dir->cur_read<dir->res_list.size()){
		node=*it;	
		it++;
		diff1=i;
		out->data[offset_asdu+i++]=node.name_len;
		for(j=0;j<node.name_len;j++){
			out->data[offset_asdu+i++]=node.name[j];
		}
		out->data[offset_asdu+i++]=0;//reverse 
		out->data[offset_asdu+i++]=node.file_size;
		out->data[offset_asdu+i++]=node.file_size>>8;
		out->data[offset_asdu+i++]=node.file_size>>16;
		out->data[offset_asdu+i++]=node.file_size>>24;
		out->data[offset_asdu+i++]=node.time.millisecond & 0x00ff;
		out->data[offset_asdu+i++]=node.time.millisecond>>8;
		out->data[offset_asdu+i++]=node.time.minute;
		out->data[offset_asdu+i++]=node.time.hour;
		out->data[offset_asdu+i++]=node.time.day;
		out->data[offset_asdu+i++]=node.time.month;
		out->data[offset_asdu+i++]=node.time.year;
		n++;
		dir->cur_read++;
		diff=i-diff1;
		data_len+=diff;
		if(data_len>200&&data_len<256){
			dir->con=1;
			break;
		}else if(data_len>256){
			dir->con=1;
			dir->cur_read--;
			i-=diff;
			n--;
			break;
		}
	}
	if(data_len<=200){
		dir->con=0;
		dir->cur_read=0;
		link->process&=~PROCESS_FILE;
	}
	out->data[offset_asdu+bak_pos]=dir->con;//con
	out->data[offset_asdu+bak_pos+1]=n;//
	
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of read file active confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_rd_file_con(frame *out,link_layer *link,_rd_file *file){//cause 7
	int i;
	int j;
	int ret;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=0;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_FILE;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=2;//addition data type: 2:file transfer
	out->data[offset_asdu+i++]=4;//act: 4:rd file confirm
	out->data[offset_asdu+i++]=0;//result: 0:successful 1 fail
	out->data[offset_asdu+i++]=file->res_file.name_len;
	for(j=0;j<file->res_file.name_len;j++)
		out->data[offset_asdu+i++]=file->res_file.name[j];
	out->data[offset_asdu+i++]=file->res_file.file_id;
	out->data[offset_asdu+i++]=file->res_file.file_id>>8;
	out->data[offset_asdu+i++]=file->res_file.file_id>>16;
	out->data[offset_asdu+i++]=file->res_file.file_id>>24;
	out->data[offset_asdu+i++]=file->res_file.file_size;
	out->data[offset_asdu+i++]=file->res_file.file_size>>8;
	out->data[offset_asdu+i++]=file->res_file.file_size>>16;
	out->data[offset_asdu+i++]=file->res_file.file_size>>24;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
	file->step=1;
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of read file segment response command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_rd_file_resp(frame *out,link_layer *link,_rd_file *file){//cause 5
	int i;
	int j;
	int ret;
	char sum;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=0;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Req;

	cmd_id=COMMAND_FILE;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=2;//addition data type: 2:file transfer
	out->data[offset_asdu+i++]=5;//act: 4:rd file segment
	out->data[offset_asdu+i++]=file->res_file.file_id;
	out->data[offset_asdu+i++]=file->res_file.file_id>>8;
	out->data[offset_asdu+i++]=file->res_file.file_id>>16;
	out->data[offset_asdu+i++]=file->res_file.file_id>>24;
	out->data[offset_asdu+i++]=file->cur_offset;
	out->data[offset_asdu+i++]=file->cur_offset>>8;
	out->data[offset_asdu+i++]=file->cur_offset>>16;
	out->data[offset_asdu+i++]=file->cur_offset>>24;
	out->data[offset_asdu+i++]=file->con;
	sum=0;
	for(j=0;j<file->segment.len;j++){
		out->data[offset_asdu+i++]=file->segment.data[j];
		sum+=file->segment.data[j];
	}
	out->data[offset_asdu+i++]=sum;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of write file active confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_wr_file_con(frame *out,link_layer *link,_rd_file *file){//cause 7
	int i;
	int j;
	int ret;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=0;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_FILE;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=2;//addition data type: 2:file transfer
	out->data[offset_asdu+i++]=8;//act: 8:wt file confirm
	out->data[offset_asdu+i++]=file->result;//result: 0:successful 1 fail
	out->data[offset_asdu+i++]=file->req_file.name_len;
	for(j=0;j<file->req_file.name_len;j++)
		out->data[offset_asdu+i++]=file->req_file.name[j];
	out->data[offset_asdu+i++]=file->req_file.file_id;
	out->data[offset_asdu+i++]=file->req_file.file_id>>8;
	out->data[offset_asdu+i++]=file->req_file.file_id>>16;
	out->data[offset_asdu+i++]=file->req_file.file_id>>24;
	out->data[offset_asdu+i++]=file->req_file.file_size;
	out->data[offset_asdu+i++]=file->req_file.file_size>>8;
	out->data[offset_asdu+i++]=file->req_file.file_size>>16;
	out->data[offset_asdu+i++]=file->req_file.file_size>>24;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of write file segment response command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_wr_file_resp(frame *out,link_layer *link,_rd_file *file){//cause 5
	int i;
	int ret;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=0;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Req;

	cmd_id=COMMAND_FILE;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=2;//addition data type: 2:file transfer
	out->data[offset_asdu+i++]=10;//act: 8:wt file segment response
	out->data[offset_asdu+i++]=file->req_file.file_id;
	out->data[offset_asdu+i++]=file->req_file.file_id>>8;
	out->data[offset_asdu+i++]=file->req_file.file_id>>16;
	out->data[offset_asdu+i++]=file->req_file.file_id>>24;
	out->data[offset_asdu+i++]=file->cur_offset;
	out->data[offset_asdu+i++]=file->cur_offset>>8;
	out->data[offset_asdu+i++]=file->cur_offset>>16;
	out->data[offset_asdu+i++]=file->cur_offset>>24;
	out->data[offset_asdu+i++]=file->result;//result: 0:successful 1 fail
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of rd dz unit command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_rd_unit_con(frame *out,link_layer *link,_para_list *para){//cause 7
	int i;
	int ret;
	i=0;
	ret = -1;
	
	link->has_data=0;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_RD_UNIT;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=para->unit;
	out->data[offset_asdu+i++]=para->unit>>8;
	out->data[offset_asdu+i++]=para->min_unit;
	out->data[offset_asdu+i++]=para->min_unit>>8;
	out->data[offset_asdu+i++]=para->max_unit;
	out->data[offset_asdu+i++]=para->max_unit>>8;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of link_fini command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_wr_unit_con(frame *out,link_layer *link,_para_list *para){//cause 7
	int i;
	int ret;
	i=0;
	ret = -1;
	
	link->has_data=0;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_WR_UNIT;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=para->unit;
	out->data[offset_asdu+i++]=para->unit>>8;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of read dz confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_rd_dz_con(frame *out,link_layer *link,_para_list *para){//cause 7
	int i;
	int j;
	int m;
	int ret;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=para->res_num;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_RD_DZ;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=para->unit;
	out->data[offset_asdu+i++]=para->unit>>8;
	out->data[offset_asdu+i++]=para->pi.data;
	for(j=0;j<para->res_num;j++){
		out->data[offset_asdu+i++]=para->nodes[j].id;
		out->data[offset_asdu+i++]=para->nodes[j].id>>8;
		out->data[offset_asdu+i++]=para->nodes[j].tag;
		out->data[offset_asdu+i++]=para->nodes[j].len;
		for(m=0;m<para->nodes[j].len;m++){
			out->data[offset_asdu+i++]=para->nodes[j].para[m];
		}
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of write dz confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_wr_dz_con(frame *out,link_layer *link,_para_list*para){//cause 7,sel =0 or 1 ,cr=0
	int i;
	int j;
	int m;
	int ret;
	i=0;
	ret = -1;
	
	link->has_data=0;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=para->req_num;
	vsq_lo.bit.sq=0;
	if(link->cause_rm.bit.cause==CAUSE_Act){
		cause_lo.bit.cause=CAUSE_Actcon;
	}else if(link->cause_rm.bit.cause==CAUSE_Deact)
		cause_lo.bit.cause=CAUSE_Deactcon;

	cmd_id=COMMAND_WR_DZ;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=para->unit;
	out->data[offset_asdu+i++]=para->unit>>8;
	out->data[offset_asdu+i++]=para->pi.data;
	for(j=0;j<para->req_num;j++){
		out->data[offset_asdu+i++]=para->nodes[j].id;
		out->data[offset_asdu+i++]=para->nodes[j].id>>8;
		out->data[offset_asdu+i++]=para->nodes[j].tag;
		out->data[offset_asdu+i++]=para->nodes[j].len;
		for(m=0;m<para->nodes[j].len;m++){
			out->data[offset_asdu+i++]=para->nodes[j].para[m];
		}
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of acc summon confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_summon_acc_con(frame *out,link_layer *link){//cause 7
	int i;
	int ret;
	i=0;
	ret = -1;
	link->summon_data.step++;	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_SUMMON_ACC;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=5;//QCC summon acc data
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of acc summon data terminal command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_summon_acc_term(frame *out,link_layer *link){//cause 10
	int i;
	int ret;
	i=0;
	ret = -1;
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actterm;

	cmd_id=COMMAND_SUMMON_ACC;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=5;//QCC summon acc data
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;	
}
/**
***********************************************************************
*  @brief	build asdu of  acc data response command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_summon_acc_resp(frame *out,link_layer *link){//cause 37
	int i,j;
	i=0;
	int ret= 0;
	int send_num;
	ycdata_t *pyc;
	
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	send_num = config_scada_data.SelectAccNum - link->summon_data.sended_acc_num;
	pyc=get_acc_yc_data(link->summon_data.sended_acc_num);
	if(send_num > MAX_SEND_YC_PER_FRAME){
		send_num = MAX_SEND_YC_PER_FRAME;
		link->summon_data.sended_acc_num+=send_num;
	}
	else{
		link->summon_data.step++;//next 
		link->summon_data.sended_acc_num = 0;
	}

	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Reqcogen;

	if(pyc[0].type == 0){
		cmd_id=COMMAND_ACC;
	}else{
		cmd_id=COMMAND_ACC_TIME;
	}
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=pyc->addr;
	out->data[offset_asdu+i++]=pyc->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(pyc[0].type == 0){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu+i++]=5;//
		}
	}else{
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i++]=pyc[j].deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu+i++]=pyc[j].time.millisecond & 0x00ff;
			out->data[offset_asdu+i++]=pyc[j].time.millisecond>>8;
			out->data[offset_asdu+i++]=pyc[j].time.minute;
			out->data[offset_asdu+i++]=pyc[j].time.hour;
			out->data[offset_asdu+i++]=pyc[j].time.day;
			out->data[offset_asdu+i++]=pyc[j].time.month;
			out->data[offset_asdu+i++]=pyc[j].time.year;
			out->data[offset_asdu+i++]=5;//
		}
	}
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
/**
***********************************************************************
*  @brief	build asdu of update confirm command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_update_con(frame *out,link_layer *link,_update_flag *data){//cause 7 sel=1 start,0 stop
	int i;
	i=0;
	int ret= 0;
	
	link->set_loc_ctl();
	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_UPDATE;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=data->start<<7;
	ret=i;
	ret=link->build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
void set_app_interface(app_layer *app){
	app->get_yx_data=get_yx_data;
	app->get_yc_data=get_yc_data;
	app->get_acc_yc_data=get_acc_yc_data;
	app->get_event_data=get_event_data;
	app->get_clock=get_clock;
	app->do_yk=do_yk;
	app->do_reset=do_reset;
	app->get_yc_cg_data=get_yc_cg_data;
	app->get_dir_data=get_dir_data;
	app->get_file_data=get_file_data;
	app->save_file_data=save_file_data;
	app->get_file_segment=get_file_segment;
	app->save_file_segment=save_file_segment;
	app->get_dz_unit=get_dz_unit;
	app->set_dz_unit=set_dz_unit;
	app->get_dz_data=get_dz_data;
	app->set_dz=set_dz;
	app->do_update=do_update;
}
// vim:tw=72
