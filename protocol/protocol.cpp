/*************************************************************************
	> File Name: protocol.cpp
	> Author: hanhj
	> Mail: hanhj@zx-jy.com 
	> Created Time: 2019年02月19日 星期二 22时09分40秒
************************************************************************/
#include "protocol.h"
#include "configurable.h"
#include "para.h"
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
int link_layer::set_link_com(com_port*c,int p){
	if(c!=NULL){
		port=p;
		com=c;
		cout<<"link link_layer "<<port<<" to com_port "<<c->port_no<<endl;
		return 0;
	}
	pfunc(DEBUG_ERROR,"invalid para\r");
	return -1;
}
int link_layer::set_app(app_layer*ap){
	app=ap;
	return 0;
}
int link_layer::save_frame(frame *f){
	memcpy(&last_send_frame,f,sizeof(last_send_frame));
	return 0;
}
int link_layer::send_frame(frame *f){
	int ret;
	ret=0;
	if(com==NULL||f==NULL){
		ret=ERR_INVALID_PARA;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid para\r");
		goto err;
	}
	pfunc(DEBUG_INFO,"send frame of link [%d]",port);
	if(f->valid==1){
		ret=com->send(f->data,f->len);
		if(ret<0){
			goto err;
		}
		f->valid=0;
	}else{
		ret=ERR_INVALID_PARA;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid frame\r");
		goto err;
	}
err:
	return ret;
}
int link_layer::check_state(){
	pfunc(DEBUG_INFO,"check state of link [%d] state=%d\n",port,link_state);
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
	if(in->data[offset_cause]==CAUSE_Act){
		clock_data.clock_syn = 1;
	}else if(in->data[offset_cause] == CAUSE_Req){
		clock_data.clock_rd = 1;
	}
	return ret;
}
int link_layer::process_clock(frame *out){
	int ret;
	ret=app->build_clock(out,this);
	return ret;
}
void link_layer::reset_yk_data(){
	yk_data.sel=0;
	yk_data.act=0;
	yk_data.start_timeout=0;
	yk_data.timeout=0;
	process&=~PROCESS_RM_CTL;
}
int link_layer::on_yk(frame *in,frame *out){
	int ret =0;
	int ctrl;
	has_data=1;
	yk_data.cmd_id=in->data[offset_ti];
	yk_data.ctrl_id=in->data[offset_data];
	yk_data.ctrl_id|=in->data[offset_data+1]<<8;
	yk_data.sco.data=in->data[offset_data+2];
	yk_data.dco.data=yk_data.sco.data;
	
	if(yk_data.ctrl_id == COMMAND_RM_CTL){
		ctrl=yk_data.sco.bit.scs;
	}else if(yk_data.ctrl_id == COMMAND_RM_CTL_D){
		ctrl=yk_data.dco.bit.dcs==1?0:1;
	}
	if(in->data[offset_cause]==CAUSE_Act){
		if(yk_data.sco.bit.sel==0){//sel
			if(yk_data.sel==0){
				yk_data.sel=1;
				ret=app->do_yk(yk_data.ctrl_id,1,ctrl);//do check
				if(ret<0)
					yk_data.fail=1;
				else if(yk_data.start_timeout==0)
					yk_data.start_timeout = 1;
			}else//re select
				yk_data.fail=1;
		}else if(yk_data.sco.bit.sel==1){//act
			if(yk_data.sel==1){
				yk_data.act=1;
				ret=app->do_yk(yk_data.ctrl_id,0,ctrl);//do act 
				if(ret<0)
					yk_data.fail=1;
			}else{
				yk_data.fail=1;
			}
		}
	}else if(in->data[offset_cause] == CAUSE_Deact){
		yk_data.deactive=1;
	}
	if(yk_data.fail){
		ret=-1;
		reset_yk_data();

	}
	if(yk_data.deactive){
		reset_yk_data();
	}

	return ret;
}
int link_layer::process_yk(frame *out){
	int ret;
	ret=app->build_yk(out,this);
	return ret;
}
int link_layer::process_event(frame *out){
	int ret=-1;
	event *e;
	e=NULL;
	if(app->get_event_data(port,e,1)==1){
		ret=app->build_event_data(out,this,e);
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
	return ret;
}
int link_layer::on_file(frame *in,frame *out){
	int ret=0;
	int i,j;
	i=3;
	j=0;
	file_data.op=in->data[offset_data+i++];
	if(file_data.op == 1){//rd dir
		file_data.rd_dir.id=in->data[offset_data+i++];
		file_data.rd_dir.id|=in->data[offset_data+i++]<<8;
		file_data.rd_dir.id|=in->data[offset_data+i++]<<16;
		file_data.rd_dir.id|=in->data[offset_data+i++]<<24;
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
	}else if(file_data.op == 6){//rd file confirm
		file_data.rd_file.req_file.file_id=in->data[offset_data+i++];
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++]<<8;
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++]<<16;
		file_data.rd_file.req_file.file_id|=in->data[offset_data+i++]<<24;
		
		file_data.rd_file.ack_offset=in->data[offset_data+i++];
		file_data.rd_file.ack_offset|=in->data[offset_data+i++]<<8;
		file_data.rd_file.ack_offset|=in->data[offset_data+i++]<<16;
		file_data.rd_file.ack_offset|=in->data[offset_data+i++]<<24;
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
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++]<<8;
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++]<<16;
		file_data.wt_file.req_file.file_id|=in->data[offset_data+i++]<<24;

		file_data.wt_file.req_file.file_size=in->data[offset_data+i++];
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++]<<8;
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++]<<16;
		file_data.wt_file.req_file.file_size|=in->data[offset_data+i++]<<24;
		file_data.wt_file.suc=0;
	}else if(file_data.op == 9){
		int data_len;
		long tmp_id;
		data_len=in->data[offset_len] - 1 - addr_size - 1 - 1 - cause_size - addr_size - msg_id_size;
		tmp_id=in->data[offset_data+i++];
		tmp_id|=in->data[offset_data+i++]<<8;
		tmp_id|=in->data[offset_data+i++]<<16;
		tmp_id|=in->data[offset_data+i++]<<24;
		file_data.wt_file.cur_offset=in->data[offset_data+i++];
		file_data.wt_file.cur_offset|=in->data[offset_data+i++]<<8;
		file_data.wt_file.cur_offset|=in->data[offset_data+i++]<<16;
		file_data.wt_file.cur_offset|=in->data[offset_data+i++]<<24;
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
	int ret=-1;
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
			if(ret>=0)
				ret=app->build_rd_file_con(out,this,&file_data.rd_file);
		}else{
			ret=app->get_file_segment(&file_data.rd_file);
			if(ret>=0)
				ret=app->build_rd_file_resp(out,this,&file_data.rd_file);
		}
	}else if(file_data.op==6){//read file ack confirm
		ret=0;
		//change file offset_asdu
		if(file_data.rd_file.ack_offset == file_data.rd_file.cur_offset)
			file_data.rd_file.cur_offset=file_data.rd_file.cur_offset+file_data.rd_file.segment.len;
		file_data.op=3;
	}else if(file_data.op==7){//wr file
		ret=app->save_file_data(&file_data.wt_file);
		if(ret>=0)
			ret=app->build_wt_file_con(out,this,&file_data.wt_file);
		if(strcmp(file_data.wt_file.req_file.name,"update.bin")==0){
			file_data.update_flag.save_ok=0;
		}
	}else if(file_data.op==9){//wr file segment
		ret=app->save_file_segment(&file_data.wt_file);
		if(ret>=0)
			ret=app->build_wt_file_resp(out,this,&file_data.wt_file);
		
		if(strcmp(file_data.wt_file.req_file.name,"update.bin")==0 && file_data.wt_file.suc==1){
			file_data.update_flag.save_ok=1;
		}
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
	para_data.unit=in->data[offset_data+i++];
	para_data.unit|=in->data[offset_data+i++]<<8;
	if(para_data.req_num==0){
		para_data.op=2;//read all para 
		para_data.req_num=30;//???
		for(j=0;j<para_data.req_num;j++){
			para_data.req_id[j]=j+0x5001;
		}
	}
	else{
		para_data.op=1;//read mul para 
		for(j=0;j<para_data.req_num;j++){
			para_data.req_id[j]=in->data[offset_data+i++];
			para_data.req_id[j]|=in->data[offset_data+i++]<<8;
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
	has_data=0;
	for(i=para_data.cur_read;i<para_data.req_num;i++){
		para_data.nodes[j].id=para_data.req_id[i];
		len=app->get_dz_data(&para_data.nodes[j]);
		data_len+=len;
		para_data.cur_read++;
		j++;
		if(data_len>200)
			break;
	}
	if(para_data.cur_read==para_data.req_num)
		para_data.pi.bit.con=0;
	else
		para_data.pi.bit.con=1;
	para_data.res_num=j;
	ret=app->build_rd_dz_con(out,this,&para_data);
	return ret;
}

int link_layer::on_rd_unit(frame *in,frame *out){
	int ret=0;
	has_data=1;
	return ret;
}
int link_layer::process_rd_unit(frame *out){
	int ret=0;
	app->get_dz_unit(&para_data);
	ret=app->build_rd_unit_con(out,this,&para_data);
	return ret;
}

int link_layer::on_wr_dz(frame *in,frame *out){
	int ret=0;
	int i=0;
	int j;
	int m;
	has_data=1;
	send_cause cause;
	cause.data=in->data[offset_cause];

	para_data.req_num=in->data[offset_vsq]&0x7f;
	para_data.unit=in->data[offset_data+i++];
	para_data.unit|=in->data[offset_data+i++]<<8;
	para_data.pi.data=in->data[offset_data+i++];
	if(para_data.pi.bit.se==1){
		para_data.op=3;//prepare set
	}else if(para_data.pi.bit.cr==1){
		para_data.op=5;//cancel set 
	}else if(para_data.pi.bit.se==0){
		para_data.op=4;//fix
	}else if(cause.bit.cause==CAUSE_Deact){
		para_data.op=0;
	}
	for(j=0;j<para_data.req_num;j++){
		para_data.nodes[j].id=in->data[offset_data+i++];
		para_data.nodes[j].id|=in->data[offset_data+i++]>>8;
		para_data.nodes[j].id|=in->data[offset_data+i++]>>16;
		para_data.nodes[j].id|=in->data[offset_data+i++]>>24;
		para_data.nodes[j].tag=in->data[offset_data+i++];
		para_data.nodes[j].len=in->data[offset_data+i++];
		for(m=0;m<para_data.nodes[j].len;m++){
			para_data.nodes[j].para[m]=in->data[offset_data+i++];
		}
	}
	return ret;
}
int link_layer::process_wr_dz(frame *out){
	int ret=0;
	int i;
	if(para_data.op==4){
		for(i=0;i<para_data.req_num;i++){
			app->set_dz(para_data.req_num,&para_data.nodes[0]);
		}
		para_data.op=0;	
	}else if(para_data.op==5){
		para_data.req_num=0;
		para_data.op=0;	
	}else if(para_data.op==0){
		para_data.req_num=0;
	}
	ret=app->build_wr_dz_con(out,this,&para_data);
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
	app->set_dz_unit(para_data.unit);
	ret=app->build_wr_unit_con(out,this,&para_data);
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
	return ret;
}

/****************************
 *  realize link_layer_101
****************************/
int link_layer_101::build_ack(frame*out,int has_data){
	int ret;
	ret=0;
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=0;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=has_data;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=0;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
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
	out->valid=1;
	ret=i;
err:
	return ret;
}
int link_layer_101::build_nak(frame*out){
	int ret;
	ret=0;
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=1;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=1;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
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
	out->valid=1;
	ret=i;
err:
	return ret;

}
int link_layer_101::build_link_ack(frame*out){
	int ret;
	ret=0;
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=11;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=11;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
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
	out->valid=1;
	ret=i;
err:
	return ret;
}
int link_layer_101::build_link_req(frame*out){
	int ret;
	ret=0;
	int i=0;
	if(balance==1){
		ctl_lo.pm.fc=9;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl_lo.data;
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
		out->valid=1;
	}	
	ret=i;
err:
	return ret;
}
int link_layer_101::build_reset_link(frame*out){
	int ret;
	ret=0;
	int i=0;
	if(balance==1){
		ctl_lo.pm.fc=0;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.fcb=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl_lo.data;
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
		out->valid=1;
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
	out->valid=1;
	ret=len;
err:
	return ret;
}
void link_layer_101::set_loc_ctl(){
	if(!balance){
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
	cout<<"on_ack"<<endl;
	if(process & PROCESS_EVENT){
		ret = process_event(out);
	}else if(process & PROCESS_SUMMON){
		ret=process_summon(out);
	}else if(process & PROCESS_SUMMON_ACC){
		ret=process_summon_acc(out);
	}else if(process & PROCESS_YC_CHANGE){
		ret=process_yc_change(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_RESET){
		app->need_reset=1;
	}
	return ret ;
}
//for unbalance
int link_layer_101::on_req_class_1(frame *in,frame *out){
	int ret ;
	cout<<"on_req_class_1"<<endl;
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
		app->need_reset=1;
	}
	return ret ;
}
//for unbalance
int link_layer_101::on_req_class_2(frame *in,frame *out){
	int ret;
	ret=-1;
	cout<<"on_req_class_2"<<endl;
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
//10ms
void link_layer_101::deal_timeout(){
	int ret=0;
	event *e;
	e=NULL;

	link_time();
	if(yk_data.start_timeout==1){
		yk_data.timeout++;
		if(yk_data.timeout ==6000){//60s
			yk_data.start_timeout = 0;
			yk_data.timeout=0;
			yk_data.sel=0;
			yk_data.act=0;
			yk_data.deactive=0;
			yk_data.act_over=0;
		}
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
	}else{
		process &=(~PROCESS_EVENT);
		event_data.need_ack[port] = 0;
		has_data=0;
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
	}else{
		process &=~PROCESS_YC_CHANGE;
		event_data.need_yc_ack[port] = 0;
	}
end:if(ret){
		send_frame(&s_var_frame);
	}
	if(app->need_reset)
		app->do_reset();
	if(app->need_update)
		app->do_update();
}

int link_layer_101::on_req(frame *in,frame *out){
	int ret;
	ret= -1;
	int ti;
	ti=in->data[offset_ti];
	switch(ti){
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
int link_layer_101::get_frame(){
	int ret;
	unsigned char c;
	int fail=0;
	if(com==NULL)
		return -1;
	pfunc(DEBUG_INFO,"get frame of link [%d]\n",port);
	ret=com->get_byte(&c);
	if(ret==-1)
		return -1;
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
					r_fix_frame.valid=1;
					rcv_fix_timer.stop();
					start_rcv_fix_flag=0;
					r_fix_pos=0;
					pdump(DEBUG_WARNING,"get fix frame",&r_fix_frame.data[0],r_fix_frame.len);
					deal_frame(&r_fix_frame);
					return 1;
				}else{
					fail=1;
				}
			}else{
				fail=1;
			}
		}else if(r_fix_pos>(addr_size+4)){
			fail=1;
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
					r_var_frame.valid=1;
					r_var_pos=0;
					start_rcv_var_flag=0;
					rcv_var_timer.stop();
					pdump(DEBUG_WARNING,"get var frame",&r_var_frame.data[0],r_var_frame.len);
					deal_frame(&r_var_frame);
					return 1;
				}else{
					fail=1;
				}
			}else{
				fail=1;
			}
		}else if(r_var_pos>exp_len){
			fail=1;
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
	return 0;		
}
int link_layer_101::link_time(){
	int ret;
	ret=0;
	pfunc(DEBUG_INFO,"time of 101 frame of link [%d]\n",port);
	if(balance==0)
		return 0;
	if(rep_timer.is_reached()==1){
		ret=send_frame(&last_send_frame);
		if(ret<0){
			goto err;
		}
		rep_times++;
		if(rep_times>=REP_TIMES){
			link_state=LINK_NOCONNECT;
		}else{
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
			ret=on_fc3(f);
			break;
		case 4:
			ret=on_fc4(f);
			break;
		case 5:
			ret=on_fc5(f);
			break;
		case 6:
			ret=on_fc6(f);
			break;
		case 7:
			ret=on_fc7(f);
			break;
		case 8:
			ret=on_fc8(f);
			break;
		case 9:
			ret=on_fc9(f);
			break;
		case 10:
			ret=on_fc10(f);
			break;
		case 11:
			ret=on_fc11(f);
			break;
	}
	return ret;
}

int link_layer_101::on_fc0(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc0"<<endl;
	if(balance!=BALANCE){
		if(link_step==2){
			link_step++;//3
			//because remote reset link so reset ctl_lo and ctl_rm
			ctl_lo.pm.fcb=0;
			ctl_rm.pm.fcb=0;
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
			ret=build_link_req(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//5
		}else if(link_step==7){
			link_step++;//8
			ret=app->build_link_fini(&s_var_frame,this);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_var_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//9
			save_frame(&s_var_frame);//save frame
			rep_timer.start(REP_TIME);
		}else if(link_step==9){
			link_step++;//10
			link_state=LINK_CONNECT;
			process=0;
			pfunc(DEBUG_ERROR,"connect\n");
		}else{//receive ack frame
			rep_timer.stop();
			rep_times=0;
			ret=on_ack(f,&s_var_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_var_frame);
			if(ret<0){
				goto err;
			}
		}
	}
err:
	return ret;
}
int link_layer_101::on_fc1(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc1"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc2(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc2"<<endl;
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
	ret=0;
	cout<<"on_fc3"<<endl;
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret>=0){
					ret=build_ack(&s_fix_frame,has_data);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}//ack
				}else{
					ret=build_nak(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}
				}
			}else{
				ret=build_nak(&s_fix_frame);
				if(ret<0){
					goto err;
				}
				ret=send_frame(&s_fix_frame);
				if(ret<0){
					goto err;
				}
			}
		}else{
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
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret>=0){
					ret=build_ack(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}//ack
					ret=send_frame(&s_var_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_var_frame);//save frame
					rep_timer.start(REP_TIME);
				}else{
					ret=build_nak(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}
				}
			}else{
				ret=send_frame(&last_send_frame);
				if(ret<0){
					goto err;
				}
			}
		}else{
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
int link_layer_101::on_fc4(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc4"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc5(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc5"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc6(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc6"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc7(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc7"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc8(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc8"<<endl;
//err:
	return ret;
}
int link_layer_101::on_fc9(frame*f){
	int ret;
	ret=0;
	cout<<"on_fc9"<<endl;
	process=PROCESS_LINK;
	link_step=1;
	link_state=LINK_NOCONNECT;
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
	cout<<"on_fc10"<<endl;
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(link_step==4){
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
			link_state=LINK_CONNECT;
			pfunc(DEBUG_ERROR,"connect\n");
			process=0;
		}else{
			if(ctl.pm.fcv){
				if(ctl.pm.fcb!=ctl_rm.pm.fcb){
					ctl_rm.data=ctl.data;//save control
					ret=on_req_class_1(f,&s_var_frame);
					if(ret>=0){
						ret=send_frame(&s_var_frame);
						if(ret<0){
							goto err;
						}
						save_frame(&s_var_frame);//save frame
					}else{
						ret=build_nak(&s_fix_frame);
						if(ret<0){
							goto err;
						}
						ret=send_frame(&s_fix_frame);
						if(ret<0){
							goto err;
						}
					}
				}else{
					ret=send_frame(&last_send_frame);
					if(ret<0){
						goto err;
					}
				}
			}else{
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
	}
err:
	return ret;
}
int link_layer_101::on_fc11(frame *f){
	int ret;
	ret=0;
	cout<<"on_fc11"<<endl;
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
					save_frame(&s_var_frame);//save frame
				}else{
					ret=build_nak(&s_fix_frame);
					if(ret<0){
						goto err;
					}
					ret=send_frame(&s_fix_frame);
					if(ret<0){
						goto err;
					}
				}
			}else{
				ret=send_frame(&last_send_frame);
				if(ret<0){
					goto err;
				}
			}
		}else{
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
			link_step++;//6
			ret=build_reset_link(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			ret=send_frame(&s_fix_frame);
			if(ret<0){
				goto err;
			}
			link_step++;//7
		}
	}	
err:
	return ret;
}
/****************************
 *	realize link_layer_104 
****************************/
void link_layer_104::set_loc_ctl(){

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
	out->data[i++]=uf.d1;
	out->data[i++]=uf.d2;
	out->data[i++]=uf.d3;
	out->data[i++]=uf.d4;

	out->len=i;
	ret=i;
	return ret;

}
int link_layer_104::check_type(unsigned char c){
	if((c&1)==0)
		return TYPE_I;
	if(c&3)
		return TYPE_U;
	if(c&1)
		return TYPE_S;
	return 0;
}
int link_layer_104::get_frame(){
	int ret;
	unsigned char c;
	int flag;
	if(com==NULL)
		return -1;
	pfunc(DEBUG_INFO,"get frame of link [%d]\n",port);
	ret=com->get_byte(&c);
	if(ret==-1)
		return -1;
	ret=0;
	if(c==0x68){
		if(!start_rcv_s_flag){
			start_rcv_s_flag=1;
		}else if(!start_rcv_u_flag){
			start_rcv_u_flag=1;
		}else if(!start_rcv_i_flag){
			start_rcv_i_flag=1;
		}
	}
	flag=start_rcv_s_flag+start_rcv_u_flag+start_rcv_i_flag;
	if(flag){
		r_tmp_frame.data[r_tmp_pos++]=c;
		if(r_tmp_pos==2){
			exp_len=c+2;
		}
		if(r_tmp_pos==3){
			frame_type=check_type(c);
			if(frame_type==TYPE_I){
				start_rcv_u_flag=0;
				start_rcv_s_flag=0;
			}else if(frame_type==TYPE_S){
				start_rcv_i_flag=0;
				start_rcv_u_flag=0;
			}else if(frame_type==TYPE_U){
				start_rcv_i_flag=0;
				start_rcv_s_flag=0;
			}
		}
		if(r_tmp_pos==exp_len){
			r_tmp_pos=0;
		}
	}
	if(start_rcv_s_flag){
		r_s_frame.data[r_s_pos++]=c;
		if(r_s_pos==exp_len){
			r_s_frame.len=r_s_pos;
			deal_frame(&r_s_frame);
			ret=1;
			r_s_pos=0;
			start_rcv_s_flag=0;
			r_s_frame.len=0;
			exp_len=0;
		}else if(r_s_pos>exp_len){
			start_rcv_s_flag=0;
			r_s_pos=0;
			r_s_frame.len=0;
			exp_len=0;
		}
	}else if(start_rcv_u_flag){
		r_u_frame.data[r_u_pos++]=c;
		if(r_u_pos==exp_len){
			r_u_frame.len=r_u_pos;
			deal_frame(&r_u_frame);
			ret=1;
			start_rcv_u_flag=0;
			r_u_pos=0;
			r_u_frame.len=0;
			exp_len=0;
		}else if(r_u_pos>exp_len){
			start_rcv_u_flag=0;
			r_u_pos=0;
			r_u_frame.len=0;
			exp_len=0;
		}
	}else if(start_rcv_i_flag){
		r_i_frame.data[r_i_pos++]=c;
		if(r_i_pos==exp_len){
			r_i_frame.len=r_i_pos;
			deal_frame(&r_i_frame);
			ret=1;
			start_rcv_i_flag=0;
			r_i_frame.len=0;
			r_i_pos=0;
			exp_len=0;
		}else if(r_i_pos>exp_len){
			start_rcv_i_flag=0;
			r_i_frame.len=0;
			r_i_pos=0;
			exp_len=0;
		}
	}
	deal_process();
	return ret;		
}
int link_layer_104::deal_frame(frame *in){
	int ret;
	int ti;
	ret=0;
	frame *out;
	out=&s_i_frame;
	if(in->type==TYPE_I){
		ifmt tmpif;
		tmpif.d1=in->data[offset_control];
		tmpif.d2=in->data[offset_control+1];
		tmpif.d3=in->data[offset_control+2];
		tmpif.d4=in->data[offset_control];
		if(tmpif.bit.s_no!=r_no){//sequence error
			link_state=0;
			return ret;
		}
		t2_timer.restart(3);
		t3_timer.restart(3);

		ack_no=tmpif.bit.r_no;
		clear_sq();
		r_no=(r_no+1)%N;
		ti=in->data[offset_ti];
		switch(ti){
			case COMMAND_SUMMON://total sum
				process|=PROCESS_SUMMON;
				ret=on_summon(in,out);
				ret=process_summon(out);
				break;
			case COMMAND_CLOCK://clock 
				process|=PROCESS_CLOCK;
				ret=on_clock(in,out);
				ret=process_clock(out);
				break;
			case COMMAND_RM_CTL:
			case COMMAND_RM_CTL_D:
				process|=PROCESS_RM_CTL;
				ret=on_yk(in,out);
				ret=process_yk(out);
				break;
			case COMMAND_TEST_LINK:
				process|=PROCESS_TEST_LINK;
				ret=on_test_link(in,out);
				ret=process_test_link(out);
				break;
			case COMMAND_RESET:
				process|=PROCESS_RESET;
				ret=on_reset_terminal(in,out);
				ret=process_reset_terminal(out);
				break;
			case COMMAND_FILE:
				process|=PROCESS_FILE;
				ret=on_file(in,out);
				ret=process_file(out);
				break;
			case COMMAND_RD_UNIT:
				process|=PROCESS_RD_UNIT;
				ret=on_rd_unit(in,out);
				ret = process_rd_unit(out);
				break;
			case COMMAND_WR_UNIT:
				process|=PROCESS_WR_UNIT;
				ret=on_wr_unit(in,out);
				ret=process_wr_unit(out);
				break;
			case COMMAND_RD_DZ:
				process|=PROCESS_RD_DZ;
				ret=on_rd_dz(in,out);
				ret=process_rd_dz(out);
				break;
			case COMMAND_WR_DZ:
				process|=PROCESS_WR_DZ;
				ret=on_wr_dz(in,out);
				ret=process_wr_dz(out);
				break;
			case COMMAND_SUMMON_ACC:
				process|=PROCESS_SUMMON_ACC;
				ret=on_summon_acc(in,out);
				ret=process_summon_acc(out);
				break;
			case COMMAND_UPDATE:
				process|=PROCESS_UPDATE;
				ret=on_update(in,out);
				ret=process_update(out);
				break;
		}
		if(ret>0)
			send_frame(&s_i_frame);
	}else if(in->type==TYPE_S){
		t3_timer.restart(3);
		sfmt tmpsf;
		tmpsf.d1=in->data[offset_control];
		tmpsf.d2=in->data[offset_control+1];
		tmpsf.d3=in->data[offset_control+2];
		tmpsf.d4=in->data[offset_control+3];
		ack_no=tmpsf.bit.r_no;
		clear_sq();
	}else if(in->type==TYPE_U){
		t3_timer.restart(3);
		ufmt tmpuf;
		tmpuf.d1=in->data[offset_control];
		tmpuf.d2=in->data[offset_control+1];
		tmpuf.d3=in->data[offset_control+2];
		tmpuf.d4=in->data[offset_control+3];
		if(tmpuf.bit.startdt_cmd){
			tmpuf.bit.startdt_cmd=0;
			tmpuf.bit.startdt_ack=1;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);
		}else if(tmpuf.bit.startdt_ack==1){
			link_state=LINK_OPEN;
		}else if(tmpuf.bit.stopdt_cmd==1){
			tmpuf.bit.stopdt_cmd=0;
			tmpuf.bit.stopdt_ack=1;
			link_state=LINK_CLOSE;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);
		}else if(tmpuf.bit.stopdt_ack==1){
			link_state=LINK_CLOSE;
		}else if(tmpuf.bit.testfr_cmd==1){
			tmpuf.bit.testfr_cmd=0;
			tmpuf.bit.testfr_ack=1;
			build_uframe(&s_u_frame,tmpuf);
			send_frame(&s_u_frame);
		}
	}
	return ret;
}
int link_layer_104::build_sframe(frame *out,sfmt &sf){
	int ret=0;
	int i;
	i=0;
	out->data[i++]=0x68;
	out->data[i++]=4;
	out->data[i++]=sf.d1;
	out->data[i++]=sf.d2;
	out->data[i++]=sf.d3;
	out->data[i++]=sf.d4;
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
	out->data[i++]=uf.d1;
	out->data[i++]=uf.d2;
	out->data[i++]=uf.d3;
	out->data[i++]=uf.d4;
	ret=i;
	out->len=ret;
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
	out->data[i++]=tmpif.d1;
	out->data[i++]=tmpif.d2;
	out->data[i++]=tmpif.d3;
	out->data[i++]=tmpif.d4;
	ret=i+asdu_len;
	out->len=ret;
	s_i_frames.push(s_no);
	s_no=(s_no+1)%N;
	return ret;
}
int link_layer_104::clear_sq(){
	int ret;
	ret=0;
	int da;
	CircleQueue<int>::iterator it(s_i_frames.MaxQueue);
	it=s_i_frames.begin();
	while(it!=s_i_frames.end()){
		da=*it;
		if(da<=ack_no){
			if(it==s_i_frames.begin())
				s_i_frames.pop();
		}
		it++;
	}
	return ret;
}
int link_layer_104::deal_process(){
	int ret=0;
	frame *out;
	out=&s_i_frame;
	if(s_i_frames.full())
		return ret;
	if(process & PROCESS_EVENT){
		ret = process_event(out);
	}else if(process & PROCESS_SUMMON){
		ret=process_summon(out);
	}else if(process & PROCESS_SUMMON_ACC){
		ret=process_summon_acc(out);
	}else if(process & PROCESS_YC_CHANGE){
		ret=process_yc_change(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_RESET){
		app->need_reset=1;
	}else if(process & PROCESS_RM_CTL){
		ret=process_yk(out);
	}else if(process & PROCESS_CLOCK){
		ret=process_clock(out);
	}else if(process & PROCESS_TEST_LINK){
		ret=process_test_link(out);
	}else if(process & PROCESS_FILE){
		ret=process_file(out);
	}else if(process & PROCESS_UPDATE){
		ret=process_update(out);
	}
	if(ret>0){
		send_frame(&s_i_frame);
	}
	return ret;
}
void link_layer_104::deal_timeout(){
	event *e;
	e=NULL;
	link_time();
	if(yk_data.start_timeout==1){
		yk_data.timeout++;
		if(yk_data.timeout ==6000){//60s
			yk_data.start_timeout = 0;
			yk_data.timeout=0;
			yk_data.sel=0;
			yk_data.act=0;
			yk_data.deactive=0;
			yk_data.act_over=0;
		}
	}
	if(app->get_event_data(port,e,0)==1){
		process|=PROCESS_EVENT;
	}else{
		process &=(~PROCESS_EVENT);
	}
	event_yc *e_yc;
	if(app->get_yc_cg_data(port,e_yc)){
		process|=PROCESS_YC_CHANGE;
	}else{
		process &=~PROCESS_YC_CHANGE;
	}

	if(app->need_reset)
		app->do_reset();
	if(app->need_update)
		app->do_update();
}
int link_layer_104::link_time(){
	int ret=0;
	if(t1_timer.is_reached()){
		if(r_no!=ack_no){
			link_state=0;
		}
	}
	if(t3_timer.is_reached()==1){//receive timeout
		build_test_link();
		send_frame(&s_u_frame);
		t3_timer.start(3);
	}else if(t2_timer.is_reached()==1){
		sfmt sf;
		sf.bit.r_no=r_no;
		ret=build_sframe(&s_s_frame,sf);
		send_frame(&s_s_frame);
	}
	if(link_state==0){
		com->close();
		com->connect();
	}
	return ret;
}
/****************************
 *	extern function for app_layer 
****************************/
SORT_YX_TAB * get_yx_data(int);
YC_TAB * get_yc_data(int );
YC_TAB * get_acc_yc_data(int );
int get_event_data(int type,event *&e,int change);
int get_clock(CP56Time2a &);
int do_yk(int id,int type,int cmd);
void do_reset();
int get_yc_cg_data(int port ,event_yc *&e);
int get_dir_data(_rd_dir*);
int get_file_data(_rd_file*);
int get_file_segment(_rd_file *file);
int save_file_segment(_rd_file *file);
int get_dz_unit(_para_list *);
int set_dz_unit(int);
int get_dz_data(para_node *);
int set_dz(int,para_node *);
int do_update();


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
	SORT_YX_TAB *pyx;
	
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
	out->data[offset_asdu+i++]=pyx->pdata->addr;
	out->data[offset_asdu+i++]=pyx->pdata->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset_asdu+i]=pyx[j].pdata->statu;
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
	SORT_YX_TAB *pyx;
	
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
	out->data[offset_asdu+i++]=pyx->pdata->addr;
	out->data[offset_asdu+i++]=pyx->pdata->addr>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset_asdu+i]=pyx[j].pdata->statu;
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
	YC_TAB *pyc;
	
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
	out->data[offset_asdu+i++]=pyc->ycdata->datasign;
	out->data[offset_asdu+i++]=pyc->ycdata->datasign>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(yc_data_type == 9 || yc_data_type ==11){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i+3*j]=pyc[j].ycdata->deadpass->intdata;
			out->data[offset_asdu+i+3*j+1]=pyc[j].ycdata->deadpass->intdata>>8;
			out->data[offset_asdu+i+3*j+2]=0;//qds
		}
		ret=i+3*j;
	}else if(yc_data_type == 13){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i+5*j]=pyc[j].ycdata->deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i+5*j+1]=pyc[j].ycdata->deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i+5*j+2]=pyc[j].ycdata->deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i+5*j+3]=pyc[j].ycdata->deadpass->floatdata.bitdata.d4;
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
	out->data[offset_asdu+i++]=e->soe.time.year;
	out->data[offset_asdu+i++]=e->soe.time.month;
	out->data[offset_asdu+i++]=e->soe.time.day;
	out->data[offset_asdu+i++]=e->soe.time.hour;
	out->data[offset_asdu+i++]=e->soe.time.minute;
	out->data[offset_asdu+i++]=e->soe.time.millisecond>>8;
	out->data[offset_asdu+i++]=e->soe.time.millisecond & 0x00ff;
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
int app_layer::build_clock(frame *out,link_layer *link){//cause 7
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
	CP56Time2a time;
	get_clock(time);
	out->data[offset_asdu+i++]=time.year;
	out->data[offset_asdu+i++]=time.month;
	out->data[offset_asdu+i++]=time.day;
	out->data[offset_asdu+i++]=time.hour;
	out->data[offset_asdu+i++]=time.minute;
	out->data[offset_asdu+i++]=time.millisecond>>8;
	out->data[offset_asdu+i++]=time.millisecond & 0x00ff;
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
	if(link->yk_data.sel==1){
		cause_lo.data=CAUSE_Actcon;
	}else if(link->yk_data.deactive==1){
		cause_lo.data=CAUSE_Deactcon;
	}else if(link->yk_data.act==1){
		cause_lo.data=CAUSE_Actcon;
		link->yk_data.act_over=1;
	}else if(link->yk_data.act_over==1){
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
	out->data[offset_asdu+i++]=e->data->datasign;
	out->data[offset_asdu+i++]=e->data->datasign>>8;
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
	dir_node *node;
	i=0;
	n=0;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
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
	while(dir->cur_read<dir->res_list.size()){
		node=&it[0];	
		out->data[offset_asdu+i++]=node->name_len;
		for(j=0;j<node->name_len;j++){
			out->data[offset_asdu+i++]=node->name[j];
		}
		out->data[offset_asdu+i++]=0;//reverse 
		out->data[offset_asdu+i++]=node->file_size;
		out->data[offset_asdu+i++]=node->file_size>>8;
		out->data[offset_asdu+i++]=node->file_size>>16;
		out->data[offset_asdu+i++]=node->file_size>>24;
		out->data[offset_asdu+i++]=node->time.year;
		out->data[offset_asdu+i++]=node->time.month;
		out->data[offset_asdu+i++]=node->time.day;
		out->data[offset_asdu+i++]=node->time.hour;
		out->data[offset_asdu+i++]=node->time.minute;
		out->data[offset_asdu+i++]=node->time.millisecond>>8;
		out->data[offset_asdu+i++]=node->time.millisecond & 0x00ff;
		dir->cur_read++;
		n++;
		if(n>5)break;
		dir->con=1;
	}
	if(n<=5){
		dir->con=0;
		dir->cur_read=0;
	}
	out->data[bak_pos]=dir->con;//con
	out->data[bak_pos+1]=n;//
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
	vsq_lo.bit.n=1;
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
	vsq_lo.bit.n=1;
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
int app_layer::build_wt_file_con(frame *out,link_layer *link,_rd_file *file){//cause 7
	int i;
	int j;
	int ret;
	i=0;
	ret = -1;
	
	link->set_loc_ctl();	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
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
int app_layer::build_wt_file_resp(frame *out,link_layer *link,_rd_file *file){//cause 5
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
	out->data[offset_asdu+i++]=file->res_file.file_id;
	out->data[offset_asdu+i++]=file->res_file.file_id>>8;
	out->data[offset_asdu+i++]=file->res_file.file_id>>16;
	out->data[offset_asdu+i++]=file->res_file.file_id>>24;
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
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	if(para->op==0){
		cause_lo.bit.cause=CAUSE_Deactcon;
	}else
		cause_lo.bit.cause=CAUSE_Actcon;

	cmd_id=COMMAND_WR_DZ;
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=0;
	out->data[offset_asdu+i++]=0;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
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
	
	link->has_data=1;
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
	link->has_data=0;	
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
	YC_TAB *pyc;
	
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

	if(pyc[0].ycdata->type == 0){
		cmd_id=COMMAND_ACC;
	}else{
		cmd_id=COMMAND_ACC_TIME;
	}
	ret=build_asdu_header(out);
	i=ret;
	out->data[offset_asdu+i++]=pyc->ycdata->datasign;
	out->data[offset_asdu+i++]=pyc->ycdata->datasign>>8;
	if(msg_id_size==3)
		out->data[offset_asdu+i++]=0;
	if(pyc[0].ycdata->type == 0){
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu+i++]=5;//
		}
	}else{
		for(j=0;j<send_num;j++){
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d1;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d2;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d3;
			out->data[offset_asdu+i++]=pyc[j].ycdata->deadpass->floatdata.bitdata.d4;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.year;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.month;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.day;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.hour;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.minute;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.millisecond>>8;
			out->data[offset_asdu+i++]=pyc[j].ycdata->time.millisecond & 0x00ff;
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
*  @brief	build asdu of link_fini command 
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
/****************************
 * realize extern interface functions
****************************/
SORT_YX_TAB * get_yx_data(int pos){
	SORT_YX_TAB *p;
	p=&SortYxTable[pos];
	pfunc(DEBUG_NORMAL,"\n");
	return p;
}
YC_TAB * get_yc_data(int pos){
	YC_TAB *p;
	p=&YcTable[pos];
	pfunc(DEBUG_NORMAL,"\n");
	return p;
}
YC_TAB * get_acc_yc_data(int pos){
	YC_TAB *p;
	pos+=config_scada_data.pos_acc;
	p=&YcTable[pos];
	pfunc(DEBUG_NORMAL,"\n");
	return p;
}
int get_event_data(int type,event *&e,int change){
	int ret;
	ret=0;
	pfunc(DEBUG_NORMAL,"\n");
	EventList::iterator end,it;
	end=event_list.end();
	it=event_list.begin();
	while(it!=end){
		if(it->readflag[type]==0){
			if(change)
				it->readflag[type]=1;
			e=&it;
			ret=1;
			break;
		}
		it++;
	}
	return ret;
}
int get_clock(CP56Time2a &time){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
//type:0 execute;1:check;
int do_yk(int id,int type,int cmd){
	pfunc(DEBUG_NORMAL,"type:%d\n",type);
	if(type == 0){
		pfunc(DEBUG_NORMAL,"do execute:%d\n",cmd);
	}else if(type == 1){
		pfunc(DEBUG_NORMAL,"do select:%d\n",cmd);
	}
	return 0;
}
void do_reset(){

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
	pfunc(DEBUG_NORMAL,"\n");
	return ret;
}
int get_dir_data(_rd_dir *dir){
	pfunc(DEBUG_NORMAL,"\n");
	dir->res_list=g_dir_list;
	return 0;
}
int get_file_data(_rd_file *file){
	pfunc(DEBUG_NORMAL,"\n");
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
	pfunc(DEBUG_NORMAL,"\n");
	return ret;
}
void load_file_list(){
	dir_node node;
	FILE *f;
	f=fopen("file_list.dat","rb");
	if(f==NULL)
		return;
	g_dir_list.clear();
	while(!feof(f)){
		fread(&node,sizeof(node),1,f);
		g_dir_list.push(node);
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
	pfunc(DEBUG_NORMAL,"\n");
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
	pfunc(DEBUG_NORMAL,"\n");
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
	pfunc(DEBUG_NORMAL,"\n");
	data->unit=LimitBounds.cur_unit;
	data->min_unit=LimitBounds.min_unit;
	data->max_unit=LimitBounds.max_unit;
	return 0;
}
int set_dz_unit(int unit){
	pfunc(DEBUG_NORMAL,"\n");
	LimitBounds.cur_unit=unit;
	return 0;
}
int get_dz_data(para_node *para){
	para->len=10;
	pfunc(DEBUG_NORMAL,"\n");
	return para->len;
}
int set_dz(int num,para_node*para){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int do_update(){
	return 0;
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
