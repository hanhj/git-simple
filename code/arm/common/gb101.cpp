#include "gb101.h"
#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "timer.h"
#include "debug.h"
#include "message.h"
#include "utils.h"
extern Protocol protocol;
extern Timer timer;
extern Message msg;
Gb101 gb101;
///////////////////////////////////////////////自定义的打包程序///////////////////////////
static uint8 PackClass1Data ( uint16 port_id, char *out_buf, uint16 *out_len );//1级数据打包
static uint8 PackClass2Data ( uint16 port_id, char *out_buf, uint16 *out_len );//2级数据打包
static uint8 PackInitOverData ( char *out_buf, uint16 *out_len );//初始化结束数据打包
static uint8 PackChangeYxData ( char *out_buf, uint16 *out_len );//变化的遥信数据打包
static uint8 PackCtrlRetData ( char *out_buf, uint16 *out_len );//控制返回的数据打包
static uint8 PackChangeYcData ( char *out_buf, uint16 *out_len );//变化的遥测数据打包
static uint8 PackTimeTbData ( char *out_buf, uint16 *out_len );//时间同步数据打包
static uint8 PackSoeData ( char *out_buf, uint16 *out_len );//SOE数据打包
static uint8 PackAckSationAskData ( char *out_buf, uint16 *out_len );//总召唤数据打包
static uint8 PackAckGroupAskData ( char *out_buf, uint16 *out_len );//组召唤数据打包
static uint8 PackAckVerifyRetData ( char *out_buf, uint16 *out_len );
/////////////////////////////////规约打包程序///////////////////////////////////////////////
static char Pack_M_SP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  1//>∶＝单点信息
static char Pack_M_SP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  2//>∶＝带时标的单点信息
static char Pack_M_DP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  3//>∶＝双点信息
static char Pack_M_DP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  4//>∶＝带时标的双点信息
#if 0
static char Pack_M_ST_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  5//>∶＝步位置信忿
static char Pack_M_ST_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  6//>∶＝带时标的步位置信忿
#endif
static char Pack_M_BO_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  7//>∶＝32比特勿
#if 0
static char Pack_M_BO_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  8//>∶＝带时标的32比特?
#endif
static char Pack_M_ME_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  9//>∶＝测量值，规一化便
static char Pack_M_ME_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  10//>∶＝带时标的测量便规一化便
static char Pack_M_ME_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  11//>∶＝测量值，标度化便
static char Pack_M_ME_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  12//>∶＝带时标的测量值，标度化便
static char Pack_M_ME_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  13//>∶＝测量值，短浮点数
static char Pack_M_ME_TC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  14//>∶＝带时标的测量值，短浮点数
#if 0
static char Pack_M_IT_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  15//>∶＝累计酿
static char Pack_M_IT_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  16//>∶＝带时标的累计酿
static char Pack_M_EP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  17//>∶＝带时标的继电保护设备事件
static char Pack_M_EP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  18//>∶＝带时标的继电保护设备成组启动事件
static char Pack_M_EP_TC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  19//>∶＝带时标的继电保护设备成组输出电路信息
static char Pack_M_PS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  20//>∶＝带变位检出的成组单点信息
static char Pack_M_ME_ND_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  21//>∶＝测量值，不带品质描述词的规一化便
#endif
static char Pack_M_SP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  30//>∶＝带CP56Time2a时标的单点信忿
static char Pack_M_DP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  31//>∶＝带CP56Time2a时标的双点信忿
#if 0
static char Pack_M_ST_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  32//>∶＝带CP56Time2a时标的步位置信息
static char Pack_M_BO_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  33//>∶＝带CP56Time2a时标瘿2比特?
#endif
static char Pack_M_ME_TD_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  34//>∶＝带CP56Time2a时标的测量值，规一化便
static char Pack_M_ME_TE_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  35//>∶＝带CP56Time2a时标的测量值，标度化便
static char Pack_M_ME_TF_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  36//>∶＝带CP56Time2a时标的测量值，短浮点数
#if 0
static char Pack_M_IT_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  37//>∶＝带CP56Time2a时标的累计量
static char Pack_M_EP_TD_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  38//>∶＝带CP56Time2a时标的继电保护设备事乿
static char Pack_M_EP_TE_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  39//>∶＝带CP56Time2a时标的继电保护设备成组启动事乿
static char Pack_M_EP_TF_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  40//>∶＝带CP56Time2a时标的继电保护装置成组输出电路信忿
#endif
// 控制方向的过程信息//////////////////////////////////////////
static char Parser_C_SC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  45//>∶＝单点命令
static char Parser_C_DC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  46//>∶＝双点命令
static char Parser_C_RC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  47//>∶＝步调节命令
//（直接控制命令）//////////////////////////////////////////////
static char Parser_C_SE_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  48//>∶＝设定值命令、规一化数
static char Parser_C_SE_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  49//>∶＝设定值命令、标度化数
static char Parser_C_SE_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  50//>∶＝设定值命令，短浮点数
static char Parser_C_BO_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  51//>∶＝32比特?
static char Parser_C_BO_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  150//>∶＝比特?
// 在监视方向的系统命令（站特定参数）///////////////////////////
static char Pack_M_EI_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  70//>∶＝初始化结束
// 在控制方向的系统命令（站特定参数）///////////////////////////
static char Parser_C_IC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  100//>∶＝总召唤命令（包括分组召唤)
static char Parser_C_CI_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  101//>∶＝累计量召唤命令）
static char Parser_C_RD_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  102//>∶＝读命令
static char Parser_C_CS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  103//>∶＝时钟同步命令
static char Parser_C_TS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  104//>∶＝测试命令
static char Parser_C_RP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  105//>∶＝复位进程命令
static char Parser_C_CD_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  106//>∶＝延时获得命令
// 在控制方向的参数命令（站特定参数）//////////////////////////
static char Parser_P_ME_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  110//>∶＝测量值参数，规一化数
static char Parser_P_ME_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  111//>∶＝测量值参数，标度化数
static char Parser_P_ME_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  112//>∶＝测量值参数，短浮点数
static char Parser_P_AC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  113//>∶＝参数激活
// 文件传输（站特定参数）//////////////////////////////////
static char Pack_F_FR_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  120//>∶＝文件准备就绪
static char Pack_F_SR_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  121//>∶＝节准备就绪
static char Parser_F_SC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  122//>∶＝召唤目录，选择文件，召唤文件召唤节
static char Pack_F_LS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  123//>∶＝最后的节，最后的
static char Parser_F_AF_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  124//>∶＝认可文件，认可节
static char Pack_F_SG_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  125//>∶＝段
static char Pack_F_DR_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  126//>∶＝目录
static char Parser_P_XX_XX_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  222//>∶＝终端参数块设置
/**
*************************************************************************************
*  @brief  输入输出参数检查,对于输入参数检查数据长度应当与输入缓冲区中的数据一致,输出参数不为零.
*  @param[in] char *inbuf
*  @param[out] char *outbuf
*  @param[in] uint16 inlen
*  @param[out] uint16 *outlen
*  @return  成功返回1，否则返回0，返回1代表参数缓存可以使用
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char VerifyRecvFormalPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen )
{
    uint16 i = 0;
    uint16 data_num = 0;
    uint16 data_len = 0;
    if ( inbuf == 0 || outbuf == 0 || outlen == 0 ) { //指针不为零校验
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the buf addr is not allow,inbuf=%x,outbuf=%x,outlen=%x\n",
                inbuf, outbuf, *outlen );
        return 0;
    }
    data_num = ( uint8 ) inbuf[0] + ( uint8 ) inbuf[1] * 256;
    data_len = 0;
    for ( i = 0;i < data_num;i++ ) { //输入数据长度匹配校验
        data_len += ( uint8 ) inbuf[4+i*3];
    }
    if ( ( data_len + data_num*3 + 2 ) != inlen ) {
        PFUNC ( TEM_ERROR, DEBUG_PRO101, "inlen != inbuf datalen,inbuf bytes=%d,inlen=%d\n",
                data_len + data_num*3 + 2, inlen );
        return 0;
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  检查帧长度是否符合要求,单帧长度不应超过256
*  @param[in] uint16 len
*  @return  成功返回1，否则返回0，返回1代表符合要求
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char CheckDataLen ( uint16 len )
{
    uint8 frame_fix_len = 12;
    if ( gb101.member.pro_cfg.common_unit_addr_size == 2 ) {//公共服务单元地址长度
        frame_fix_len += 1;
    }
    if ( gb101.member.pro_cfg.link_addr_size == 2 ) {//链路地址长度
        frame_fix_len += 1;
    }
    if ( gb101.member.pro_cfg.send_cause_size == 2 ) {//发送原因长度
        frame_fix_len += 1;
    }
    if ( len > 256 - frame_fix_len ) {//帧长度是否超过最大的长度
        return 0;
    } else {
        return 1;
    }
}
/**
*************************************************************************************
*  @brief  检查发送的数据信息体地址是否是连续的，提供后续重新组帧的信息
*  @param[in] char *databuf
*  @param[in] uint16 buflen
*  @param[out] uint8  datanum
*  @return  成功返回1，否则返回0，返回1代表数据中的信息体地址连续
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char CheckAddrInOrder ( char* databuf, uint16 buflen, uint8 datanum )
{
    int i = 0;
    uint8 gap = 0;
    uint32 addr = 0;
    if ( datanum == 0 || datanum == 1 ) {
        PFUNC ( TEM_INFO, DEBUG_PRO101, "data_num too short %d\n", datanum );
        return 0;
    }
    gap = buflen / datanum;//获得数据跨度
    for ( i = 0;i < datanum - 1;i++ ) {
        addr = 0;
        if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {//信息体地址长度为3个字节
            memcpy ( ( char* ) &addr, &databuf[i*gap], 3 );
            addr += 1;
            if ( memcmp ( &addr, &databuf[ ( i+1 ) *gap], 3 ) == 0 ) {//前一个与后一个比较
                continue;
            } else {
		        PFUNC ( TEM_INFO, DEBUG_PRO101, "data not in order\n");
                return 0;
            }
        } else {//信息体地址长度为2个字节
            memcpy ( ( char* ) &addr, &databuf[i*gap], 2 );
            addr += 1;
            if ( memcmp ( ( char* ) &addr, &databuf[ ( i+1 ) *gap], 2 ) == 0 ) {//前一个与后一个比较
                continue;
            } else {
		        PFUNC ( TEM_INFO, DEBUG_PRO101, "data not in order\n");
                return 0;
            }
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  根据信息体地址连续的返回结果，重新对数据帧进行组装
*  @param[in] char *inbuf
*  @param[in] uint16 *inlen
*  @param[out] uint16 num
*  @return  成功返回1，否则返回0，返回1代表数据重新组装成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char ReBuildDataBuf ( char *inbuf, uint16 *inlen, uint8 num )
{
    int i = 0;
    uint8 gap = 0;
    char data[256];
    uint16 len = 0;
    if ( num == 0 || inlen == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "num=%d,&inlen=%08x\n", num , inlen );
        return 0;
    }
    memcpy ( ( char* ) &len, ( char* ) inlen, 2 );
    gap = len / num;
    *inlen = 0;
    if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
        memcpy ( data, inbuf, 3 );
        for ( i = 0;i < num;i++ ) {
            memcpy ( &data[i* ( gap-3 ) +3], &inbuf[i*gap+3], gap - 3 );
        }
        *inlen = i * ( gap - 3 ) + 3;
    } else if ( gb101.member.pro_cfg.datasign_addr_size == 2 ) {
        memcpy ( data, inbuf, 2 );
        for ( i = 0;i < num;i++ ) {
            memcpy ( &data[i* ( gap-2 ) +2], &inbuf[i*gap+2], gap - 2 );
        }
        len = i * ( gap - 2 ) + 2;
    }
    memcpy ( inbuf, data, len );
    memcpy ( ( char* ) inlen, ( char* ) &len, 2 );
    return 1;
}
/**
*************************************************************************************
*  @brief  读应用数据后将数据向规约格式的数据转换
*  @param[in] char *inbuf
*  @param[in] uint16 inlen
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表数据转换成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char PrepareProData ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    uint16 i = 0;
    uint16 data_num = 0;
    uint16 data_id = 0;
    uint16 data_start_pos = 0;
    uint32 data_addr = 0;
    PRO_LOGIC_BUF *plb = 0;
    ID_MAP *imp = 0;
    uint16 len = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
	//dump_binary((uint8*)in_buf,in_len);
    if ( plb != 0 ) {
        if ( !VerifyRecvFormalPara ( in_buf, in_len, out_buf, out_len ) ) { //校验参数
            return 0;
        }
        data_num = ( uint8 ) in_buf[0] + ( uint8 ) in_buf[1] * 256;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_num=%d\n", data_num );
        data_start_pos = data_num * 3 + 2;
        memset ( ( char* ) out_len, 0, 2 );
        /////////////////////数据向规约转化//////////////////
        for ( i = 0;i < data_num;i++ ) {
            memcpy ( ( char* ) &data_id, &in_buf[2+i*3], 2 );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "id:%x size:%d\n",data_id,user_data.GetDataSize(data_id));
            if ( data_id == 0 ) {//终端ID的数据，终端ID为0x0000
                data_addr = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "this is terminal selfaddr data\n\n\n\n" );
            } else {
                imp = gb101.GetDataAddr ( data_id );
                if ( imp == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "data id no found in map table %x\n", data_id );
                } else {
                    data_addr = imp->data_addr;//根据数据的ID获得信息体地址
                }
            }
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {//信息体地址为3个字节的数据组装
                memcpy ( &out_buf[ ( 3+in_buf[4+i*3] ) *i], ( char* ) &data_addr, 3 );//组装信息体地址
                len += 3;
                memcpy ( &out_buf[ ( 3+ ( uint8 ) in_buf[4+i*3] ) *i + 3], &in_buf[data_start_pos], ( uint8 ) in_buf[4+i*3] );//组装数据
                data_start_pos += ( uint8 ) in_buf[4+i*3];//取数据游标
                len += ( uint8 ) in_buf[4+i*3];
            } else {//信息体地址为2个字节的数据组装
                memcpy ( &out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i], ( uint8* ) &data_addr, 2 );
                //dump_binary( ( uint8* ) &data_addr, 2);
                len += 2;
                memcpy ( &out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 2], &in_buf[data_start_pos], ( uint8 ) in_buf[4+i*3] );
                data_start_pos += ( uint8 ) in_buf[4+i*3];
                //dump_binary( ( uint8* ) &out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 2], ( uint8 ) in_buf[4+i*3]);
                len += ( uint8 ) in_buf[4+i*3];
            }
        }
        memcpy ( ( char* ) out_len, ( char* ) &len, 2 );
        //dump_binary((uint8 *)out_buf, *out_len);
        ///////////////////填规约逻辑缓存的数据个数及顺序标志
        plb->VSQ_Word.BsVsq.SQ = 0;
        plb->VSQ_Word.BsVsq.Number = data_num;
        if ( CheckAddrInOrder ( out_buf, *out_len, data_num ) ) {
            //PFUNC(TEM_INFO,DEBUG_PRO101,"in order 1\n");
            ReBuildDataBuf ( out_buf, out_len, data_num );
            plb->VSQ_Word.BsVsq.SQ = 1;
        }
        if ( plb->SendCause.BsCause.code >= CAUSE_Introgen && plb->SendCause.BsCause.code < CAUSE_Intro16 + 1 ) {
        	//PFUNC(TEM_INFO,DEBUG_PRO101,"in order 2\n");
            plb->VSQ_Word.BsVsq.SQ = 1;
        }
        ////////////////检查数据帧长度是否符合规约帧长度要求
        //dump_binary((uint8*)out_buf,(int16)*out_len);
        if ( !CheckDataLen ( *out_len ) ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "data_len=%d over flow\n", *out_len );
            return 0;
        }
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  68开始的规约解析时查找关键字的位置和长度
*  @param[in] uint8 keyword_id
*  @param[in] uint16 inlen
*  @param[out] uint8 *pos
*  @param[out] uint8 *len
*  @return  成功返回1，否则返回0，返回1代表找到关键字成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101:: GetKeyWordPositionAndLen ( uint8 keyword_id, uint8 *pos, uint8 *len )
{
    char ret = 0;
    switch ( keyword_id ) {
    case KEY_WORD_LINKADDR:
        *pos = 5;
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *len = 2;
        } else {
            *len = 1;
        }
        ret = 1;
        break;
    case KEY_WORD_COMADDR:
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *pos = 7;
        } else {
            *pos = 6;
        }
        *pos += 2;
        if ( member.pro_cfg.send_cause_size == 2 ) {
            *pos += 2;
        } else {
            *pos += 1;
        }
        if ( member.pro_cfg.common_unit_addr_size == 2 ) {
            *len = 2;
        } else {
            *len = 1;
        }
        ret = 1;
        break;
    case KEY_WORD_TYPE:
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *pos = 7;
        } else {
            *pos = 6;
        }
        *len = 1;
        break;
    case KEY_WORD_VSQ:
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *pos = 7;
        } else {
            *pos = 6;
        }
        *pos += 1;
        *len = 1;
        break;
    case KEY_WORD_SENDCAUSE:
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *pos = 7;
        } else {
            *pos = 6;
        }
        *pos += 2;
        if ( member.pro_cfg.send_cause_size == 2 ) {
            *len = 2;
        } else {
            *len = 1;
        }
        break;
    case KEY_WORD_CTRLWORD:
        *pos = 4;
        *len = 1;
        break;
    case KEY_WORD_DATA_START:
        if ( member.pro_cfg.link_addr_size == 2 ) {
            *pos = 7;
        } else {
            *pos = 6;
        }
        *pos += 2;
        if ( member.pro_cfg.send_cause_size == 2 ) {
            *pos += 2;
        } else {
            *pos += 1;
        }
        if ( member.pro_cfg.common_unit_addr_size == 2 ) {
            *pos += 2;
        } else {
            *pos += 1;
        }
        if ( member.pro_cfg.datasign_addr_size == 3 )
            *len = 3;
        else
            *len = 2;
        ret = 1;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}
/**
*************************************************************************************
*  @brief  6816最终的组帧函数
*  @param[in] uint8 type
*  @param[in] char *data
*  @param[in] uint16 data_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char BuildFrame6816 ( uint8 type , char *out_buf, uint16 *out_len, char *data, uint16 data_len )
{
    int k = 0;
    PRO_LOGIC_BUF *plb = 0;
    PRO_TIMEOUT *to = 0;
    to = gb101.SelectTimeOut ( gb101.member.CurPortId );//查找配置的对应端口的超时变量缓存
    if ( to != 0 && gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
        to->resend_flag = 1;//预先设置重发标志，收到确认后清除
    }
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );//查找对应端口的逻辑缓存
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "NO logic buf to build frame 6816\n" );
        return 0;
    }
    out_buf[0] = 0x68; //帧头
    out_buf[3] = 0x68;
    k = 4;
    out_buf[k++] = plb->CtrlWordFromSelf.data;//控制字
    if ( gb101.member.pro_cfg.link_addr_size == 2 ) {//链路地址
        memcpy ( &out_buf[k], ( char* ) &plb->linkaddr, 2 );
        k += 2;
    } else {
        out_buf[k] = plb->linkaddr;
        k += 1;
    }
    out_buf[k++] = type;//类型标识
    out_buf[k++] = plb->VSQ_Word.data;//可变限定词
    if ( gb101.member.pro_cfg.send_cause_size == 2 ) {//发送原因
        memcpy ( &out_buf[k], ( char* ) & plb->SendCause.data, 2 );
        k += 2;
    } else {
        out_buf[k] = plb->SendCause.data;
        k += 1;
    }
    if ( gb101.member.pro_cfg.common_unit_addr_size == 2 ) {//公共服务地址，一般和链路地址一致
        memcpy ( &out_buf[k], ( char* ) & plb->hostaddr, 2 );
        k += 2;
    } else {
        out_buf[k] = plb->hostaddr;
        k += 1;
    }
    memcpy ( &out_buf[k], data, data_len );//包括信息体地址的数据
    k += data_len;
    out_buf[1] = k - 4;//帧数据长度
    out_buf[2] = k - 4;
    out_buf[k] = GetSum ( ( uint8* ) & out_buf[4], k - 4 );//帧校验和
    k += 1;
    out_buf[k++] = 0x16;//帧尾
    memcpy ( ( char* ) out_len, ( char* ) &k, 2 );
    return 1;
}
/**
*************************************************************************************
*  @brief  1016最终的组帧函数
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char BuildFrame1016 ( char *out_buf, uint16 *out_len )
{
    int i = 0;
    PRO_LOGIC_BUF *plb = 0;
    PRO_TIMEOUT *to = 0;
    to = gb101.SelectTimeOut ( gb101.member.CurPortId );//查找配置的对应端口的超时变量缓存
    if ( to != 0 && gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
        to->resend_flag = 1;//预先设置重发标志，收到确认后清除
    }
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );//查找对应端口的逻辑缓存
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "NO logic buf to build frame1016\n" );
        return 0;
    }
    out_buf[i++] = 0x10;//帧头
    out_buf[i++] = plb->CtrlWordFromSelf.data;//控制字
    if ( gb101.member.pro_cfg.link_addr_size == 2 ) {//链路地址
        memcpy ( &out_buf[i], ( char* ) &plb->linkaddr, 2 );
        i += 2;
    } else {
        out_buf[i] = plb->linkaddr;
        i += 1;
    }
    out_buf[i] = GetSum ( ( uint8* ) & out_buf[1], i - 1 );//帧校验和
    i++;
    out_buf[i++] = 0x16;//帧尾
    memcpy ( ( char* ) out_len, ( char* ) &i, 2 );
    return i;
}
/**
*************************************************************************************
*  @brief  单点信息组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_SP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  1//>∶＝单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_SP_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  带时标的单点信息组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_SP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  2//>∶＝带时标的单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_SP_TA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  双点信息组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_DP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  3//>∶＝双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_DP_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
static char Pack_M_DP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  4//>∶＝带时标的双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_len=%d\n", in_len );
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_DP_TA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  32比特串组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_BO_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  7//>∶＝32比特串
{
    Data<BaseDataType> data2;
    DataType3 *d3;
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    char ter_st_data[32];
    uint16 st_data_len = 0;
    ter_st_data[0] = 1;
    ter_st_data[1] = 0;
    ter_st_data[2] = 0;
    ter_st_data[3] = 0;
    ter_st_data[4] = 4;
    user_data.GetData ( PARA_6801, &data2 );
    d3=(DataType3*)data2.Data;
    gb101.member.TerSt.data=d3->Data;
    memcpy ( &ter_st_data[5], ( char* ) &gb101.member.TerSt.data, 4 );
    st_data_len = 9;
    ret = PrepareProData ( ter_st_data, st_data_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_BO_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
#if 0
/**
*************************************************************************************
*  @brief  带时标的32比特串组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_BO_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  8//>∶＝带时标的32比特?
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO101, "data_len=%d\n", in_len );
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_BO_TA_1 , out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
#endif
/**
*************************************************************************************
*  @brief  测量值，规一化值组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  9//>∶＝测量值，规一化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
static char Pack_M_ME_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  10//>∶＝带时标的测量便规一化值
{
    return 0;
}
/**
*************************************************************************************
*  @brief  测量值，标度化值组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  11//>∶＝测量值，标度化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_NB_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
static char Pack_M_ME_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  12//>∶＝带时标的测量值，标度化值
{
    return 0;
}
/**
*************************************************************************************
*  @brief  测量值，短浮点值组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  13//>∶＝测量值，短浮点数
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_NC_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
static char Pack_M_ME_TC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  14//>∶＝带时标的测量值，短浮点数
{
    return 0;
}
#if 0
static char Pack_M_IT_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  15//>∶＝累计酿
{
    return 0;
}
static char Pack_M_IT_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  16//>∶＝带时标的累计酿
{
    return 0;
}
static char Pack_M_EP_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  17//>∶＝带时标的继电保护设备事件
{
    return 0;
}
static char Pack_M_EP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  18//>∶＝带时标的继电保护设备成组启动事件
{
    return 0;
}
static char Pack_M_EP_TC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  19//>∶＝带时标的继电保护设备成组输出电路信息
{
    return 0;
}
static char Pack_M_PS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  20//>∶＝带变位检出的成组单点信息
{
    return 0;
}
static char Pack_M_ME_ND_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  21//>∶＝测量值，不带品质描述词的规一化便
{
    return 0;
}
#endif
/**
*************************************************************************************
*  @brief  带CP56Time2a时标的单点信息组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_SP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  30//>∶＝带CP56Time2a时标的单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_SP_TB_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  带CP56Time2a时标的双点信息组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_DP_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  31//>∶＝带CP56Time2a时标的双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_DP_TB_1, out_buf, out_len, data, data_len );
		return 1;
    }
    return 0;
}
#if 0
static char Pack_M_ST_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  32//>∶＝带CP56Time2a时标的步位置信息
{
    return 0;
}
static char Pack_M_BO_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  33//>∶＝带CP56Time2a时标瘿2比特?
{
    return 0;
}
#endif
/**
*************************************************************************************
*  @brief  带CP56Time2a时标的测量值，规一化值组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_TD_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  34//>∶＝带CP56Time2a时标的测量值，规一化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_TD_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  带CP56Time2a时标的测量值，标度化值组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_TE_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  35//>∶＝带CP56Time2a时标的测量值，标度化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_TE_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  带CP56Time2a时标的测量值，短浮点数组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_ME_TF_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  36//>∶＝带CP56Time2a时标的测量值，短浮点数
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_ME_TF_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
#if 0
static char Pack_M_IT_TB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  37//>∶＝带CP56Time2a时标的累计量
{
    return 0;
}
static char Pack_M_EP_TD_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  38//>∶＝带CP56Time2a时标的继电保护设备事乿
{
    return 0;
}
static char Pack_M_EP_TE_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  39//>∶＝带CP56Time2a时标的继电保护设备成组启动事乿
{
    return 0;
}
static char Pack_M_EP_TF_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  40//>∶＝带CP56Time2a时标的继电保护装置成组输出电路信忿
{
    return 0;
}
#endif
/**
*************************************************************************************
*  @brief  单双控制命令统一解析处理函数
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char ParserCtrl ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    DCOWORD dcoword;
    uint8 pos = 0;
    uint8 len = 0;
    uint8 msgdata;
    char datasign[4];
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    PRO_LOGIC_BUF *plb = 0;
    PRO_TIMEOUT *to = 0;
    ID_MAP *imap = 0;
    uint8 ret = 0;
    if ( protocol.member.YkPortNum != 0 && gb101.member.CurPortId != protocol.member.YkPortNum ) {
        return 0;
    }
    protocol.member.YkPortNum = gb101.member.CurPortId;
    to = gb101.SelectTimeOut ( gb101.member.CurPortId );
    if ( to == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no timeout buf to parser\n" );
        return 0;
    }
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    plb->M_type = type;
    plb->yk_cmd = type;
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 )
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    dcoword.data = ( uint8 ) in_buf[in_len-3];
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    memset ( datasign, 0, 4 );
    memcpy ( datasign, &in_buf[pos], len );
    switch ( plb->M_SendCause.BsCause.code ) {
    case CAUSE_Act:
        if ( dcoword.DcoWord.S_E == 1 ) {
            //选择
            if(to->yk_timeout_flag==1){//遥控选择超时,重新选择
            	plb->yk_datasign=0;
            	plb->DCO_Word.DcoWord.S_E=0;
            	to->yk_timeout_flag=0;
    			PFUNC ( TEM_WARNING,DEBUG_PRO101, "timeout\n");
            }
            if ( plb->DCO_Word.DcoWord.S_E == 0 ) {
                to->yk_en = 1;
                to->yk_cnt = 0;
                plb->yk_st = 1;
                memcpy ( ( char* ) &plb->yk_datasign, datasign, len );
                plb->DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb101.GetDataId ( plb->yk_datasign );
                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "yk [%x]'s id not found\n", plb->yk_datasign );
                    return 0;
                }
				msgdata=plb->DCO_Word.data;
				if(type==C_SC_NA_1)//单双点命令区分采用第五位来区分0代表单命令,1代表双命令
					cbi(msgdata,4);
				else if(type==C_DC_NA_1)
					sbi(msgdata,4);
                if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &msgdata, 1 ) ) {
                    ret = 0;
                    break;
                }
            } else {
                ///已经选择没有执行
                ret = 0;
            }
        } else if ( dcoword.DcoWord.S_E == 0 && plb->DCO_Word.DcoWord.S_E == 1 ) {
            if ( memcmp ( datasign, ( char* ) &plb->yk_datasign, len ) == 0 && to->yk_timeout_flag == 0 ) {
                //执行
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb101.GetDataId ( plb->yk_datasign );
                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "yk [%x]'s id not found\n", plb->yk_datasign );
                    return 0;
                }
				msgdata=plb->DCO_Word.data;
				if(type==C_SC_NA_1)//单双点命令区分采用第五位来区分0代表单命令,1代表双命令
					cbi(msgdata,4);
				else if(type==C_DC_NA_1)
					sbi(msgdata,4);
                if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &msgdata , 1 ) ) {
                    ret = 0;
                    break;
                }
                plb->yk_datasign = 0;//
                plb->yk_st = 2;
            } else {
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->DCO_Word.data = 0;
                to->yk_timeout_flag = 0;
                ret = 0;
                plb->yk_datasign = 0;//
                plb->yk_st = 0;
                protocol.member.YkPortNum = 0;
            }
        } else {
            to->yk_en = 0;
            to->yk_cnt = 0;
            memset ( ( char* ) &plb->yk_datasign, 0, 4 );
            plb->DCO_Word.data = 0;
            to->yk_timeout_flag = 0;
            ret = 0;
            plb->yk_st = 0;
            protocol.member.YkPortNum = 0;//避免两个主站同时遥控的通道绑定变量
        }
        break;
    case CAUSE_Deact:
        ret = 1;
        to->yk_en = 0;
        to->yk_cnt = 0;
        to->yk_timeout_flag = 0;
        plb->yk_st = 4;
        memcpy ( ( char* ) &plb->yk_datasign, datasign, 4 );
        imap = gb101.GetDataId ( plb->yk_datasign );
        if ( imap == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "yk [%x]'s id not found\n", plb->yk_datasign );
            return 0;
        }
		msgdata=dcoword.data;
		if(type==C_SC_NA_1)//单双点命令区分采用第五位来区分0代表单命令,1代表双命令
			cbi(msgdata,4);
		else if(type==C_DC_NA_1)
			sbi(msgdata,4);
       if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &msgdata, 1 ) ) {
            ret = 0;
            break;
        }
        plb->DCO_Word.data = 0;
        memset ( ( char* ) &plb->yk_datasign, 0, 4 );
        ret = 1;
        break;
    }
    //请求后续服务
    class_data_type = CTRL_RET_DATA;
    if ( ret == 1 ) {
        SetCuaseWord ( CAUSE_Actcon );
        tab = gb101.SearchClassTable ( CTRL_RET_DATA );
        if ( tab != 0 ) {//向规约后续服务列表中添加服务号
            if ( tab->grade == CLASS1 ) {
                SetAcdDfc ( 1, 0 );
                gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS1, class_data_type );
            } else {
                SetAcdDfc ( 0, 0 );
                gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS2, class_data_type );
            }
            SetFuncCode ( S_ACK );
        }
    } else {
        SetCuaseWord ( CAUSE_Deact );
        SetAcdDfc ( 0, 0 );
        SetFuncCode ( S_NACK );
    }
    if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {//如果是平衡传输，交由事件处理机制去完成遥控的确认。
        plb->confirm_flag = 1;
    }
    BuildFrame1016 ( out_buf, out_len );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "out_len addr=%x\n", out_len );
    return 1;
}
/**
*************************************************************************************
*  @brief  单点遥控命令解析
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_SC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  45//>∶＝单点命令
{
    if ( ParserCtrl ( C_SC_NA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}
/**
*************************************************************************************
*  @brief  双点遥控命令解析
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_DC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  46//>∶＝双点命令
{
    if ( ParserCtrl ( C_DC_NA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}
static char Parser_C_RC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  47//>∶＝步调节命乿
{
    return 0;
}
// 在控制方向的参数命令（站特定参数）//////////////////////////////////////////////
/**
*************************************************************************************
*  @brief  解析设置参数规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char ParserSetPara ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    ////////////参数限定词QPA not in use，但预留的帧位置
    uint8 pos = 0;
    int i = 0;
    uint8 len = 0;
    char data[256];
    char data2[256];
    uint16 data2len = 0;
    uint32 data_addr = 0;
    uint8 para_len = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    uint16 data_id = 0;
    uint8 gap = 0;
    ID_MAP *i_map = 0;
    PRO_LOGIC_BUF *plb = 0;
    char data3[256];
    uint16 data3len = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill formal para Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    memcpy ( data3, &in_buf[pos], in_len - pos - 2 );
    data3len = in_len - pos - 2;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "pos=%d len=%d\n", pos, len );
    data[0] = vsq.BsVsq.Number;
    data[1] = 0;
    plb->VSQ_Word.data = vsq.data;
    data_pos = 2;
    if ( vsq.BsVsq.SQ == 1 ) {//信息体地址连续
        if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 5 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }
        para_len = gap;
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb101.GetDataId ( data_addr + i );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
                return 0;
            }
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
            data[4+3*i] = para_len;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );
            data2len += para_len;
        }
    } else {//信息体地址不连续
        gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "gap=%d \n", gap  );
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }
            //fill the data buf header
            i_map = gb101.GetDataId ( data_addr );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_ERROR, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
            data[4+3*i] = para_len;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb101.member.pro_cfg.datasign_addr_size], para_len );
            pos += gap;
            data2len += para_len;
        }
    }
    //merge the the transform para buf
    memcpy ( in_buf, data, data_pos );
    plb->confirm_flag = 1;
    memcpy ( &in_buf[data_pos], data2, data2len );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_pos=%d,data2len=%d\n", data_pos, data2len );
    in_len = data_pos + data2len;
    //do the interface
    gb101.member.pc_mse_nx = type;
    protocol.member.pc_mse_nx = gb101.member.pc_mse_nx;
    if ( ProSetPara ( in_buf, in_len, out_buf, out_len ) ) {
        SetFuncCode ( S_ACK );
        SetCuaseWord ( CAUSE_Actcon );
    }
    else {
        SetFuncCode ( S_NACK );
        SetCuaseWord ( CAUSE_Deact );
    }
///////////////////////////////////////////
    //BuildFrame1016 ( out_buf, out_len );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "id =%x type=%d,num=%d\n" , data_id, type, plb->VSQ_Word.BsVsq.Number );
    if ( data_id == PARA_5097 && type == 222 && plb->VSQ_Word.BsVsq.Number == 1 ) {
        PRO_DATA_CLASS_CFG *tab = 0;
        if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
            plb->confirm_flag = 1;
        }
        protocol.member.ParaPortNum = gb101.member.CurPortId;
        tab = gb101.SearchClassTable ( VERIFY_RET_DATA );
        if ( tab != 0 ) { //判断总召数据采用1级还是用2级数据应答
            if ( tab->grade == CLASS1 ) {
                SetAcdDfc ( 1, 0 );
                gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS1, VERIFY_RET_DATA );
            } else {
                SetAcdDfc ( 0, 0 );
                gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS2, VERIFY_RET_DATA );
            }
            BuildFrame1016 ( out_buf, out_len );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "*out_len=%d\n\n\n", *out_len );
            return 1;
        }
        return 0;
    } else {
        BuildFrame6816 ( type, out_buf , out_len, data3, data3len );
        return 1;
    }
}
//（直接控制命令）
/**
*************************************************************************************
*  @brief  解析设定值命令、规一化值规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_SE_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  48//>∶＝设定值命令、规一化值
{
    if ( ParserSetPara ( C_SE_NA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}
/**
*************************************************************************************
*  @brief  解析设定值命令、标度化值规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_SE_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  49//>∶＝设定值命令、标度化值
{
    if ( ParserSetPara ( C_SE_NB_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}
/**
*************************************************************************************
*  @brief  解析设定值命令，短浮点数规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_SE_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  50//>∶＝设定值命令，短浮点数
{
    if ( ParserSetPara ( C_SE_NC_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_BO_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  51//>∶＝32比特串
{
    uint8 pos = 0;
    int i = 0;
    uint8 len = 0;
    char data[256];
    char data2[256];
    uint16 data2len = 0;
    uint32 data_addr = 0;
    uint8 para_len = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    uint16 data_id = 0;
    uint8 gap = 0;
    ID_MAP *i_map = 0;
    char data3[256];
    uint16 data3len = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
            if ( in_buf[pos] != C_BO_NC_1 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
                return 0;
            }
        }
        gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
        if ( len == 2 ) {
            plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
            data_pos = pos + 2;
        } else {
            plb->M_SendCause.data = ( uint8 ) in_buf[pos];
            data_pos = pos + 1;
        }
        if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
            SetCuaseWord ( CAUSE_Actterm );
        } else {
            SetCuaseWord ( CAUSE_Actcon );
        }
        //Fill Table Header
        gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
        vsq.data = in_buf[pos];
        gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
        memcpy ( data3, &in_buf[pos], in_len - pos - 2 );
        data3len = in_len - pos - 2;
        data[0] = vsq.BsVsq.Number;
        data[1] = 0;
        plb->VSQ_Word.data = vsq.data;
        data_pos = 2;
        if ( vsq.BsVsq.SQ == 1 ) {//paser the sequence data
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                gap = ( in_len - pos - 5 ) / vsq.BsVsq.Number;
                memcpy ( &data_addr, &in_buf[pos], 3 );
            } else {
                gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;
                memcpy ( &data_addr, &in_buf[pos], 2 );
            }
            para_len = gap;
            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                //fill the table header
                i_map = gb101.GetDataId ( data_addr + i );
                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_ERROR, DEBUG_PRO101, "data addr error %08x\n", data_addr );
                }
                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
                data[4+3*i] = 4;
                data_pos += 4;
                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );
                data2len += para_len;
            }
        } else {//parser the unsequence data
            gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                    para_len = gap - 3;
                    memcpy ( &data_addr, &in_buf[pos], 3 );
                } else {
                    para_len = gap - 2;
                    memcpy ( &data_addr, &in_buf[pos], 2 );
                }
                //fill the header
                i_map = gb101.GetDataId ( data_addr );
                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
                }
                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
                data[4+3*i] = 4;
                data_pos += 3;
                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+gb101.member.pro_cfg.datasign_addr_size], para_len );
                pos += gap;
                data2len += para_len;
            }
        }
        //merge the the transform para buf
        memcpy ( in_buf, data, data_pos );
        memcpy ( &in_buf[data_pos], data2, data2len );
        in_len = data_pos + data2len;
        //do the interface
        if ( ProSetData ( in_buf, in_len, out_buf, out_len ) ) {
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_len=%d\n", in_len );
            SetFuncCode ( S_ACK );
            SetCuaseWord ( CAUSE_Actcon );
        } else {
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_len=%d\n", in_len );
            SetFuncCode ( S_NACK );
            SetCuaseWord ( CAUSE_Deactcon );
        }
        plb->confirm_flag = 1;
        //build the ack frame to station
        BuildFrame6816 ( C_BO_NC_1, out_buf, out_len, data3, data3len );
        return 1;
    }
    return 0;
}
// 在监视方向的系统命令（站特定参数)
/**
*************************************************************************************
*  @brief  初始化结束组帧函数
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_M_EI_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  70//>∶＝初始化结束
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    ret = PrepareProData ( in_buf, in_len, data, &data_len );
    if ( ret ) {
        BuildFrame6816 ( M_EI_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }
    return 0;
}
// 在控制方向的系统命令（站特定参数）
/**
*************************************************************************************
*  @brief  总召唤命令帧解析
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_IC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  100//>∶＝总召唤命令（包括分组召唤?
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[8];
    uint8 datalen = 0;
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "\n" );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_IC_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    plb->M_type = C_IC_NA_1;//告诉外部逻辑是总召唤，结束后请清除
    plb->total_data_send_over = 1;//设置发送结束标志为未结束
    plb->total_ask_begin = 1;//设置召唤起始标志
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 )
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    plb->QOI = in_buf[in_len-3];
    if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }
    data[datalen++] = in_buf[in_len-3];//召唤限定词
    SetCuaseWord ( CAUSE_Actcon );
    SetFuncCode ( S_ACK );
    if ( plb->QOI == CAUSE_Introgen ) {//总召唤
        class_data_type = ACK_STATION_ASK_DATA;
    } else if ( plb->QOI > CAUSE_Introgen && plb->QOI < CAUSE_Intro16 + 1 ) {//分组召唤
        class_data_type = ACK_GROUP_ASK_DATA;
    } else {
        SetCuaseWord ( CAUSE_Deact );
    }
    tab = gb101.SearchClassTable ( class_data_type );
    if ( tab == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no support the service No.%d service\n", class_data_type );
        return 0;
    }
    if ( tab->grade == CLASS1 ) {//判断总召数据采用1级还是2级数据应答
        SetAcdDfc ( 1, 0 );
        gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS1, class_data_type );
    } else {
        SetAcdDfc ( 0, 0 );
        gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS2, class_data_type );
    }
    SetFuncCode ( S_ACK );
    if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {//平衡传输处理
        plb->confirm_flag = 1;
    }
    BuildFrame1016 ( out_buf, out_len );
    return 1;
}
static char Parser_C_CI_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  101//>∶＝累计量召唤命令）
{
    return 0;
}
/**
*************************************************************************************
*  @brief  读命令规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_RD_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  102//>∶＝读命令
{
    uint8 pos;
    int i;
    uint8 len;
    uint8 ret = 0;
    char data3[512];
    uint16 data3len = 0;
    char data[256];
    uint32 data_addr = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    uint16 data_id = 0;
    uint8 type = 0;
    ID_MAP *i_map = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "\n" );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RD_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "pos=%d,len=%d\n", pos, len );
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    data[0] = vsq.BsVsq.Number;
    data[1] = 0;
    data_pos = 2;
    if ( vsq.BsVsq.SQ == 1 ) {
        if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            i_map = gb101.GetDataId ( data_addr + i );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            //fill the table header
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
            data[4+3*i] = 0;
            data_pos += 3;
        }
    } else {
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                pos += 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                pos += 2;
            }
            i_map = gb101.GetDataId ( data_addr );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
                type = i_map->type;
                SetFuncCode ( S_ACK );
                SetCuaseWord ( CAUSE_Req );
            } else {
                type = C_RD_NA_1;
                PFUNC ( TEM_ERROR, DEBUG_PRO101, "data addr error %08x\n", data_addr );
                SetFuncCode ( S_NACK );
                SetCuaseWord ( CAUSE_Unknowndataaddr );
            }
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );
            data[4+3*i] = 0;
            data_pos += 3;
        }
    }
    protocol.member.data_type = type;
    if ( ProGetData ( data, data_pos, data3, &data3len ) ) {
        ret = PrepareProData ( data3, data3len, out_buf, out_len );
        if ( ret ) {
            BuildFrame6816 ( type, data3, &data3len , out_buf, *out_len );
            memcpy ( out_buf, data3, data3len );
            *out_len = data3len;
            return 1;
        } else{
        	PFUNC ( TEM_WARNING, DEBUG_PRO101, "\n");
        	return 0;
        	}
    } else {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "\n");
       	return 0;
    }
}
/**
*************************************************************************************
*  @brief  解析时钟同步命令
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_CS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  103//>∶＝时钟同步命令
{
    uint8 pos = 0;
    uint8 len = 0;
    PRO_DATA_CLASS_CFG *tab;
    uint8 data_pos = 0;
    PRO_LOGIC_BUF *plb = 0;
    CP56Time2a ct;
    uint16 datalen = 0;
    char data[32];
    timer.GetCP56Time ( &ct );
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_CS_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    plb->M_type = C_CS_NA_1;
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
        data_pos = pos + 3;
    } else {
        data_pos = pos + 2;
    }
    memcpy ( ( char* ) &ct, &in_buf[data_pos], 7 );
    /*PFUNC ( TEM_INFO, DEBUG_PRO101, "[%d]%d-%d-%d  %d:%d:%dms @week:%d  \n", data_pos \
            , ct.year + 2000 \
            , ct.month \
            , ct.day_week&0x1f \
            , ct.hour_su&0x1f \
            , ct.minute_iv&0x3f \
            , ct.millisecond \
            , ( ct.day_week >> 5 ) &0x07 );
    */        
////////////////pack data for prosettime func//////////
    data[0] = 1;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 7;
    memcpy ( &data[5], &in_buf[data_pos], 7 );
    datalen = 12;
    ProSetTime ( data, datalen, data, &datalen );
    SetFuncCode ( S_ACK );
    if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
        plb->confirm_flag = 1;
    }
    tab = gb101.SearchClassTable ( TIME_TB_DATA );
    if ( tab != 0 ) { //判断总召数据采用1级还是用2级数据应答
        if ( tab->grade == CLASS1 ) {
            SetAcdDfc ( 1, 0 );
            gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS1, TIME_TB_DATA );
        } else {
            SetAcdDfc ( 0, 0 );
            gb101.AddServiceBuf ( gb101.member.CurPortId, CLASS2, TIME_TB_DATA );
        }
        BuildFrame1016 ( out_buf, out_len );
        ///PFUNC ( TEM_INFO, DEBUG_PRO101, "*out_len=%d\n\n\n", *out_len );
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析测试命令帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_TS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  104//>∶＝测试命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_TS_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data num is not 1\n" );
        return 0;
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    if ( len == 3 ) {
        memcpy ( &data_addr, &in_buf[pos], 3 );
        memcpy ( data, &data_addr, 3 );
        data_pos = 3;
    } else {
        memcpy ( &data_addr, &in_buf[pos], 2 );
        memcpy ( data, &data_addr, 2 );
        data_pos = 2;
    }
    pos += len;
    if ( data_addr != 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data addr is not 0\n" );
        return 0;
    }
    memcpy ( &data[data_pos], &in_buf[pos], 2 );
    data_pos += 2;
    SetFuncCode ( S_ACK );
    BuildFrame6816 ( C_TS_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}
/**
*************************************************************************************
*  @brief  解析复位进程命令帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_RP_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  105//>∶＝复位进程命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    //uint8 datalen = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RP_NA_1  ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data num is not 1\n" );
        return 0;
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    if ( len == 3 ) {
        memcpy ( &data_addr, &in_buf[pos], 3 );
        memcpy ( data, &data_addr, 3 );
        pos += 3;
        data_pos = 3;
    } else {
        memcpy ( &data_addr, &in_buf[pos], 2 );
        memcpy ( data, &data_addr, 2 );
        pos += 2;
        data_pos = 2;
    }
    if ( data_addr != 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data addr is not 0\n" );
        return 0;
    }
    data[data_pos] = in_buf[pos];
    switch ( data[data_pos] ) {
    case 1://进程总复位
        //terminal reset;
        SetFuncCode ( S_ACK );
        break;
    case 2://复位事件缓存带时间的信息
        //event by time bufer reset;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "event buf has been reset by zero\n" );
        SetFuncCode ( S_ACK );
        break;
    default:
        SetFuncCode ( S_NACK );
        break;
    }
    data_pos += 1;
    BuildFrame6816 ( C_RP_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}
/**
*************************************************************************************
*  @brief  解析延时获得命令规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_C_CD_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  106//>∶＝延时获得命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "\n" );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_CD_NA_1  ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data num is not 1\n" );
        return 0;
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    if ( len == 3 ) {
        memcpy ( &data_addr, &in_buf[pos], 3 );
        memcpy ( data, &data_addr, 3 );
        pos += 3;
        data_pos = 3;
    } else {
        memcpy ( &data_addr, &in_buf[pos], 2 );
        memcpy ( data, &data_addr, 2 );
        pos += 2;
        data_pos = 2;
    }
    if ( data_addr != 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "the data addr is not 0\n" );
        return 0;
    }
    CP56Time2a _t;
    timer.GetCP56Time ( &_t );
    memcpy ( &data[data_pos], ( char* ) &_t.millisecond, 2 );
    data_pos += 2;
    BuildFrame6816 ( C_CD_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}
/**
*************************************************************************************
*  @brief  解析获得参数规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char ParserGetPara ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    ////////////QPM not in use
    uint8 pos = 0;
    int i = 0;
    uint8 len = 0;
    uint8 ret = 0;
    char data3[512];
    uint16 data3len = 0;
    char data[256];
    char data2[256];
    uint16 data2len = 0;
    uint32 data_addr = 0;
    uint8 para_len = 0;
    BSVSQ vsq;
    uint8 data_pos;
    uint16 data_id = 0;
    uint8 gap = 0;
    ID_MAP *i_map = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    data[0] = vsq.BsVsq.Number;
    data[1] = 0;
    data_pos = 2;
    if ( vsq.BsVsq.SQ == 1 ) {
        if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 5 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }
        para_len = gap;
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb101.GetDataId ( data_addr + i );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );
            data[4+3*i] = para_len;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );
            data2len += para_len;
        }
    } else {
        gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }
            i_map = gb101.GetDataId ( data_addr );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );
            data[4+3*i] = para_len;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb101.member.pro_cfg.datasign_addr_size], para_len );
            pos += gap;
            data2len += para_len;
        }
    }
    memcpy ( in_buf, data, data_pos );
    plb->confirm_flag = 1;
    memcpy ( &in_buf[data_pos], data2, data2len );
    in_len = data_pos + data2len;
    gb101.member.pc_mse_nx = type;
    protocol.member.pc_mse_nx = gb101.member.pc_mse_nx;
    if ( ProGetPara ( in_buf, in_len, data3, &data3len ) ) {
        ret = PrepareProData ( data3, data3len, out_buf, out_len );
	    if ( ret ) {
            SetFuncCode ( S_ACK );
            BuildFrame6816 ( type, data3, &data3len , out_buf, *out_len );
            memcpy ( out_buf, data3, data3len );
            *out_len = data3len;
            return 1;
        }
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "fail on prepareprodata\n");
       	return 0;
    } else {
		PFUNC ( TEM_WARNING, DEBUG_PRO101, "fail on get para\n");
        ret = PrepareProData ( data3, data3len, out_buf, out_len );
        if ( ret ) {
            SetFuncCode ( S_NACK );
            BuildFrame6816 ( type, data3, &data3len , out_buf, *out_len );
            memcpy ( out_buf, data3, data3len );
            *out_len = data3len;
            return 1;
        }
        return 0;
    }
}
/**
*************************************************************************************
*  @brief  解析测量值参数，规一化值规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_P_ME_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  110//>∶＝测量值参数，规一化值
{
    if ( ( in_buf[in_len-3]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara ( P_ME_NA_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara ( P_ME_NA_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
*************************************************************************************
*  @brief  解析测量值参数，标度化值规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_P_ME_NB_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  111//>∶＝测量值参数，标度化值
{
    if ( ( in_buf[in_len-3]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara ( P_ME_NB_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara ( P_ME_NB_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
*************************************************************************************
*  @brief  解析测量值参数，短浮点数规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_P_ME_NC_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  112//>∶＝测量值参数，短浮点数
{
    if ( ( in_buf[in_len-3]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara ( P_ME_NC_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara ( P_ME_NC_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
*************************************************************************************
*  @brief  解析参数激活规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_P_AC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  113//>∶＝参数激活
{
    ////////////QPA not in use
    uint8 pos;
    int i;
    uint8 len;
    char data[256];
    char data2[256];
    uint16 data2len = 0;
    uint32 data_addr = 0;
    uint8 para_len = 0;
    BSVSQ vsq;
    uint8 data_pos;
    uint16 data_id = 0;
    uint8 gap = 0;
    ID_MAP *i_map = 0;
    char data3[256];
    uint16 data3len = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != P_AC_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "type error\n" );
            return 0;
        }
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 ) {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }
    if ( plb->M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord ( CAUSE_Actterm );
    } else {
        SetCuaseWord ( CAUSE_Actcon );
    }
    //Fill Table Header
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );
    vsq.data = in_buf[pos];
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    memcpy ( data3, &in_buf[in_len], in_len - pos - 2 );
    data3len = in_len - pos - 2;
    data[0] = vsq.BsVsq.Number;
    data[1] = 0;
    plb->VSQ_Word.data = vsq.data;
    data_pos = 2;
    if ( vsq.BsVsq.SQ == 1 ) {
        if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }
        para_len = gap;
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb101.GetDataId ( data_addr + i );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );
            data[4+3*i] = 1;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );
            data2len += para_len;
        }
    } else {
        gap = ( in_len - pos - 1 ) / vsq.BsVsq.Number;
        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }
            //fill the data buf header
            i_map = gb101.GetDataId ( data_addr );
            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "data addr error %08x\n", data_addr );
            }
            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );
            data[4+3*i] = 1;
            data_pos += 3;
            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb101.member.pro_cfg.datasign_addr_size], para_len );
            pos += gap;
            data2len += para_len;
        }
    }
    //merge the the transform para buf
    memcpy ( in_buf, data, data_pos );
    memcpy ( &in_buf[data_pos], data2, data2len );
    in_len = data_pos + data2len;
    //do the interface
    if ( ProActPara ( in_buf, in_len, out_buf, out_len ) ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_len=%d\n", in_len );
        SetFuncCode ( S_ACK );
        SetCuaseWord ( CAUSE_Actcon );
    }
    else {
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_len=%d\n", in_len );
        SetFuncCode ( S_NACK );
        SetCuaseWord ( CAUSE_Deactcon );
    }
    BuildFrame6816 ( P_AC_NA_1, out_buf, out_len, data3, data3len );
    return 1;
}
// 文件传输（站特定参数）
/**
*************************************************************************************
*  @brief  解析文件准备就绪规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_F_FR_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  120//>∶＝文件准备就绪
{
    SetCuaseWord ( CAUSE_File );
    SetFuncCode ( S_ACK );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) return 0;
    plb->VSQ_Word.BsVsq.SQ = 0;
    plb->VSQ_Word.BsVsq.Number = 1;
    if ( BuildFrame6816 ( F_FR_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析节准备就绪规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_F_SR_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  121//>∶＝节准备就绪
{
    SetCuaseWord ( CAUSE_File );
    SetFuncCode ( S_ACK );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) return 0;
    plb->VSQ_Word.BsVsq.SQ = 0;
    plb->VSQ_Word.BsVsq.Number = 1;
    if ( BuildFrame6816 ( F_SR_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  根据文件的ID创建文件的规约信息
*  @param[in] uint16 file_name
*  @param[out] FILE_INFO *fi
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char CreateFileInfo ( uint16 file_name, FILE_INFO *fi )
{
    uint8 tmp_cnt = 0;
    char data[32];
    uint8 datalen = 0;
    char outbuf[32];
    uint16 outlen = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "file_name=%04x\n", file_name );
    data[datalen++] = 1;
    data[datalen++] = 0;
    memset ( ( char* ) fi, 0, sizeof ( FILE_INFO ) );
    memcpy ( &data[datalen], ( char* ) &file_name, 2 );
    datalen += 2;
    data[datalen++] = 4;//4个字节存放长度
    ProReadFileInfo ( data, datalen, outbuf, &outlen );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "out_len=%d\n", outlen );
    memcpy ( ( char* ) &fi->file_len, &outbuf[5], 4 );
    if ( fi->file_len != 0 ) {
        fi->file_name = file_name;//记录文件名
        fi->segment_size = MAX_SEGSIZE;
        fi->sect_segment_num = SECT_MAXSEGNUM;
        tmp_cnt = fi->file_len / fi->segment_size;
        fi->file_seg_num = tmp_cnt;
        //计算文件的总段数，及最后段的长度
        fi->last_segment_size = MAX_SEGSIZE;
        tmp_cnt = fi->file_len % MAX_SEGSIZE;
        if ( tmp_cnt != 0 ) {
            fi->file_seg_num += 1;
            fi->last_segment_size = tmp_cnt;
            //fi->file_len += ( MAX_SEGSIZE - tmp_cnt );
        }
        //计算文件的节数
        tmp_cnt = fi->file_seg_num;
        while ( tmp_cnt - fi->sect_segment_num >= 0 ) {
            fi->file_section_num++;
            tmp_cnt -= fi->sect_segment_num;
        }
        //计算最后的节中的段数
        if ( tmp_cnt > 0 ) {
            fi->file_section_num += 1;
            fi->last_sect_seg_num = tmp_cnt;
        }
        /*
        PFUNC ( TEM_INFO, DEBUG_PRO101, "file_name=%04x\nfile_len=%d\n_sec_seg_num=%d\nfile_sec_num=%d\nseg_size=%d\nfile_seg_num=%d\n", \
                fi->file_name,\
                fi->file_len,\
                fi->sect_segment_num,\
                fi->file_section_num,\
                fi->segment_size,\
                fi->file_seg_num );
		*/                
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析召唤目录，选择文件，召唤文件召唤节规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_F_SC_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  122//>∶＝召唤目录，选择文件，召唤文件召唤节
{
    BSCAUSE Cause;
    SCQWORD scw;
    SRQWORD srw;
    FRQWORD fw;
    uint8 pos = 0;
    uint8 len = 0;
    uint32 type = 0;
    uint16 file_name = 0;
    PRO_LOGIC_BUF *plb = 0;
    char data[256];
    char segment_buf[256];
    uint16 segment_len = 0;
    uint8 datalen = 0;
    char file_para[64];
    uint16 file_para_len = 0;
    uint8 ret = 0;
    uint32 section_size = 0;
    uint32 offset = 0;
    BUF_ADDR_REGIST_TABLE *bart = 0;
    int i;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "inlen=%d\n", in_len );
    bart = protocol.SearchBufAddrRegistTable ( gb101.member.CurPortId );
    if ( bart == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no send buf for port%d sending ,type=%d\n", gb101.member.CurPortId , F_SC_NA_1 );
        return 0;
    }
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );//选择对应传输口的逻辑缓存
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no config logic buf for port%d paraser ,type=%d\n", gb101.member.CurPortId , F_SC_NA_1 );
        return 0;
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    if ( len == 2 )
        Cause.data = * ( uint16* ) & in_buf[pos];
    else
        Cause.data = in_buf[pos];
    scw.data = in_buf[in_len-3];//品质描述词，倒数第三位
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len );
    memcpy ( ( char* ) &type, &in_buf[pos], len );
    plb->M_type = type;//保持文件传输标识
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    memcpy ( data, &in_buf[pos], len );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "data_addr=%02x%02x\n", data[1], data[0] );
    pos += len;
    datalen += len;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "cause=%d\n", Cause.BsCause.code );
    if ( Cause.BsCause.code == CAUSE_Req ) {//目录用
        return 0; //SendDirectory();召唤目录不响应
    } else if ( Cause.BsCause.code == CAUSE_File ) { //召唤文件
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "scqword.ui=%d\n", scw.ScqWord.UI1 );
        switch ( scw.ScqWord.UI1 ) { //根据召唤限定词执行相应的文件传输
        case SelectFile:
            file_name = in_buf[pos] + in_buf[pos+1] * 256;//获取文件名
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "file_name=%04x\n", file_name );
            memcpy ( &data[datalen], &in_buf[pos], 2 );
            datalen += 2;
            //get file info to file the out buf
            if ( CreateFileInfo ( file_name, &plb->FileInfo ) != 0 ) { //bug0024
                fw.FrqWord.BS = 0;//文件准备就绪限定词，肯定确认
            } else {
                fw.FrqWord.BS = 1;
            }
            //prepare ack data
            memcpy ( &data[datalen], &plb->FileInfo.file_len, 3 );
            datalen += 3;
            data[datalen] = fw.data;
            datalen += 1;
            ret = Pack_F_FR_NA_1 ( data, datalen, out_buf, out_len );
            plb->file_section_cnt = 1;
            plb->file_segment_cnt = 0;
            plb->FileCheckSum = 0;
            plb->SectionCheckSum = 0;
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "Select file %d\n", SelectFile );
            return ret;
            break;
        case AskFile://召唤文件
            if ( plb->FileInfo.file_name != ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1]*256 ) { //选择的文件名和请求的文件名不同,否定回答            SRQ_Word.SrqWord.BS = 1;
                memcpy ( &data[datalen], &in_buf[pos], 2 );
                datalen += 2;
                memset ( &data[datalen], 0, 4 );
                srw.SrqWord.BS = 1;
                datalen += 4;
                data[datalen] = srw.data;
                datalen += 1;
            } else {
                srw.SrqWord.BS = 0;
                memcpy ( &data[datalen], &in_buf[pos], 2 );
                datalen += 2;
                section_size = plb->FileInfo.sect_segment_num * plb->FileInfo.segment_size;
                data[datalen] = plb->FileInfo.file_section_num;
                datalen += 1;
                memcpy ( &data[datalen], &section_size, 3 );
                datalen += 3;
                data[datalen] = srw.data;
                datalen += 1;
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "ask file %d\n", AskFile );
            ret = Pack_F_SR_NA_1 ( data, datalen, out_buf, out_len );
            return ret;
            break;
        case DeactFile:
            plb->file_section_cnt = 1;
            plb->file_segment_cnt = 0;
            plb->FileCheckSum = 0;
            plb->SectionCheckSum = 0;
            PFUNC ( TEM_INFO, DEBUG_PRO101, "DeactFile %d\n", DeactFile );
            return 0;
        case DeleteFile:
            PFUNC ( TEM_INFO, DEBUG_PRO101, "DeleteFile %d\n", DeleteFile );
            break;
        case SelectSection:
            PFUNC ( TEM_INFO, DEBUG_PRO101, "SelectSection %d\n", SelectSection );
            break;
        case AskSection://召唤节
            for ( i = 0;i < plb->FileInfo.sect_segment_num;i++ ) {
                datalen = len;
                memcpy ( &data[datalen], ( char* ) & plb->FileInfo.file_name, 2 );
                // * ( uint16* ) &data[datalen] = plb->FileInfo.file_name;
                datalen += 2;
                data[datalen++] = plb->file_section_cnt;
                //build read file para
                file_para[file_para_len] = 1;
                file_para_len += 1;
                file_para[file_para_len] = 0;
                file_para_len += 1;
                memcpy ( &file_para[file_para_len], ( char* ) &plb->FileInfo.file_name, 2 );
                file_para_len += 2;
                file_para[file_para_len++] = 5;
                offset = ( i + plb->FileInfo.sect_segment_num * ( plb->file_section_cnt - 1 ) )
                         * plb->FileInfo.segment_size;
                memcpy ( &file_para[file_para_len], ( char* ) &offset, 4 );
                file_para_len += 4;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "file_name =%04x,off set= %d\n", plb->FileInfo.file_name, offset );
                ////////////////////////////////////////////////////
                if ( ( i + ( plb->file_section_cnt - 1 ) *plb->FileInfo.sect_segment_num ) < plb->FileInfo.file_seg_num - 1 ) {
                    file_para[file_para_len++] = plb->FileInfo.segment_size;
                    data[datalen++] = plb->FileInfo.segment_size;
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "segment len= %d\n", plb->FileInfo.segment_size );
                } else if ( ( i + ( plb->file_section_cnt - 1 ) *plb->FileInfo.sect_segment_num ) > plb->FileInfo.file_seg_num - 1 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "%x file send over\n", plb->FileInfo.file_name );
                    break;
                } else if ( ( i + ( plb->file_section_cnt - 1 ) *plb->FileInfo.sect_segment_num ) == plb->FileInfo.file_seg_num - 1 ) {
                    file_para[file_para_len++] = plb->FileInfo.last_segment_size;
                    data[datalen++] = plb->FileInfo.last_segment_size;
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "segment len= %d\n", plb->FileInfo.last_segment_size );
                }
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "send segment =%d,datalen=%d\n", segment_len , datalen );
                if ( ProReadFile ( file_para, file_para_len, segment_buf, &segment_len ) ) {
                    if ( segment_len > 238 ) {
                        return 0;
                    }
                    memcpy ( &data[datalen], segment_buf, segment_len );
                    datalen += segment_len;
                } else {
                    return 0;
                }
                file_para_len = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "datalen=%d,seg_len=%d\n", datalen, segment_len );
                plb->SectionCheckSum += GetSum ( ( uint8* ) segment_buf, segment_len );
                ret = Pack_F_SG_NA_1 ( data, datalen, out_buf, out_len );
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "////////++++++++++++++send sg datalen =%d out_len=%d\n", datalen , *out_len );
                if ( ret )  {
                    protocol.AddSendBuf ( bart->paddr, out_buf, *out_len );
                    protocol.CopySendFrame ( bart->paddr, out_buf, *out_len );
                }
                mdelay ( 100 );//间隔50ms发送1段
            }
            //ack last segment
            datalen = len;
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "++++++++++++++send last segment =%d datalen=%d\n", segment_len , datalen );
            memcpy ( &data[datalen], ( char* ) &plb->FileInfo.file_name, 2 );
            datalen += 2;
            data[datalen++] = plb->file_section_cnt;
            data[datalen++] = 4;//最后的段
            data[datalen++] = plb->SectionCheckSum;
            plb->SectionCheckSum = 0;
            ret = Pack_F_LS_NA_1 ( data, datalen, out_buf, out_len );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "////////////////send last datalen =%d out_len=%d\n", datalen , *out_len );
            if ( ret )  {
                protocol.AddSendBuf ( bart->paddr, out_buf, *out_len );
                protocol.CopySendFrame ( bart->paddr, out_buf, *out_len );
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "AskSection %d\n", AskSection );
            return 0;
            break;
        case DeactSection:
            PFUNC ( TEM_INFO, DEBUG_PRO101, "DeactSection %d\n", DeactSection );
            break;
        default:
            PFUNC ( TEM_INFO, DEBUG_PRO101, "no option %d\n", 0 );
            break;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  打包最后的节，最后的段规约帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_F_LS_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  123//>∶＝最后的节，最后的段
{
    SetCuaseWord ( CAUSE_File );
    SetFuncCode ( S_ACK );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) return 0;
    plb->VSQ_Word.BsVsq.SQ = 0;
    plb->VSQ_Word.BsVsq.Number = 1;
    if ( BuildFrame6816 ( F_LS_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析认可文件，认可节规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Parser_F_AF_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  124//>∶＝认可文件，认可节
{
    uint8 ret = 0;
    uint8 pos = 0, len = 0;
    uint16 file_name;
    char data[32];
    uint8 datalen = 0;
    AFQWORD afw;
    SRQWORD srw;
    PRO_LOGIC_BUF *plb = 0;
    uint32 datasign = 0;
    afw.data = 0;
    srw.data = 0;
    uint32 section_size = 0;
    uint8 sec_name = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser <port_id=%d>\n", gb101.member.CurPortId );
        return 0;
    }
    gb101.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
    memcpy ( ( char* ) &datasign, &in_buf[pos], len );
    memcpy ( data, &in_buf[pos], len );//信息体地址添加到数据包中
    pos += len;
    datalen += len;
    memcpy ( ( char* ) &file_name, &in_buf[pos], 2 );
    memcpy ( &data[datalen], &in_buf[pos], 2 );//文件名添加到数据包中
    datalen += 2;
    pos += 2;
    sec_name = in_buf[pos];
    pos += 1;
    /*
    if(sec_name!=plb->file_section_cnt){
     PFUNC ( TEM_WARNING,DEBUG_PRO101, "section name error sn=%d,tn=%d<port_id=%d>\n",sec_name,plb->file_section_cnt, gb101.CurPortId );
     return 0;
     }
     */
    afw.data = in_buf[in_len-3];
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "afq.data=%d\n", afw.data );
    switch ( afw.AfqWord .UI1 ) {
    case 1://文件肯定确认
        plb->file_section_cnt = 1;
        plb->file_segment_cnt = 0;
        //删除文件
        //传输目录
        plb->FileCheckSum = 0;
        plb->SectionCheckSum = 0;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "file confirm<port_id=%d>\n", gb101.member.CurPortId );
        return 0;
        //      break;
    case 2://文件否定确认，不重发
        plb->file_section_cnt = 1;
        plb->file_segment_cnt = 0;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, " file nack no resend<port_id=%d>\n", gb101.member.CurPortId );
        break;
    case 3://节肯定确认
        plb->FileCheckSum += plb->SectionCheckSum;
        plb->SectionCheckSum = 0;
        /*
        PFUNC ( TEM_INFO, DEBUG_PRO101, "station confirm ack section ok FileCheckSum=%x,SectionCheckSum=%x\n",\
                plb->FileCheckSum, plb->SectionCheckSum );
		*/                
        if ( plb->file_section_cnt >= plb->FileInfo.file_section_num ) { //节发送完，文件发送结束
            plb->file_segment_cnt = 0;
            data[datalen++] = plb->file_section_cnt;
            data[datalen++] = 2;// LSQ = 2;//最后的节
            data[datalen++] = plb->FileCheckSum;
            ret = Pack_F_LS_NA_1 ( data, datalen, out_buf, out_len );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "section over\n" );
        } else {
            plb->file_section_cnt++;//节序号++，起始序号为1
            srw.SrqWord.BS = 0;
            section_size = plb->FileInfo.sect_segment_num * plb->FileInfo.segment_size;
            data[datalen] = plb->file_section_cnt;
            datalen += 1;
            memcpy ( &data[datalen], &section_size, 3 );
            datalen += 3;
            data[datalen] = srw.data;
            datalen += 1;
            ret = Pack_F_SR_NA_1 ( data, datalen, out_buf, out_len );
        }
        plb->file_segment_cnt = 0;
        return ret;
        break;
    case 4://节否定确认,重发
        if ( afw.AfqWord.UI2 == 2 ) { //如果是校验出错，重发
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "station Ack Section Checksun Error <section:%d\n", plb->file_section_cnt );
            plb->file_section_cnt -= 1;
        }
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "section nack and resend <port_id=%d>\n", gb101.member.CurPortId );
        plb->file_segment_cnt = 0;
        break;
    default:
        break;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  段组帧函数
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_F_SG_NA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  125//>∶＝段
{
    SetCuaseWord ( CAUSE_File );
    SetFuncCode ( S_ACK );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) return 0;
    plb->VSQ_Word.BsVsq.SQ = 0;
    plb->VSQ_Word.BsVsq.Number = 1;
    if ( BuildFrame6816 ( F_SG_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  目录响应组帧
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char Pack_F_DR_TA_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )   //  126//>∶＝目录
{
    //暂不实现
    SetCuaseWord ( CAUSE_Req );
    SetFuncCode ( S_ACK );
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    //信息对象地址 2或三
    //1~n个文件////////////////
    //文件名或者子目录名
    //文件长度
    //SOF文件状态
    //7字节的CP56Time2a时间
    //////////////////////////////
    //gb101.member.pro_logic_buf[0].SOF_Word.SofWord.FA = 0;//0-文件等待传输，1-文件传输已激活
    //gb101.member.pro_logic_buf[0].SOF_Word.SofWord.LFD = 0;//0-后面还有目录文件，1-最后的目录文件
    //gb101.member.pro_logic_buf[0].SOF_Word.SofWord._FOR = 0;//0-定义文件名，1-定义子目录名
    //gb101.member.pro_logic_buf[0].SOF_Word.SofWord.STATUS = 0;//缺省为0
    if ( plb == 0 ) return 0;
    plb->VSQ_Word.BsVsq.SQ = 1;
    plb->VSQ_Word.BsVsq.Number = 64;
    if ( BuildFrame6816 ( F_DR_TA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }
    return 0;
}
static char Parser_P_XX_XX_1 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    if ( ( in_buf[in_len-3]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara ( P_XX_XX_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara ( P_XX_XX_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}
/**
*************************************************************************************
*  @brief  组帧和解析函数初始化列表
*  @param
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::InitFunc ( void )
{
//   控制方向过程信息命令//////////////////////////////////////////
    member.parser_func[0].type = C_SC_NA_1;
    member.parser_func[0].fp = Parser_C_SC_NA_1;
    member.parser_func[0].func_id = GET_DATA;
    member.parser_func[0].pcb = 0;
    member.parser_func[1].type = C_DC_NA_1;
    member.parser_func[1].fp = Parser_C_DC_NA_1;
    member.parser_func[1].func_id = YK_APP;
    member.parser_func[1].pcb = 0;
    member.parser_func[2].type = C_RC_NA_1;
    member.parser_func[2].fp = Parser_C_RC_NA_1;
    member.parser_func[2].func_id = YK_APP;
    member.parser_func[2].pcb = 0;
    member.parser_func[3].type = C_SE_NA_1;
    member.parser_func[3].fp = Parser_C_SE_NA_1;
    member.parser_func[3].func_id = ADJUST_APP;
    member.parser_func[3].pcb = 0;
    member.parser_func[4].type = C_SE_NB_1;
    member.parser_func[4].fp = Parser_C_SE_NB_1;
    member.parser_func[4].func_id = YK_APP;
    member.parser_func[4].pcb = 0;
    member.parser_func[5].type = C_SE_NC_1;
    member.parser_func[5].fp = Parser_C_SE_NC_1;
    member.parser_func[5].func_id = SET_PARA;
    member.parser_func[5].pcb = 0;
    member.parser_func[6].type = C_BO_NC_1;
    member.parser_func[6].fp = Parser_C_BO_NC_1;
    member.parser_func[6].func_id = SET_PARA;
    member.parser_func[6].pcb = 0;
//   在控制方向的系统命令（站特定参数)
    member.parser_func[7].type = C_IC_NA_1;
    member.parser_func[7].fp = Parser_C_IC_NA_1;
    member.parser_func[7].func_id = GET_DATA;
    member.parser_func[7].pcb = 0;
    member.parser_func[8].type = C_CI_NA_1;
    member.parser_func[8].fp = Parser_C_CI_NA_1;
    member.parser_func[8].func_id = GET_DATA;
    member.parser_func[8].pcb = 0;
    member.parser_func[9].type = C_RD_NA_1;
    member.parser_func[9].fp = Parser_C_RD_NA_1;
    member.parser_func[9].func_id = GET_DATA;
    member.parser_func[9].pcb = 0;
    member.parser_func[10].type = C_TS_NA_1;
    member.parser_func[10].fp = Parser_C_TS_NA_1;
    member.parser_func[10].func_id = 0;
    member.parser_func[10].pcb = 0;
    member.parser_func[11].type = C_RP_NA_1;
    member.parser_func[11].fp = Parser_C_RP_NA_1;
    member.parser_func[11].func_id = RESET_LINK;
    member.parser_func[11].pcb = 0;
    member.parser_func[12].type = C_CD_NA_1;
    member.parser_func[12].fp = Parser_C_CD_NA_1;
    member.parser_func[12].func_id = GET_TIME;
    member.parser_func[12].pcb = 0;
// 在控制方向的参数命令（站特定参数)////////////////////////////////////
    member.parser_func[13].type = P_ME_NA_1;
    member.parser_func[13].fp = Parser_P_ME_NA_1;
    member.parser_func[13].func_id = SET_PARA;
    member.parser_func[13].pcb = 0;
    member.parser_func[14].type = P_ME_NB_1;
    member.parser_func[14].fp = Parser_P_ME_NB_1;
    member.parser_func[14].func_id = SET_PARA;
    member.parser_func[14].pcb = 0;
    member.parser_func[15].type = P_ME_NC_1;
    member.parser_func[15].fp = Parser_P_ME_NC_1;
    member.parser_func[15].func_id = SET_PARA;
    member.parser_func[15].pcb = 0;
    member.parser_func[16].type = P_AC_NA_1;
    member.parser_func[16].fp = Parser_P_AC_NA_1;
    member.parser_func[16].func_id = ACTIVE_PARA;
    member.parser_func[16].pcb = 0;
// 文件传输（站特定参数)//////////////////////////////////////
    member.parser_func[17].type = F_SC_NA_1;
    member.parser_func[17].fp = Parser_F_SC_NA_1;
    member.parser_func[17].func_id = READ_FILE_INFO;
    member.parser_func[17].pcb = 0;
    member.parser_func[18].type = F_AF_NA_1;
    member.parser_func[18].fp = Parser_F_AF_NA_1;
    member.parser_func[18].func_id = READ_FILE;
    member.parser_func[18].pcb = 0;
    member.parser_func[19].type = C_CS_NA_1;
    member.parser_func[19].fp = Parser_C_CS_NA_1;
    member.parser_func[19].func_id = SET_TIME;
    member.parser_func[19].pcb = 0;
//////////////终端参数
    member.parser_func[20].type = P_XX_XX_1;
    member.parser_func[20].fp = Parser_P_XX_XX_1;
    member.parser_func[20].func_id = 0;
    member.parser_func[20].pcb = 0;
//监视方向////////////////////////////////////////////
    member.pack_func[0].type = M_SP_NA_1;
    member.pack_func[0].fp = Pack_M_SP_NA_1;
    member.pack_func[1].type = M_SP_TA_1;
    member.pack_func[1].fp = Pack_M_SP_TA_1;
    member.pack_func[2].type = M_DP_NA_1;
    member.pack_func[2].fp = Pack_M_DP_NA_1;
    member.pack_func[3].type = M_DP_TA_1;
    member.pack_func[3].fp = Pack_M_DP_TA_1;
    member.pack_func[4].type = M_ME_NA_1;
    member.pack_func[4].fp = Pack_M_ME_NA_1;
    member.pack_func[5].type = M_ME_TA_1;
    member.pack_func[5].fp = Pack_M_ME_TA_1;
    member.pack_func[6].type = M_ME_NB_1;
    member.pack_func[6].fp = Pack_M_ME_NB_1;
    member.pack_func[7].type = M_ME_TB_1;
    member.pack_func[7].fp = Pack_M_ME_TB_1;
    member.pack_func[8].type = M_ME_NC_1;
    member.pack_func[8].fp = Pack_M_ME_NC_1;
    member.pack_func[9].type = M_ME_TC_1;
    member.pack_func[9].fp = Pack_M_ME_TC_1;
    member.pack_func[10].type = M_SP_TB_1;
    member.pack_func[10].fp = Pack_M_SP_TB_1;
    member.pack_func[11].type = M_DP_TB_1;
    member.pack_func[11].fp = Pack_M_DP_TB_1;
    member.pack_func[12].type = M_ME_TD_1;
    member.pack_func[12].fp = Pack_M_ME_TD_1;
    member.pack_func[13].type = M_ME_TE_1;
    member.pack_func[13].fp = Pack_M_ME_TE_1;
    member.pack_func[14].type = F_FR_NA_1;
    member.pack_func[14].fp = Pack_F_FR_NA_1;
    member.pack_func[15].type = F_SR_NA_1;
    member.pack_func[15].fp = Pack_F_SR_NA_1;
    member.pack_func[16].type = F_LS_NA_1;
    member.pack_func[16].fp = Pack_F_LS_NA_1;
    member.pack_func[17].type = F_SG_NA_1;
    member.pack_func[17].fp = Pack_F_SG_NA_1;
    member.pack_func[18].type = F_DR_TA_1;
    member.pack_func[18].fp = Pack_F_DR_TA_1;
    member.pack_func[19].type = M_BO_NA_1;
    member.pack_func[19].fp = Pack_M_BO_NA_1;
    return 1;
}
/**
*************************************************************************************
*  @brief  设置发送控制字功能码
*  @param[in] uint8 fc
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char SetFuncCode ( uint8 fc )
{
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        plb->CtrlWordFromSelf.cword.DIR = 1;
        plb->CtrlWordFromSelf.cword.FuncCode = fc;
        plb->CtrlWordFromSelf.cword.PRM = 0;
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  设置发送控制字ACD位与DFC位
*  @param[in] uint8 acd
*  @param[in] uint8 dfc
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char SetAcdDfc ( uint8 acd , uint8 dfc )
{
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        plb->CtrlWordFromSelf.cword.ACD = acd;
        plb->CtrlWordFromSelf.cword.DFC = dfc;
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  设置发送原因关键字
*  @param[in] uint8 cause
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char SetCuaseWord ( uint8 cause )
{
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        plb->SendCause.BsCause.code = cause;
        plb->SendCause.BsCause.P_N = 0;
        plb->SendCause.BsCause.T = 0;
        plb->SendCause.BsCause.res = 0;
        return 1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  检查FCB位并进行更新
*  @param[in] char ctrlword
*  @return  成功返回1，否则返回0，返回1代表不需要进行重发上一帧
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char CheckFCB ( char ctrlword )
{
    MCTRLWORD CWord;
    CWord.data = ctrlword;
    PRO_LOGIC_BUF *plb = 0;
    static uint8 fcb_no_change_cnt = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    PRO_TIMEOUT *to = 0;
    to = gb101.SelectTimeOut ( gb101.member.CurPortId );
    if ( to == 0 ) return 0;
    if ( plb != 0 ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "CheckFCB fcb=%d,fcv=%d,dir=%d,prm=%d,fc=%d\n", CWord.cword.FCB, CWord.cword.FCV, CWord.cword.DIR, CWord.cword.PRM, CWord.cword.FuncCode );
        if ( CWord.cword.FCV == 1 ) {
            if ( plb->CtrlWordFromMaster.cword.FCB == CWord.cword.FCB ) { //resend last frame
                fcb_no_change_cnt++;
                if ( fcb_no_change_cnt > 3 ) {
                    to->link_flag = 1;
                    to->link = 0;
                    plb->CtrlWordFromMaster.cword.FCB = 1;
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "the FCB no change 3 times i will reset the pro link\n" );
                }
                return 0;
            } else {
                plb->CtrlWordFromMaster.data = CWord.data;
                fcb_no_change_cnt = 0;
                return 1;
            }
        } else {
            plb->CtrlWordFromMaster.data = CWord.data;
            fcb_no_change_cnt = 0;
            return 1;
        }
    }
    return 0;
}
//事件数据扫描，并通知做相应处理
/**
*************************************************************************************
*  @brief  扫描对应端口的事件缓存
*  @param[in] uint16 port_id
*  @return  成功返回1或2，否则返回0，返回1代表成功有1级数据事件产生，2代表有2级数据事件产生
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::ScanSpontBuf ( uint16 port_id )
{
    int i;
    member.CurPortId = port_id;
    for ( i = 0;i < MAX_C1_EVENT_NUM;i++ ) {
        if ( member.c1_data_buf.c1_data[i].port_id == port_id ) {
            if ( member.c1_data_buf.c1_data[i].read_flag == EVENT_NOREAD ) {
            	//PFUNC ( TEM_INFO, DEBUG_PRO101,"datatype:0x%x\n",member.c1_data_buf.c1_data[i].data_type&0x0f);
                SetFuncCode ( S_ACK_User_Data );
                SetAcdDfc ( 1, 0 );
                AddServiceBuf ( port_id, CLASS1, member.c1_data_buf.c1_data[i].data_type&0x0f );
                return CLASS1;
            }
        }
    }
    for ( i = 0;i < MAX_C2_EVENT_NUM;i++ ) {
        if ( member.c2_data_buf.c2_data[i].port_id == port_id ) {
            if ( member.c2_data_buf.c2_data[i].read_flag == EVENT_NOREAD ) {
                AddServiceBuf ( port_id, CLASS2, member.c2_data_buf.c2_data[i].data_type&0x0f );
                return CLASS2;
            }
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  对应端口接收数据缓存捡帧，对帧做初步的判断，并将有效帧放入对应的帧缓存。
*  @param[in] BUF_MUX *buf
*  @param[in] uint16 port_id
*  @return  成功返回1，否则返回0，返回1代表成功收到一帧
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint16 Gb101:: DealComBuf ( BUF_MUX *buf, uint16 port_id )
{
    PRO_TIMEOUT *tab = 0;
    uint16 bytes = 0;
    tab = SelectTimeOut ( port_id );
    if ( tab == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "port id error %d,please check the port is setting to pro\n", port_id );
        return 0;
    }
    while ( buf->recv.out_cnt != buf->recv.in_cnt ) {
        if ( tab->begin68 == 0 && tab->begin10 == 0 ) {
            if ( buf->recv.buf[buf->recv.out_cnt] == 0x68 ) {
                tab->begin68 = 1;
                buf->frame.in_cnt = 0;
            } else if ( buf->recv.buf[buf->recv.out_cnt] == 0x10 ) {
                tab->begin10 = 1;
                buf->frame.in_cnt = 0;
            }
        }
        if ( tab->begin68 == 1 ) {
            buf->frame.buf[buf->frame.in_cnt++] = buf->recv.buf[buf->recv.out_cnt];
           //PFUNC(TEM_INFO,DEBUG_PRO101,"frame byte %02x begin68=%d,begin10=%d\n",buf->recv.buf[buf->recv.out_cnt],tab->begin68,tab->begin10);
            if ( buf->frame.in_cnt == 4 && buf->frame.buf[3] != 0x68 ) {
                tab->begin68 = 0;
                buf->frame.in_cnt = 0;
            }
            //PFUNC(TEM_INFO,DEBUG_PRO101,"in_cnt=%d,expect_cnt=%d\n",buf->frame.in_cnt,(buf->frame.buf[2]+6));
            if ( buf->frame.in_cnt == ( ( uint8 ) ( buf->frame.buf[2] ) + 6 ) ) {
                if ( buf->recv.buf[buf->recv.out_cnt] == 0x16 ) {
                    tab->begin68 = 0;
                    memcpy ( member.ProtBuf.parser_buf, ( char* ) buf->frame.buf, buf->frame.in_cnt );
                    member.ProtBuf.parser_frame_len = buf->frame.in_cnt;
                    member.ProtBuf.valid_flag = 1;
                    buf->frame.in_cnt = 0;
                }
            }
            if ( buf->frame.in_cnt > 256 ) {
                tab->begin68 = 0;
                buf->frame.in_cnt = 0;
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "frame len over flow 256 \n" );
            }
        } else if ( tab->begin10 == 1 ) {
            buf->frame.buf[buf->frame.in_cnt++] = buf->recv.buf[buf->recv.out_cnt];
            if ( buf->frame.in_cnt == ( 4 + member.pro_cfg.link_addr_size ) ) {
                if ( buf->recv.buf[buf->recv.out_cnt] == 0x16 ) {
                    tab->begin10 = 0;
                    memcpy ( member.ProtBuf.parser_buf, ( char* ) buf->frame.buf, buf->frame.in_cnt );
                    member.ProtBuf.parser_frame_len = buf->frame.in_cnt;
                    member.ProtBuf.valid_flag = 1;
                    buf->frame.in_cnt = 0;
                }
            }
            if ( buf->frame.in_cnt > ( 4 + member.pro_cfg.link_addr_size ) ) {
                tab->begin10 = 0;
                buf->frame.in_cnt = 0;
                PFUNC ( TEM_WARNING, DEBUG_PRO101, "frame len over flow %d\n", 4 + member.pro_cfg.link_addr_size );
            }
        }
        buf->recv.out_cnt++;
        if ( buf->recv.out_cnt >= BUFFER_SIZE ) {
            buf->recv.out_cnt = 0;
        }
        bytes++;
        if ( bytes > 255 ) {
            bytes = 0;
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "break cycle %d\n", bytes );
            break;
        }
        if ( ( tab->begin68 == 0 ) && ( tab->begin10 == 0 ) ) { //其他情况，由超时处理.
            tab->frame = 0;
            tab->frame_flag = 0;
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "no wait\n" );
        } else {
            tab->frame_flag = 1;
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  规约各种超时处理函数
*  @param[in] BUF_MUX *buf
*  @param[in] uint16 port_id
*  @return  始终返回1
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::DealTimeOut ( BUF_MUX *buf, uint16 port_id )
{
    int i;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( port_id );
    DataType3 *ykseltime;
    Data < BaseDataType > data;
    user_data.GetData ( PARA_5091, &data );
    ykseltime = ( DataType3* ) data.Data;
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to deal pack frame\n" );
        return 0;
    }
    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.timeout101[i].en == 1 ) {
            if ( member.timeout101[i].frame_flag == 1 ) {//if the frame is timeout ,the process is here
                member.timeout101[i].frame++;
                if ( member.timeout101[i].frame > 3 ) {
                    member.timeout101[i].begin10 = 0;
                    member.timeout101[i].begin68 = 0;
                    member.timeout101[i].frame = 0;
                    member.timeout101[i].frame_flag = 0;
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "frame timeout\n" );
                }
            }
            if ( member.timeout101[i].byte_flag == 1 ) {
                member.timeout101[i].byte_flag = 0;
            }
            if ( member.timeout101[i].link_flag == 1 ) {//if the link flag is set ,after 5s ,send the reset link cmd to station ,and reset self link
                member.timeout101[i].link++;
                if ( member.timeout101[i].link >= 5 ) {
                    member.timeout101[i].rst_link = 1;
                    int j;
                    for ( j = 0;j < MAX_PORT;j++ ) {
                        if ( port_id == member.pro_logic_buf[j].port_id ) {
                            member.pro_logic_buf[j].CtrlWordFromMaster.cword.FCB = 0;
                        }
                    }
                    SetFuncCode ( M_Reset_Remote_Link );
                    plb->CtrlWordFromSelf.cword.PRM = 1;
                    BuildFrame1016 ( member.ProtBuf.pack_buf, &member.ProtBuf.pack_frame_len );
                    protocol.AddSendBuf ( buf, member.ProtBuf.pack_buf, member.ProtBuf.pack_frame_len );
                    sleep ( 2 );
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "101 reset link %d\n", port_id );
                    ProRstLink ( ( char* ) &port_id, 2, ( char* ) &port_id, &member.ProtBuf.pack_frame_len );
                    member.timeout101[i].link = 0;
                    member.timeout101[i].link_flag = 0;
                }
            }
            if ( member.timeout101[i].resend_flag == 1 && member.pro_cfg.trans_style == EQ_TRANS ) {//if equal trans is set ,after 5s if need resend ,send the last frame
                member.timeout101[i].resend++;
                if ( member.timeout101[i].resend >= member.pro_cfg.resend_times ) {
                    member.timeout101[i].resend = 0;
                    member.timeout101[i].resend_cnt++;
                    if ( member.timeout101[i].resend_cnt >= 3 ) {
                        member.timeout101[i].test_flag = 1;
                        member.timeout101[i].resend_flag = 0;
                        member.timeout101[i].resend_cnt = 0;
                    } else {
                        protocol.AddSendBuf ( buf, buf->send_frame_copy.buf, buf->send_frame_copy.in_cnt );
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no confirm  RESEND LAST FRAME\n" );
                    }
                }
            }
            if ( member.timeout101[i].test_flag == 1 ) {//delay 10s send request link status
                member.timeout101[i].test++;
                if ( member.timeout101[i].test >= 10 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "send test frame\n" );
                    member.timeout101[i].test_cnt++;
                    member.timeout101[i].test = 0;
                    SetFuncCode ( M_Request_Link_St );
                    plb->CtrlWordFromSelf.cword.PRM = 1;
                    BuildFrame1016 ( member.ProtBuf.pack_buf, &member.ProtBuf.pack_frame_len );
                    protocol.AddSendBuf ( buf, member.ProtBuf.pack_buf, member.ProtBuf.pack_frame_len );
                    if ( member.timeout101[i].test_cnt >= 3 ) {//send 3 times ,if no ack from station ,change to reset link status
                        member.timeout101[i].test_cnt = 0;
                        member.timeout101[i].link_flag = 1;
                        member.timeout101[i].test_flag = 0;
                    }
                }
            }
            if ( member.timeout101[i].yk_en == 1 ) {//delay 10s send request link status
                member.timeout101[i].yk_cnt++;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "yk cnt:%d timeout:%d\n",member.timeout101[i].yk_cnt,ykseltime->Data );
                if ( member.timeout101[i].yk_cnt >= ykseltime->Data ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "yk timeout\n" );
                    member.timeout101[i].yk_cnt = 0;
                    member.timeout101[i].yk_en = 0;
                    member.timeout101[i].yk_timeout_flag = 1;
                }
            }
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  设置初始化原因
*  @param[in] char coi
*  @return  void
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Gb101::SetCauseOfInit ( char coi )
{
    COIDATA coidata;
    SYSTIME _t;
    char time[7];
    PRO_DATA_CLASS_CFG *pdcc = 0;
    coidata.data = 0;
    coidata.CoiData.UI = coi;
    timer.GetLocalTime ( &_t );
    time[0] = ( char ) _t.sec;
    time[1] = ( char ) _t.min;
    time[2] = ( char ) _t.hour;
    time[3] = ( char ) _t.mday;
    time[4] = ( char ) _t.mon;
    time[5] = ( char ) _t.year;
    time[6] = ( char ) _t.year >> 8;
    pdcc = gb101.SearchClassTable ( INIT_OVER_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            SetC1Event ( 0, ( char* ) &coidata.data, 1, INIT_OVER_DATA, time );
        } else {
            SetC2Event ( 0, ( char* ) &coidata.data, 1, INIT_OVER_DATA, time );
        }
    }
}
/**
*************************************************************************************
*  @brief  规约解析入口，供protocol调用
*  @param[in] BUF_MUX *buf
*  @param[in] uint16 port_id
*  @return  没有实际意义
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char deal_101 ( BUF_MUX *buf, uint16 port_id )
{
    static uint16 timer_id = 0;
    uint8 class_data_id = 0;
    char frame_type = 0;
    static uint8 flag = 0;
    char ret = 0;
    uint8 keyword_pos;
    uint8 keyword_len;
    PRO_TIMEOUT *to = 0;
    PRO_LOGIC_BUF *plb = 0;
    char event_ret = 0;
    to = gb101.SelectTimeOut ( port_id );
    plb = gb101.SelectLogicBuf ( port_id );
    gb101.member.CurPortId = port_id;
    if ( flag == 0 ) {//定时器开启
        if ( timer_id == 0 ) {
            timer.SetTimer ( &timer_id, 1, 0 );
        } else {
            timer.KillTimer ( timer_id );
            timer.SetTimer ( &timer_id, 1, 0 );
        }
        flag = 1;
    }
    if ( plb == 0 ) return 0;
    if ( plb->connect_st == 2 && gb101.member.pro_cfg.trans_style == EQ_TRANS ) {//如果接收到主站的复位链路并发送确认后，请求链路。
        gb101.member.ProtBuf.pack_frame_len = 0;
        SetFuncCode ( M_Request_Link_St );
        plb->CtrlWordFromSelf.cword.PRM = 1;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "Send  Ask Link Status %d\n" , M_Request_Link_St );
        BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
        protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
        protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
        plb->connect_st = 3;
        if ( to != 0 ) {
            to->resend_flag = 0;
            to->resend = 0;
            to->resend_cnt = 0;
        }
    }
    if ( timer.IsTimesUp ( timer_id ) ) {//规约超时处理
        gb101.DealTimeOut ( buf, port_id );
        ////////this is testing code for produce 1 2 class data 1s per times
    }
    event_ret = gb101.ScanSpontBuf ( port_id );//事件判断
    gb101.member.ProtBuf.pack_frame_len = 0;
    gb101.DealComBuf ( buf, port_id );//处理通信口数据
    frame_type = gb101.CheckFrame ( buf, port_id );//规约帧判断
    if ( frame_type == APP_HANDLE ) {//68帧处理
        gb101.member.ProtBuf.valid_flag = 0;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "frame type =%d\n", frame_type );
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "APP HANDLE\n" );
        gb101.GetKeyWordPositionAndLen ( KEY_WORD_CTRLWORD, &keyword_pos, &keyword_len );
        ret = gb101.Parser ( gb101.member.ProtBuf.parser_buf, gb101.member.ProtBuf.parser_frame_len, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "Parser ret=%d,len=%d\n" , ret, gb101.member.ProtBuf.pack_frame_len );
        if ( ret == 2 ) { //the out data need to be packed
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "the out data need to be packed\n" );
            SetFuncCode ( S_ACK_User_Data );
            SetAcdDfc ( 1, 0 );
        } else if ( ret == 1 ) { //the out data can be send directly
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "the out data can be send directly\n" );
            protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            gb101.member.ProtBuf.pack_frame_len = 0;
        } else {
            //do nothing
        }
    } else if ( frame_type == LINK_HANDLE ) {//10帧处理
        gb101.member.ProtBuf.valid_flag = 0;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "LINK HANDLE\n" );
        plb->CtrlWordFromMaster.data = buf->frame.buf[1];//no check de FCB
        switch ( plb->CtrlWordFromMaster.cword.FuncCode ) {//根据链路控制字来处理
        case M_Request_Class1_Data://主站请求1级数据
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Request_Class1_Data\n" );
            if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
                if ( to != 0 ) {
                    to->test_cnt = 0;
                    to->test = 0;
					to->test_flag=0;
                    to->link = 0;
                    to->link_flag = 0;
                    to->resend_flag = 0;
                    to->resend = 0;
                    to->resend_cnt = 0;
                }
            }
            gb101.member.ProtBuf.pack_frame_len = 0;
            if ( gb101.GetServiceId ( port_id, CLASS1, &class_data_id ) ) {
                SetCuaseWord ( CAUSE_Req );
                if(PackClass1Data ( port_id, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len )){
            		//PFUNC ( TEM_INFO, DEBUG_PRO101, "Ack Class 1 data\n" );
            	}
            	else{
	                SetFuncCode ( S_NACK );
	                SetAcdDfc ( 0, 0 );
	                BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
            	}
                protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            	protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            } else {
                SetFuncCode ( S_NACK );
                SetAcdDfc ( 0, 0 );
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Request_Class1_Data, NACK\n" );
                BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            }
            break;
        case M_Request_Class2_Data://  &&S_ACK_Link_St:主站请求2级数据和应答链路状态
            if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {//平衡传输，清重发标志
                if ( to != 0 ) {
                    to->test_cnt = 0;
                    to->test = 0;
					to->test_flag=0;
                    to->link = 0;
                    to->link_flag = 0;
                    to->resend_flag = 0;
                    to->resend = 0;
                    to->resend_cnt = 0;
                }
                if ( plb->CtrlWordFromMaster.cword.PRM == 0 ) {//station ack
                    if ( plb->connect_st == 3 ) {//主站相应链路状态，终端发复位链路
                        SetFuncCode ( M_Reset_Remote_Link );
                        plb->CtrlWordFromSelf.cword.PRM = 1;
                        //PFUNC ( TEM_INFO, DEBUG_PRO101, "Station ack link status and ter send rst link %d\n" , M_Reset_Remote_Link );
                        SetAcdDfc ( 0, 0 );
                        BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                        protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                        protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                        plb->connect_st = 4;
                        gb101.member.LastDataClass1Id = 0;
                        gb101.member.LastDataClass2Id = 0;
                        plb->intro_row = 0;//total ask reset
                        plb->intro_col = 0;//total ask reset
                        if ( plb != 0 ) {
                            plb->CtrlWordFromMaster.cword.FCB = 1;
                        }
                    }
                }
            } else {//非平衡传输响应2级数据请求命令
                gb101.member.ProtBuf.pack_frame_len = 0;
                if ( gb101.GetServiceId ( port_id, CLASS2, &class_data_id ) ) {
                    SetCuaseWord ( CAUSE_Req );
                    PackClass2Data ( port_id, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Request_Class2_Data, Ack class 2 data\n" );
                    protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                    protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                }
                else {//没有2级数据相应否认帧
                    SetFuncCode ( S_NACK );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Request_Class2_Data, NACK\n" );
                    BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                    protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                    protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                }
            }
            break;
        case M_Request_Link_St://主站请求链路状态
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Request_Link_St\n" );
            SetFuncCode ( S_ACK_Link_St );
            SetAcdDfc ( 0, 0 );
            gb101.member.ProtBuf.pack_frame_len = 0;
            BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "pack_frame_len=%d,%x\n", gb101.member.ProtBuf.pack_frame_len, &gb101.member.ProtBuf.pack_frame_len );
            protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            if ( to != 0 ) {
                to->link = 0;
                to->link_flag = 0;
                to->resend_flag = 0;
                to->resend = 0;
                to->resend_cnt = 0;
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "station ask link st,ter Ack Link Status %d\n", M_Request_Link_St );
            plb->connect_st = 1;
            break;
        case M_Reset_Remote_Link://主站复位终端链路，以及确认帧
            PFUNC ( TEM_INFO, DEBUG_PRO101, "S_ACK\n" );
            if ( to != 0 ) {
                to->link = 0;
                to->link_flag = 0;
                to->resend_flag = 0;
                to->resend = 0;
                to->resend_cnt = 0;
                to->test = 0;
                to->test_cnt = 0;
				to->test_flag=0;
            }
            if ( plb->CtrlWordFromMaster.cword.PRM == 1 ) { //rst cmd
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "rstcmdd\n" );
                if ( plb->connect_st == 1 ) {//在主站请求链路状态后，收到复位链路命令
                    plb->connect_st = 2;
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "station rst link ,and ter send ack %d\n", M_Reset_Remote_Link );
                    SetFuncCode ( S_ACK );
                    SetAcdDfc ( 0, 0 );
                    gb101.member.ProtBuf.pack_frame_len = 0;
                    BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                    protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                    protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                    if ( plb != 0 ) {
                        plb->CtrlWordFromMaster.cword.FCB = 1;
                    }
                    gb101.member.LastDataClass1Id = 0;
                    gb101.member.LastDataClass2Id = 0;
                    plb->intro_row = 0;//total ask reset
                    plb->intro_col = 0;//total ask reset
                }
            } else if ( plb->connect_st == 4 ) {//终端发出的复位链路命令，主站回复确认
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "rst ack\n" );
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "station ack rst and ter Send Init Over %d\n", M_Reset_Remote_Link );
                gb101.SetCauseOfInit ( 2 );
                PackInitOverData ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                plb->connect_st = 5;
            } else {//在确认上一帧后，主动上送1级或2级数据
                plb->confirm_flag = 1;
            }
            break;
        case M_Reset_User_Process:
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "M_Reset_User_Process\n" );
            SetFuncCode ( S_ACK );
            gb101.member.ProtBuf.pack_frame_len = 0;
            BuildFrame1016 ( gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
            protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            //reset terminal;
            ProRstTer ( gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
            break;
        default:
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "No Such Function Code\n" );
            break;
        }
    }else {//主动上送突发数据，在初始化结束后
        if ( plb->confirm_flag == 1 && plb->connect_st == 5 ) {
            gb101.member.ProtBuf.pack_frame_len = 0;
            gb101.GetServiceId ( port_id, CLASS1, &class_data_id );
            if ( class_data_id ) {
                SetCuaseWord ( CAUSE_Spont );
                PackClass1Data ( port_id, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                if(gb101.member.ProtBuf.pack_frame_len!=0){
                	plb->confirm_flag = 0;
                }
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "self send, class 1 data\n" );
                protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
            } else  {
                gb101.GetServiceId ( port_id, CLASS2, &class_data_id );
                if ( class_data_id ) {
                    SetCuaseWord ( CAUSE_Spont );
                    PackClass2Data ( port_id, gb101.member.ProtBuf.pack_buf, &gb101.member.ProtBuf.pack_frame_len );
                      if(gb101.member.ProtBuf.pack_frame_len!=0){
		                plb->confirm_flag = 0;
		              }
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "self send, class 2 data\n" );
                    protocol.AddSendBuf ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                    protocol.CopySendFrame ( buf, gb101.member.ProtBuf.pack_buf, gb101.member.ProtBuf.pack_frame_len );
                }
            }
        }
        //exit(0);
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析入口函数
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::Parser ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    PARSER_FUNC *tab;
    char ret = 0;
    uint8 type=0;
    uint8 pos,len;
    GetKeyWordPositionAndLen(KEY_WORD_TYPE,&pos,&len);
    type=in_buf[pos];
    tab = SearchParserFuncTable ( type);
    if ( tab != 0 ) {
        if ( tab->fp != 0 ) { //功能解析
            ret = ( *tab->fp ) ( in_buf, in_len , out_buf, out_len );
        }
    }
    return ret;
}
/**
*************************************************************************************
*  @brief  组包入口函数
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::Pack ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    PACK_FUNC *tab;
    char ret = 0;
    tab = SearchPackFuncTable ( type );
    if ( tab != 0 ) {
        if ( tab->fp != 0 ) {
            ret = ( *tab->fp ) ( in_buf, in_len , out_buf, out_len );
        }
        return ret;
    }
    return ret;
}
/**
*************************************************************************************
*  @brief  判断数据服务，并决定执行哪个服务
*  @param[in] uint8 class_data_id
*  @param[out] uint8 *last_class_data_id
*  @return  成功返回1或2，否则返回0，返回1代表先来先服务，2表示后来先服务
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char JudgeDataService ( uint8 class_data_id, uint8 *last_class_data_id )
{
    PRO_DATA_CLASS_CFG *tab1, *tab;
    if ( class_data_id == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no insert service change1 L=%d,C=%d\n", *last_class_data_id, class_data_id );
        return 0;
    }
    if ( gb101.member.pro_cfg.enable_interrupt_process == 0 ) { //上一个过程不允许打断
        if ( *last_class_data_id == 0 ) {
            *last_class_data_id = class_data_id;
            PFUNC ( TEM_INFO, DEBUG_PRO101, "no insert service change2 L=%d:C=%d\n", gb101.member.LastDataClass1Id, class_data_id );
            return 1;
        }
    } else { //允许打断上一个过程
        if ( *last_class_data_id != class_data_id ) {
            if ( *last_class_data_id == 0 ) {
                *last_class_data_id = class_data_id;
                PFUNC ( TEM_INFO, DEBUG_PRO101, "occupy service change1 L=%d:C=%d\n", gb101.member.LastDataClass1Id, class_data_id );
                return 2;
            }
            tab = gb101.SearchClassTable ( class_data_id );
            tab1 = gb101.SearchClassTable ( *last_class_data_id );
            if ( tab != 0 && tab1 != 0 ) {
                if ( tab->pri < tab1->pri ) { //优先级比较
                    *last_class_data_id = class_data_id;
                    PFUNC ( TEM_INFO, DEBUG_PRO101, "occupy service change2 L=%d:C=%d\n", gb101.member.LastDataClass1Id, class_data_id );
                    return 2;
                }
            }
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  对1级数据进行组帧函数
*  @param[in] uint16 port_id
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackClass1Data ( uint16 port_id, char *out_buf, uint16 *out_len )
{
    PRO_DATA_CLASS_CFG *tab;
    PRO_LOGIC_BUF *plb = 0;
    *out_len = 0;
    plb = gb101.SelectLogicBuf ( port_id );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to pack data\n" );
        return 0;
    }
    if ( gb101.member.LastDataClass1Id != 0 ) {
        tab = gb101.SearchClassTable ( gb101.member.LastDataClass1Id );
        if ( tab != 0 ) {
            if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
                SetFuncCode ( S_ACK_User_Data );
                plb->CtrlWordFromSelf.cword.PRM = 1;
            } else {
                SetFuncCode ( S_ACK_User_Data );
            }
            if ( ( *tab->fp ) ( out_buf, out_len ) ) { //璿及数据应答主站召测的后续数据
                return *out_len;
            }else
            	PFUNC ( TEM_WARNING, DEBUG_PRO101, "\n" );
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  二级数据组帧
*  @param[in] uint16 port_id
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackClass2Data ( uint16 port_id, char *out_buf, uint16 *out_len )
{
    PRO_DATA_CLASS_CFG *tab;
    *out_len = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( port_id );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to pack data\n" );
        return 0;
    }
    if ( gb101.member.LastDataClass2Id != 0 ) { //
        tab = gb101.SearchClassTable ( gb101.member.LastDataClass2Id );
        if ( tab != 0 ) {
            if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
                SetFuncCode ( S_ACK_User_Data );
                plb->CtrlWordFromSelf.cword.PRM = 1;
            } else {
                SetFuncCode ( S_ACK_User_Data );
            }
            if ( ( *tab->fp ) ( out_buf, out_len ) ) { //璿级数据应答主站召测的后续数据
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "len=%d\n", *out_len );
                return 1;
            }
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  初始化结束组帧
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackInitOverData ( char *out_buf, uint16 *out_len )
{
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    pdcc = gb101.SearchClassTable ( INIT_OVER_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, INIT_OVER_DATA, CP00 ) )
                ret = Pack_M_EI_NA_1 ( data.buf, data.bytes, out_buf, out_len );
        } else {
            gb101.member.LastDataClass2Id = 0;
            if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, INIT_OVER_DATA, CP00 ) )
                ret = Pack_M_EI_NA_1 ( data.buf, data.bytes, out_buf, out_len );
        }
        if ( ret != 0 ) {
            return 1;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  变化遥信数据组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackChangeYxData ( char *out_buf, uint16 *out_len )
{
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    ID_MAP *imp = 0;
    PACK_FUNC *pf = 0;
    uint16 data_id = 0;
    pdcc = gb101.SearchClassTable ( CHANGE_YX_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, CHANGE_YX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb101.GetDataAddr ( data_id );
                if ( imp == 0 ) return 0;
                pf = gb101.SearchPackFuncTable ( imp->type );
                if ( pf == 0 ) return 0;
                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "syx id=%d,msg_type=%d,data_type=%d\n", data.buf[2] + data.buf[3]*256, imp->type, CHANGE_YX_DATA );
            } else if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, CHANGE_DYX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb101.GetDataAddr ( data_id );
                if ( imp == 0 ) return 0;
                pf = gb101.SearchPackFuncTable ( imp->type );
                if ( pf == 0 ) return 0;
                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "dyx id=%d,msg_type=%d,data_type=%d\n", data.buf[2] + data.buf[3]*256, imp->type, CHANGE_DYX_DATA );
            }
        } else {
            gb101.member.LastDataClass2Id = 0;
            if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, CHANGE_YX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb101.GetDataAddr ( data_id );
                if ( imp == 0 ) return 0;
                pf = gb101.SearchPackFuncTable ( imp->type );
                if ( pf == 0 ) return 0;
                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
            } else  if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, CHANGE_DYX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb101.GetDataAddr ( data_id );
                if ( imp == 0 ) return 0;
                pf = gb101.SearchPackFuncTable ( imp->type );
                if ( pf == 0 ) return 0;
                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
            }
        }
        if ( ret != 0 ) {
            return 1;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  控制返回数据组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackCtrlRetData ( char *out_buf, uint16 *out_len )
{
    char ret = 0;
    int8 suc=0;
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF *plb = 0;
    uint16 data_id = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf tu pack data\n" );
        return 0;
    }
    pdcc = gb101.SearchClassTable ( CTRL_RET_DATA );
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "pdcc:%x\n",pdcc );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, CTRL_RET_DATA, CP00 ) ) {
                SetFuncCode ( S_ACK );
                plb->CtrlWordFromSelf.cword.PRM = 1;
				if(is_set(data.buf[5],4))//双命令
				{
					suc=data.buf[5]&0x3;
					if(suc==3)
						suc=0;
					else
						suc=1;	
					cbi(data.buf[5],4)	;//清除DCO中QOC中的自定义单双选择
				}
				else//单命令
				{
					if(is_set(data.buf[5],1))
						{
							suc=0;
						}
					else
						{
							suc=1;	
						}
					cbi(data.buf[5],4)	;//清除SCO中RES中的自定义成功失败选择
				}	                
                if ( plb->yk_st == 1 ) {
                	if(suc)
                		SetCuaseWord ( CAUSE_Actcon );
                	else
                		SetCuaseWord ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因
                }	
                else if( plb->yk_st == 2 ) {
                	if(suc)
                		SetCuaseWord ( CAUSE_Actcon );
                	else
                		SetCuaseWord ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "control end \n" );
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    ProSendMsg ( TASK_PRO, MSG_YK, data_id, &data.buf[4] , 1 );
                    plb->yk_st = 3;
                    //}
                } else if ( plb->yk_st == 3 ) {
                    //SetCuaseWord ( CAUSE_Actterm );
                    protocol.member.YkPortNum = 0;
                } else if ( plb->yk_st == 4 ) {
                    SetCuaseWord ( CAUSE_Deactcon );
                    plb->yk_st = 0;
                    protocol.member.YkPortNum = 0;
                }
                ret = PrepareProData ( data.buf, data.bytes, out_buf, out_len );
                if ( ret ) {
                    BuildFrame6816 ( plb->yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    memcpy ( out_buf, data.buf, data.bytes );
                    *out_len = data.bytes;
                    return 1;
                }
            }
        } else {
            gb101.member.LastDataClass2Id = 0;
            if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, CTRL_RET_DATA, CP00 ) ) {
                SetFuncCode ( S_ACK );
                SetCuaseWord ( CAUSE_Actcon );
                plb->CtrlWordFromSelf.cword.PRM = 1;
                if ( plb->yk_st == 2 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "control end \n" );
                    //imp = gb101.GetDataAddr ( * ( uint16* ) & data.buf[2] );
                    //if ( imp != 0 ) {
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    ProSendMsg ( TASK_PRO, MSG_YK, data_id, &data.buf[4], 1 );
                    plb->yk_st = 3;
                    //}
                } else if ( plb->yk_st == 3 ) {
                    SetCuaseWord ( CAUSE_Actterm );
                    protocol.member.YkPortNum = 0;
                } else if ( plb->yk_st == 4 ) {
                    SetCuaseWord ( CAUSE_Deactcon );
                    plb->yk_st = 0;
                    protocol.member.YkPortNum = 0;
                }
                ret = PrepareProData ( data.buf, data.bytes, out_buf, out_len );
                if ( ret ) {
                    BuildFrame6816 ( plb->yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    memcpy ( out_buf, data.buf, data.bytes );
                    *out_len = data.bytes;
                    return 1;
                }
            }
        }
        if ( ret != 0 ) {
            return 1;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  遥测变化组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackChangeYcData ( char *out_buf, uint16 *out_len )
{
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "XXXXXXXXXXXXXX%d\n", CHANGE_YC_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    uint8 time_tag;
    time_tag = gb101.member.pro_cfg.time_tag;
    pdcc = gb101.SearchClassTable ( CHANGE_YC_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, CHANGE_YC_DATA, time_tag ) ) {
                if ( time_tag == CP24 ) {
                    ret = 0;//ret=Pack_M_ME_TA_1(data.buf,data.bytes,out_buf,out_len);
                } else if ( time_tag == CP56 ) {
                    if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TD_1 )
                        ret = Pack_M_ME_TD_1 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TE_1 )
                        ret = Pack_M_ME_TE_1 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb101.member.class1_datatype_cfg.yc_type == M_ME_TF_1 )
                        ret = Pack_M_ME_TF_1 ( data.buf, data.bytes, out_buf, out_len );
                }
            }
        } else {
            gb101.member.LastDataClass2Id = 0;
            if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, CHANGE_YC_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = 0;//ret=Pack_M_SP_TA_1(data.buf,data.bytes,out_buf,out_len);
                else if ( time_tag == CP56 ) {
                    if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TD_1 )
                        ret = Pack_M_ME_TD_1 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TE_1 )
                        ret = Pack_M_ME_TE_1 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb101.member.class2_datatype_cfg.yc_type == M_ME_TF_1 )
                        ret = Pack_M_ME_TF_1 ( data.buf, data.bytes, out_buf, out_len );
                }
            }
        }
        if ( ret != 0 ) {
            return 1;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  时间同步组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackTimeTbData ( char *out_buf, uint16 *out_len )
{
    char data[100];
    uint8 datalen = 0;
    PRO_LOGIC_BUF *plb = 0;
    CP56Time2a ct;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "\n" );
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }
    SetCuaseWord ( CAUSE_Actcon );
    timer.GetCP56Time ( &ct );
    /*
    PFUNC ( TEM_INFO, DEBUG_PRO101, "[%d]%d-%d-%d  %d:%d:%dms @week:%d  \n", 0 \
            , ct.year + 2000 \
            , ct.month \
            , ct.day_week&0x1f \
            , ct.hour_su&0x1f \
            , ct.minute_iv&0x3f \
            , ct.millisecond \
            , ( ct.day_week >> 5 ) &0x07 );
	*/            
    memcpy ( &data[datalen], ( char* ) &ct, 7 );
    //dump_binary ( ( uint8* ) &data[datalen], 7 );
    //dump_binary ( ( uint8* ) &ct, 7 );
    datalen += 7;
    SetFuncCode ( S_ACK );
    if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
        plb->CtrlWordFromSelf.cword.PRM = 1;
    }
    BuildFrame6816 ( C_CS_NA_1, out_buf, out_len, data, datalen );
    pdcc = gb101.SearchClassTable ( TIME_TB_DATA );
    if ( pdcc == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "The Service %d not find\n", TIME_TB_DATA );
        return 0;
    }
    if ( pdcc->grade == CLASS1 ) {
        gb101.member.LastDataClass1Id = 0;
    } else {
        gb101.member.LastDataClass2Id = 0;
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  Soe数据组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackSoeData ( char *out_buf, uint16 *out_len )
{
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "%d\n", SOE_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    uint8 time_tag;
    time_tag = gb101.member.pro_cfg.time_tag;
    pdcc = gb101.SearchClassTable ( SOE_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, SOE_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_SP_TA_1 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_SP_TB_1 ( data.buf, data.bytes, out_buf, out_len );
            } else if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, SOE2_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_DP_TA_1 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_DP_TB_1 ( data.buf, data.bytes, out_buf, out_len );
            }
        } else {
            gb101.member.LastDataClass2Id = 0;
            if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, SOE_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_SP_TA_1 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_SP_TB_1 ( data.buf, data.bytes, out_buf, out_len );
            } else if ( gb101.GetC2Event ( gb101.member.CurPortId, &data, SOE2_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_DP_TA_1 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_DP_TB_1 ( data.buf, data.bytes, out_buf, out_len );
            }
        }
        if ( ret != 0 ) {
            return 1;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  总召唤无数据帧组包，如总召唤确认，总召唤结束等不包含数据的帧
*  @param[in] uint8 qoi
*  @param[in] uint8 coi
*  @param[in]  uint8 cw
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackTotalAskNoData ( uint8 qoi, uint8 coi, uint8 cw, char *out_buf, uint16 *out_len )
{
    char data[32];
    uint8 datalen;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF *plb = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to pack data\n" );
        return 0;
    }
    pdcc = gb101.SearchClassTable ( ACK_STATION_ASK_DATA );
    if ( gb101.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }
    data[datalen++] = qoi;
    SetCuaseWord ( coi );
    if ( coi == CAUSE_Actterm )
        SetFuncCode ( S_ACK );
    else {
        if ( pdcc->grade == CLASS1 )
            SetFuncCode ( cw );
        else
            SetFuncCode ( cw );
    }
    if ( gb101.member.pro_cfg.trans_style == EQ_TRANS ) {
        plb->CtrlWordFromSelf.cword.PRM = 1;
    }
    BuildFrame6816 ( C_IC_NA_1, out_buf, out_len, data, datalen );
    return 1;
}
/**
*************************************************************************************
*  @brief  总召唤组包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackAckSationAskData ( char *out_buf, uint16 *out_len )
{
    PRO_LOGIC_BUF *plb = 0;
    int k;
    PACK_FUNC *tab;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    ID_MAP *im_tab;
    pdcc = gb101.SearchClassTable ( ACK_STATION_ASK_DATA );
    if ( pdcc == 0 ) return 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        if ( plb->total_ask_begin == 1  ) {
            plb->total_ask_begin = 0;
            PackTotalAskNoData ( plb->QOI, CAUSE_Actcon, S_ACK, out_buf, out_len );
            return 1;
        }
    	*out_len = 0;
        if ( plb->total_data_send_over == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "over station ask data\n" );
            if ( plb->total_ask_begin == 0 ) {
                PackTotalAskNoData ( plb->QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );
                if ( pdcc->grade == CLASS1 )
                    gb101.member.LastDataClass1Id = 0;
                else
                    gb101.member.LastDataClass2Id = 0;
                plb->M_type = 0;
                return 1;
            }
        }
        if ( plb->M_type == C_IC_NA_1 ) { //总召唤
            if ( plb->QOI == CAUSE_Introgen ) {
                //PFUNC(TEM_INFO,DEBUG_PRO101,"all\n");
                SetCuaseWord ( CAUSE_Introgen );
                while ( gb101.member.introx_info[plb->intro_col][plb->intro_row].flag == 0 ) {
                    plb->intro_row++;
                    if ( plb->intro_row >= 8 ) {
                        plb->intro_row = 0;
                        plb->intro_col++;
                        if ( plb->intro_col >= 16 ) {
                            plb->intro_row = 0;
                            plb->intro_col = 0;
                            plb->total_data_send_over = 0;
                            //PFUNC ( TEM_INFO, DEBUG_PRO101, "total:data Send over\n" );
                            if ( plb->total_data_send_over == 0 ) {
                                //PFUNC ( TEM_INFO, DEBUG_PRO101, "over station ask data\n" );
                                if ( plb->total_ask_begin == 0 ) {
                                    PackTotalAskNoData ( plb->QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );
                                    plb->M_type = 0;
                                    if ( pdcc->grade == CLASS1 )
                                        gb101.member.LastDataClass1Id = 0;
                                    else
                                        gb101.member.LastDataClass2Id = 0;
                                    return 1;
                                }
                            }
                            return 1;
                        }
                    }
                }
                memcpy ( &gb101.member.pb.buf[0], ( char* ) &gb101.member.introx_info[plb->intro_col][plb->intro_row].num, 1 );
                gb101.member.pb.buf[1] = 0;
                for ( k = 0;k < gb101.member.introx_info[plb->intro_col][plb->intro_row].num;k++ )
                {
                    im_tab = gb101.GetDataId ( gb101.member.introx_info[plb->intro_col][plb->intro_row].start_addr + k );//id
                    if ( im_tab != 0 ) {
                        memcpy ( &gb101.member.pb.buf[3*k+2], ( char* ) &im_tab->data_id, 2 );
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "error no addr's id %x\n", gb101.member.introx_info[plb->intro_col][plb->intro_row].start_addr + k );
                    }
                    gb101.member.pb.buf[3*k+4] = 0;//size
                }
                gb101.member.pb.bytes = 3 * k + 2;
                protocol.member.data_type = gb101.member.introx_info[plb->intro_col][plb->intro_row].typesign;
                if ( ProGetData ( gb101.member.pb.buf, gb101.member.pb.bytes, out_buf, out_len ) ) {
                    tab = gb101.SearchPackFuncTable ( gb101.member.introx_info[plb->intro_col][plb->intro_row].typesign );
                    if ( tab != 0 ) {
                        tab->fp ( out_buf, *out_len, gb101.member.pb.buf, &gb101.member.pb.bytes );
                        memcpy ( out_buf, gb101.member.pb.buf, gb101.member.pb.bytes );
                        memcpy ( ( char* ) out_len, ( char* ) & ( gb101.member.pb.bytes ), 2 );
                        //dump_binary((uint8 *) out_len-1,10);
                    }
                }
                plb->intro_row++;
                if ( plb->intro_row >= 8 ) {
                    plb->intro_row = 0;
                    plb->intro_col++;
                    if ( plb->intro_col >= 16 ) {
                        plb->intro_row = 0;
                        plb->intro_col = 0;
                        plb->total_data_send_over = 0;
                        PFUNC ( TEM_INFO, DEBUG_PRO101, "total:data Send over\n" );
                    }
                }
                if ( *out_len != 0 ) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  组召唤数据打包
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackAckGroupAskData ( char *out_buf, uint16 *out_len )
{
    PRO_LOGIC_BUF *plb = 0;
    int k;
    PACK_FUNC *tab;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    ID_MAP *im_tab;
    pdcc = gb101.SearchClassTable ( ACK_GROUP_ASK_DATA );
    if ( pdcc == 0 ) return 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb != 0 ) {
        plb->intro_col = plb->QOI - 21;
        if ( ( char ) plb->intro_col < 0 ) {
            return 0;
        }
        if ( plb->total_ask_begin == 1 ) {
            plb->total_ask_begin = 0;
            if ( gb101.member.introx_info[plb->intro_col][plb->intro_row].flag == 0 ) {
                PackTotalAskNoData ( plb->QOI, CAUSE_Deactcon, S_ACK, out_buf, out_len );
            } else {
                PackTotalAskNoData ( plb->QOI, CAUSE_Actcon, S_ACK, out_buf, out_len );
            }
            plb->intro_col = 0;
            return 1;
        }
        *out_len = 0;
        if ( plb->total_data_send_over == 0 ) {
            if ( plb->total_ask_begin == 0 ) {
                PackTotalAskNoData ( plb->QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );
                plb->M_type = 0;
                if ( pdcc->grade == CLASS1 )
                    gb101.member.LastDataClass1Id = 0;
                else
                    gb101.member.LastDataClass2Id = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "group:Ack Send over\n" );
                return 1;
            }
        }
        if ( gb101.member.introx_info[plb->intro_col][plb->intro_row].flag == 1 ) {
            gb101.member.pb.buf[0] = gb101.member.introx_info[plb->intro_col][plb->intro_row].num;
            gb101.member.pb.buf[1] = 0;
            for ( k = 0;k < gb101.member.introx_info[plb->intro_col][plb->intro_row].num;k++ )
            {
                im_tab = gb101.GetDataId ( gb101.member.introx_info[plb->intro_col][plb->intro_row].start_addr + k );//id
                if ( im_tab != 0 ) {
                    memcpy ( &gb101.member.pb.buf[3*k+2], ( char* ) &im_tab->data_id, 2 );
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "error no addr's id %d\n",
                            gb101.member.introx_info[plb->intro_col][plb->intro_row].start_addr + k );
                }
                gb101.member.pb.buf[3*k+4] = 0;//size
            }
            gb101.member.pb.bytes = 3 * k + 2;
            protocol.member.data_type = gb101.member.introx_info[plb->intro_col][plb->intro_row].typesign;
            if ( ProGetData ( gb101.member.pb.buf, gb101.member.pb.bytes, out_buf, out_len ) ) {
                tab = gb101.SearchPackFuncTable ( gb101.member.introx_info[plb->intro_col][plb->intro_row].typesign );
                if ( tab != 0 ) {
                    SetCuaseWord ( plb->QOI );
                    tab->fp ( out_buf, *out_len, gb101.member.pb.buf, &gb101.member.pb.bytes );
                    memcpy ( out_buf, gb101.member.pb.buf, gb101.member.pb.bytes );
                    *out_len = gb101.member.pb.bytes;
                }
            }
            plb->intro_row++;
            if ( plb->intro_row >= 8 || gb101.member.introx_info[plb->intro_col][plb->intro_row].flag == 0 ) {
                plb->total_data_send_over = 0;
                plb->intro_row = 0;
                plb->intro_col = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "group:data Send over\n" );
            }
        } else {
            plb->total_data_send_over = 0;
            plb->intro_row = 0;
            plb->intro_col = 0;
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "group:data Send over\n" );
        }
    }
    if ( *out_len != 0 )
        return 1;
    else
        return 0;
}
uint8 PackAckVerifyRetData ( char *out_buf, uint16 *out_len )
{
    char ret = 0;
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF *plb = 0;
   // uint16 data_id = 0;
    plb = gb101.SelectLogicBuf ( gb101.member.CurPortId );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf tu pack data\n" );
        return 0;
    }
    pdcc = gb101.SearchClassTable ( VERIFY_RET_DATA );
    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb101.member.LastDataClass1Id = 0;
            if ( gb101.GetC1Event ( gb101.member.CurPortId, &data, VERIFY_RET_DATA, CP00 ) ) {
                SetFuncCode ( S_ACK );
                SetCuaseWord ( CAUSE_Actcon );
                plb->CtrlWordFromSelf.cword.PRM = 1;
                ret = PrepareProData ( data.buf, data.bytes, out_buf, out_len );
                if ( ret ) {
                    BuildFrame6816 ( 222, data.buf, &data.bytes, out_buf, *out_len );
                    memcpy ( out_buf, data.buf, data.bytes );
                    *out_len = data.bytes;
                    plb->confirm_flag = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  初始化内存的数据类配置，使用外部配置文件，可以不使用
*  @return  不关心
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::InitDataClassTable ( void )
{
    //attention :need read cfg file to fill the table before use
    // ProDataClassCfgTable[0].grade = CLASS1;
    member.ProDataClassCfgTable[0].type = INIT_OVER_DATA;
    // ProDataClassCfgTable[0].pri = 1;
    member.ProDataClassCfgTable[0].fp = PackInitOverData;
    //ProDataClassCfgTable[1].grade = CLASS1;
    member.ProDataClassCfgTable[1].type = CHANGE_YX_DATA;
    // ProDataClassCfgTable[1].pri = 2;
    member.ProDataClassCfgTable[1].fp = PackChangeYxData;
    //ProDataClassCfgTable[2].grade = CLASS1;
    member.ProDataClassCfgTable[2].type = CTRL_RET_DATA;
    //ProDataClassCfgTable[2].pri = 3;
    member.ProDataClassCfgTable[2].fp = PackCtrlRetData;
    // ProDataClassCfgTable[3].grade = CLASS1;
    member.ProDataClassCfgTable[3].type = CHANGE_YC_DATA;
    //ProDataClassCfgTable[3].pri = 4;
    member.ProDataClassCfgTable[3].fp = PackChangeYcData;
    //ProDataClassCfgTable[4].grade = CLASS2;
    member.ProDataClassCfgTable[4].type = TIME_TB_DATA;
    // ProDataClassCfgTable[4].pri = 5;
    member.ProDataClassCfgTable[4].fp = PackTimeTbData;
    // ProDataClassCfgTable[5].grade = CLASS2;
    member.ProDataClassCfgTable[5].type = SOE_DATA;
    // ProDataClassCfgTable[5].pri = 6;
    member.ProDataClassCfgTable[5].fp = PackSoeData;
    // ProDataClassCfgTable[6].grade = CLASS2;
    member.ProDataClassCfgTable[6].type = ACK_STATION_ASK_DATA;
    // ProDataClassCfgTable[6].pri = 7;
    member.ProDataClassCfgTable[6].fp = PackAckSationAskData;
    // ProDataClassCfgTable[7].grade = CLASS2;
    member.ProDataClassCfgTable[7].type = ACK_GROUP_ASK_DATA;
    // ProDataClassCfgTable[7].pri = 8;
    member.ProDataClassCfgTable[7].fp = PackAckGroupAskData;
    return 1;
}
/**
*************************************************************************************
*  @brief  根据给定的数据类型查表，返回表指针
*  @param[in] uint8 data_type
*  @return  成功返回表指针，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PRO_DATA_CLASS_CFG* Gb101::SearchClassTable ( uint8 data_type )
{
    int i;
    for ( i = 0;i < DATA_CLASS_TABLE_NUM_1;i++ ) {
        if ( member.ProDataClassCfgTable[i].type == data_type ) {
            return &member.ProDataClassCfgTable[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  组数据配置
*  @param[in] DATA_GROUP_CFG *pgf
*  @param[in] ID_MAP *pmap
*  @param[in] uint16 data_num
*  @param[in] uint8 data_type
*  @param[out] INTRO_INFO ii[16][8]
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Gb101::ConfigGroupData ( DATA_GROUP_CFG *pgf, INTRO_INFO ii[16][8], ID_MAP *pmap, uint16 data_num, uint8 data_type )
{
    int data_cnt, i, j, k = 0, h = 0;
    uint16 data_num_per_group = 0;
    uint16 last_group_data_num = 0;
    uint8 max_frame_data_num = 40;
    uint8 group_data_num = 40;
    uint16 limit_num = 40;
    uint8 datasize = 0;
    uint16 addr_pos = 0;
    uint8 frame_data_num = 0;
    uint8 data_inuse_cnt = 0;
    uint8 tmp_type = pgf->data_type;
    if ( data_type == GROUP_YX_DATA ) {
        max_frame_data_num = 200;
        group_data_num = 80;
        limit_num = 80;
        datasize = 1;
    } else if ( data_type == GROUP_YC_DATA ) {
        if ( member.pro_data_type_cfg.yc_type == M_ME_NC_1 ) {
            max_frame_data_num = 40;
            datasize = 4;
        } else {
            max_frame_data_num = 70;
            datasize = 2;
        }
        group_data_num = 80;
        limit_num = 80;
    }
    ////////////////配置组内数据的分配一组多帧传输的配置////////////////////
    data_cnt = data_num;
    if ( data_cnt > limit_num ) { //如果数据个数大于默认的1组数量,每组平局分配
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "data num larger than %d\n", limit_num );
        pgf->used_group_num = pgf->max_group_num;
        data_num_per_group = data_cnt / (pgf->used_group_num);
        last_group_data_num = data_num_per_group+data_cnt%data_num_per_group;
        for ( i = 0;i < pgf->used_group_num;i++ ) {
            if ( i < pgf->start_group_id+pgf->max_group_num - 1 ) {
                data_cnt = data_num_per_group;
            } else {
                data_cnt = last_group_data_num;
            }
            j = 0;
            data_inuse_cnt = 0;
            frame_data_num = 0;
            for ( k = 0;k < data_cnt;k++ ) {
                if ( tmp_type != pmap[h].type ) {
                    ii[i][j].flag = 1;
                    ii[i][j].num = frame_data_num;
                    data_inuse_cnt += ii[i][j].num;
                    ii[i][j].start_addr = pmap[addr_pos].data_addr;
                    addr_pos += ii[i][j].num;
                    frame_data_num = 0;
                    ii[i][j].typesign = tmp_type;
                    ii[i][j].size = datasize;
                    j++;
                    tmp_type = pmap[addr_pos].type;
                } else if ( frame_data_num >= max_frame_data_num ) {
                    ii[i][j].flag = 1;
                    ii[i][j].num = max_frame_data_num;
                    data_inuse_cnt += ii[i][j].num;
                    frame_data_num = 0;
                    ii[i][j].start_addr = pmap[addr_pos].data_addr;
                    addr_pos += ii[i][j].num;
                    ii[i][j].typesign = tmp_type;
                    ii[i][j].size = datasize;
                    j++;
                    tmp_type = pmap[addr_pos].type;
                }
                frame_data_num++;
                h++;
                if ( j >= 8 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "the group max frame is reached =%d\n", j );
                    break;
                }
            }
            if ( data_cnt - data_inuse_cnt > 0 ) {
                ii[i][j].flag = 1;
                ii[i][j].num = data_cnt - data_inuse_cnt;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                addr_pos += ii[i][j].num;
                ii[i][j].typesign = tmp_type;
                ii[i][j].size = datasize;
            }
        }
    } else { //如果数据个数小于默认的1组数量
        //PFUNC ( TEM_INFO, DEBUG_PRO101, " data num less than %d\n", limit_num );
        pgf->used_group_num = data_cnt / group_data_num + 1;
        for ( i = 0;i < pgf->used_group_num;i++ ) {
            if ( i < pgf->used_group_num - 1 ) {
                data_num_per_group = group_data_num;
                data_cnt = data_num_per_group;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "group_num=%d\n",  data_cnt );
            } else {
                last_group_data_num = data_num % group_data_num;
                data_cnt = last_group_data_num;
                //PFUNC ( TEM_INFO, DEBUG_PRO101, "last_group_data_num=%d\n",  data_cnt );
            }
            j = 0;
            data_inuse_cnt = 0;
            frame_data_num = 0;
            for ( k = 0;k < data_cnt;k++ ) {
                if ( tmp_type != pmap[h].type ) {
                    ii[i][j].flag = 1;
                    ii[i][j].num = frame_data_num;
                    data_inuse_cnt += ii[i][j].num;
                    ii[i][j].start_addr = pmap[addr_pos].data_addr;
                    addr_pos += ii[i][j].num;
                    frame_data_num = 0;
                    ii[i][j].typesign = tmp_type;
                    ii[i][j].size = datasize;
                    j++;
                    tmp_type = pmap[addr_pos].type;
                } else if ( frame_data_num >= max_frame_data_num ) {
                    ii[i][j].flag = 1;
                    ii[i][j].num = max_frame_data_num;
                    data_inuse_cnt += ii[i][j].num;
                    frame_data_num = 0;
                    ii[i][j].start_addr = pmap[addr_pos].data_addr;
                    addr_pos += ii[i][j].num;
                    ii[i][j].typesign = tmp_type;
                    ii[i][j].size = datasize;
                    j++;
                    tmp_type = pmap[addr_pos].type;
                }
                if ( j >= 8 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "the group max frame is reached =%d\n", j );
                    break;
                }
                frame_data_num++;
                h++;
            }
            if ( data_cnt - data_inuse_cnt > 0 ) {
                ii[i][j].flag = 1;
                ii[i][j].num = data_cnt - data_inuse_cnt;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                addr_pos += ii[i][j].num;
                ii[i][j].typesign = tmp_type;
                ii[i][j].size = datasize;
            }
        }
    }
    /*
    PFUNC ( TEM_INFO, DEBUG_PRO101, "group data num:%d group type=%d,group_id=%d,max_group_num=%d,used_group_num=%d\n", data_num, \
	        pgf->data_type , \
            pgf->start_group_id,\
            pgf->max_group_num,\
            pgf->used_group_num );
    */        
/*   
   for(i=0;i<16;i++)
	for(j=0;j<8;j++){
		PFUNC(TEM_INFO,DEBUG_PRO101,"intro_info:[%d][%d] start_addr:0x%x,num:%d,flag:%d,typesign:%d,size:%d\n",i,j,\
		member.introx_info[i][j].start_addr,member.introx_info[i][j].num,member.introx_info[i][j].flag,member.introx_info[i][j].typesign,member.introx_info[i][j].size);	
	}  
*/	
}
/**
*************************************************************************************
*  @brief  检查终端的状态
*  @return  返回终端的状态
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::CheckTerSt ( void )
{
    member.TerSt.status.cpu1_com_status = 1;
    return member.TerSt.status.cpu1_com_status;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Gb101::InitGb101 ( void )
{
    int i;
    ComBasePara *ComPara;
    Data < BaseDataType > data;
    user_data.GetData ( PARA_5139, &data );
    ComPara = ( ComBasePara * ) data.Data;
    member.xml_para_cnt = 0;
    member.TerSt.data = 0;//terminal status is initialized by zero
    ReadCfg();//read para config file and fill the para buffer
    member.ProDataClassCfgTable[8].grade = CLASS1;
    member.ProDataClassCfgTable[8].pri = 9;
    member.ProDataClassCfgTable[8].type = VERIFY_RET_DATA;
    member.ProDataClassCfgTable[8].fp = PackAckVerifyRetData;
    ParaCheck();//check the para is valid
    InitFunc();//config the datatype link to the function
    InitIdMapTable();//config the datasign and data id map
    memset ( member.pro_logic_buf, 0, sizeof ( member.pro_logic_buf ) );//reset the logic buffer by zero
    for ( i = 0;i < MAX_PORT;i++ ) {
        member.pro_logic_buf[i].CtrlWordFromSelf.cword.DIR = 1;
        member.pro_logic_buf[i].CtrlWordFromSelf.cword.PRM = 0;
    }
    //////////////////////////////
    for ( i = 0;i < MAX_PORT;i++ ) {
        member.pro_logic_buf[i].linkaddr = ComPara->Data.LocalId;
        member.pro_logic_buf[i].hostaddr = ComPara->Data.LocalId;
    }
    member.data_group_cfg[0].data_type = member.pro_data_type_cfg.syx_type;
    member.data_group_cfg[1].data_type = member.pro_data_type_cfg.yc_type;
    member.data_group_cfg[2].data_type = 0;
    member.data_group_cfg[3].data_type = 0;
    member.data_group_cfg[4].data_type = M_BO_NA_1;
    member.data_group_cfg[5].data_type = 0;
//遥信遥测等分组配置
	InitGroup();
//复位1、2级数据缓存
    memset ( &member.c1_data_buf, 0, sizeof ( member.c1_data_buf ) );
    memset ( &member.c2_data_buf, 0, sizeof ( member.c2_data_buf ) );
//超时计数器初始化
    memset ( member.timeout101, 0, sizeof ( member.timeout101 ) );
    member.InitOver = 1;
}
Gb101::Gb101 ( void )
{
    //InitGb101();
}
void Gb101::InitGroup ( void )
{
	ConfigGroupData ( &member.data_group_cfg[0], &member.introx_info[member.data_group_cfg[0].start_group_id - 1], member.yx_id_map, member.pro_yx_data_num + member.pro_dyx_data_num, GROUP_YX_DATA );
    ConfigGroupData ( &member.data_group_cfg[1], &member.introx_info[member.data_group_cfg[1].start_group_id - 1], member.yc_id_map, member.pro_yc_data_num, GROUP_YC_DATA );
    ConfigGroupData ( &member.data_group_cfg[4], &member.introx_info[member.data_group_cfg[4].start_group_id - 1], member.bit32_id_map, 1, 0 );
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::SetC1Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time )
{
    int i;
    if ( data == 0 || time == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "error para addr is zero\n" );
        return 0;
    }
    for ( i = 0;i < protocol.GetBufAddrRegistNum();i++ ) {
        if ( protocol.member.buf_addr_regist_table[i].pro_id != PRO101 ) {
            continue;
        }
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "[%d]port_id=%d\n", i, protocol.member.buf_addr_regist_table[i].port_id );
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].port_id = protocol.member.buf_addr_regist_table[i].port_id;
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].dataid = dataid;//GetDataAddr(dataid)->data_addr;
        memcpy ( member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].data, data, datasize );
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].datasize = datasize;
        memcpy ( member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].time, time, 7 );
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].read_flag = EVENT_NOREAD;
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].data_type = data_type;
        member.c1_data_buf.in_cnt++;
        if ( member.c1_data_buf.in_cnt >= MAX_C1_EVENT_NUM ) {
            member.c1_data_buf.in_cnt = 0;
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101:: GetC1Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag )
{
    int i, k = 0, j = 0;
    out_buf->bytes = 0;
    out_buf->data_num = 0;
    int data_pos = 0;
    uint8 max_num = 1;
    for ( i = 0;i < MAX_C1_EVENT_NUM;i++ ) {
        if ( member.c1_data_buf.c1_data[i].port_id == port_id ) {
            if ( member.c1_data_buf.c1_data[i].read_flag == EVENT_NOREAD && member.c1_data_buf.c1_data[i].data_type == data_type ) {
                memcpy ( &out_buf->buf[k*3+2], ( char* ) &member.c1_data_buf.c1_data[i].dataid, 2 );
                out_buf->buf[k*3+4] = member.c1_data_buf.c1_data[i].datasize;
                if ( time_tag == CP56 ) {
                    out_buf->buf[k*3+4] += 7;
                    max_num = 20;
                } else if ( time_tag == CP24 ) {
                    out_buf->buf[k*3+4] += 3;
                    max_num = 30;
                } else {
                    max_num = 50;
                }
                k++;
                out_buf->data_num += 1;
                memcpy ( &out_buf->buf[0], ( char* ) &out_buf->data_num, 2 );
                if ( out_buf->data_num >= max_num ) {
                    break;
                }
            }
        }
    }
    data_pos = k * 3 + 2;
    j = 0;
    for ( i = 0;i < MAX_C1_EVENT_NUM;i++ ) {
        if ( member.c1_data_buf.c1_data[i].port_id == port_id ) {
            if ( member.c1_data_buf.c1_data[i].read_flag == EVENT_NOREAD && member.c1_data_buf.c1_data[i].data_type == data_type ) {
                member.c1_data_buf.c1_data[i].read_flag = EVENT_READ;
                memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c1_data_buf.c1_data[i].data, member.c1_data_buf.c1_data[i].datasize );
                data_pos += member.c1_data_buf.c1_data[i].datasize;
                if ( time_tag == CP56 ) {
                    memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c1_data_buf.c1_data[i].time, 7 );
                    data_pos += 7;
                } else if ( time_tag == CP24 ) {
                    memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c1_data_buf.c1_data[i].time, 3 );
                    data_pos += 3;
                }
                out_buf->bytes = data_pos;
                j++;
                if ( j >= max_num ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "111111111111=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );
                    return j;
                }
            }
        }
    }
    return j;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::SetC2Event ( uint16 dataid, char*data, uint8 datasize, uint8 data_type, char *time )
{
    int i;
    if ( data == 0 || time == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "error para addr is zero\n" );
        return 0;
    }
    for ( i = 0;i < protocol.GetBufAddrRegistNum();i++ ) {
        if ( protocol.member.buf_addr_regist_table[i].pro_id != PRO101 ) {
            continue;
        }
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].port_id = protocol.member.buf_addr_regist_table[i].port_id;
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].dataid = dataid;//GetDataAddr(dataid)->data_addr;
        memcpy ( member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].data, data, datasize );
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].datasize = datasize;
        memcpy ( member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].time, time, 7 );
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].read_flag = EVENT_NOREAD;
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].data_type = data_type;
        member.c2_data_buf.in_cnt++;
        if ( member.c2_data_buf.in_cnt >= MAX_C2_EVENT_NUM ) {
            member.c2_data_buf.in_cnt = 0;
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::GetC2Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag )
{
    int i, k = 0, j = 0;
    out_buf->bytes = 0;
    out_buf->data_num = 0;
    int data_pos = 0;
    uint8 max_num = 0;
    for ( i = 0;i < MAX_C2_EVENT_NUM;i++ ) {
        if ( member.c2_data_buf.c2_data[i].port_id == port_id ) {
            if ( member.c2_data_buf.c2_data[i].read_flag == EVENT_NOREAD && member.c2_data_buf.c2_data[i].data_type == data_type ) {
                memcpy ( &out_buf->buf[k*3+2], ( char* ) &member.c2_data_buf.c2_data[i].dataid, 2 );
                out_buf->buf[k*3+4] = member.c2_data_buf.c2_data[i].datasize;
                if ( time_tag == CP56 ) {
                    out_buf->buf[k*3+4] += 7;
                    max_num = 20;
                } else if ( time_tag == CP24 ) {
                    out_buf->buf[k*3+4] += 3;
                    max_num = 30;
                } else {
                    max_num = 50;
                }
                k++;
                out_buf->data_num += 1;
                memcpy ( &out_buf->buf[0], ( char* ) &out_buf->data_num, 2 );
                if ( out_buf->data_num >= max_num ) {
                    break;
                }
            }
        }
    }
    data_pos = k * 3 + 2;
    j = 0;
    for ( i = 0;i < MAX_C2_EVENT_NUM;i++ ) {
        if ( member.c2_data_buf.c2_data[i].port_id == port_id ) {
            if ( member.c2_data_buf.c2_data[i].read_flag == EVENT_NOREAD && member.c2_data_buf.c2_data[i].data_type == data_type ) {
                member.c2_data_buf.c2_data[i].read_flag = EVENT_READ;
                memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c2_data_buf.c2_data[i].data, member.c2_data_buf.c2_data[i].datasize );
                data_pos += member.c2_data_buf.c2_data[i].datasize;
                if ( time_tag == CP56 ) {
                    memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c2_data_buf.c2_data[i].time, 7 );
                    data_pos += 7;
                } else if ( time_tag == CP24 ) {
                    memcpy ( &out_buf->buf[data_pos], ( char* ) &member.c2_data_buf.c2_data[i].time, 3 );
                    data_pos += 3;
                }
                out_buf->bytes = data_pos;
                j++;
                if ( j >= max_num ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "222222=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );
                    return j;
                }
            }
        }
    }
    return j;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint8  GetSum ( uint8 *pBuf, int16 len )
{
    uint16 i;
    uint8 Sum = 0;
    for ( i = 0;i < len;i++ ) {
        Sum += pBuf[i];
    }
    return Sum;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
int16 Gb101::CheckFrame ( BUF_MUX *buf, uint16 port_id )
{
    uint8 pos;
    uint8 len;
    uint8 sum;
    uint16 addr;
    PRO_LOGIC_BUF *plb = 0;
    plb = SelectLogicBuf ( port_id );
    if ( plb == 0 || buf == 0 ) return 0;
    if ( member.ProtBuf.valid_flag == 1 ) {
        member.ProtBuf.valid_flag = 0;
        if ( member.ProtBuf.parser_buf[0] == 0x68 && member.ProtBuf.parser_buf[3] == 0x68 ) {
            GetKeyWordPositionAndLen ( KEY_WORD_LINKADDR, &pos, &len );
            if ( len == 1 ) {
                addr = ( uint8 ) member.ProtBuf.parser_buf[pos];
                if ( addr == plb->linkaddr || addr == 0xff ) {
                    GetKeyWordPositionAndLen ( KEY_WORD_CTRLWORD, &pos, &len );
                    sum = GetSum ( ( uint8* ) & member.ProtBuf.parser_buf[pos], ( uint8 ) ( member.ProtBuf.parser_frame_len - 6 ) );
                    if ( ( uint8 ) member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2] == sum ) {
                        return APP_HANDLE;
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "check sum error 68in sum=%02x my sum=%02x\n",
                                member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2], sum );
                    }
                } else if ( buf->forward_port_id != 0 ) { //转发口
                    protocol.ForwordSendData ( buf->forward_port_id, member.ProtBuf.parser_buf, member.ProtBuf.parser_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "ForwordSendData\n" );
                    return 0;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "link addr error %d\n", addr );
                }
            } else if ( len == 2 ) {
                addr = ( uint8 ) member.ProtBuf.parser_buf[pos] + ( uint8 ) member.ProtBuf.parser_buf[pos+1] * 256;
                if ( addr == plb->linkaddr || addr == 0xffff ) {
                    GetKeyWordPositionAndLen ( KEY_WORD_CTRLWORD, &pos, &len );
                    sum = GetSum ( ( uint8* ) & member.ProtBuf.parser_buf[pos], ( uint8 ) ( member.ProtBuf.parser_frame_len - 6 ) );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "68in sum=%02x my sum=%02x\n", member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2], sum );
                    if ( ( uint8 ) member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2] == sum ) {
                        return APP_HANDLE;
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "check sum error 68in sum=%02x my sum=%02x\n",
                                member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2], sum );
                    }
                } else if ( buf->forward_port_id != 0 ) { //转发口
                    protocol.ForwordSendData ( buf->forward_port_id, member.ProtBuf.parser_buf, member.ProtBuf.parser_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "ForwordSendData\n" );
                    return 0;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "link addr error %d\n", addr );
                }
            }
            return 0;
        } else if ( member.ProtBuf.parser_buf[0] == 0x10 ) {
            if ( member.pro_cfg.link_addr_size == 1 ) {
                addr = ( uint8 ) member.ProtBuf.parser_buf[2];
                if ( addr == plb->linkaddr || addr == 0xff ) {
                    sum = GetSum ( ( uint8* ) & member.ProtBuf.parser_buf[1], ( uint8 ) ( member.ProtBuf.parser_frame_len - 3 ) );
                    if ( ( uint8 ) member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2] == sum ) {
                        return LINK_HANDLE;
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "check sum error 10in sum=%02x my sum=%02x\n",
                                member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2], sum );
                    }
                } else if ( buf->forward_port_id != 0 ) { //转发口
                    protocol.ForwordSendData ( buf->forward_port_id, member.ProtBuf.parser_buf, member.ProtBuf.parser_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "ForwordSendData\n" );
                    return 0;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "link addr error %d\n", addr );
                }
            } else if ( member.pro_cfg.link_addr_size == 2 ) {
                addr = ( uint8 ) member.ProtBuf.parser_buf[2] + ( uint8 ) member.ProtBuf.parser_buf[3] * 256;
                if ( addr == plb->linkaddr || addr == 0xffff ) {
                    sum = GetSum ( ( uint8* ) & member.ProtBuf.parser_buf[1], ( uint8 ) ( member.ProtBuf.parser_frame_len - 3 ) );
                    if ( ( uint8 ) member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2] == sum ) {
                        return LINK_HANDLE;
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO101, "check sum error 10in sum=%02x my sum=%02x\n",
                                member.ProtBuf.parser_buf[member.ProtBuf.parser_frame_len-2], sum );
                    }
                } else if ( buf->forward_port_id != 0 ) { //转发口
                    protocol.ForwordSendData ( buf->forward_port_id, member.ProtBuf.parser_buf, member.ProtBuf.parser_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO101, "ForwordSendData\n" );
                    return 0;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "link addr error %d\n", addr );
                }
            }
            return 0;
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PARSER_FUNC * Gb101::SearchParserFuncTable ( uint8 type )
{
    int i;
    for ( i = 0;i < MAX_PARSER_FUNC_NUM;i++ ) {
        //PFUNC(TEM_INFO,DEBUG_PRO101,"[%d]intype=%d,lookup type=%d\n",i,type, parser_func[i].type);
        if ( member.parser_func[i].type == type ) {
            return &member.parser_func[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PACK_FUNC * Gb101::SearchPackFuncTable ( uint8 type )
{
    int i;
    for ( i = 0;i < MAX_PACK_FUNC_NUM;i++ ) {
        if ( member.pack_func[i].type == type ) {
            return &member.pack_func[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  根据信息体地址,返回信息体配置表地址
*  @param[in] uint32 dataaddr
*  @param[out] ID_MAP *
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
ID_MAP *Gb101::GetDataId ( uint32 dataaddr )
{
    int i;
    //PFUNC ( TEM_INFO,DEBUG_PRO101, "in data address=%04x\n", dataaddr );
    for ( i = 0;i < MAXYXDATA;i++ ) {
        if ( member.yx_id_map[i].data_addr == dataaddr ) {
            return &member.yx_id_map[i];
        }
    }
    for ( i = 0;i < MAXYCDATA;i++ ) {
        if ( member.yc_id_map[i].data_addr == dataaddr ) {
            return &member.yc_id_map[i];
        }
    }
    for ( i = 0;i < MAXYKDATA;i++ ) {
        if ( member.yk_id_map[i].data_addr == dataaddr ) {
            return &member.yk_id_map[i];
        }
        //PFUNC ( TEM_INFO,DEBUG_PRO101, "in data address[%d]=%04x\n", i, yk_id_map[i].data_addr );
    }
    for ( i = 0;i < MAXPARADATA;i++ ) {
        if ( member.para_id_map[i].data_addr == dataaddr ) {
            return &member.para_id_map[i];
        }
    }
    for ( i = 0;i < MAX_BIT32_NUM;i++ ) {
        if ( member.bit32_id_map[i].data_addr == dataaddr ) {
            return &member.bit32_id_map[i];
        }
    }
 //PFUNC ( TEM_WARNING,DEBUG_PRO101, "not found address=%04x\n", dataaddr );
    return 0;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
ID_MAP *Gb101::GetDataAddr ( uint16 dataid )
{
    int i;
    // PFUNC(TEM_INFO,DEBUG_PRO101,"in data id=%02x\n", dataid);
    for ( i = 0;i < MAXYXDATA;i++ ) {
        if ( member.yx_id_map[i].data_id == dataid ) {
            return &member.yx_id_map[i];
        }
    }
    for ( i = 0;i < MAXYCDATA;i++ ) {
        if ( member.yc_id_map[i].data_id == dataid ) {
            return &member.yc_id_map[i];
        }
    }
    for ( i = 0;i < MAXYKDATA;i++ ) {
        if ( member.yk_id_map[i].data_id == dataid ) {
            return &member.yk_id_map[i];
        }
    }
    for ( i = 0;i < MAXPARADATA;i++ ) {
        if ( member.para_id_map[i].data_id == dataid ) {
            return &member.para_id_map[i];
        }
    }
    for ( i = 0;i < MAX_BIT32_NUM;i++ ) {
        if ( member.bit32_id_map[i].data_id == dataid ) {
            return &member.bit32_id_map[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::SortYxIdMap ( void )
{
    int i;
    for ( i = 0;i < MAXYXDATA;i++ ) {
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::SortYcIdMap ( void )
{
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::SortYkIdMap ( void )
{
    return 1;
}
/**
*************************************************************************************
*  @brief  初始化信息体地址表
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::InitIdMapTable ( void )
{
    int i, j;
    member.pro_dyx_data_num = 0;
    member.pro_yx_data_num = 0;
    member.pro_yc_data_num = 0;
    member.pro_yk_data_num = 0;
//配置遥信信息体地址和ID映射表///////////////////////////////////////////////////////////////////
    Data<BaseDataType> data;
    YXMsgSet *para;
    YCMsgSet *ycPara;
    YKMsgSet *ykPara;
    uint16 paranum;
    uint16 yx_num = 0;
    for ( j = 0;j < PARA_50CA - PARA_50AD + 1;j++ ) {
        user_data.GetData ( PARA_50AD + j, &data );
        para = ( YXMsgSet * ) data.Data;
        paranum = para->Data.Num;
        if ( paranum != 0 ) {
            for ( i = 0;i < paranum;i++ ) {
            	if(para->Data.Define[i].MsgId<0)continue;
                member.yx_id_map[yx_num].bytes = 1;
                member.yx_id_map[yx_num].data_addr = para->Data.Define[i].MsgId;
                member.yx_id_map[yx_num].data_id = para->Data.Define[i].DataId;
                if ( isDyx(para->Data.Define[i].DataId)) {
                    member.yx_id_map[yx_num].type = member.pro_data_type_cfg.dyx_type;
                    member.pro_dyx_data_num += 1;
                } else {
                    member.yx_id_map[yx_num].type = member.pro_data_type_cfg.syx_type;
                    member.pro_yx_data_num += 1;
                }
                yx_num = member.pro_dyx_data_num + member.pro_yx_data_num;
                if ( yx_num >= MAXYXDATA ) {
                     PFUNC ( TEM_WARNING, DEBUG_PRO101, "the yx config num more than %d\n", MAXYXDATA );
                    break;
                }
            }
        } else {
            break;
        }
    }
	PFUNC ( TEM_INFO, DEBUG_PRO101, "total yx num %d syx num:%d,dyx_num:%d\n", yx_num,member.pro_yx_data_num,member.pro_dyx_data_num );
//配置遥测信息体地址和ID映射表//////////////////////////////////////////////////////////////////////
    for ( j = 0;j < PARA_50AC - PARA_5098 + 1;j++ ) {
        user_data.GetData ( PARA_5098 + j, &data );
        ycPara = ( YCMsgSet * ) data.Data;
        paranum = ycPara->Data.Num;
        if ( paranum != 0 ) {
            for ( i = 0;i < paranum;i++ ) {
            	if(ycPara->Data.Define[i].MsgId<0)continue;
                if ( member.pro_data_type_cfg.yc_type == M_ME_NC_1 )
                    member.yc_id_map[i].bytes = 4;
                else
                    member.yc_id_map[i].bytes = 2;
                member.yc_id_map[member.pro_yc_data_num].data_addr = ycPara->Data.Define[i].MsgId;
                member.yc_id_map[member.pro_yc_data_num].data_id = ycPara->Data.Define[i].DataId;
                member.yc_id_map[member.pro_yc_data_num].type = member.pro_data_type_cfg.yc_type;
			    /*
			    PFUNC ( TEM_INFO, DEBUG_PRO101, "yc id map :%d addr:%x id:%x \n", member.pro_yc_data_num,\
			    member.yc_id_map[member.pro_yc_data_num].data_addr, \
			    member.yc_id_map[member.pro_yc_data_num].data_id
			    );
			    */
                member.pro_yc_data_num += 1;
                if ( member.pro_yc_data_num >= MAXYCDATA ) {
                      PFUNC ( TEM_WARNING, DEBUG_PRO101, "the yc config num more than %d\n", MAXYCDATA );
                    break;
                }
            }
        } else {
            break;
        }
    }
    PFUNC ( TEM_INFO, DEBUG_PRO101, "total yc num %d\n", member.pro_yc_data_num );
//配置遥控信息体地址和ID映射表////////////////////////////////////////////////////////////////////
    for ( j = 0;j < 1;j++ ) {
        user_data.GetData ( PARA_50CB + j, &data );
        ykPara = ( YKMsgSet * ) data.Data;
        paranum = ykPara->Data.Num;
        if ( paranum != 0 ) {
            for ( i = 0;i < paranum;i++ ) {
            	if(ykPara->Data.Define[i].MsgId<0)continue;
                member.yk_id_map[i].bytes = 1;
                member.yk_id_map[member.pro_yk_data_num].data_addr = ykPara->Data.Define[i].MsgId;
                member.yk_id_map[member.pro_yk_data_num].data_id = ykPara->Data.Define[i].DataId;
                member.yk_id_map[member.pro_yk_data_num].type = member.pro_data_type_cfg.yk_type;
                member.pro_yk_data_num += 1;
                if ( member.pro_yk_data_num >= MAXYKDATA ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO101, "the yk config num more than %d\n", MAXYKDATA );
                    break;
                }
            }
        } else {
            break;
        }
    }
	PFUNC ( TEM_INFO, DEBUG_PRO101, "total yk num %d\n", member.pro_yk_data_num );
//配置参数信息体地址和ID映射表////////////////////////////////////////////
    Data<BaseDataType> *tb = 0;
    tb = user_data.GetDataTablePtr ( MSG_PARA );
    if ( tb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "Para Table Not Found\n" );
        return 0;
    }
    for ( i = 0;i < user_data.GetDataTableSize ( MSG_PARA );i++ ) {
        member.para_id_map[i].data_addr = tb[i].Id;
        member.para_id_map[i].data_id = tb[i].Id;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
    }
    member.pro_user_para_num = i;
    PFUNC ( TEM_INFO, DEBUG_PRO101, "total pro_user_para_num num %d\n",member.pro_user_para_num );
//配置32位串信息体地址与ID映射表////////////////////////////////////////////////////
    for ( i = 0;i < MAX_BIT32_NUM;i++ ) {
        member.bit32_id_map[i].data_addr = 0x6801 + i;
        member.bit32_id_map[i].data_id = 0x6801 + i;
        member.bit32_id_map[i].bytes = user_data.GetDataSize ( member.bit32_id_map[i].data_id );
        member.bit32_id_map[i].type = M_BO_NA_1;
    }
    member.pro_user_bit32_num = i;
	PFUNC ( TEM_INFO, DEBUG_PRO101, "total 32 bit num %d\n",member.pro_user_bit32_num );
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::ConfigTimeOut ( uint16 port_id )
{
    static uint8 cnt = 0;
    int i;
    if ( port_id == 0 ) return 0;
    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.timeout101[i].port_id == port_id ) {
            member.timeout101[i].en = 1;
            return 1;
        }
    }
    if ( cnt >= MAX_PORT ) {
        cnt = 0;
        return 0;
    }
    member.timeout101[cnt].en = 1;
    member.timeout101[cnt].port_id = port_id;
    cnt++;
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PRO_TIMEOUT* Gb101::SelectTimeOut ( uint16 port_id )
{
    int i;
    if ( port_id == 0 ) return 0;
    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.timeout101[i].port_id == port_id ) {
            return &member.timeout101[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::ConfigLogicBuf ( uint16 port_id )
{
    static uint8 cnt = 0;
    int i;
    if ( port_id == 0 ) return 0;
    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.pro_logic_buf[i].port_id == port_id ) {
            return 1;
        }
    }
    if ( cnt >= MAX_PORT ) {
        PFUNC ( TEM_WARNING,DEBUG_PRO101, "Logic Buf is used over,reconfig\n"  );
        cnt = 0;
        return 0;
    }
    member.pro_logic_buf[cnt].port_id = port_id;
    cnt++;
    return 1;
}
/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PRO_LOGIC_BUF* Gb101::SelectLogicBuf ( uint16 port_id )
{
    int i;
    if ( port_id == 0 ) return 0;
    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.pro_logic_buf[i].port_id == port_id ) {
            return &member.pro_logic_buf[i];
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  向服务缓冲区添加需要等待的服务
*  @param[in] port id 通信端口号
*  @param[in] type		数据级别 1级2级
*  @param[in] s_id		服务号或者叫优先级
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb101::AddServiceBuf ( uint16 port_id, uint8 type, uint8 s_id )
{
    int i;
    PRO_LOGIC_BUF *plb = 0;
    plb = SelectLogicBuf ( port_id );
    if ( plb == 0 ) return 0;
    if ( type == CLASS1 ) {
        for ( i = 0;i < MAX_SERVICE_ID;i++ ) {
            if ( plb->Service1Buf.id[i] == s_id )
                if ( plb->Service1Buf.flag[i] == EVENT_NOREAD )//如果服务缓冲区中有未被服务的同样服务,返回错误
                    {
                    	return 0;
                    }
        }
        plb->Service1Buf.id[plb->Service1Buf.in_cnt] = s_id;
        plb->Service1Buf.flag[plb->Service1Buf.in_cnt] = EVENT_NOREAD;
        plb->Service1Buf.in_cnt++;
        if ( plb->Service1Buf.in_cnt >= MAX_SERVICE_ID ) {
            plb->Service1Buf.in_cnt = 0;
        }
    } else if ( type == CLASS2 ) {
        for ( i = 0;i < MAX_SERVICE_ID;i++ ) {
            if ( plb->Service2Buf.id[i] == s_id )
                if ( plb->Service2Buf.flag[i] == EVENT_NOREAD )
                    return 0;
        }
        plb->Service2Buf.id[plb->Service2Buf.in_cnt] = s_id;
        plb->Service2Buf.flag[plb->Service2Buf.in_cnt] = EVENT_NOREAD;
        plb->Service2Buf.in_cnt++;
        if ( plb->Service2Buf.in_cnt >= MAX_SERVICE_ID ) {
            plb->Service2Buf.in_cnt = 0;
        }
    }
    return 1;
}
/**
*************************************************************************************
*  @brief  获得数据服务等级ID
*  @param[in] uint16 port_id
*  @param[in] uint8 type
*  @param[out] uint8 *s_id
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char  Gb101::GetServiceId ( uint16 port_id, uint8 type, uint8 *s_id )
{
    PRO_LOGIC_BUF *plb = 0;
    plb = SelectLogicBuf ( port_id );
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "no logic buf to parser\n" );
        return 0;
    }
    if ( type == CLASS1 ) {
        if ( plb->Service1Buf.in_cnt != plb->Service1Buf.out_cnt ) {
            *s_id = plb->Service1Buf.id[plb->Service1Buf.out_cnt];
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "before:L=%d,C=%d:%d\n", gb101.member.LastDataClass1Id, *s_id&0x0f, port_id );
            if ( ( !JudgeDataService ( *s_id&0x0f, &gb101.member.LastDataClass1Id ) ) && gb101.member.LastDataClass1Id != 0 ) {//service no change and last serId is not zero
                *s_id = gb101.member.LastDataClass1Id;
                return 1;
            }
            if ( gb101.member.LastDataClass1Id == 0 ) {
                *s_id = 0;
                return 0;
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "after:L=%d,C=%d:%d\n", gb101.member.LastDataClass1Id, *s_id&0x0f, port_id );
            plb->Service1Buf.flag[plb->Service1Buf.out_cnt] = EVENT_READ;
            plb->Service1Buf.out_cnt++;
            if ( plb->Service1Buf.out_cnt >= MAX_SERVICE_ID ) {
                plb->Service1Buf.out_cnt = 0;
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "[%d][%d]pt=%d,type=%d,s_id=%d\n" ,plb->Service1Buf.in_cnt, plb->Service1Buf.out_cnt, port_id, type, *s_id&0x0f );
            return 1;
        }
        if ( gb101.member.LastDataClass1Id != 0 ) {
            *s_id = gb101.member.LastDataClass1Id;
            return 1;
        }
    } else if ( type == CLASS2 ) {
        if ( plb->Service2Buf.in_cnt != plb->Service2Buf.out_cnt ) {
            *s_id = plb->Service2Buf.id[plb->Service2Buf.out_cnt];
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "before:L=%d,C=%d:%d\n", gb101.member.LastDataClass2Id, *s_id&0x0f, port_id );
            if ( ( !JudgeDataService ( *s_id&0x0f, &gb101.member.LastDataClass2Id ) ) && gb101.member.LastDataClass2Id != 0 ) {//service no change and last serId is not zero
                *s_id = gb101.member.LastDataClass2Id;
                return 1;
            }
            if ( gb101.member.LastDataClass2Id == 0 ) {
                *s_id = 0;
                return 0;
            }
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "after:L=%d,C=%d:%d\n", gb101.member.LastDataClass2Id, *s_id&0x0f, port_id );
            plb->Service2Buf.flag[plb->Service2Buf.out_cnt] = EVENT_READ;
            plb->Service2Buf.out_cnt++;
            if ( plb->Service2Buf.out_cnt >= MAX_SERVICE_ID ) {
                plb->Service2Buf.out_cnt = 0;
            }
            /*
            PFUNC ( TEM_INFO, DEBUG_PRO101, "[%d][%d]pt=%d,type=%d,s_id=%d\n" ,\
                    plb->Service1Buf.in_cnt, plb->Service1Buf.out_cnt, port_id, type, *s_id&0x0f );
			*/                    
            return 2;
        }
        if ( gb101.member.LastDataClass2Id != 0 ) {
            *s_id = gb101.member.LastDataClass2Id;
            return 1;
        }
    }
    return 0;
}
int Gb101::ParaCheck ( void )
{
    int i;
///////////////////////////////////protocol frame key word len and trans style check/////////////
    if ( member.pro_cfg.time_tag > CP56 ) {
        member.pro_cfg.time_tag = CP56;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "Time Tag is Larger than 7,will be set by %d\n", member.pro_cfg.time_tag );
    }
    if ( member.pro_cfg.common_unit_addr_size > 2 ) {
        member.pro_cfg.common_unit_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "common_unit_addr_size is Larger than 2,will be set by %d\n", member.pro_cfg.common_unit_addr_size );
    } else if ( member.pro_cfg.common_unit_addr_size < 1 ) {
        member.pro_cfg.common_unit_addr_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "common_unit_addr_size is less than 1,will be set by %d\n", member.pro_cfg.common_unit_addr_size );
    }
    if ( member.pro_cfg.datasign_addr_size > 3 ) {
        member.pro_cfg.datasign_addr_size = 3;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "datasign_addr_size is Larger than 3,will be set by %d\n", member.pro_cfg.datasign_addr_size );
    } else if ( member.pro_cfg.datasign_addr_size < 2 ) {
        member.pro_cfg.datasign_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "datasign_addr_size is less than 2,will be set by %d\n", member.pro_cfg.datasign_addr_size );
    }
    if ( member.pro_cfg.link_addr_size > 2 ) {
        member.pro_cfg.link_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "link_addr_size is Larger than 2,will be set by %d\n", member.pro_cfg.link_addr_size );
    } else if ( member.pro_cfg.link_addr_size < 1 ) {
        member.pro_cfg.link_addr_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "link_addr_size is less than 1,will be set by %d\n", member.pro_cfg.link_addr_size );
    }
    if ( member.pro_cfg.send_cause_size > 2 ) {
        member.pro_cfg.send_cause_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "send_cause_size is Larger than 2,will be set by %d\n", member.pro_cfg.send_cause_size );
    } else if ( member.pro_cfg.send_cause_size < 1 ) {
        member.pro_cfg.send_cause_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "send_cause_size is less than 1,will be set by %d\n", member.pro_cfg.send_cause_size );
    }
    if ( member.pro_cfg.trans_style != EQ_TRANS && member.pro_cfg.trans_style != UNEQ_TRANS ) {
        member.pro_cfg.trans_style = UNEQ_TRANS;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "trans_style is invalid,will be set by %d\n", member.pro_cfg.trans_style );
    } else if ( member.pro_cfg.trans_style < 1 ) {
        member.pro_cfg.trans_style = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "trans_style is less than 1,will be set by %d\n", member.pro_cfg.trans_style );
    }
    if ( member.pro_cfg.enable_interrupt_process > 1 ) {
        member.pro_cfg.enable_interrupt_process = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "enable_interrupt_process is invalid,will be set by %d\n", member.pro_cfg.enable_interrupt_process );
    }
///////////////////////////////////////total ask data type check///////
    if ( member.pro_data_type_cfg.syx_type != M_SP_NA_1 && member.pro_data_type_cfg.syx_type != M_PS_NA_1 ) {
        member.pro_data_type_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "syx_type is invalid,will be set by %d\n", member.pro_data_type_cfg.syx_type );
    }
    if ( member.pro_data_type_cfg.dyx_type != M_DP_NA_1 ) {
        member.pro_data_type_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "dyx_type is invalid,will be set by %d\n", member.pro_data_type_cfg.dyx_type );
    }
    if ( member.pro_data_type_cfg.yc_type != M_ME_NA_1
            && member.pro_data_type_cfg.yc_type != M_ME_NB_1
            && member.pro_data_type_cfg.yc_type != M_ME_NC_1 ) {
        member.pro_data_type_cfg.yc_type = M_ME_NB_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "yc_type is invalid,will be set by %d\n", member.pro_data_type_cfg.yc_type );
    }
    if ( member.pro_data_type_cfg.yk_type != C_SC_NA_1
            && member.pro_data_type_cfg.yk_type != C_DC_NA_1 ) {
        member.pro_data_type_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "yk_type is invalid,will be set by %d\n", member.pro_data_type_cfg.yk_type );
    }
////////////////////////////////////////class 1 data type check///////////
    if ( member.class1_datatype_cfg.syx_type != M_SP_NA_1
            && member.class1_datatype_cfg.syx_type != M_PS_NA_1
            && member.class1_datatype_cfg.syx_type != M_SP_TA_1
            && member.class1_datatype_cfg.syx_type != M_SP_TB_1 ) {
        member.class1_datatype_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class1 syx_type is invalid,will be set by %d\n", member.class1_datatype_cfg.syx_type );
    }
    if ( member.class1_datatype_cfg.dyx_type != M_DP_NA_1
            && member.class1_datatype_cfg.syx_type != M_DP_TA_1
            && member.class1_datatype_cfg.syx_type != M_DP_TB_1 ) {
        member.class1_datatype_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class1 dyx_type is invalid,will be set by %d\n", member.class1_datatype_cfg.dyx_type );
    }
    if ( member.class1_datatype_cfg.yc_type != M_ME_NA_1
            && member.class1_datatype_cfg.yc_type != M_ME_NB_1
            && member.class1_datatype_cfg.yc_type != M_ME_NC_1
            && member.class1_datatype_cfg.yc_type != M_ME_TA_1
            && member.class1_datatype_cfg.yc_type != M_ME_TB_1
            && member.class1_datatype_cfg.yc_type != M_ME_TC_1
            && member.class1_datatype_cfg.yc_type != M_ME_TD_1
            && member.class1_datatype_cfg.yc_type != M_ME_TE_1
            && member.class1_datatype_cfg.yc_type != M_ME_TF_1 ) {
        member.class1_datatype_cfg.yc_type = M_ME_NB_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class1 yc_type is invalid,will be set by %d\n", member.class1_datatype_cfg.yc_type );
    }
    if ( member.class1_datatype_cfg.yk_type != C_SC_NA_1
            && member.class1_datatype_cfg.yk_type != C_DC_NA_1 ) {
        member.class1_datatype_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class1 yk_type is invalid,will be set by %d\n", member.class1_datatype_cfg.yk_type );
    }
    ////////////////////////class2 data type check////////////////
    if ( member.class2_datatype_cfg.syx_type != M_SP_NA_1
            && member.class2_datatype_cfg.syx_type != M_PS_NA_1
            && member.class2_datatype_cfg.syx_type != M_SP_TA_1
            && member.class2_datatype_cfg.syx_type != M_SP_TB_1 ) {
        member.class2_datatype_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class2 syx_type is invalid,will be set by %d\n", member.class2_datatype_cfg.syx_type );
    }
    if ( member.class2_datatype_cfg.dyx_type != M_DP_NA_1
            && member.class2_datatype_cfg.dyx_type != M_DP_TA_1
            && member.class2_datatype_cfg.dyx_type != M_DP_TB_1 ) {
        member.class2_datatype_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class2 dyx_type is invalid,will be set by %d\n", member.class2_datatype_cfg.dyx_type );
    }
    if ( member.class2_datatype_cfg.yc_type != M_ME_NA_1
            && member.class2_datatype_cfg.yc_type != M_ME_NB_1
            && member.class2_datatype_cfg.yc_type != M_ME_NC_1
            && member.class2_datatype_cfg.yc_type != M_ME_TA_1
            && member.class2_datatype_cfg.yc_type != M_ME_TB_1
            && member.class2_datatype_cfg.yc_type != M_ME_TC_1
            && member.class2_datatype_cfg.yc_type != M_ME_TD_1
            && member.class2_datatype_cfg.yc_type != M_ME_TE_1
            && member.class2_datatype_cfg.yc_type != M_ME_TF_1 ) {
        member.class2_datatype_cfg.yc_type = M_ME_TE_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class2 yc_type is invalid,will be set by %d\n", member.class2_datatype_cfg.yc_type );
    }
    if ( member.class2_datatype_cfg.yk_type != C_SC_NA_1
            && member.class2_datatype_cfg.yk_type != C_DC_NA_1 ) {
        member.class2_datatype_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "class1 yk_type is invalid,will be set by %d\n", member.class2_datatype_cfg.yk_type );
    }
//////////////////////////////data class grade and priority check////////////
    for ( i = 0;i < 8;i++ ) {
        if ( member.ProDataClassCfgTable[i].grade != CLASS1 && member.ProDataClassCfgTable[i].grade != CLASS2 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "please confirm the data class is set by \"1\" or \"2\",you set [%d]\n", member.ProDataClassCfgTable[i].grade );
        }
        if ( member.ProDataClassCfgTable[i].pri == 0 )  {
            member.ProDataClassCfgTable[i].pri = i + 1;
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "The Priority is forbidden setting by zero\n", member.ProDataClassCfgTable[i].grade );
        }
    }
/////////////////////////data_group_cfg check//////////////////////
    for ( i = 0;i < 6;i++ ) {
        if ( member.data_group_cfg[i].start_group_id == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "start_group_id is forbidden setting by zero,\n" );
        }
        if ( ( member.data_group_cfg[i].max_group_num + member.data_group_cfg[i].start_group_id - 1 ) > 16 )  {
            PFUNC ( TEM_WARNING, DEBUG_PRO101, "The Group Id adding group Num is more than 16\n" );
        }
    }
    return 0;
}
/**
*************************************************************************************
*  @brief  xml 节点内容向参数缓存转换
*  @param[in] xmlNodePtr node
*  @return  成功返回0，
*  @note 此函数递归调用，注意xml文件的子节点级数不能太深，要根据硬件资源来定。
*  @see  NULL
**************************************************************************************
*/
int Gb101:: ParaConfig ( xmlNodePtr node )
{
    while ( node ) {
//////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"LinkAddrSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.link_addr_size = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "LinkAddrSize=%d\n", member.pro_cfg.link_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"CommonAddrSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.common_unit_addr_size = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "CommonAddrSize=%d\n", member.pro_cfg.common_unit_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TransCauseSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.send_cause_size = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "TransCauseSize=%d\n", member.pro_cfg.send_cause_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"DataSignSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.datasign_addr_size = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "DataSignSize=%d\n", member.pro_cfg.datasign_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
//////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TransType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.trans_style = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"ResendTimes" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.resend_times = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "ResendTimes=%d\n", member.pro_cfg.resend_times );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TsYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.syx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TsYxType=%d\n", member.pro_data_type_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TdYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.dyx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TdYxType=%d\n", member.pro_data_type_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TYcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.yc_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TYcType=%d\n", member.pro_data_type_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TYkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.yk_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TYkType=%d\n", member.pro_data_type_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////////////class 1 data type
        if ( xmlStrcmp ( node->name, BAD_CAST"C1sYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.syx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C1sYxType=%d\n", member.class1_datatype_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C1dYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.dyx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C1dYxType=%d\n", member.class1_datatype_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C1YcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.yc_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C1YcType=%d\n", member.class1_datatype_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C1YkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.yk_type = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "C1YkType=%d\n", member.class1_datatype_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////////////class 2 data type
        if ( xmlStrcmp ( node->name, BAD_CAST"C2sYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.syx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C2sYxType=%d\n", member.class2_datatype_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C2dYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.dyx_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C2dYxType=%d\n", member.class2_datatype_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C2YcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.yc_type = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "C2YcType=%d\n", member.class2_datatype_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"C2YkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.yk_type = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "C2YkType=%d\n", member.class2_datatype_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
//////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TimeTag" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.time_tag = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "TimeTag=%d\n", member.pro_cfg.time_tag );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"InitOverGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[0].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[0].type = INIT_OVER_DATA;
            member.ProDataClassCfgTable[0].fp = PackInitOverData;
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "InitOverGrade=%d\n", member.ProDataClassCfgTable[0].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"AckTotalAskGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[1].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[1].type = ACK_STATION_ASK_DATA;
            member.ProDataClassCfgTable[1].fp = PackAckSationAskData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "AckTotalAskGrade=%d\n", member.ProDataClassCfgTable[1].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YxChangeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[2].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[2].type = CHANGE_YX_DATA;
            member.ProDataClassCfgTable[2].fp = PackChangeYxData;
            //    PFUNC ( TEM_INFO, DEBUG_PRO101, "YxChangeGrade=%d\n", member.ProDataClassCfgTable[2].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"CtrlAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[3].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[3].type = CTRL_RET_DATA;
            member.ProDataClassCfgTable[3].fp = PackCtrlRetData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "CtrlAckGrade=%d\n", member.ProDataClassCfgTable[3].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"SoeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[4].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[4].type = SOE_DATA;
            member.ProDataClassCfgTable[4].fp = PackSoeData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "SoeGrade=%d\n", member.ProDataClassCfgTable[4].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TimeAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[5].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[5].type = TIME_TB_DATA;
            member.ProDataClassCfgTable[5].fp = PackTimeTbData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "TimeAckGrade=%d\n", member.ProDataClassCfgTable[5].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"GroupAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[6].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[6].type = ACK_GROUP_ASK_DATA;
            member.ProDataClassCfgTable[6].fp = PackAckGroupAskData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "GroupAckGrade=%d\n", member.ProDataClassCfgTable[6].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YcChangeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[7].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[7].type = CHANGE_YC_DATA;
            member.ProDataClassCfgTable[7].fp = PackChangeYcData;
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YcChangeGrade=%d\n", member.ProDataClassCfgTable[7].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"InitOverPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[0].pri = atoi ( ( char* ) content );
            //    PFUNC ( TEM_INFO, DEBUG_PRO101, "InitOverPri=%d\n", member.ProDataClassCfgTable[0].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"AckTotalAskPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[1].pri = atoi ( ( char* ) content );
            //    PFUNC ( TEM_INFO, DEBUG_PRO101, "AckTotalAskPri=%d\n", member.ProDataClassCfgTable[1].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YxChangePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[2].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YxChangePri=%d\n", member.ProDataClassCfgTable[2].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"CtrlAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[3].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "CtrlAckPri=%d\n", member.ProDataClassCfgTable[3].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"SoePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[4].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "SoePri=%d\n", member.ProDataClassCfgTable[4].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"TimeAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[5].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "TimeAckPri=%d\n", member.ProDataClassCfgTable[5].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"GroupAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[6].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "GroupAckPri=%d\n", member.ProDataClassCfgTable[6].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YcChangePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[7].pri = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YcChangePri=%d\n", member.ProDataClassCfgTable[7].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"ServiceType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.enable_interrupt_process = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "ServiceType=%d\n", member.pro_cfg.enable_interrupt_process );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
//////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].start_group_id = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YxGroupStartId=%d\n", member.data_group_cfg[0].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].max_group_num = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YxGroupNum=%d\n", member.data_group_cfg[0].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].group_data_num = atoi ( ( char* ) content );
            //   PFUNC ( TEM_INFO, DEBUG_PRO101, "YxGroupDataNum=%d\n", member.data_group_cfg[0].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
///////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].start_group_id = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "YcGroupStartId=%d\n", member.data_group_cfg[1].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].max_group_num = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "YcGroupNum=%d\n", member.data_group_cfg[1].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].group_data_num = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "YcGroupDataNum=%d\n", member.data_group_cfg[1].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].start_group_id = atoi ( ( char* ) content );
            //  PFUNC ( TEM_INFO, DEBUG_PRO101, "StepGroupStartId=%d\n", member.data_group_cfg[2].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].max_group_num = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "StepGroupNum=%d\n", member.data_group_cfg[2].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].group_data_num = atoi ( ( char* ) content );
            PFUNC ( TEM_INFO, DEBUG_PRO101, "StepGroupDataNum=%d\n", member.data_group_cfg[2].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
//////////////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].start_group_id = atoi ( ( char* ) content );
            // PFUNC ( TEM_INFO, DEBUG_PRO101, "BcdGroupStartId=%d\n", member.data_group_cfg[3].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "BcdGroupNum=%d\n", member.data_group_cfg[3].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "BcdGroupDataNum=%d\n", member.data_group_cfg[3].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
///////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "StGroupStartId=%d\n", member.data_group_cfg[4].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "StGroupNum=%d\n", member.data_group_cfg[4].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "StGroupDataNum=%d\n", member.data_group_cfg[4].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
//////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "ResGroupStartId=%d\n", member.data_group_cfg[5].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "ResGroupNum=%d\n", member.data_group_cfg[5].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO101, "ResGroupDataNum=%d\n", member.data_group_cfg[5].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }
///////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"para" ) == 0 ) {
            if ( ParaConfig ( node->children ) != -1 ) {
                // PFUNC ( TEM_INFO,DEBUG_PRO101, " 888digui //child name is \"%s\"\n", node->children->name );
            }
        }
        node = node->next;
    }
    if ( node == 0 ) {
        // PFUNC ( TEM_INFO,DEBUG_PRO101, "@@@@@@@@@@@@@@@@@@ digui return\n" );
        return -1;
    }
    return 0;
}
/**
*************************************************************************************
*  @brief 从参数xml文件中读取参数
*  @return  成功返回0，
*  @note
*  @see  NULL
**************************************************************************************
*/
int  Gb101::ReadCfg ( void )
{
    xmlDocPtr doc;
    xmlNodePtr root;
	if(isFileExist((char*)CONFIG_XML_PATH)==0)
    doc = xmlReadFile ( CONFIG_XML_PATH, "UTF-8", XML_PARSE_RECOVER );
    else{
    	copy(DEFAULT_CONFIG_XML_PATH,CONFIG_XML_PATH);
    }
	if(isFileExist((char *)CONFIG_XML_PATH)==0)
    doc = xmlReadFile ( CONFIG_XML_PATH, "UTF-8", XML_PARSE_RECOVER );	
    else{
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "Fail open %s\n", CONFIG_XML_PATH );
    	return -1;
    }
    root = xmlDocGetRootElement ( doc );
    if ( root == NULL ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "Empty document\n" );
        xmlFreeDoc ( doc );
        return -1;
    }
    PFUNC ( TEM_INFO, DEBUG_PRO101, "root name is \"%s\"\n", root->name );
    while ( root ) {
        if ( xmlStrcmp ( root->name, BAD_CAST"para" ) == 0 ) {
            ParaConfig ( root->children );
        }
        root = root->next;
    }
    xmlFreeDoc ( doc );
    if ( member.xml_para_cnt != XML_PARA_NUM ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "The Para Read Fail! respect num=%d,read num=%d\n", XML_PARA_NUM, member.xml_para_cnt );
        return -1;
    }
    return 0;
}
int ParaModifyConfig ( xmlNodePtr node, char *data, char *paraname )
{
    uint8 len ;
    while ( node ) {
//////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST ( paraname ) ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            xmlChar content1[8];
            memset ( ( char* ) content1, 0, 8 );
            len = strlen ( data );
            if ( len <= 8 ) {
                memcpy ( ( char* ) content1, data, len );
            } else {
                PFUNC ( TEM_INFO, DEBUG_PRO101, "the para len too long  %d>%d\n", len , 8 );
            }
            if ( xmlStrcmp ( content1, content ) == 0 ) {
                return 0;
            } else {
                xmlNodeSetContent ( node, content1 );
                PFUNC ( TEM_INFO, DEBUG_PRO101, "Write %s=%s\n", paraname , data );
                xmlFree ( content );
                return 0;
            }
        }
///////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"para" ) == 0 ) {
            if ( ParaModifyConfig ( node->children, data , paraname ) != -1 ) {
            }
        }
        node = node->next;
    }
    if ( node == 0 ) {
        return -1;
    }
    return 0;
}
int ModifyParaXmlFile ( char *filedir, char *para, char *paraname )
{
    xmlNodePtr root;
    xmlDocPtr doc;
    doc = xmlReadFile ( filedir, "UTF-8", XML_PARSE_RECOVER );
    if ( doc == NULL ) {
        fprintf ( stderr, "Fail Open xml file %s\n", filedir );
        return -1;
    }
    root = xmlDocGetRootElement ( doc );
    if ( root == NULL ) {
        fprintf ( stderr, "Empty document\n" );
        xmlFreeDoc ( doc );
        return -1;
    }
    while ( root ) {
        if ( xmlStrcmp ( root->name, BAD_CAST"para" ) == 0 ) {
            ParaModifyConfig ( root->children , para, paraname );
        }
        root = root->next;
    }
    xmlSaveFile ( filedir, doc );
    xmlFree ( doc );
    return 1;
}
int Gb101::WriteCfg ( uint32 para_id , char *para )
{
    char paraname[32];
    char paraStr[8];
    char filename[128];
    char states = 0;
    sprintf ( filename, "%s", CONFIG_XML_PATH );
    PFUNC ( TEM_INFO, DEBUG_PRO101, "\n" );
    if ( para_id == PARA_5100 ) {
        Pro101Para ext_para;
        memcpy ( ( char* ) &ext_para.Data, para, user_data.GetDataSize ( para_id ) );
        if ( ext_para.Data.TransMode != member.pro_cfg.trans_style ) {//′??·??
            sprintf ( paraname, "%s", "TransType" );
            sprintf ( paraStr, "%d", ext_para.Data.TransMode );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( ext_para.Data.AddressLen != member.pro_cfg.link_addr_size ) {//t·μ?·3¤??
            sprintf ( paraname, "%s", "LinkAddrSize" );
            sprintf ( paraStr, "%d", ext_para.Data.AddressLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( ext_para.Data.TransCauseLen != member.pro_cfg.send_cause_size ) {//′????3¤??
            sprintf ( paraname, "%s", "TransCauseSize" );
            sprintf ( paraStr, "%d", ext_para.Data.TransCauseLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( ext_para.Data.MsgAddrLen != member.pro_cfg.datasign_addr_size ) {//х??μ?·3¤??
            sprintf ( paraname, "%s", "DataSignSize" );
            sprintf ( paraStr, "%d", ext_para.Data.MsgAddrLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( ext_para.Data.RepeatTimes != member.pro_cfg.resend_times ) {//?·￠′??sprintf(paraname,"%s","ResendTimes");
            sprintf ( paraname, "%s", "ResendTimes" );
            sprintf ( paraStr, "%d", ext_para.Data.RepeatTimes );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( ext_para.Data.CommAddresLen != member.pro_cfg.common_unit_addr_size ) {//1?12μ￥?μ?·3¤??
            sprintf ( paraname, "%s", "CommonAddrSize" );
            sprintf ( paraStr, "%d", ext_para.Data.CommAddresLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        states = 1;
    } else if ( para_id == PARA_5139 ) {
        ComBasePara com_para;
        memcpy ( ( char* ) &com_para.Data, para, user_data.GetDataSize ( para_id ) );
        PFUNC ( TEM_INFO, DEBUG_PRO101, "YcGroupDataNum=%d,\n%d\n%d\n",
                com_para.Data.PointsYCGroup, com_para.Data.PointsYXGroup, com_para.Data.YcDataType );
        if ( com_para.Data.YcDataType == 1 ) {//1?12μ￥?μ?·3¤??
        	member.pro_data_type_cfg.yc_type=M_ME_NB_1;
        	if(member.pro_cfg.time_tag==CP56)
        	member.class2_datatype_cfg.yc_type =M_ME_TE_1 ;
        	if(member.pro_cfg.time_tag==CP24)
        	member.class2_datatype_cfg.yc_type =M_ME_TB_1 ;
        }else if( com_para.Data.YcDataType == 2 ){
			member.pro_data_type_cfg.yc_type=M_ME_NA_1;
        	if(member.pro_cfg.time_tag==CP56)
			member.class2_datatype_cfg.yc_type =M_ME_TD_1 ;
        	if(member.pro_cfg.time_tag==CP24)
			member.class2_datatype_cfg.yc_type =M_ME_TA_1 ;
		}else if( com_para.Data.YcDataType == 3 ){
			member.pro_data_type_cfg.yc_type=M_ME_NC_1;
        	if(member.pro_cfg.time_tag==CP56)
			member.class2_datatype_cfg.yc_type =M_ME_TF_1 ;
        	if(member.pro_cfg.time_tag==CP24)
			member.class2_datatype_cfg.yc_type =M_ME_TC_1 ;
		}
        sprintf ( paraname, "%s", "TYcType" );
        sprintf ( paraStr, "%d", member.pro_data_type_cfg.yc_type );
        ModifyParaXmlFile ( filename, paraStr, paraname );
        sprintf ( paraname, "%s", "C2YcType" );
        sprintf ( paraStr, "%d", member.class2_datatype_cfg.yc_type );
        ModifyParaXmlFile ( filename, paraStr, paraname );
        if ( com_para.Data.PointsYCGroup != member.data_group_cfg[1].group_data_num ) {//1?12μ￥?μ?·3¤??
            sprintf ( paraname, "%s", "YcGroupDataNum" );
            sprintf ( paraStr, "%d", com_para.Data.PointsYCGroup );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        if ( com_para.Data.PointsYXGroup != member.data_group_cfg[0].group_data_num ) {//1?12μ￥?μ?·3¤??
            sprintf ( paraname, "%s", "YxGroupDataNum" );
            sprintf ( paraStr, "%d", com_para.Data.PointsYXGroup );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }
        states = 1;
    } else {
        states = 2;
    }
    if ( states == 1 ) {
        member.InitOver = 0;
        InitGb101();
    } else if ( states == 2 ) {
        InitIdMapTable();
        InitGroup();
    }
    //reinitial the gb101 para and run from begin
    return 0;
}
uint32 Gb101::IsMyPara ( uint32 paraid )
{
    if ( paraid == 0 ) {
        return INVALID_PARA;
    }
    if ( paraid >= PARA_5098 && paraid <= PARA_50CB ) { //三遥属性配置参数
        return paraid;
    }
    if ( paraid == PARA_5100 || paraid == PARA_5139 ) { //规约参数
        return paraid;
    }
    if ( paraid == PARA_5097 ) { //规约参数
        return paraid;
    }
    return INVALID_PARA;
}
