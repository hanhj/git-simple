/*************************************************************************
	> File Name: protocol.cpp
	> Author: hanhj
	> Mail: hanhj@zx-jy.com 
	> Created Time: 2019年02月19日 星期二 22时09分40秒
 ************************************************************************/
#include "protocol.h"
#include "configurable.h"

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
int link_layer::send_frame(frame *f){
	int ret;
	ret=0;
	if(com==NULL||f==NULL){
		ret=ERR_INVALID_PARA;
		errno=ret;
		pfunc(DEBUG_ERROR,"invalid para\r");
		goto err;
	}
	cout<<"send frame of link "<<port<<",physical port :"<<com->port_no<<endl;
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
	cout<<"check state of link "<<port<<",state:"<<link_state<<endl;
	return 0;
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
		ctl_lo.sl.acd_rev=1;
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
int link_layer_101::build_link_fini(frame *out){
	int ret;
	ret=0;
	if(!balance){
		ctl_lo.sl.fc=8;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=1;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;

	}else if(balance==BALANCE){
		ctl_lo.pm.fc=3;
		ctl_lo.pm.fcv=1;
		ctl_lo.pm.fcb=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
	}
	ret=app->build_link_fini(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_summon_con(frame *out){
	int ret;
	ret=0;
	set_loc_ctl();
	ret=app->build_summon_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
	summon_step = 1;
err:
	return ret;
}
int link_layer_101::build_summon_term(frame *out){	
	int ret;
	set_loc_ctl();
	if(!balance){
		ctl_lo.sl.acd_rev=0;//no more data;
	}
	ret=app->build_summon_term(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);	
	if(ret<0){
		errno=ret;
		goto err;
	}
	summon_step = 0;
err:
	return ret;
}
int link_layer_101::build_yx_data(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_yx_data(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);		
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_dyx_data(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_dyx_data(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);		
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_yc_data(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_yc_data(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);			
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_event_data(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_event_data(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_clock(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_clock(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_yk(frame *in,frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_yk(in,out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_link_test_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_link_test_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_yc_cg_data(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_yc_cg_data(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);					
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_reset_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_reset_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);					
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_rd_dir_resp(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dir_resp(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);					
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_rd_file_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_file_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);				
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_rd_file_resp(frame *out,file_segment *file){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_file_resp(out,this,file);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);								
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_wr_file_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_wr_file_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);					
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_wr_file_resp(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_wr_file_resp(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);					
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_rd_dz_unit_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dz_unit_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_wr_dz_unit_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_wr_dz_unit_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_rd_dz_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dz_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_dz_con(frame *out,int sel){	
	int ret;
	set_loc_ctl();
	ret=app->build_dz_con(out,this,sel);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_dz_dact_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_dz_dact_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_summon_acc_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_con(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_summon_acc_term(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_term(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_summon_acc_resp(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_resp(out,this);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
int link_layer_101::build_update_con(frame *out,int sel){	
	int ret;
	set_loc_ctl();
	ret=app->build_update_con(out,this,sel);
	if(ret<0){
		errno=ret;
		goto err;
	}
	ret=build_link_layer(out,ret);
	if(ret<0){
		errno=ret;
		goto err;
	}
err:
	return ret;
}
//for balance
int link_layer_101::on_ack(frame *in,frame *out){
	cout<<"on_ack"<<endl;
	if(process & PROCESS_SUMMON){
		process_summon(out);
	}

	return 0 ;
}
//for unbalance
int link_layer_101::on_req_class_1(frame *in,frame *out){
	int ret ;
	cout<<"on_req_class_1"<<endl;
	if(process & PROCESS_SUMMON){
		ret = process_summon(out);
	}
	return ret ;
}
//for unbalance
int link_layer_101::on_req_class_2(frame *in,frame *out){
	int ret;
	cout<<"on_req_class_2"<<endl;
	if(process & PROCESS_CLOCK){
		ret=process_clock(in,out);
	}
	return ret;
}
int link_layer_101::process_summon(frame *out){
	int ret;
	if(summon_step==0){
		ret=build_summon_con(out);
	}else if(summon_step == 1){
		ret=build_yx_data(out);
	}else if(summon_step == 2){
		ret=build_dyx_data(out);
	}else if(summon_step == 3){
		ret=build_yc_data(out);
	}else if(summon_step == 4){
		ret=build_summon_term(out);
		process &=~PROCESS_SUMMON;
	}
	return ret;
}
int link_layer_101::process_clock(frame *in,frame *out){
	int ret;
	ret =0;
	if(in->data[offset_cause]==CAUSE_Act){
		clock_syn = 1;
	}else if(in->data[offset_cause] == CAUSE_Req){
		clock_rd = 1;
	}
	return ret;
}
int link_layer_101::process_yk(frame *in,frame *out){
	int ret =0;
	return ret;
}
int link_layer_101::on_req(frame *in,frame *out){
	int ret;
	ret= 0;
	int ti;
	ti=in->data[offset_ti];
	int act=0;//for file command
	switch(ti){
		case COMMAND_SUMMON://total sum
			process|=PROCESS_SUMMON;
			if(balance == BALANCE){
				ret=process_summon(out);
			}
			break;
		case COMMAND_CLOCK://clock 
			process|=PROCESS_CLOCK;
			process_clock(in,out);
			if(balance ==BALANCE)
				ret=build_clock(out);
			break;
		case COMMAND_RM_CTL:
		case COMMAND_RM_CTL_D:
			process|=PROCESS_RM_CTL;
			if(balance == BALANCE)
				ret=process_yk(in,out);
			break;
		case COMMAND_TEST_LINK:
			process|=PROCESS_TEST_LINK;
			break;
		case COMMAND_RESET:
			process|=PROCESS_RESET;
			break;
		case COMMAND_FILE:
			if(act==1){
				process|=PROCESS_RD_DIR;
			}else if(act==3){
				process|=PROCESS_RD_FILE;
			}else if(act==7){
				process|=PROCESS_WR_FILE;
			}
			break;
		case COMMAND_RD_DZ_UNIT:
			process|=PROCESS_RD_DZ_UNIT;
			break;
		case COMMAND_RD_DZ:
			process|=PROCESS_RD_DZ;
			break;
		case COMMAND_WR_DZ:
			process|=PROCESS_WR_DZ;
			break;
		case COMMAND_WR_DZ_UNIT:
			process|=PROCESS_WR_DZ_UNIT;
			break;
		case COMMAND_SUMMON_ACC:
			process|=PROCESS_SUMMON_ACC;
			break;
		case COMMAND_UPDATE:
			process|=PROCESS_UPDATE;
			break;
	}
	return ret;
}
int link_layer_101::save_frame(frame *f){
	if(f->type==VAR_FRAME){
		memcpy(&last_send_frame,f,sizeof(last_send_frame));
	}
	return 0;
}
int link_layer_101::get_frame(){
	int ret;
	unsigned char c;
	int fail=0;
	if(com==NULL)
		return -1;
	cout<<"get frame of link "<<port<<",physical port :"<<com->port_no<<endl;
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
					pdump(DEBUG_INFO,"get fix frame",&r_fix_frame.data[0],r_fix_frame.len);
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
					pdump(DEBUG_INFO,"get var frame",&r_var_frame.data[0],r_var_frame.len);
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
int link_layer_101::active_send(){
	int ret;
	ret=0;
	cout<<"active send  101 frame of link "<<port<<endl;
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
			ret=build_link_fini(&s_var_frame);
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
//err:
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
			ret=build_link_fini(&s_var_frame);
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
					if(ret==0){
						ret=send_frame(&s_var_frame);
						if(ret<0){
							goto err;
						}
						save_frame(&s_var_frame);//save frame
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
				if(ret==0){
					ret=send_frame(&s_var_frame);
					if(ret<0){
						goto err;
					}
					save_frame(&s_var_frame);//save frame
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
 *	extern function for app_layer 
****************************/
SORT_YX_TAB * get_yx_data(int);
YC_TAB * get_yc_data(int );
int get_event_list(EventList*from,int pos,buffer *data);
int get_clock(CP56Time2a &);
int do_yk(int id,int type,int cmd);
int get_yc_cg_data(cg_yc_list *list,int pos,buffer*data);
int get_dir_list(dir_list *list,buffer*data);
int get_file_segment(char *filename,int pos,file_segment *file);
int save_file_segment(char *filename,int pos,file_segment *file);
int get_dz_unit(buffer*data);
int set_dz_unit(int);
int get_dz_data(int id,buffer*data);
int set_dz(int id,int sel,buffer*data);
int get_summon_acc_data(buffer*data);
int save_update_file(char *filename,file_segment *file);


/****************************
 *	realize app_layer 
****************************/
int app_layer::get_link_info(link_layer*link){
	int ret;
	ret=0;
	offset=link->offset_asdu;
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
*  @brief	build asdu of link_fini command 
*  @param[in] link point to link_layer  
*  @param[out]  out point to out frame 
*  @return upon successful return number of asdu size\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int app_layer::build_link_fini(frame*out,link_layer*link){
	int i;
	int ret;

	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Init;

	i=0;
	out->data[offset+i++]=COMMAND_LINK_FINI;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	ret=i;
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

	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	i=0;
	out->data[offset+i++]=COMMAND_SUMMON;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	out->data[offset+i++]=20;
	ret=i;
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

	ret=get_link_info(link);
	if(ret<0)
		goto err;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actterm;

	i=0;
	out->data[offset+i++]=COMMAND_SUMMON;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	out->data[offset+i++]=20;
	ret=i;
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
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	pyx=get_yx_data(link->sended_yx_num);
	send_num = SelectYxNum - link->sended_yx_num;
	if(send_num > MAX_SEND_YX_PER_FRAME){
		send_num = MAX_SEND_YX_PER_FRAME;
	}else{
		link->summon_step++;//next for yc
	}

	link->sended_yx_num+=send_num;
	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;

	i=0;
	out->data[offset+i++]=COMMAND_SP;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=pyx->pdata->addr;
	out->data[offset+i++]=pyx->pdata->addr>>8;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset+i]=pyx[j].pdata->statu;
		i++;
	}
	ret=i;
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
int app_layer::build_dyx_data(frame *out,link_layer *link){//cause 20
	int i,j;
	i=0;
	int ret= 0;
	int send_num;
	SORT_YX_TAB *pyx;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	send_num = SelectDpYxNum - link->sended_yx_num;
	pyx=get_yx_data(link->sended_yx_num);
	if(send_num > MAX_SEND_YX_PER_FRAME){
		send_num = MAX_SEND_YX_PER_FRAME;
		link->sended_yx_num+=send_num;
	}
	else{
		link->summon_step++;//next for yc
		link->sended_yx_num = 0;
	}

	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;

	i=0;
	out->data[offset+i++]=COMMAND_DP;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=pyx->pdata->addr;
	out->data[offset+i++]=pyx->pdata->addr>>8;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	for(j=0;j<send_num;j++){
		out->data[offset+i]=pyx[j].pdata->statu;
		i++;
	}
	ret=i;
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
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	send_num = SelectYcNum - link->sended_yc_num;
	pyc=get_yc_data(link->sended_yc_num);
	if(send_num > MAX_SEND_YC_PER_FRAME){
		send_num = MAX_SEND_YC_PER_FRAME;
		link->sended_yc_num+=send_num;
	}
	else{
		link->summon_step++;//next 
		link->sended_yc_num = 0;
	}

	vsq_lo.bit.n=send_num;
	vsq_lo.bit.sq=1;
	cause_lo.bit.cause=CAUSE_Introgen;

	out->data[offset+i++]=yc_data_type;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=pyc->ycdata->datasign;
	out->data[offset+i++]=pyc->ycdata->datasign>>8;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	if(yc_data_type == 9 || yc_data_type ==11){
		for(j=0;j<send_num;j++){
			out->data[offset+i+3*j]=pyc[j].ycdata->deadpass->intdata;
			out->data[offset+i+3*j+1]=pyc[j].ycdata->deadpass->intdata>>8;
			out->data[offset+i+3*j+2]=0;//qds
		}
		ret=i+3*j;
	}else if(yc_data_type == 13){
		for(j=0;j<send_num;j++){
			out->data[offset+i+5*j]=pyc[j].ycdata->deadpass->floatdata.bitdata.d1;
			out->data[offset+i+5*j+1]=pyc[j].ycdata->deadpass->floatdata.bitdata.d2;
			out->data[offset+i+5*j+2]=pyc[j].ycdata->deadpass->floatdata.bitdata.d3;
			out->data[offset+i+5*j+3]=pyc[j].ycdata->deadpass->floatdata.bitdata.d4;
			out->data[offset+i+5*j+4]=0;//qds
		}
		ret=i+5*j;
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
int app_layer::build_event_data(frame *out,link_layer *link){//cause 3
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	if(link->clock_syn){
		cause_lo.bit.cause=CAUSE_Actcon;
		link->clock_syn = 0;
	}else if(link->clock_rd){
		cause_lo.bit.cause=CAUSE_Req;
		link->clock_rd = 0;
	}

	out->data[offset+i++]=COMMAND_CLOCK;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0;
	out->data[offset+i++]=0;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	CP56Time2a time;
	get_clock(time);
	out->data[offset+i++]=time.year;
	out->data[offset+i++]=time.month;
	out->data[offset+i++]=time.day;
	out->data[offset+i++]=time.hour;
	out->data[offset+i++]=time.minute;
	out->data[offset+i++]=time.millisecond>>8;
	out->data[offset+i++]=time.millisecond & 0x00ff;
	ret = i;
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
int app_layer::build_yk(frame *in,frame *out,link_layer *link){//cause=7,sel=0 or 1
	int i;
	i=0;
	int ret= 0;
	int ctrl_id;
	int cmd_id;
	int cmd;//0:open;1:close
	sco sco;
	dco dco;
	int cancel;
	cmd=0;
	cancel = 0;

	//sco 0:open;1:close;
	//dco 1:open;2:close
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cmd_id=in->data[link->offset_ti];
	ctrl_id=in->data[link->offset_data];
	ctrl_id=(ctrl_id<<8)+in->data[link->offset_data+1];

	if(in->data[link->offset_cause]==CAUSE_Act){
		cause_lo.bit.cause=CAUSE_Actcon;
	}else if(in->data[link->offset_cause] == CAUSE_Deact){
		cause_lo.bit.cause=CAUSE_Deactcon;
		cancel = 1;
	}

	if(cmd_id == COMMAND_RM_CTL){
		sco.data=in->data[link->offset_data+2];
		cmd=sco.bit.scs;
		if(sco.bit.sel == 0){
			ret=do_yk(ctrl_id,0,cmd);
		}else if(sco.bit.sel == 1){
			ret = do_yk(ctrl_id,1,cmd);
		}
		if(ret <0)
			goto err;
	}else if(cmd_id == COMMAND_RM_CTL_D){
		dco.data = in->data[link->offset_data + 2];
		cmd=dco.bit.dcs==1?0:1;
		if(dco.bit.sel == 0){
			ret=do_yk(ctrl_id,0,cmd);
		}else if(dco.bit.sel == 1){
			ret = do_yk(ctrl_id,1,cmd);
		}
		if(ret <0)
			goto err;
	}else
		goto err;

	if(cancel==1){
		do_yk(ctrl_id,2,cmd);
	}
	out->data[offset+i++]=cmd_id;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=ctrl_id;
	out->data[offset+i++]=ctrl_id>>8;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	out->data[offset+i++]=cmd_id == COMMAND_RM_CTL?sco.data:dco.data;
	ret = i;
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
int app_layer::build_link_test_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
*/int app_layer::build_yc_cg_data(frame *out,link_layer *link){//cause 3
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
*/int app_layer::build_reset_con(frame *out,link_layer *link){//reset terminal cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::on_file(frame *in,link_layer *link){
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_rd_dir_resp(frame *out,link_layer *link){//cause 5
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_rd_file_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_rd_file_resp(frame *out,link_layer *link,file_segment *file){//cause 5
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_wr_file_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_wr_file_resp(frame *out,link_layer *link){//cause 5
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_rd_dz_unit_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_wr_dz_unit_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_rd_dz_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::on_set_dz(frame *in,link_layer *link){
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_dz_con(frame *out,link_layer *link,int sel){//cause 7,sel =0 or 1 ,cr=0
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_dz_dact_con(frame *out,link_layer *link){//cause 9,sel=0,cr=1
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_summon_acc_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_summon_acc_term(frame *out,link_layer *link){//cause 10
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_summon_acc_resp(frame *out,link_layer *link){//cause 37
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::on_update(frame *in,link_layer *link){
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int app_layer::build_update_con(frame *out,link_layer *link,int sel){//cause 7 sel=1 start,0 stop
	int i;
	i=0;
	int ret;
	
	ret=get_link_info(link);
	if(ret<0)
		goto err;
	ret=i;
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
int get_event_list(EventList*from,int pos,buffer *data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_clock(CP56Time2a &time){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
//type:0 do;1:select;2:cancel
int do_yk(int id,int type,int cmd){
	pfunc(DEBUG_NORMAL,"type:%d\n",type);
	if(type == 0){
		pfunc(DEBUG_NORMAL,"do execute:%d\n",cmd);
	}else if(type == 1){
		pfunc(DEBUG_NORMAL,"do select:%d\n",cmd);
	}else if(type == 2){
		pfunc(DEBUG_NORMAL,"do cancel:%d\n",cmd);
	}
	return 0;
}
int get_yc_cg_data(cg_yc_list *list,int pos,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dir_list(dir_list *list,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_file_segment(char *filename,int pos,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int save_file_segment(char *filename,int pos,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dz_unit(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int set_dz_unit(int){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dz_data(int id,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int set_dz(int id,int sel,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_summon_acc_data(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int save_update_file(char *filename,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
/*
int get_yx_data(buffer*data);
int get_yc_data(buffer*data);
int get_event_list(event_list*from,int pos,buffer *data);
int get_clock(buffer*data);
int do_yk(int id,int type,int cmd);
int get_yc_cg_data(cg_yc_list *list,int pos,buffer*data);
int get_dir_list(dir_list *list,buffer*data);
int get_file_segment(char *filename,int pos,file_segment *file);
int save_file_segment(char *filename,int pos,file_segment *file);
int get_dz_unit(buffer*data);
int set_dz_unit(int);
int get_dz_data(int id,buffer*data);
int set_dz(int id,int sel,buffer*data);
int get_summon_acc_data(buffer*data);
int save_update_file(char *filename,file_segment *file);
*/
void set_app_interface(app_layer *app){
	app->get_yx_data=get_yx_data;
	app->get_yc_data=get_yc_data;
	app->get_event_list=get_event_list;
	app->get_clock=get_clock;
	app->do_yk=do_yk;
	app->get_yc_cg_data=get_yc_cg_data;
	app->get_dir_list=get_dir_list;
	app->get_file_segment=get_file_segment;
	app->save_file_segment=save_file_segment;
	app->get_dz_unit=get_dz_unit;
	app->set_dz_unit=set_dz_unit;
	app->get_dz_data=get_dz_data;
	app->set_dz=set_dz;
	app->get_summon_acc_data=get_summon_acc_data;
	app->save_update_file=save_update_file;
}
// vim:tw=72
