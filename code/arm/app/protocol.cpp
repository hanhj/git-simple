#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "os.h"// for mutex
#include "datatype.h"
#include "message.h"
#include "statis.h"
#include "task.h"
#include "protocol.h"
#include "gb101.h"
#include "gb104.h"
#include "com.h"
#include "timer.h"
#include "gps.h"
#include "update_pro.h"
#include "sntp.h"
extern Gb101 gb101;
extern Gb104 gb104;
extern GPS gps;
extern Comm comm;
extern UpdatePro updp;
Protocol protocol;
extern  UserData user_data;
pack_buf_t PackBuf;
char pack_buf[200];
uint8 data_num;
/**
 * @name protocol_101_main
 *
 * @param void *arg
 *
 * @return NULL
 */
//extern CAC Ac;
//report_pipe_t ReportPipeYc;
//report_pipe_t ReportPipeYx;
//ac_data_t pro_ac_data;
/**
*************************************************************************************
*  @brief  获得消息处理函数，供message线程回调
*  @author  liht
*  @param[in] MessageData<BaseDataType>message
*  @return  不关心
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static int16 GetMsg2 ( MessageData<BaseDataType>message )
{
    protocol.GetMsg ( message );
    return 1;
}
/** /brief 规约线程入口
*/
/**
*************************************************************************************
*  @brief  规约线程入口
*  @author  liht
*  @param[in] void *arg
*  @return  无
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void* protocol_main ( void *arg )
{
    protocol.Run ( arg );
    return NULL;
}
/**
*************************************************************************************
*  @brief  规约线程构造函数
*  @author  liht
*  @return 无
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
Protocol::Protocol()
{
    strcpy ( name, "Protocol" );
    Id = TASK_PRO;
}
/**
*************************************************************************************
*  @brief  规约线程析构函数
*  @author  liht
*  @return  无
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
Protocol::~Protocol()
{
}
/**
*************************************************************************************
*  @brief  规约线程初始化函数
*  @author  liht
*  @return  1
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::Init()
{
    msg.RegistMsg ( Id, GetMsg2 );///<向message类注册线程号以及消息回调函数
    gb101.member.InitOver = 0;
    gb104.member.InitOver = 0;
    if ( first_run == 0 ) {
        thread_wait_cond ( &user_data.cond );//等待参数装载完毕
        first_run = 1;
    }
    gb101.InitGb101();
    gb104.InitGb104();
    sleep(2);
    ClearBufRegistTable();
    member.YkPortNum = 0;
    return 1;
}
//判断是否是双点遥信
int isDyx(int32 id){
	if(id>=YX_0171&&id<=YX_0180)
		return 1;
	return 0;	
}
char deal_RareTrans ( BUF_MUX *buf, uint16 port_id )
{
	//mdelay(100);
	uint8 i=0;
	uint16 bytes_cnt=0;
	BUF_ADDR_REGIST_TABLE *bart=0;
	RetransPortSet rp_para;
	Data<BaseDataType> Para;
	user_data.GetData(PARA_513A,&Para);
    memcpy ( ( char* ) &rp_para.Data, Para.Data, user_data.GetDataSize ( PARA_513A ) );
	if(buf->recv.in_cnt==buf->recv.out_cnt){//no data return;
		return 0;
	}
	bart=protocol.SearchBufAddrRegistTable(port_id);
	if(bart==0){
		return 0;
	}
	bart=protocol.SearchBufAddrRegistTable(bart->paddr->forward_port_id);
	if(bart==0){
		return 0;
	}
	for(i=0;i<8;i++){
		while(buf->recv.in_cnt!=buf->recv.out_cnt){//move the data to forward send buffer
			bart->paddr->send.buf[bart->paddr->send.in_cnt++]=buf->recv.buf[buf->recv.out_cnt++];
			if(bart->paddr->send.in_cnt>=BUFFER_SIZE){
				bart->paddr->send.in_cnt=0;
			}
			if(buf->recv.out_cnt>=BUFFER_SIZE){
				buf->recv.out_cnt=0;
			}
			bytes_cnt++;
			if(bytes_cnt>=512){
				return 0;
			}
		}
		if(rp_para.Data.ByteOverTime>0){
			mdelay(rp_para.Data.ByteOverTime);//delay
		}
		if(buf->recv.in_cnt==buf->recv.out_cnt){//check the new data
			return 0;
		}
	}
	return 0;
}
/**
*************************************************************************************
*  @brief  规约线程主函数
*  @author  liht
*  @return
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::Run ( void*arg )
{
    int i;
    //char port_id[2];
    //uint16 len = 0;
    Init();
    PFUNC ( TEM_INFO,DEBUG_PRO, "i am in the thread protocol_main \n");
    while ( 1 ) {
        mdelay ( 10 );
        for ( i = 0;i < member.buf_addr_regist_num;i++ ) {
            //PFUNC ( TEM_INFO,DEBUG_PRO, "GetPortStatus[%d]'flag=[%d]\n",member.buf_addr_regist_table[i].port_id,comm.GetPortStatus(member.buf_addr_regist_table[i].port_id)  );
            if ( comm.GetPortStatus ( member.buf_addr_regist_table[i].port_id ) ) {
                member.buf_addr_regist_table[i].port_status = 1;
				//PFUNC ( TEM_INFO,DEBUG_PRO, "%p,%d\n",member.buf_addr_regist_table[i].paddr,member.buf_addr_regist_table[i].port_id );
                if ( member.buf_addr_regist_table[i].paddr != 0 && member.buf_addr_regist_table[i].port_id != 0 ) {
                    member.buf_addr_regist_table[i].handle ( member.buf_addr_regist_table[i].paddr, member.buf_addr_regist_table[i].port_id );
                }
            } else if ( member.buf_addr_regist_table[i].port_status == 1 ) {
                sleep ( 1 );
                member.buf_addr_regist_table[i].port_status = 0;
                if ( member.buf_addr_regist_table[i].pro_id == PRO104 ){
                    gb104.ProLinkReset ( member.buf_addr_regist_table[i].port_id );
					PFUNC ( TEM_WARNING, DEBUG_PRO,"22\n");
                }else if ( member.buf_addr_regist_table[i].pro_id == PRO101 ){
                    ;//ProRstLink ( port_id, len, port_id, &len );
                }
                else if ( member.buf_addr_regist_table[i].pro_id == PROUPDATE ) {
                    updp.SetSendHeartFlag ( 0 );
                }
                PFUNC ( TEM_WARNING, DEBUG_PRO, "i reset the %d protocol %d \n",
                        member.buf_addr_regist_table[i].pro_id, member.buf_addr_regist_table[i].port_id  );
                protocol.member.YkPortNum = 0;
                protocol.member.BtjPortNum = 0;
            }
        }
        Heart();
        CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }
    msg.UnRegistMsg ( Id );
    PFUNC ( TEM_ERROR, DEBUG_PRO, "Exit protocol Run\r\n" );
    return 1;
}
/**
*************************************************************************************
*  @brief  读配置文件
*  @author  liht
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::ReadCfg()
{
    return 1;
}
/**
*************************************************************************************
*  @brief  写配置文件
*  @author  liht
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::WriteCfg()
{
    return 1;
}
/**< 自己处理消息函数，不给message线程回调 */
/**
*************************************************************************************
*  @brief  自己处理消息函数，不给message线程回调
*  @author  liht
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::GetMsg ( void )
{
    MessageData<BaseDataType> *msgdata;
    MessageData<BaseDataType> msgbuf;
    PRO_DATA_CLASS_CFG *pcdtc;
    char  data[5];
    uint8 len = 0;
    char time[7] = {33, 33, 33, 33, 33, 33, 33};
    float ftmp, coff;
    int32 itmp;
    Data<BaseDataType> u_data;
    ID_MAP *imp = 0;
    uint8 change_yx_type = 0, soe_type = 0;
    if ( msg.GetMsg ( TASK_PRO, &msgdata ) ) {
        msgbuf = *msgdata;
        memcpy ( time, ( char* ) &msgbuf.Time, 7 );
        PFUNC ( TEM_INFO, DEBUG_PRO, "protocol get msgID=%x,msgDataType=%d,msgOwner=%d,dataid=%x,data=%d\n", msgbuf.Id, msgbuf.Data->DataType, msgbuf.Owner, msgbuf.Id, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        if ( msgbuf.MessageId == MSG_YX ) {
            PFUNC ( TEM_INFO, DEBUG_PRO, "this is yx change msg\n" );
            len = user_data.GetDataSize ( msgbuf.Id );
            if ( len == 0 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "error data len=%d\n", len );
                return 0;
            }
            memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ) , len );
            imp = gb101.GetDataAddr ( ( uint16 ) msgbuf.Id );
            if ( imp == 0 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "error msg id not found =%d\n", msgbuf.Id );
                return 0;
            }
            if (  isDyx(imp->data_id)) {
                change_yx_type = CHANGE_DYX_DATA;
                soe_type = SOE2_DATA;
            } else {
                change_yx_type = CHANGE_YX_DATA;
                soe_type = SOE_DATA;
            }
            PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
            pcdtc = gb101.SearchClassTable ( CHANGE_YX_DATA );
            if ( pcdtc != 0 ) {
                gb101.SetC1Event ( msgbuf.Id, data, len, change_yx_type, time );
                PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 1 data len=%d\n", msgbuf.Id, len );
            }
            pcdtc = gb101.SearchClassTable ( SOE_DATA );
            if ( pcdtc != 0 ) {
                gb101.SetC2Event ( msgbuf.Id, data, len, soe_type, time );
                PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 2 data\n", msgbuf.Id );
            }
        } else if ( msgbuf.MessageId == MSG_YC ) {
            PFUNC ( TEM_INFO, DEBUG_PRO, "this is yc change msg\n" );
            len = user_data.GetDataSize ( msgbuf.Id );
            user_data.GetData ( msgbuf.Id, &u_data );
            pcdtc = gb101.SearchClassTable ( CHANGE_YC_DATA );
            if ( pcdtc == 0 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "yc change msg class not find\n" );
                return 0;
            }
            if ( u_data.Data == 0 ) return 0;
            if ( pcdtc->grade == CLASS1 ) {
                if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TD_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    coff = u_data.UnitaryCoff;
                    ftmp = ftmp / coff;
                    itmp = ftmp * 32767;
                    memcpy ( data, ( char* ) &itmp, 2 );
                    data[2] = 0;
                    len = 3;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
                } else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TE_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    coff = u_data.ScaleCoff;
                    itmp = ftmp * coff;
                    memcpy ( ( char* ) &itmp, ( char* ) &ftmp, 4 );
                    memcpy ( data, ( char* ) &itmp, 2 );
                    data[2] = 0;
                    len = 3;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
                } else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TF_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    coff = u_data.ShorFloCoff;
                    memcpy ( data, ( char* ) &ftmp, 4 );
                    data[4] = 0;
                    len = 5;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TF_1, itmp, ftmp, coff );
                }
            }
            else if ( pcdtc->grade == CLASS2 ) {
                if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TD_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    // ftmp = * ( float* ) & ( ( DataType1* ) & u_data.Data )->Data;
                    coff = u_data.UnitaryCoff;
                    ftmp = ftmp / coff;
                    itmp = ftmp * 32767;
                    memcpy ( data, ( char* ) &itmp, 2 );
                    data[2] = 0;
                    len = 3;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
                } else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TE_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    coff = u_data.ScaleCoff;
                    itmp = ftmp * coff;
                    memcpy ( data, ( char* ) &itmp, 2 );
                    data[2] = 0;
                    len = 3;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
                } else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TF_1 ) {
                    memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    coff = u_data.ShorFloCoff;
                    memcpy ( data, ( char* ) &ftmp, 4 );
                    data[4] = 0;
                    len = 5;
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TF_1, itmp, ftmp, coff );
                }
            }
            if ( pcdtc->grade == CLASS1 ) {
                gb101.SetC1Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
            } else {
                gb101.SetC2Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
            }
        } else if ( msgbuf.MessageId == MSG_YK ) {
            PFUNC ( TEM_INFO, DEBUG_PRO, "this is yk ack msg\n" );
            len = user_data.GetDataSize ( msgbuf.Id );
            memcpy ( data, msgbuf.Data, len );
            pcdtc = gb101.SearchClassTable ( CTRL_RET_DATA );
            if ( pcdtc != 0 ) {
                if ( pcdtc->grade == CLASS1 ) {
                    PFUNC ( TEM_INFO, DEBUG_PRO, "yk is class 1 data\n" );
                    gb101.SetC1Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
                } else {
                    PFUNC ( TEM_INFO, DEBUG_PRO, "yk is class 2 data\n" );
                    gb101.SetC2Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
                }
            }
        }
        return 1;
    }
    // PFUNC ( TEM_INFO,DEBUG_PRO, "no msg to be get\n" );
    return 0;
}
int16 Fomat101Msg ( MessageData<BaseDataType>msgbuf )
{
    PRO_DATA_CLASS_CFG *pcdtc;
    char  data[8];
    uint8 len = 0;
    char time[7];
    float ftmp, coff;
    int32 itmp;
    Data<BaseDataType> u_data;
    ID_MAP *imp = 0;
    BUF_ADDR_REGIST_TABLE *bart = 0;
    uint8 change_yx_type = 0, soe_type = 0;
    memcpy ( time, ( char* ) &msgbuf.Time, 7 );
	//PFUNC ( TEM_INFO,DEBUG_PRO, "get message Id:%x DataId:%x \n",msgbuf.MessageId,msgbuf.Id );
    PFUNC ( TEM_INFO, DEBUG_PRO, "protocol get msgID=%d,msgDataType=%d,msgOwner=%d,dataid=0x%x,data=%d\n", msgbuf.MessageId, msgbuf.Data->DataType, msgbuf.Owner, msgbuf.Id, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
    if ( msgbuf.MessageId == MSG_YX ) {
        len = user_data.GetDataSize ( msgbuf.Id );
        if ( len == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "error data len=%d\n", len );
            return 0;
        }
        memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
        imp = gb101.GetDataAddr ( ( uint16 ) msgbuf.Id );
        if ( imp == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "error msg id not found =%x\n", msgbuf.Id );
            return 0;
        }
        if ( isDyx(imp->data_id)) {
            change_yx_type = CHANGE_DYX_DATA;
            soe_type = SOE2_DATA;
        } else {
            change_yx_type = CHANGE_YX_DATA;
            soe_type = SOE_DATA;
        }
        //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        pcdtc = gb101.SearchClassTable ( CHANGE_YX_DATA );
        if ( pcdtc != 0 ) {
            gb101.SetC1Event ( msgbuf.Id, data, len, change_yx_type, time );
            //PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 1 data len=%d\n", msgbuf.Id, len );
        }
        pcdtc = gb101.SearchClassTable ( SOE_DATA );
        if ( pcdtc != 0 ) {
            gb101.SetC2Event ( msgbuf.Id, data, len, soe_type, time );
            //PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 2 data\n", msgbuf.Id );
        }
    } else if ( msgbuf.MessageId == MSG_YC ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is yc change msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        user_data.GetData ( msgbuf.Id, &u_data );
        pcdtc = gb101.SearchClassTable ( CHANGE_YC_DATA );
        if ( pcdtc == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "yc change msg class not find\n" );
            return 0;
        }
        if ( u_data.Data == 0 ) return 0;
        if ( pcdtc->grade == CLASS1 ) {
            if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TD_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.UnitaryCoff;
                ftmp = ftmp / coff;
                itmp = ftmp * 32767;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
            } else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TE_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( ( char* ) &itmp, ( char* ) &ftmp, 4 );
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
            } else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TF_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ShorFloCoff;
                memcpy ( data, ( char* ) &ftmp, 4 );
                data[4] = 0;
                len = 5;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,ftmp=%f,<%f>\n", M_ME_TF_1, ftmp, coff );
            }
        }
        else if ( pcdtc->grade == CLASS2 ) {
            if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TD_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.UnitaryCoff;
                ftmp = ftmp / coff;
                itmp = ftmp * 32767;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
            } else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TE_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
            } else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TF_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                memcpy ( data, ( char* ) &ftmp, 4 );
                coff = u_data.ShorFloCoff;
                data[4] = 0;
                len = 5;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,ftmp=%f,<%f>\n", M_ME_TF_1, ftmp, coff );
            }
        }
        if ( pcdtc->grade == CLASS1 ) {
            gb101.SetC1Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
        } else {
            gb101.SetC2Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
        }
    } else if ( msgbuf.MessageId == MSG_YK ) {
        bart = protocol.SearchBufAddrRegistTable ( protocol.member.YkPortNum );
        if ( bart != 0 ) {
            if ( bart->pro_id != PRO101 ) {
                return 0;
            }
        }
        PFUNC ( TEM_INFO, DEBUG_PRO101, "this is yk ack msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
        PFUNC ( TEM_INFO, DEBUG_PRO101, "data=%x msgbuf.Data->Data=%x\n", data[0], ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        pcdtc = gb101.SearchClassTable ( CTRL_RET_DATA );
        if ( pcdtc != 0 ) {
            if ( pcdtc->grade == CLASS1 ) {
                PFUNC ( TEM_INFO, DEBUG_PRO101, "yk is class 1 data\n" );
                gb101.SetC1Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
            } else {
                PFUNC ( TEM_INFO, DEBUG_PRO101, "yk is class 2 data\n" );
                gb101.SetC2Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
            }
        }
    } else if ( msgbuf.MessageId == MSG_PARA ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is PARA  msg\n" );
        uint32 data_id;
        data_id = gb101.IsMyPara ( msgbuf.Id );
        if ( data_id == PARA_5097 ) {
            bart = protocol.SearchBufAddrRegistTable ( protocol.member.ParaPortNum );
            if ( bart != 0 ) {
                if ( bart->pro_id != PRO101 ) {
                    return 0;
                } else {
                    len = user_data.GetDataSize ( data_id );
                    memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data[0], ( ( DataType1* ) ( msgbuf.Data ) )->Data );
                    pcdtc = gb101.SearchClassTable ( VERIFY_RET_DATA );
                    data[len] = 0;//add the xiandingci
                    if ( pcdtc != 0 ) {
                        if ( pcdtc->grade == CLASS1 ) {
                            //PFUNC ( TEM_INFO, DEBUG_PRO, "verify ack is class 1 data\n" );
                            gb101.SetC1Event ( msgbuf.Id, data, len + 1, VERIFY_RET_DATA, time );
                        } else {
                            //PFUNC ( TEM_INFO, DEBUG_PRO, "verify ack is class 2 data\n" );
                            gb101.SetC2Event ( msgbuf.Id, data, len + 1, VERIFY_RET_DATA, time );
                        }
                    }
                }
            }
        } else if ( data_id ) {
            //PFUNC ( TEM_INFO, DEBUG_PRO, "this is gb101 PARA  msg,%x\n" , msgbuf.Id );
            gb101.WriteCfg ( data_id, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ) );
        }
    }
    return 1;
}
int16 Fomat104Msg ( MessageData<BaseDataType>msgbuf )
{
    PRO_DATA_CLASS_CFG *pcdtc;
    char  data[8];
    uint8 len = 0;
    char time[7];
    float ftmp, coff;
    int32 itmp;
    Data<BaseDataType> u_data;
    ID_MAP *imp = 0;
    BUF_ADDR_REGIST_TABLE *bart = 0;
    uint8 change_yx_type = 0, soe_type = 0;
    memcpy ( time, ( char* ) &msgbuf.Time, 7 );
	PFUNC ( TEM_INFO,DEBUG_PRO, "get message Id:%x DataId:%x \n",msgbuf.MessageId,msgbuf.Id );
    //PFUNC ( TEM_INFO, DEBUG_PRO, "protocol get msgID=%d,msgDataType=%d,msgOwner=%d,dataid=%x,data=%d\n", msgbuf.MessageId, msgbuf.Data->DataType, msgbuf.Owner, msgbuf.Id, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
    if ( msgbuf.MessageId == MSG_YX ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is yx change msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        if ( len == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "error data len=%d\n", len );
            return 0;
        }
        memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
        imp = gb104.GetDataAddr ( ( uint16 ) msgbuf.Id );
        if ( imp == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "error msg id not found =%x\n", msgbuf.Id );
            return 0;
        }
        if ( isDyx(imp->data_id)) {
            change_yx_type = CHANGE_DYX_DATA;
            soe_type = SOE2_DATA;
        } else {
            change_yx_type = CHANGE_YX_DATA;
            soe_type = SOE_DATA;
        }
        //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data, ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        pcdtc = gb104.SearchClassTable ( CHANGE_YX_DATA );
        if ( pcdtc != 0 ) {
            gb104.SetC1Event ( msgbuf.Id, data, len, change_yx_type, time );
            //PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 1 data len=%d\n", msgbuf.Id, len );
        }
        pcdtc = gb104.SearchClassTable ( SOE_DATA );
        if ( pcdtc != 0 ) {
            gb104.SetC2Event ( msgbuf.Id, data, len, soe_type, time );
            //PFUNC ( TEM_INFO, DEBUG_PRO, "yx[%d] is class 2 data\n", msgbuf.Id );
        }
    } else if ( msgbuf.MessageId == MSG_YC ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is yc change msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        user_data.GetData ( msgbuf.Id, &u_data );
        pcdtc = gb104.SearchClassTable ( CHANGE_YC_DATA );
        if ( pcdtc == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO, "yc change msg class not find\n" );
            return 0;
        }
        if ( u_data.Data == 0 ) return 0;
        if ( pcdtc->grade == CLASS1 ) {
            if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TD_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.UnitaryCoff;
                ftmp = ftmp / coff;
                itmp = ftmp * 32767;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
            } else if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TE_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( ( char* ) &itmp, ( char* ) &ftmp, 4 );
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
            } else if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TF_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ShorFloCoff;
                memcpy ( data, ( char* ) &ftmp, 4 );
                data[4] = 0;
                len = 5;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,ftmp=%f,<%f>\n", M_ME_TF_1,  ftmp, coff );
            }
        }
        else if ( pcdtc->grade == CLASS2 ) {
            if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TD_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.UnitaryCoff;
                ftmp = ftmp / coff;
                itmp = ftmp * 32767;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TD_1, itmp, ftmp, coff );
            } else if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TE_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( data, ( char* ) &itmp, 2 );
                data[2] = 0;
                len = 3;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TE_1, itmp, ftmp, coff );
            } else if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TF_1 ) {
                memcpy ( ( char* ) &ftmp, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                memcpy ( data, ( char* ) &ftmp, 4 );
                coff = u_data.ShorFloCoff;
                data[4] = 0;
                len = 5;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "type %d,itmp=%d,ftmp=%f,<%f>\n", M_ME_TF_1, ftmp, coff );
            }
        }
        if ( pcdtc->grade == CLASS1 ) {
            gb104.SetC1Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
        } else {
            gb104.SetC2Event ( msgbuf.Id, data, len, CHANGE_YC_DATA, time );
        }
    } else if ( msgbuf.MessageId == MSG_YK ) {
        bart = protocol.SearchBufAddrRegistTable ( protocol.member.YkPortNum );
        if ( bart != 0 ) {
            if ( bart->pro_id != PRO104 ) {
                return 0;
            }
        }
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is yk ack msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
        //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data[0], ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        pcdtc = gb104.SearchClassTable ( CTRL_RET_DATA );
        if ( pcdtc != 0 ) {
            if ( pcdtc->grade == CLASS1 ) {
                //PFUNC ( TEM_INFO, DEBUG_PRO, "yk is class 1 data\n" );
                gb104.SetC1Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
            } else {
                //PFUNC ( TEM_INFO, DEBUG_PRO, "yk is class 2 data\n" );
                gb104.SetC2Event ( msgbuf.Id, data, len, CTRL_RET_DATA, time );
            }
        }
    } else if ( msgbuf.MessageId == MSG_STEP ) {
        bart = protocol.SearchBufAddrRegistTable ( protocol.member.YkPortNum );
        if ( bart != 0 ) {
            if ( bart->pro_id != PRO104 ) {
                return 0;
            }
        }
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is btj ack msg\n" );
        len = user_data.GetDataSize ( msgbuf.Id );
        memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
        //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data[0], ( ( DataType1* ) ( msgbuf.Data ) )->Data );
        pcdtc = gb104.SearchClassTable ( STEP_RET_DATA );
        if ( pcdtc != 0 ) {
            if ( pcdtc->grade == CLASS1 ) {
                //PFUNC ( TEM_INFO, DEBUG_PRO, "btj is class 1 data\n" );
                gb104.SetC1Event ( msgbuf.Id, data, len, STEP_RET_DATA, time );
            } else {
                //PFUNC ( TEM_INFO, DEBUG_PRO, "btj is class 2 data\n" );
                gb104.SetC2Event ( msgbuf.Id, data, len, STEP_RET_DATA, time );
            }
        }
    } else if ( msgbuf.MessageId == MSG_PARA ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO, "this is PARA  msg\n" );
        uint32 para_id;
        para_id = gb104.IsMyPara ( msgbuf.Id );
        if ( para_id == PARA_5097 ) {
            bart = protocol.SearchBufAddrRegistTable ( protocol.member.ParaPortNum );
            if ( bart != 0 ) {
                if ( bart->pro_id != PRO104 ) {
                    return 0;
                } else {
                    len = user_data.GetDataSize ( para_id );
                    memcpy ( data, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ), len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO, "data=%d msgbuf.Data->Data=%d\n", data[0], ( ( DataType1* ) ( msgbuf.Data ) )->Data );
                    pcdtc = gb101.SearchClassTable ( VERIFY_RET_DATA );
                    data[len] = 0;//add the xiandingci
                    if ( pcdtc != 0 ) {
                        if ( pcdtc->grade == CLASS1 ) {
                            //PFUNC ( TEM_INFO, DEBUG_PRO, "verify ack is class 1 data\n" );
                            gb101.SetC1Event ( msgbuf.Id, data, len + 1, VERIFY_RET_DATA, time );
                        } else {
                            //PFUNC ( TEM_INFO, DEBUG_PRO, "verify ack is class 2 data\n" );
                            gb101.SetC2Event ( msgbuf.Id, data, len + 1, VERIFY_RET_DATA, time );
                        }
                    }
                }
            }
        } else if ( para_id ) {
            //PFUNC ( TEM_INFO, DEBUG_PRO, "this is gb104 PARA  msg,%x\n" , msgbuf.Id );
            gb104.WriteCfg ( para_id, ( char* ) & ( ( ( DataType1* ) ( msgbuf.Data ) )->Data ) );
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  消息处理回调函数，提供给msg线程调用
*  @author  liht
*  @param[in] MessageData<BaseDataType>msgbuf
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::GetMsg ( MessageData<BaseDataType>msgbuf )
{
    Fomat101Msg ( msgbuf );
    Fomat104Msg ( msgbuf );
    return 1;
}
/**
*************************************************************************************
*  @brief  规约线程发送消息函数
*  @author  liht
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::SendMsg ( void )
{
    static int i = 0;
    DataType1 d1;
    i++;
    d1.Data = 123;
    CP56Time2a _t;
    msg.SendMsg ( TASK_STATIS, MSG_YK, i + 50, &d1, &_t );
    //PFUNC(TEM_INFO,DEBUG_PRO,"prot send msg reciever=%d msgid=%d msg=%d dataid=%d\n",msginfo.Owner,msginfo.MessageId,d1.Data,50+i);
    return 1;
}
/**
*************************************************************************************
*  @brief  规约线程与通信线程的发送数据接口
*  @author  liht
*  @param[in] BUF_MUX *p
*  @param[in] char *buf
*  @param[in] uint8 len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Protocol::AddSendBuf ( BUF_MUX *p, char *buf, uint8 len )
{
    int i;
    if ( p == 0 ) return ;
    if ( len > 255 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO, "len=%d exceed 255\n", len );
        return ;
    }
    usleep ( 10000 );
    PFUNC ( TEM_INFO, DEBUG_PRO, "port:%d,send len=%d,int_cnt=%d\n",p->port_id, len, p->send.in_cnt );
    for ( i = 0;i < len;i++ ) {
        p->send.buf[p->send.in_cnt++] = buf[i];
        //PFUNC(TEM_WARNING,DEBUG_PRO,"%02X \n",buf[i]);
        if ( p->send.in_cnt >= BUFFER_SIZE ) {
            //PFUNC ( TEM_WARNING, DEBUG_PRO, "int_cnt=%d exceed %d\n", p->send.in_cnt , BUFFER_SIZE );
            p->send.in_cnt = 0;
        }
    }
    //PFUNC ( TEM_INFO, DEBUG_PRO, "int_cnt=%d,out_cnt=%d\n", p->send.in_cnt, p->send.out_cnt );
}
/**
*************************************************************************************
*  @brief  保存发送帧，以备重传
*  @author  liht
*  @param[in] BUF_MUX *p
*  @param[in] char *buf
*  @param[in] uint8 len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Protocol::CopySendFrame ( BUF_MUX *p, char *buf, uint8 len )
{
    if ( p == 0 ) return ;
    PFUNC ( TEM_INFO, DEBUG_PRO, "save last send frame len=%d\n", len );
    memcpy ( p->send_frame_copy.buf, buf, len );
    p->send_frame_copy.in_cnt = len;
}
/**
*************************************************************************************
*  @brief  通信线程向规约设置解析缓存接口
*  @author  liht
*  @param[in] BUF_MUX *buf
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::SetBuff ( BUF_MUX *buf )
{
    if ( buf == 0 )
        return 0;
AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
/*
    if ( gb101.member.InitOver != 0 && buf->pro_id == PRO101 ) {
        AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
    }
    if ( gb104.member.InitOver != 0 && buf->pro_id == PRO104 ) {
        AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
    }
    if ( buf->pro_id == PROGPS ) {
        AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
    }
    if ( buf->pro_id == PROUPDATE ) {
        AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
    }
	if(buf->pro_id==PRO_RARETRANS){
		AddBufRegistTable ( buf, buf->port_id, buf->pro_id );
	}
*/
    return 1;
}
/**
*************************************************************************************
*  @brief  规约解析缓存注册表添加函数
*  @author  liht
*  @param[in] BUF_MUX *buf,
*  @param[in] uint16 port_id
*  @param[in] uint8 pro_id
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::AddBufRegistTable ( BUF_MUX *buf, uint16 port_id, uint8 pro_id )
{
    int i;
    uint8 recover_flag = 0;
    uint8 table_row = member.buf_addr_regist_num;
    if ( member.buf_addr_regist_num >= MAX_PORT ) {
        return 0;
    }
    if ( /*port_id > MAX_DEAL_PORT ||*/ port_id == 0 || pro_id == 0 ) {
        return 0;
    }
    for ( i = 0;i <= member.buf_addr_regist_num;i++ ) {
        if ( member.buf_addr_regist_table[i].port_id == port_id ) {
            if ( ( member.buf_addr_regist_table[i].paddr == buf )
                    && ( member.buf_addr_regist_table[i].pro_id == pro_id ) ) {
                recover_flag = 1;
                table_row = i;
                //PFUNC ( TEM_INFO, DEBUG_PRO, "recover regist table-> buf:%x,port:%d,pro:%d,\n", buf, port_id, pro_id );
                break;
            } else {
                recover_flag = 1;
                table_row = i;
                //PFUNC ( TEM_WARNING, DEBUG_PRO, "modify regist table-> buf:%x,port:%d,pro:%d,\n", buf, port_id, pro_id );
                break;
            }
        }
    }
    member.buf_addr_regist_table[table_row].paddr = buf;
    member.buf_addr_regist_table[table_row].port_id = port_id;
    member.buf_addr_regist_table[table_row].pro_id = pro_id;
    if ( pro_id == PRO101 ) {
        member.buf_addr_regist_table[table_row].handle = deal_101;
        gb101.ConfigTimeOut ( port_id );
        gb101.ConfigLogicBuf ( port_id );
        //PFUNC ( TEM_WARNING, DEBUG_PRO, "&&&&&&&&&&&&&&&&&&&&&%x,port:%d,pro:%d,fun:%x\n", buf, port_id, pro_id, deal_101 );
    } else if ( pro_id == PRO104 ) {
        member.buf_addr_regist_table[table_row].handle = deal_104;
        gb104.ConfigTimeOut ( port_id );
        gb104.ConfigLogicBuf ( port_id );
        //PFUNC ( TEM_WARNING, DEBUG_PRO, "&&&&&&&&&&&&&&&&&&&&&%x,%d,%d,%x\n", buf, port_id, pro_id, deal_104 );
    }  else if ( pro_id == PROGPS ) {
        member.buf_addr_regist_table[table_row].handle = deal_progps;
        //gps.ConfigTimeOut ( port_id );
        //gps.ConfigLogicBuf ( port_id );
        //PFUNC ( TEM_WARNING, DEBUG_PRO, "&&&&&&&&&&&&&&&&&&&&&%x,%d,%d,%x\n", buf, port_id, pro_id, deal_progps );
    } else if ( pro_id == PROUPDATE ) {
        member.buf_addr_regist_table[table_row].handle = DealUpdatePro;
        updp.SetSendHeartFlag ( 1 );
    } else if ( pro_id == PRO_RARETRANS ) {
        member.buf_addr_regist_table[table_row].handle = deal_RareTrans;
    }else if ( pro_id == PRO_SNTP ) {
        member.buf_addr_regist_table[table_row].handle = deal_sntp;
	}
	else {
        return 0;
    }
    if ( recover_flag == 0 ) {
        member.buf_addr_regist_num++;
        PFUNC ( TEM_INFO, DEBUG_PRO, "add regist table-> buf:%x,port:%d,pro:%d,\n", buf, port_id, pro_id );
        //PFUNC ( TEM_INFO, DEBUG_PRO, "buf_addr_regist_num:%d\n",member.buf_addr_regist_num );
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  清空解析缓存注册表函数
*  @author  liht
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Protocol::ClearBufRegistTable ( void )
{
    PFUNC ( TEM_INFO, DEBUG_PRO, "buf_addr_regist_table all clear\n" );
    memset ( member.buf_addr_regist_table, 0, sizeof ( member.buf_addr_regist_table ) );
    member.buf_addr_regist_num = 0;
    return 1;
}
int16 Protocol::ClearBufRegistTable ( uint8 pro_id )
{
    int i;
    return 0;
    for ( i = 0;i < member.buf_addr_regist_num;i++ ) {
        if ( member.buf_addr_regist_table[i].pro_id == pro_id ) {
            member.buf_addr_regist_table[i].port_id = 0;
            member.buf_addr_regist_table[i].pro_id = 0;
            member.buf_addr_regist_table[i].port_status = 0;
            member.buf_addr_regist_table[i].paddr = 0;
            PFUNC ( TEM_INFO, DEBUG_PRO, "buf_addr_regist_table pro %d clear\n", pro_id );
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  根据端口号查注册缓存
*  @author  liht
*  @param[in] uint16 port_id
*  @return  成功返回注册表地址，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
BUF_ADDR_REGIST_TABLE * Protocol:: SearchBufAddrRegistTable ( uint16 port_id )
{
    int i;
    for ( i = 0;i < member.buf_addr_regist_num;i++ ) {
        if ( member.buf_addr_regist_table[i].port_id == port_id ) {
            return &member.buf_addr_regist_table[i];
        }
    }
    return 0;
}
BUF_ADDR_REGIST_TABLE * Protocol:: SearchBufAddrRegistTable2 ( uint8 pro_id )
{
    int i;
    for ( i = 0;i < member.buf_addr_regist_num;i++ ) {
        if ( member.buf_addr_regist_table[i].pro_id == pro_id ) {
            return &member.buf_addr_regist_table[i];
        }
    }
    return 0;
}
////////////////////////////////////规约对应用函数的统一封装/////////////////////////
/**************************************************************************************
*inbuf[0]=
*|数据个数|表头                                                    |数据                                  |
*|NUM(uint16 )     |ID(uint16)|LEN(uint8)|IDn(uint16)|LENn(uint8)|DATA1|DATA2|DATA3|...|DATAn|
*
*,inlen求出每个数据的长度，将数据转换成对应调用处理
*函数的参数。
*outbuf的格式与inbuf的格式一样。
*返回值说明:
*0  操作失败
*1  操作成功，但outbuf缓存数据无效
*2  操作成功，outbuf缓存有数据需要规约处理。
*
***************************************************************************************/
/**
*************************************************************************************
*  @brief  设置时间
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProSetTime ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //设置时间
{
    uint16 data_num;
	uint8 time[6];
    if ( !VerifyRecvFormalPara ( inbuf, inlen, outbuf, outlen ) ) {
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    if ( data_num != 1 ) {
        return 0;
    }
    //add the function interface to apply
    PFUNC ( TEM_INFO, DEBUG_PRO, "%d-%d-%d  %d:%d:%dms @week:%d  \n", ( uint8 ) inbuf[11] + 2000
            , ( uint8 ) inbuf[10]
            , ( uint8 ) inbuf[9]&0x1f
            , ( uint8 ) inbuf[8]&0x1f
            , ( uint8 ) inbuf[7]&0x3f
            , ( uint8 ) inbuf[5] + ( uint8 ) inbuf[6]*256
            , ( ( uint8 ) inbuf[9] >> 5 ) &0x07 );
	//cp56time2a time format change to rtc time format
	time[0]=(( uint8 )inbuf[5] + ( uint8 ) inbuf[6]*256)/1000;
	time[1]=( uint8 ) inbuf[7]&0x3f;
	time[2]=( uint8 ) inbuf[8]&0x1f;
	time[3]=( uint8 ) inbuf[9]&0x1f;
	time[4]=( uint8 ) inbuf[10];
	time[5]=( uint8 ) inbuf[11];
	PFUNC ( TEM_INFO, DEBUG_PRO, "%d-%d-%d  %d:%d:%d \n", time[5],time[4], time[3],time[2], time[1],time[0]);
    timer.SetLocalTime (time);
    CP56Time2a time2;
	Data<BaseDataType> para;
	TerminalTime d2;
	para.Data=&d2;    
    timer.GetCP56Time ( &time2 );
	memcpy(&d2.Data.time,&time2,sizeof(CP56Time2a));
	user_data.SetData(PARA_5215,&para);	
	msg.SendMsg(TASK_STATIS|TASK_PRO|TASK_VDSP,MSG_PARA,PARA_5215,&d2,&time2);
    return 0;
}
/**
*************************************************************************************
*  @brief  规约读时间
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProGetTime ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读时间
{
    uint16 i;
    uint16 data_num;
    uint16 data_id;
    uint8 data_len;
    uint16 data_start_pos;
    //char data[256];
    uint16 max_out_len;
    if ( !VerifyRecvFormalPara ( inbuf, inlen, outbuf, outlen ) ) {
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    data_start_pos = data_num * 3 + 5;
    max_out_len = *outlen;
    *outlen = 0;
    memset ( outbuf, 0, *outlen );
    memcpy ( outbuf, inbuf, data_start_pos );
    *outlen += data_start_pos;
    for ( i = 0;i < data_num;i++ ) {
        data_id = * ( uint16* ) & inbuf[2+i*3];
        //add the function interface to apply
        //if(ReadTime(data_id,data))
        //{
        memcpy ( &outbuf[2+3*i], ( char* ) &data_id, 2 );
        //* ( uint16* ) &outbuf[2+3*i] = data_id;
        data_len = 7;
        data_start_pos += data_len;
        if ( data_start_pos > max_out_len ) {
            break;
        }
        *outlen += data_len;
        //  return 2;
        //}
        //else
        //{
        // return 0;
        //}
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  按照规约要求读数据
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProGetData ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读取数据
{
    uint16 i;
    uint16 data_num;
    uint16 data_id;
    uint16 data_start_pos = 0;
    char data[8];
    float ftmp = 0;
    int16 itmp = 0;
    float coff = 0;
    uint16 max_out_len;
    Data<BaseDataType> u_data;
    if ( !VerifyRecvFormalPara ( inbuf, inlen, outbuf, outlen ) ) {
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    data_start_pos = data_num * 3 + 2;
    max_out_len = *outlen;
    *outlen = 0;
    memcpy ( outbuf, inbuf, data_start_pos );
    *outlen += data_start_pos;
    for ( i = 0;i < data_num;i++ ) { //outbuf 0 1 :num,2 3: id ,4:size
        memcpy ( ( char* ) &data_id, & inbuf[2+3*i], 2 );
        memcpy ( &outbuf[2+3*i], ( char* ) &data_id, 2 );
        outbuf[3*i+4] = user_data.GetDataSize ( data_id );
    }
    data_start_pos = 3 * i + 2;
    for ( i = 0;i < data_num;i++ ) {
        memcpy ( ( char* ) &data_id, & inbuf[2+3*i], 2 );
        user_data.GetData ( data_id, &u_data, data );
        if ( outbuf[i*3+4] == 1 ) {//μ￥μ??
            if ( protocol.member.data_type == M_SP_NA_1 || protocol.member.data_type == M_DP_NA_1 ) {
                memcpy ( &outbuf[data_start_pos], data, 1 );
                data_start_pos += ( uint8 ) outbuf[i*3+4];
                *outlen = data_start_pos;
            } else if (  protocol.member.data_type == M_ST_NA_1 ) {
                memcpy ( &outbuf[data_start_pos], data, 1 );
                data_start_pos += 1;
                outbuf[data_start_pos] = 0;
                data_start_pos += 1;
                outbuf[i*3+4] = 2;
                *outlen = data_start_pos;
            }
        } else if ( ( uint8 ) outbuf[i*3+4] > 2 ) {
            if ( protocol.member.data_type == M_ME_NA_1 ) {
                memcpy ( ( char* ) &ftmp, data, 4 );
				PFUNC ( TEM_INFO, DEBUG_PRO, "value[%x]=%f\n",data_id,ftmp );
                coff = u_data.UnitaryCoff;
                if ( coff != ( float ) 0 ) {
                    ftmp = ftmp / coff;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO, "divid by zero\n" );
                }
                itmp = ftmp * 32767;
                memcpy ( &outbuf[data_start_pos], ( char* ) &itmp, 2 );
                data_start_pos += 2;
                outbuf[data_start_pos] = 0;//QOI
                data_start_pos += 1;
                outbuf[3*i+4] = 3;
            } else if ( protocol.member.data_type == M_ME_NB_1 ) {
                memcpy ( ( char* ) &ftmp, data, 4 );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( &outbuf[data_start_pos], ( char* ) &itmp, 2 );
                 data_start_pos += 2;
                 outbuf[data_start_pos] = 0;//QOI
                data_start_pos += 1;
                outbuf[3*i+4] = 3;
            } else if ( protocol.member.data_type == M_ME_NC_1 ) {
                memcpy ( ( char* ) &ftmp, data, 4 );
                coff = u_data.ShorFloCoff;
                memcpy ( &outbuf[data_start_pos], ( char* ) &ftmp, 4 );
                data_start_pos += 4;
                outbuf[data_start_pos] = 0;//QOI
                data_start_pos += 1;
                outbuf[3*i+4] = 5;
            }
            *outlen = data_start_pos;
        }
        //PFUNC(TEM_INFO,DEBUG_PRO,"%d,%d\n",outbuf[data_start_pos],outbuf[3*i+4]);
    }
    //PFUNC(TEM_INFO,DEBUG_PRO,"data_pos=%d,out_len=%d\n",data_start_pos,*outlen);
    return 1;
}
/**
*************************************************************************************
*  @brief  设置数据
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProSetData ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //设置数据
{
    return 1;
}
/**
*************************************************************************************
*  @brief  按照规约要求读参数
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProGetPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读取参数
{
    uint16 i = 0;
    uint16 data_num = 0;
    uint16 data_id = 0;
    uint16 data_start_pos = 0;
    float ftmp = 0;
    int32 itmp = 0;
    float coff = 0;
    char data[256];
    uint16 datalen = 0;
    char data2[256];
    uint16 data2len = 0;
    Data<BaseDataType> u_data;
    if ( !VerifyRecvFormalPara ( inbuf, inlen, outbuf, outlen ) ) {
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    data_start_pos = data_num * 3 + 2;
    * ( uint16* ) &outbuf[0] = data_num;
    *outlen = 2;
    for ( i = 0;i < data_num;i++ ) { 
        memcpy ( ( char* ) &data_id, & inbuf[2+3*i], 2 );
        memcpy ( &outbuf[2+i*3], ( char* ) &data_id, 2 );
        *outlen += 2;
        user_data.GetData ( data_id, &u_data , data );
        if ( protocol.member.pc_mse_nx == P_ME_NA_1 || protocol.member.pc_mse_nx == C_SE_NA_1 ) {
            if ( user_data.GetData ( data_id, &u_data , data ) ) {
                memcpy ( ( char* ) &ftmp, data, 4 );
                itmp = ftmp / 1000 * 32767;
                memcpy ( &data2[data2len], ( char* ) &itmp, 2 );
                data2len += 2;
                data2[data2len] = 0;
                data2len += 1;
                outbuf[4+i*3] = 3;
                *outlen += 1;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "para get error ,type %d,itmp=%d,ftmp=%f,<%f>\n", protocol.member.pc_mse_nx, itmp, ftmp, coff );
                return 0;
            }
        } else if ( protocol.member.pc_mse_nx == P_ME_NB_1 || protocol.member.pc_mse_nx == C_SE_NB_1 ) {
            if ( user_data.GetData ( data_id, &u_data, data ) ) {
                memcpy ( ( char* ) &ftmp, data, 4 );
                coff = u_data.ScaleCoff;
                itmp = ftmp * coff;
                memcpy ( &data2[data2len], ( char* ) &itmp, 2 );
                data2len += 2;
                data2[data2len] = 0;
                data2len += 1;
                outbuf[4+i*3] = 3;
                *outlen += 1;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "para get error ,type %d,itmp=%d,ftmp=%f,<%f>\n", protocol.member.pc_mse_nx, itmp, ftmp, coff );
                return 0;
            }
        } else if ( protocol.member.pc_mse_nx == P_ME_NC_1 || protocol.member.pc_mse_nx == C_SE_NC_1 ) {
            if ( user_data.GetData ( data_id, &u_data, data ) ) {
                memcpy ( ( char* ) &ftmp, &data, 4 );
                coff = u_data.ShorFloCoff;
                memcpy ( &data2[data2len], ( char* ) &ftmp, 4 );
                data2len += 4;
                data2[data2len] = 0;
                data2len += 1;
                outbuf[4+i*3] = 5;
                *outlen += 1;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "para get error ,type %d,itmp=%x,ftmp=%f,<%f>\n", protocol.member.pc_mse_nx, itmp, ftmp, coff );
                return 0;
            }
        } else {
            memcpy ( ( char* ) &data, &inbuf[data_start_pos], ( uint8 ) inbuf[4+3*i] - 1 );
            datalen = user_data.GetDataSize ( data_id );
            if ( datalen < inbuf[4+3*i] - 1 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "para len too long  ,type= %d  id=%d,len=%d<%d\n", protocol.member.pc_mse_nx, data_id, datalen, ( uint8 ) inbuf[4+3*i] - 1 );
                return 0;
            }
            if ( user_data.GetData ( data_id, &u_data, data ) ) {
                memcpy ( &data2[data2len], data, datalen );
                data2len += datalen;
                data2[data2len] = ( uint8 ) inbuf[inlen-1];
                data2len += 1;
                outbuf[4+i*3] = datalen + 1;
                *outlen += 1;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO, "para get error ,type %d  id=%d,len=%d\n", protocol.member.pc_mse_nx, data_id, datalen );
                return 0;
            }
        }
        //PFUNC(TEM_WARNING,DEBUG_PRO,"%d,%d\n",*(uint16*)&outbuf[2+3*i],outbuf[3*i+4]);
    }
    memcpy ( &outbuf[*outlen], data2, data2len );
    *outlen += data2len;
    //PFUNC(TEM_WARNING,DEBUG_PRO,"data_pos=%d,out_len=%d\n",data_start_pos,*outlen);
    return 1;
}
/**
*************************************************************************************
*  @brief  写参数
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProSetPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //设置参数
{
    uint16 i;
    uint16 data_num;
    uint16 data_id;
    uint16 data_start_pos = 0;
    float ftmp = 0;
    int32 itmp = 0;
    float coff = 0;
    char data[256];
    Data<BaseDataType> u_data;
    if ( !VerifyRecvFormalPara ( inbuf, inlen, outbuf, outlen ) ) {
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    data_start_pos = data_num * 3 + 2;
    for ( i = 0;i < data_num;i++ ) { 
        memcpy ( ( char* ) &data_id, &inbuf[2+3*i], 2 );
        user_data.GetData ( data_id, &u_data , data );
        itmp = 0;
        if ( protocol.member.pc_mse_nx == P_ME_NA_1 || protocol.member.pc_mse_nx == C_SE_NA_1 ) {
            memcpy ( ( char* ) &itmp, &inbuf[data_start_pos], ( uint8 ) inbuf[4+3*i] - 1 );
            ftmp = float ( itmp ) / 32767;
            coff = u_data.UnitaryCoff;
            ftmp *= coff;
            memcpy ( data, ( char* ) &ftmp, 4 );
            data_start_pos += ( uint8 ) inbuf[4+3*i];
        } else if ( protocol.member.pc_mse_nx == P_ME_NB_1 || protocol.member.pc_mse_nx == C_SE_NB_1 ) {
            memcpy ( ( char* ) &itmp, &inbuf[data_start_pos], ( uint8 ) inbuf[4+3*i] - 1 );
            coff = u_data.ScaleCoff;
            ftmp = itmp / coff;
            memcpy ( data, ( char* ) &ftmp, 4 );
            data_start_pos += inbuf[4+3*i];
        } else if ( protocol.member.pc_mse_nx == P_ME_NC_1 || protocol.member.pc_mse_nx == C_SE_NC_1 ) {
            memcpy ( ( char* ) &ftmp, &inbuf[data_start_pos], ( uint8 ) inbuf[4+3*i] - 1 );
            coff = u_data.ShorFloCoff;
            memcpy ( data, ( char* ) &ftmp, 4 );
            data_start_pos += inbuf[4+3*i];
        } else {
            memcpy ( ( char* ) &data, &inbuf[data_start_pos], ( uint8 ) inbuf[4+3*i] - 1 );
            if ( user_data.GetDataSize ( data_id ) < ( uint8 ) inbuf[4+3*i] - 1 ) {
            	PFUNC(TEM_ERROR,DEBUG_PRO,"data size is not match datasize:%d,%d\n",user_data.GetDataSize ( data_id ) , ( uint8 ) inbuf[4+3*i] - 1);
                return 0;
            }
        }
        ProSendMsg ( PARA_MSG_TASK_ALL, MSG_PARA, data_id, data, inbuf[4+3*i] - 1 );//send message to inform other moule and save data to Data Table
        user_data.SavePara ( data_id );//save para
        PFUNC(TEM_INFO,DEBUG_PRO,"save id:%x,size:%d\n",*(uint16*)&outbuf[2+3*i],outbuf[3*i+4]);
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  遥控数据写
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProSetYk ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //设置遥控
{
    return 0;
}
/**
*************************************************************************************
*  @brief  遥控数据读
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProGetYk ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读取遥控
{
    return 0;
}
/**
*************************************************************************************
*  @brief  复位外部的通信链路
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProRstLink ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //复位连接
{
    uint16 port_id;
    port_id = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    comm.Reset ( port_id );
    PFUNC ( TEM_WARNING, DEBUG_PRO, "ProRstLink %d port\n", port_id );
    ///////////reset(port_id);
    return 0;
}
/**
*************************************************************************************
*  @brief  复位终端
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProRstTer ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //复位终端
{
    return 0;
}
/**
*************************************************************************************
*  @brief  读文件
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProReadFile ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读文件
{
    uint16 file_name = 0;
    uint32 offset = 0;
    uint16 len = 0;
    int16 ret;
    memcpy ( ( char* ) &file_name, &inbuf[2], 2 );
    //file_name = * ( uint16* ) & inbuf[2];
    memcpy ( ( char* ) &offset, &inbuf[5], 4 );
    len = ( uint8 ) inbuf[9];
    if ( len > 230 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO, "len is overflow =%d>230\n", len );
        return 0;
    }
    PFUNC ( TEM_INFO, DEBUG_PRO, "file_name=%04x,offset=%d,len=%d\n", file_name, offset, len );
	ret=statis.ReadHisData(outbuf,offset,len,file_name);
       if(ret>=0)
       {
        *outlen=ret;
        PFUNC ( TEM_WARNING, DEBUG_PRO, "len=%d\n", *outlen );
     	return 1;
       }
#if 0
    memset ( outbuf, inbuf[2], len );
    *outlen = len;
    return 1;
#endif
 return 0;
}
/**
*************************************************************************************
*  @brief  写文件
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProWriteFile ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //写文件
{
    return 0;
}
/**
*************************************************************************************
*  @brief  读连接状态
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProGetLinkSt ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读连接状态
{
    return 0;
}
/**
*************************************************************************************
*  @brief  读文件信息
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProReadFileInfo ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //读文件信息
{
    uint16 file_name = 0;
    memcpy ( ( char* ) &file_name, &inbuf[2], 2 );
    uint32 file_len = 0;
    PFUNC ( TEM_INFO, DEBUG_PRO, "file_name=%04x\n", file_name );
    file_len = statis.GetHisDataLen ( file_name );
    PFUNC ( TEM_INFO, DEBUG_PRO, "file_len=%d\n", file_len );
    memcpy ( outbuf, inbuf, 5 );
    *outlen = 5;
    memcpy ( &outbuf[*outlen], ( char* ) &file_len, 4 );
    *outlen += 4;
    return 1;
}
/**
*************************************************************************************
*  @brief  参数激活数据
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 ProActPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen ) //使用（激活）上次设置的参数
{
    return 1;
}
/**
*************************************************************************************
*  @brief  规约向应用线程发送消息,同时保存数据到数据表中
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功，否则发送失败，没有获得对应的消息ID
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char ProSendMsg ( uint32 to_id, uint32 msg_id, uint32 dataid, char *data, uint8 len )
{
    CP56Time2a _t;
    uint8 data_size;
    data_size = user_data.GetDataSize ( dataid );
    if ( data_size < len ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO, "msg[%x] size is not pair\"send size=%d,need size=%d\"\n", dataid, len, data_size );
        return 0;
    }
    if ( !user_data.SetData ( dataid, data ) ) {
    	PFUNC ( TEM_WARNING, DEBUG_PRO, "error set data\r\n");
        return 0;
    }    
    timer.GetCP56Time ( &_t );
    msg.SendMsg ( to_id, msg_id, dataid, data, &_t );
    PFUNC ( TEM_INFO, DEBUG_PRO101, "pro send msg to=%d,msgid=%x,dataid=%x,data:%x\n", to_id, msg_id, dataid,*data );
    //dump_binary((uint8*)data,len);
    return 1;
}
/**
*************************************************************************************
*  @brief  向转发口发送数据
*  @author  liht
*  @param[in] uint16 fport
*  @param[out] char *buf
*  @param[out] uint16 len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8 Protocol::ForwordSendData ( uint16 fport, char *buf, uint16 len )
{
    BUF_ADDR_REGIST_TABLE *bart = 0;
    bart = protocol.SearchBufAddrRegistTable ( fport );
    if ( bart == 0 ) return 0;
    protocol.AddSendBuf ( bart->paddr, buf, len );
    protocol.CopySendFrame ( bart->paddr, buf, len );
    return 1;
}
