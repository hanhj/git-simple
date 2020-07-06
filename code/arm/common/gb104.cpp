#include "gb104.h"
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "protocol.h"
#include "gb101.h"
#include "utils.h"

extern Gb101 gb101;
Gb104 gb104;

char Build_U_S_Frame ( char *out_buf, uint16 *out_len );
char Build_I_Frame ( uint8 type , char *out_buf, uint16 *out_len  , char *in_buf, uint16 in_len );
char SetCuaseWord_4 ( uint8 cause );
static char PrepareProData_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
static char JudgeDataService_4 ( uint8 class_data_id, uint8 *last_class_data_id );


///////////////////////////////////////////////自定义的打包程序///////////////////////////
static uint8 PackClass1Data_4 ( uint16 port_id, char *out_buf, uint16 *out_len );//1级数据打包
static uint8 PackClass2Data_4 ( uint16 port_id, char *out_buf, uint16 *out_len );//2级数据打包
static uint8 PackInitOverData_4 ( char *out_buf, uint16 *out_len );//初始化结束数据打包
static uint8 PackChangeYxData_4 ( char *out_buf, uint16 *out_len );//变化的遥信数据打包
static uint8 PackCtrlRetData_4 ( char *out_buf, uint16 *out_len );//控制返回的数据打包
static uint8 PackChangeYcData_4 ( char *out_buf, uint16 *out_len );//变化的遥测数据打包
static uint8 PackTimeTbData_4 ( char *out_buf, uint16 *out_len );//时间同步数据打包
static uint8 PackSoeData_4 ( char *out_buf, uint16 *out_len );//SOE数据打包
static uint8 PackAckSationAskData_4 ( char *out_buf, uint16 *out_len );//总召唤数据打包
static uint8 PackAckGroupAskData_4 ( char *out_buf, uint16 *out_len );//组召唤数据打包
static uint8 PackStepRetData_4 ( char *out_buf, uint16 *out_len );

/////////////////////////////////规约打包程序///////////////////////////////////////////////
static char Pack_M_SP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  1//>∶＝单点信息
static char Pack_M_SP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  2//>∶＝带时标的单点信息
static char Pack_M_DP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  3//>∶＝双点信息
static char Pack_M_DP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  4//>∶＝带时标的双点信息
#if 1

static char Pack_M_ST_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  5//>∶＝步位置信忿
static char Pack_M_ST_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  6//>∶＝带时标的步位置信忿
#endif
static char Pack_M_BO_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  7//>∶＝32比特勿
#if 0

static char Pack_M_BO_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  8//>∶＝带时标的32比特䶿
#endif
static char Pack_M_ME_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  9//>∶＝测量值，规一化便
static char Pack_M_ME_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  10//>∶＝带时标的测量便规一化便
static char Pack_M_ME_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  11//>∶＝测量值，标度化便
static char Pack_M_ME_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  12//>∶＝带时标的测量值，标度化便
static char Pack_M_ME_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  13//>∶＝测量值，短浮点数
static char Pack_M_ME_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  14//>∶＝带时标的测量值，短浮点数
#if 0

static char Pack_M_IT_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  15//>∶＝累计酿
static char Pack_M_IT_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  16//>∶＝带时标的累计酿
static char Pack_M_EP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  17//>∶＝带时标的继电保护设备事件
static char Pack_M_EP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  18//>∶＝带时标的继电保护设备成组启动事件
static char Pack_M_EP_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  19//>∶＝带时标的继电保护设备成组输出电路信息
static char Pack_M_PS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  20//>∶＝带变位检出的成组单点信息
static char Pack_M_ME_ND_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  21//>∶＝测量值，不带品质描述词的规一化便
#endif

static char Pack_M_SP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  30//>∶＝带CP56Time2a时标的单点信忿
static char Pack_M_DP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  31//>∶＝带CP56Time2a时标的双点信忿
#if 0

static char Pack_M_ST_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  32//>∶＝带CP56Time2a时标的步位置信息
static char Pack_M_BO_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  33//>∶＝带CP56Time2a时标瘿2比特䶿
#endif
static char Pack_M_ME_TD_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  34//>∶＝带CP56Time2a时标的测量值，规一化便
static char Pack_M_ME_TE_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  35//>∶＝带CP56Time2a时标的测量值，标度化便
static char Pack_M_ME_TF_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  36//>∶＝带CP56Time2a时标的测量值，短浮点数
#if 0

static char Pack_M_IT_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  37//>∶＝带CP56Time2a时标的累计量
static char Pack_M_EP_TD_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  38//>∶＝带CP56Time2a时标的继电保护设备事乿
static char Pack_M_EP_TE_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  39//>∶＝带CP56Time2a时标的继电保护设备成组启动事乿
static char Pack_M_EP_TF_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  40//>∶＝带CP56Time2a时标的继电保护装置成组输出电路信忿
#endif
// 控制方向的过程信息//////////////////////////////////////////
static char Parser_C_SC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  45//>∶＝单点命令
static char Parser_C_DC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  46//>∶＝双点命令
static char Parser_C_RC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  47//>∶＝步调节命令
//（直接控制命令）//////////////////////////////////////////////
static char Parser_C_SE_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  48//>∶＝设定值命令、规一化便
static char Parser_C_SE_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  49//>∶＝设定值命令、标度化便
static char Parser_C_SE_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  50//>∶＝设定值命令，短浮点数
static char Parser_C_BO_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  51//>∶＝32比特䶿

//////////////带时标的控制方向信息//////////////////////////////////
static char Parser_C_SC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  58//>∶＝单点命令
static char Parser_C_DC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  59//>∶＝双点命令
static char Parser_C_RC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  60//>∶＝步调节命令
//（直接控制命令）//////////////////////////////////////////////
static char Parser_C_SE_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  61//>∶＝设定值命令、规一化便
static char Parser_C_SE_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  62//>∶＝设定值命令、标度化便
static char Parser_C_SE_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  63//>∶＝设定值命令，短浮点数
static char Parser_C_BO_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  64//>∶＝32比特䶿


static char Parser_C_BO_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  150//>∶＝比特䶿
// 在监视方向的系统命令（站特定参数）///////////////////////////
static char Pack_M_EI_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  70//>∶＝初始化结替
// 在控制方向的系统命令（站特定参数）///////////////////////////
static char Parser_C_IC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  100//>∶＝总召唤命令（包括分组召唤ﺿ
static char Parser_C_CI_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  101//>∶＝累计量召唤命令）
static char Parser_C_RD_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  102//>∶＝读命乿
static char Parser_C_CS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  103//>∶＝时钟同步命令
static char Parser_C_TS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  104//>∶＝测试命令
static char Parser_C_RP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  105//>∶＝复位进程命令
static char Parser_C_CD_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  106//>∶＝延时获得命令
// 在控制方向的参数命令（站特定参数）//////////////////////////
static char Parser_P_ME_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  110//>∶＝测量值参数，规一化便
static char Parser_P_ME_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  111//>∶＝测量值参数，标度化便
static char Parser_P_ME_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  112//>∶＝测量值参数，短浮点数
static char Parser_P_AC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  113//>∶＝参数激沿
// 文件传输（站特定参数）//////////////////////////////////
static char Pack_F_FR_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  120//>∶＝文件准备就绪
static char Pack_F_SR_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  121//>∶＝节准备就繿
static char Parser_F_SC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  122//>∶＝召唤目录，选择文件，召唤文件召唤节
static char Pack_F_LS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  123//>∶＝最后的节，最后的欿
static char Parser_F_AF_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  124//>∶＝认可文件，认可节
static char Pack_F_SG_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  125//>∶＝段
static char Pack_F_DR_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  126//>∶＝目录
static char Parser_P_XX_XX_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );//  222//>∶＝终端参数块设置



/**
*************************************************************************************
*  @brief  单点信息组帧
*  @author  liht
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表组帧成功,可以发送out_buf中的数据
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Pack_M_SP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  1//>∶＝单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_SP_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_SP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  2//>∶＝带时标的单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_SP_TA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_DP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  3//>∶＝双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_DP_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_DP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  4//>∶＝带时标的双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_DP_TA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

#if 1

static char Pack_M_ST_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  5//>∶＝步位置信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ST_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_ST_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  6//>∶＝带时标的步位置信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ST_TA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

#endif
static char Pack_M_BO_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  7//>∶＝32比特单
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
    gb104.member.TerSt.data=d3->Data;
    memcpy ( &ter_st_data[5], ( char* ) &gb104.member.TerSt.data, 4 );
    st_data_len = 9;
    ret = PrepareProData_4 ( ter_st_data, st_data_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_BO_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

#if 0

static char Pack_M_BO_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  8//>∶＝带时标的32比特串
{
    return 1;
}

#endif
static char Pack_M_ME_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  9//>∶＝测量值，规一化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_ME_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  10//>∶＝带时标的测量值,规一化值
{
    return 0;
}

static char Pack_M_ME_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  11//>∶＝测量值，标度化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_NB_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_ME_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  12//>∶＝带时标的测量值，标度化值
{
    return 0;
}

static char Pack_M_ME_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  13//>∶＝测量值，短浮点数
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_NC_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}


static char Pack_M_ME_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  14//>∶＝带时标的测量值，短浮点数
{
    return 0;
}

#if 0

static char Pack_M_IT_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  15//>∶＝累计量
{
    return 0;
}

static char Pack_M_IT_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  16//>∶＝带时标的累计量
{
    return 0;
}

static char Pack_M_EP_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  17//>∶＝带时标的继电保护设备事件
{
    return 0;
}

static char Pack_M_EP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  18//>∶＝带时标的继电保护设备成组启动事件
{
    return 0;
}

static char Pack_M_EP_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  19//>∶＝带时标的继电保护设备成组输出电路信息
{
    return 0;
}

static char Pack_M_PS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  20//>∶＝带变位检出的成组单点信息
{
    return 0;
}

static char Pack_M_ME_ND_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  21//>∶＝测量值，不带品质描述词的规一化值
{
    return 0;
}

#endif

static char Pack_M_SP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  30//>∶＝带CP56Time2a时标的单点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_SP_TB_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_DP_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  31//>∶＝带CP56Time2a时标的双点信息
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_DP_TB_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

#if 0

static char Pack_M_ST_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  32//>∶＝带CP56Time2a时标的步位置信息
{
    return 0;
}

static char Pack_M_BO_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  33//>∶＝带CP56Time2a时标的32比特串
{
    return 0;
}

#endif
static char Pack_M_ME_TD_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  34//>∶＝带CP56Time2a时标的测量值，规一化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    // PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_TD_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_ME_TE_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  35//>∶＝带CP56Time2a时标的测量值，标度化值
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_TE_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Pack_M_ME_TF_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  36//>∶＝带CP56Time2a时标的测量值，短浮点数
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    // PFUNC ( TEM_INFO,DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        Build_I_Frame ( M_ME_TF_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

#if 0

static char Pack_M_IT_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  37//>∶＝带CP56Time2a时标的累计量
{
    return 0;
}

static char Pack_M_EP_TD_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  38//>∶＝带CP56Time2a时标的继电保护设备事件
{
    return 0;
}

static char Pack_M_EP_TE_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  39//>∶＝带CP56Time2a时标的继电保护设备成组启动事件
{
    return 0;
}

// 控制方向的过程信息
static char Pack_M_EP_TF_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  40//>∶＝带CP56Time2a时标的继电保护装置成组输出电路信息

{
    return 0;
}

#endif

/**
*************************************************************************************
*  @brief  单双控制命令统一解析处理函数
*  @author  liht
*  @param[in] uint8 type
*  @param[in] char *in_buf
*  @param[in] uint16 in_len
*  @param[out] char *out_buf
*  @param[out] u)int16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char ParserCtrl_4 ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    DCOWORD dcoword;
    uint8 pos = 0;
    uint8 len = 0;
    uint8 msgdata;
    char datasign[4];
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    PRO_TIMEOUT_4 *to = 0;
    ID_MAP *imap = 0;
    uint8 ret = 0;
    to = gb104.SelectTimeOut ( gb104.member.CurPortId );

    if ( to == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no timeout buf to parser\n" );
        return 0;
    }

    if ( protocol.member.YkPortNum != 0 && gb104.member.CurPortId != protocol.member.YkPortNum ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Yk is busy by other station other=%d,my=%d\n", protocol.member.YkPortNum, gb104.member.CurPortId );
        return 0;
    }

    protocol.member.YkPortNum = gb104.member.CurPortId;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            SetCuaseWord_4 ( CAUSE_Unknowntype );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;
        }
    }

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    plb->pro_logic_buf.M_type = type;

    plb->pro_logic_buf.yk_cmd = type;

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    dcoword.data = ( uint8 ) in_buf[in_len-1];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memset ( datasign, 0, 4 );

    memcpy ( datasign, &in_buf[pos], len );

   
   //PFUNC ( TEM_INFO,DEBUG_PRO104,"cause code:%d,s_e:%d\n",plb->pro_logic_buf.M_SendCause.BsCause.code,dcoword.DcoWord.S_E);
   
    switch ( plb->pro_logic_buf.M_SendCause.BsCause.code ) {

    case CAUSE_Act:

        if ( dcoword.DcoWord.S_E == 1 ) {
            //选择
            if(to->yk_timeout_flag==1){//遥控选择超时
            	plb->pro_logic_buf.yk_datasign=0;
            	plb->pro_logic_buf.DCO_Word.DcoWord.S_E=0;
            	to->yk_timeout_flag=0;
            }
            //PFUNC ( TEM_INFO,DEBUG_PRO104,"%d,%d\n",plb->pro_logic_buf.yk_datasign , plb->pro_logic_buf.DCO_Word.DcoWord.S_E);
            if ( plb->pro_logic_buf.yk_datasign == 0 && plb->pro_logic_buf.DCO_Word.DcoWord.S_E == 0 ) {
                to->yk_en = 1;
                to->yk_cnt = 0;
                plb->pro_logic_buf.yk_st = 1;
                memcpy ( ( char* ) &plb->pro_logic_buf.yk_datasign, datasign, len );
                plb->pro_logic_buf.DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );
				//PFUNC ( TEM_INFO,DEBUG_PRO104, "yk_datasign=%x dco=%d,timeout=%d,cause=%x\n",
					   //plb->pro_logic_buf.yk_datasign,plb->pro_logic_buf.DCO_Word.data,to->yk_timeout_flag, plb->pro_logic_buf.M_SendCause.data);

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }
				
				msgdata=plb->pro_logic_buf.DCO_Word.data;
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
        } else if ( dcoword.DcoWord.S_E == 0 && plb->pro_logic_buf.DCO_Word.DcoWord.S_E == 1 ) {
            if ( memcmp ( datasign, ( char* ) &plb->pro_logic_buf.yk_datasign, len ) == 0 && to->yk_timeout_flag == 0 ) {
                //执行
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->pro_logic_buf.DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );
				//PFUNC ( TEM_INFO,DEBUG_PRO104, "yk_datasign=%x dco=%d,timeout=%d,cause=%x\n",
					  // plb->pro_logic_buf.yk_datasign,plb->pro_logic_buf.DCO_Word.data,to->yk_timeout_flag, plb->pro_logic_buf.M_SendCause.data);

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

				msgdata=plb->pro_logic_buf.DCO_Word.data;
				if(type==C_SC_NA_1)//单双点命令区分采用第五位来区分0代表单命令,1代表双命令
					cbi(msgdata,4);
				else if(type==C_DC_NA_1)
					sbi(msgdata,4);
                if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &msgdata, 1 ) ) {
                    ret = 0;
                    break;
                }

                plb->pro_logic_buf.yk_datasign = 0;//

                plb->pro_logic_buf.yk_st = 2;
            } else {
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->pro_logic_buf.DCO_Word.data = 0;
                to->yk_timeout_flag = 0;
                ret = 0;
                plb->pro_logic_buf.yk_datasign = 0;//
                plb->pro_logic_buf.yk_st = 0;
                protocol.member.YkPortNum = 0;
            }
        } else {
            to->yk_en = 0;
            to->yk_cnt = 0;
            memset ( ( char* ) &plb->pro_logic_buf.yk_datasign, 0, 4 );
            plb->pro_logic_buf.DCO_Word.data = 0;
            to->yk_timeout_flag = 0;
            ret = 0;
            plb->pro_logic_buf.yk_st = 0;
            protocol.member.YkPortNum = 0;
        }

        break;

    case CAUSE_Deact:
        ret = 1;
        to->yk_en = 0;
        to->yk_cnt = 0;

        to->yk_timeout_flag = 0;
        plb->pro_logic_buf.yk_st = 4;
        memcpy ( ( char* ) &plb->pro_logic_buf.yk_datasign, datasign, 4 );
        imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );

        if ( imap == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
            SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;

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

        plb->pro_logic_buf.DCO_Word.data = 0;

        memset ( ( char* ) &plb->pro_logic_buf.yk_datasign, 0, 4 );
        ret = 1;
        break;

    default:
        SetCuaseWord_4 ( CAUSE_Unknowncause );
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
        Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
        return 1;
    }

    //请求后续服务
    class_data_type = CTRL_RET_DATA;

    if ( ret == 1 ) {
        tab = gb104.SearchClassTable ( CTRL_RET_DATA );

        if ( tab != 0 ) {//向规约后续服务列表中添加服务号
            if ( tab->grade == CLASS1 ) {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, class_data_type );
            } else {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, class_data_type );
            }
        }
    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "yk ack wait command ok\n" );

    return 0;

    //}

}

static char Parser_C_SC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  45//>∶＝单点命令
{
    if ( ParserCtrl_4 ( C_SC_NA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_DC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  46//>∶＝双点命令
{
    if ( ParserCtrl_4 ( C_DC_NA_1, in_buf, in_len, out_buf, out_len ) ) {

        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_RC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  47//>∶＝步调节命令
{
    RCOWORD rcoword;
    uint8 pos = 0;
    uint8 len = 0;
    char datasign[4];
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    PRO_TIMEOUT_4 *to = 0;
    ID_MAP *imap = 0;
    uint8 ret = 0;
    to = gb104.SelectTimeOut ( gb104.member.CurPortId );

    if ( to == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no timeout buf to parser\n" );
        return 0;
    }

    if ( protocol.member.BtjPortNum != 0 && gb104.member.CurPortId != protocol.member.BtjPortNum ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Yk is busy by other station other=%d,my=%d\n", protocol.member.BtjPortNum, gb104.member.CurPortId );
        return 0;
    }

    protocol.member.BtjPortNum = gb104.member.CurPortId;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RC_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            SetCuaseWord_4 ( CAUSE_Unknowntype );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( C_RC_NA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;
        }
    }

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    plb->pro_logic_buf.M_type = C_RC_NA_1;

    plb->pro_logic_buf.btj_cmd = C_RC_NA_1;

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    rcoword.data = ( uint8 ) in_buf[in_len-1];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memset ( datasign, 0, 4 );

    memcpy ( datasign, &in_buf[pos], len );

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "yk_datasign=%d dco=%d,timeout=%d\n",plb->yk_datasign,plb->DCO_Word.data,to->yk_timeout_flag);
    switch ( plb->pro_logic_buf.M_SendCause.BsCause.code ) {

    case CAUSE_Act:

        if ( rcoword.RcoWord.S_E == 1 ) {
            //选择
            if ( plb->pro_logic_buf.btj_datasign == 0 && plb->pro_logic_buf.RCO_Word.RcoWord.S_E == 0 ) {
                to->btj_en = 1;
                to->btj_cnt = 0;
                plb->pro_logic_buf.btj_st = 1;
                memcpy ( ( char* ) &plb->pro_logic_buf.btj_datasign, datasign, len );
                plb->pro_logic_buf.RCO_Word.data = rcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( C_RC_NA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

                if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &plb->pro_logic_buf.RCO_Word.data, 1 ) ) {
                    ret = 0;
                    break;
                }
            } else {
                ///已经选择没有执行
                ret = 0;
            }
        } else if ( rcoword.RcoWord.S_E == 0 && plb->pro_logic_buf.RCO_Word.RcoWord.S_E == 1 ) {
            if ( memcmp ( datasign, ( char* ) &plb->pro_logic_buf.btj_datasign, len ) == 0 && to->btj_timeout_flag == 0 ) {
                //执行
                to->btj_en = 0;
                to->btj_cnt = 0;
                plb->pro_logic_buf.RCO_Word.data = rcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( C_RC_NA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

                if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &plb->pro_logic_buf.RCO_Word.data , 1 ) ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "msg send error\n" );
                    ret = 0;
                    break;
                }

                plb->pro_logic_buf.btj_datasign = 0;//

                plb->pro_logic_buf.btj_st = 2;
            } else {
                to->btj_en = 0;
                to->btj_cnt = 0;
                plb->pro_logic_buf.RCO_Word.data = 0;
                to->btj_timeout_flag = 0;
                ret = 0;
                plb->pro_logic_buf.btj_datasign = 0;//
                plb->pro_logic_buf.btj_st = 0;
                protocol.member.BtjPortNum = 0;
            }
        } else {
            to->yk_en = 0;
            to->yk_cnt = 0;
            memset ( ( char* ) &plb->pro_logic_buf.btj_datasign, 0, 4 );
            plb->pro_logic_buf.RCO_Word.data = 0;
            to->btj_timeout_flag = 0;
            ret = 0;
            plb->pro_logic_buf.btj_st = 0;
            protocol.member.BtjPortNum = 0;
        }

        break;

    case CAUSE_Deact:
        ret = 1;
        to->btj_en = 0;
        to->btj_cnt = 0;

        to->btj_timeout_flag = 0;
        plb->pro_logic_buf.btj_st = 4;
        memcpy ( ( char* ) &plb->pro_logic_buf.btj_datasign, datasign, 4 );
        imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

        if ( imap == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );
            SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( C_RC_NA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;

        }

        if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &rcoword.data , 1 ) ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "msg send error\n" );
            ret = 0;
            break;
        }

        plb->pro_logic_buf.RCO_Word.data = 0;

        memset ( ( char* ) &plb->pro_logic_buf.btj_datasign, 0, 4 );
        ret = 1;
        break;

    default:
        SetCuaseWord_4 ( CAUSE_Unknowncause );
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
        Build_I_Frame ( C_RC_NA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
        return 1;
    }

    //请求后续服务
    class_data_type = STEP_RET_DATA;

    if ( ret == 1 ) {
        tab = gb104.SearchClassTable ( STEP_RET_DATA );

        if ( tab != 0 ) {//向规约后续服务列表中添加服务号
            if ( tab->grade == CLASS1 ) {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, class_data_type );
            } else {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, class_data_type );
            }
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "step ack wait command ok\n" );

    return 0;
}

//#define C_SC_TA_1  58//>∶＝单点命令,带时标
//#define C_DC_TA_1  59//>∶＝双点命令带时标
//#define C_RC_TA_1  60//>∶＝步调节命令带时标
static char CheckTimeValid ( CP56Time2a time )
{
    CP56Time2a lt;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "frame time %d-%d-%d  %d:%d:%dms @week:%d  \n"
            , time.year + 2000
            , time.month
            , time.day_week&0x1f
            , time.hour_su&0x1f
            , time.minute_iv&0x3f
            , time.millisecond
            , ( time.day_week >> 5 ) &0x07 );
    timer.GetCP56Time ( &lt );
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "locoal time %d-%d-%d  %d:%d:%dms @week:%d  \n"
            , lt.year + 2000
            , lt.month
            , lt.day_week&0x1f
            , lt.hour_su&0x1f
            , lt.minute_iv&0x3f
            , lt.millisecond
            , ( lt.day_week >> 5 ) &0x07 );

    if ( time.month > lt.month ) {
        return 1;
    } else if ( time.month == lt.month ) {

        if ( time.day_week > ( lt.day_week&0x1f ) ) {
            return 1;
        } else if ( time.day_week == ( lt.day_week&0x1f ) ) {

            if ( time.hour_su > ( lt.hour_su&0x1f ) ) {
                return 1;
            } else if ( time.hour_su == ( lt.hour_su&0x1f ) ) {

                if ( time.minute_iv > ( lt.minute_iv&0x3f ) ) {
                    return 1;
                } else if ( time.minute_iv == ( lt.minute_iv&0x3f ) ) {

                    if ( time.millisecond >= lt.millisecond ) {
                        return 1;
                    } else {
                        return 0;

                    }
                }
            }
        }
    }

    return 0;
}

static char ParserCtrl_T4 ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    DCOWORD dcoword;
    uint8 pos = 0;
    uint8 len = 0;
    char datasign[4];
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    CP56Time2a _t;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    PRO_TIMEOUT_4 *to = 0;
    ID_MAP *imap = 0;
    uint8 ret = 0;

    memcpy ( ( char* ) &_t, &in_buf[in_len-7], 7 );

    if ( !CheckTimeValid ( _t ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the frame timeout,cmd cancel\n" );
        return 0;

    }

    to = gb104.SelectTimeOut ( gb104.member.CurPortId );

    if ( to == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no timeout buf to parser\n" );
        return 0;
    }

    if ( protocol.member.YkPortNum != 0 && gb104.member.CurPortId != protocol.member.YkPortNum ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Yk is busy by other station other=%d,my=%d\n", protocol.member.YkPortNum, gb104.member.CurPortId );
        return 0;
    }

    protocol.member.YkPortNum = gb104.member.CurPortId;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            SetCuaseWord_4 ( CAUSE_Unknowntype );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;
        }
    }

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    plb->pro_logic_buf.M_type = type;

    plb->pro_logic_buf.yk_cmd = type;

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk_cause data=%d code=%d,timeout=%d\n", plb->pro_logic_buf.M_SendCause.data,
            plb->pro_logic_buf.M_SendCause.BsCause.code );

    dcoword.data = ( uint8 ) in_buf[in_len-1-7];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memset ( datasign, 0, 4 );

    memcpy ( datasign, &in_buf[pos], len );

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );

    switch ( plb->pro_logic_buf.M_SendCause.BsCause.code ) {

    case CAUSE_Act:

        if ( dcoword.DcoWord.S_E == 1 ) {
            //选择
            if(to->yk_timeout_flag==1){//遥控选择超时
            	plb->pro_logic_buf.yk_datasign=0;
            	plb->pro_logic_buf.DCO_Word.DcoWord.S_E=0;
            	to->yk_timeout_flag=0;
            }            
            if ( plb->pro_logic_buf.yk_datasign == 0 && plb->pro_logic_buf.DCO_Word.DcoWord.S_E == 0 ) {
                to->yk_en = 1;
                to->yk_cnt = 0;
                plb->pro_logic_buf.yk_st = 1;
                memcpy ( ( char* ) &plb->pro_logic_buf.yk_datasign, datasign, len );
                plb->pro_logic_buf.DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

                if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &plb->pro_logic_buf.DCO_Word.data, 1 ) ) {
                    ret = 0;
                    break;
                }

                PFUNC ( TEM_WARNING, DEBUG_PRO104, "sel yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );
            } else {
                ///已经选择没有执行
                ret = 0;
            }
        } else if ( dcoword.DcoWord.S_E == 0 && plb->pro_logic_buf.DCO_Word.DcoWord.S_E == 1 ) {
            if ( memcmp ( datasign, ( char* ) &plb->pro_logic_buf.yk_datasign, len ) == 0 && to->yk_timeout_flag == 0 ) {
                //执行
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->pro_logic_buf.DCO_Word.data = dcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

                if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &plb->pro_logic_buf.DCO_Word.data , 1 ) ) {
                    ret = 0;
                    break;
                }

                plb->pro_logic_buf.yk_datasign = 0;//

                plb->pro_logic_buf.yk_st = 2;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, " done yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );
            } else {
                to->yk_en = 0;
                to->yk_cnt = 0;
                plb->pro_logic_buf.DCO_Word.data = 0;
                to->yk_timeout_flag = 0;
                ret = 0;
                plb->pro_logic_buf.yk_datasign = 0;//
                plb->pro_logic_buf.yk_st = 0;
                protocol.member.YkPortNum = 0;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, " addr no equal yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );
            }
        } else {
            to->yk_en = 0;
            to->yk_cnt = 0;
            memset ( ( char* ) &plb->pro_logic_buf.yk_datasign, 0, 4 );
            plb->pro_logic_buf.DCO_Word.data = 0;
            to->yk_timeout_flag = 0;
            ret = 0;
            plb->pro_logic_buf.yk_st = 0;
            protocol.member.YkPortNum = 0;
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "error yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );
        }

        break;

    case CAUSE_Deact:
        ret = 1;
        to->yk_en = 0;
        to->yk_cnt = 0;

        to->yk_timeout_flag = 0;
        plb->pro_logic_buf.yk_st = 4;
        memcpy ( ( char* ) &plb->pro_logic_buf.yk_datasign, datasign, 4 );
        imap = gb104.GetDataId ( plb->pro_logic_buf.yk_datasign );

        if ( imap == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk [%x]'s id not found\n", plb->pro_logic_buf.yk_datasign );
            SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;

        }

        if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_YK, imap->data_id, ( char* ) &dcoword.data , 1 ) ) {
            ret = 0;
            break;
        }

        plb->pro_logic_buf.DCO_Word.data = 0;

        memset ( ( char* ) &plb->pro_logic_buf.yk_datasign, 0, 4 );
        ret = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "cancel yk_datasign=%d dco=%d,timeout=%d\n", plb->pro_logic_buf.yk_datasign, plb->pro_logic_buf.DCO_Word.data, to->yk_timeout_flag );
        break;

    default:
        SetCuaseWord_4 ( CAUSE_Unknowncause );
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
        Build_I_Frame ( type, out_buf, out_len, &in_buf[pos], in_len - pos );
        return 1;
    }

    //请求后续服务
    class_data_type = CTRL_RET_DATA;

    if ( ret == 1 ) {
        tab = gb104.SearchClassTable ( CTRL_RET_DATA );

        if ( tab != 0 ) {//向规约后续服务列表中添加服务号
            if ( tab->grade == CLASS1 ) {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, class_data_type );
            } else {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, class_data_type );
            }
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk ack wait command ok\n" );

    return 0;
}

static char Parser_C_SC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  58//>∶＝单点命令
{
    if ( ParserCtrl_T4 ( C_SC_TA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_DC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  59//>∶＝双点命令
{
    if ( ParserCtrl_T4 ( C_DC_TA_1, in_buf, in_len, out_buf, out_len ) ) {

        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_RC_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  60//>∶＝步调节命令
{
    RCOWORD rcoword;
    uint8 pos = 0;
    uint8 len = 0;
    char datasign[4];
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    PRO_TIMEOUT_4 *to = 0;
    ID_MAP *imap = 0;
    uint8 ret = 0;
    CP56Time2a _t;

    memcpy ( ( char* ) &_t, &in_buf[in_len-7], 7 );

    if ( !CheckTimeValid ( _t ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the frame timeout,btj cmd cancel\n" );
        return 0;

    }

    to = gb104.SelectTimeOut ( gb104.member.CurPortId );

    if ( to == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no timeout buf to parser\n" );
        return 0;
    }

    if ( protocol.member.BtjPortNum != 0 && gb104.member.CurPortId != protocol.member.BtjPortNum ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Btj is busy by other station other=%d,my=%d\n", protocol.member.BtjPortNum, gb104.member.CurPortId );
        return 0;
    }

    protocol.member.BtjPortNum = gb104.member.CurPortId;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RC_TA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            SetCuaseWord_4 ( CAUSE_Unknowntype );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( C_RC_TA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;
        }
    }

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    plb->pro_logic_buf.M_type = C_RC_TA_1;

    plb->pro_logic_buf.btj_cmd = C_RC_TA_1;

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    rcoword.data = ( uint8 ) in_buf[in_len-8];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memset ( datasign, 0, 4 );

    memcpy ( datasign, &in_buf[pos], len );

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "yk_datasign=%d dco=%d,timeout=%d\n",plb->yk_datasign,plb->DCO_Word.data,to->yk_timeout_flag);
    switch ( plb->pro_logic_buf.M_SendCause.BsCause.code ) {

    case CAUSE_Act:

        if ( rcoword.RcoWord.S_E == 1 ) {
            //选择
            if ( plb->pro_logic_buf.btj_datasign == 0 && plb->pro_logic_buf.RCO_Word.RcoWord.S_E == 0 ) {
                to->btj_en = 1;
                to->btj_cnt = 0;
                plb->pro_logic_buf.btj_st = 1;
                memcpy ( ( char* ) &plb->pro_logic_buf.btj_datasign, datasign, len );
                plb->pro_logic_buf.RCO_Word.data = rcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );
                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( C_RC_TA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;

                }

                if ( ! ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &plb->pro_logic_buf.RCO_Word.data, 1 ) ) {
                    ret = 0;
                    break;
                }
            } else {
                ///已经选择没有执行
                ret = 0;
            }
        } else if ( rcoword.RcoWord.S_E == 0 && plb->pro_logic_buf.RCO_Word.RcoWord.S_E == 1 ) {
            if ( memcmp ( datasign, ( char* ) &plb->pro_logic_buf.btj_datasign, len ) == 0 && to->btj_timeout_flag == 0 ) {
                //执行
                to->btj_en = 0;
                to->btj_cnt = 0;
                plb->pro_logic_buf.RCO_Word.data = rcoword.data;
                ret = 1;
                imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

                if ( imap == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );

                    SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
                    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
                    Build_I_Frame ( C_RC_TA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
                    return 1;
                }

                if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &plb->pro_logic_buf.RCO_Word.data , 1 ) ) {
                    ret = 0;
                    break;
                }

                plb->pro_logic_buf.btj_datasign = 0;//

                plb->pro_logic_buf.btj_st = 2;
            } else {
                to->btj_en = 0;
                to->btj_cnt = 0;
                plb->pro_logic_buf.RCO_Word.data = 0;
                to->btj_timeout_flag = 0;
                ret = 0;
                plb->pro_logic_buf.btj_datasign = 0;//
                plb->pro_logic_buf.btj_st = 0;
                protocol.member.BtjPortNum = 0;
            }
        } else {
            to->btj_en = 0;
            to->btj_cnt = 0;
            memset ( ( char* ) &plb->pro_logic_buf.btj_datasign, 0, 4 );
            plb->pro_logic_buf.RCO_Word.data = 0;
            to->btj_timeout_flag = 0;
            ret = 0;
            plb->pro_logic_buf.btj_st = 0;
            protocol.member.BtjPortNum = 0;
        }

        break;

    case CAUSE_Deact:
        ret = 1;
        to->btj_en = 0;
        to->btj_cnt = 0;

        to->btj_timeout_flag = 0;
        plb->pro_logic_buf.btj_st = 4;
        memcpy ( ( char* ) &plb->pro_logic_buf.btj_datasign, datasign, 4 );
        imap = gb104.GetDataId ( plb->pro_logic_buf.btj_datasign );

        if ( imap == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj [%x]'s id not found\n", plb->pro_logic_buf.btj_datasign );
            SetCuaseWord_4 ( CAUSE_Unknowndataaddr );
            gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
            Build_I_Frame ( C_RC_TA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
            return 1;

        }

        if ( !ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, imap->data_id, ( char* ) &rcoword.data , 1 ) ) {
            ret = 0;
            break;
        }

        plb->pro_logic_buf.RCO_Word.data = 0;

        memset ( ( char* ) &plb->pro_logic_buf.btj_datasign, 0, 4 );
        ret = 1;
        break;

    default:
        SetCuaseWord_4 ( CAUSE_Unknowncause );
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );
        Build_I_Frame ( C_RC_TA_1, out_buf, out_len, &in_buf[pos], in_len - pos );
        return 1;
    }

    //请求后续服务
    class_data_type = STEP_RET_DATA;

    if ( ret == 1 ) {
        tab = gb104.SearchClassTable ( STEP_RET_DATA );

        if ( tab != 0 ) {//向规约后续服务列表中添加服务号
            if ( tab->grade == CLASS1 ) {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, class_data_type );
            } else {
                gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, class_data_type );
            }
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "step ack wait command ok\n" );

    return 0;
}


// 在控制方向的参数命令（站特定参数）//////////////////////////////////////////////
/**
*************************************************************************************
*  @brief  解析设置参数规约帧
*  @author  liht
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
char ParserSetPara_4 ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
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
    PRO_LOGIC_BUF04 *plb = 0;
    char data3[256];
    uint16 data3len = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }


    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill formal para Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memcpy ( data3, &in_buf[pos], in_len - pos );

    data3len = in_len - pos ;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d len=%d\n", pos, len );

    //* ( uint16* ) &data[0] = vsq.BsVsq.Number;
    data[0] = vsq.BsVsq.Number;

    data[1] = 0;

    plb->pro_logic_buf.VSQ_Word.data = vsq.data;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "para_num=%d \n", vsq.BsVsq.Number );

    data_pos = 2;

    if ( vsq.BsVsq.SQ == 1 ) {//信息体地址连续

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;

            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }

        para_len = gap;

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "para-len=%d \n", para_len );


        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            i_map = gb104.GetDataId ( data_addr + i );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                return 0;
            }

            //fill the table header
            //  data_id = data_id + i;
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id + i;
            data[4+3*i] = para_len;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

            data2len += para_len;
        }
    } else {//信息体地址不连续
        gap = ( in_len - pos ) / vsq.BsVsq.Number;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "gap=%d \n", gap  );

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "para_len=%d \n", para_len  );

            //fill the data buf header
            i_map = gb104.GetDataId ( data_addr );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            // * ( uint16* ) &data[2+3*i] = data_id;
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            data[4+3*i] = para_len;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len );

            pos += gap;

            data2len += para_len;
        }
    }

    //merge the the transform para buf
    memcpy ( in_buf, data, data_pos );

    memcpy ( &in_buf[data_pos], data2, data2len );

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_pos=%d,data2len=%d\n", data_pos, data2len );

    in_len = data_pos + data2len;

    //do the interface
    gb104.member.pc_mse_nx = type;

    protocol.member.pc_mse_nx = gb104.member.pc_mse_nx;

    if ( ProSetPara ( in_buf, in_len, out_buf, out_len ) ) {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Actcon );
    }


    else {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Deact );

    }

///////////////////////////////////////////

    //BuildFrame1016 ( out_buf, out_len );


    //BuildFrame6816 ( type, out_buf , out_len, data3, data3len );
    Build_I_Frame ( type, out_buf , out_len, data3, data3len );

    return 1;
}

static char Parser_C_SE_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //（直接控制命令）  48//>∶＝设定值命令、规一化值
{
    if ( ParserSetPara_4 ( C_SE_NA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_SE_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  49//>∶＝设定值命令、标度化值
{
    if ( ParserSetPara_4 ( C_SE_NB_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_SE_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  50//>∶＝设定值命令，短浮点数
{
    if ( ParserSetPara_4 ( C_SE_NC_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

//#define C_SE_TA_1  61//（直接控制命令） ＝设定值命令、规一化值带时标
//#define C_SE_TB_1  62//>∶＝设定值命令、标度化值带时标
//#define C_SE_TC_1  63//>∶＝设定值命令，短浮点数带时标
// 在控制方向的参数命令（站特定参数）//////////////////////////////////////////////
/**
*************************************************************************************
*  @brief  解析设置参数规约帧
*  @author  liht
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
char ParserSetPara_T4 ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
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
    PRO_LOGIC_BUF04 *plb = 0;
    char data3[256];
    uint16 data3len = 0;
    CP56Time2a _t;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    memcpy ( ( char* ) &_t, &in_buf[in_len-7], 7 );

    if ( !CheckTimeValid ( _t ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the frame timeout,cmd cancel\n" );
        return 0;

    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill formal para Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memcpy ( data3, &in_buf[pos], in_len - pos );

    data3len = in_len - pos ;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d len=%d\n", pos, len );

    //* ( uint16* ) &data[0] = vsq.BsVsq.Number;
    data[0] = vsq.BsVsq.Number;

    data[1] = 0;

    plb->pro_logic_buf.VSQ_Word.data = vsq.data;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "para_num=%d \n", vsq.BsVsq.Number );

    data_pos = 2;

    if ( vsq.BsVsq.SQ == 1 ) {//信息体地址连续

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - 7 - pos - 3 ) / vsq.BsVsq.Number;

            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - 7 - pos - 2 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }

        para_len = gap;

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "para-len=%d \n", para_len );


        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb104.GetDataId ( data_addr + i );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                return 0;
            }

            //data_id = data_id + i;
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id + i;
            data[4+3*i] = para_len;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

            data2len += para_len;
        }
    } else {//信息体地址不连续
        gap = ( in_len - 7 - pos ) / vsq.BsVsq.Number;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "gap=%d \n", gap  );

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "para_len=%d \n", para_len  );

            //fill the data buf header
            i_map = gb104.GetDataId ( data_addr );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            // * ( uint16* ) &data[2+3*i] = data_id;
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            data[4+3*i] = para_len;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len );

            pos += gap;

            data2len += para_len;
        }
    }

    //merge the the transform para buf
    memcpy ( in_buf, data, data_pos );

    memcpy ( &in_buf[data_pos], data2, data2len );

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_pos=%d,data2len=%d\n", data_pos, data2len );

    in_len = data_pos + data2len;

    //do the interface
    gb104.member.pc_mse_nx = type - 13;

    protocol.member.pc_mse_nx = gb104.member.pc_mse_nx;

    if ( ProSetPara ( in_buf, in_len, out_buf, out_len ) ) {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Actcon );
    }


    else {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Deact );

    }

///////////////////////////////////////////

    //BuildFrame1016 ( out_buf, out_len );


    //BuildFrame6816 ( type, out_buf , out_len, data3, data3len );
    Build_I_Frame ( type, out_buf , out_len, data3, data3len );

    return 1;
}


static char Parser_C_SE_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //（直接控制命令）  48//>∶＝设定值命令、规一化值
{
    if ( ParserSetPara_T4 ( C_SE_TA_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_SE_TB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  49//>∶＝设定值命令、标度化值
{
    if ( ParserSetPara_T4 ( C_SE_TB_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_SE_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  50//>∶＝设定值命令，短浮点数
{
    if ( ParserSetPara_T4 ( C_SE_TC_1, in_buf, in_len, out_buf, out_len ) ) {
        return 1;
    } else {
        return 0;
    }
}

static char Parser_C_BO_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  51//>∶＝32比特串
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
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb != 0 ) {
        if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
            if ( in_buf[pos] != C_BO_NC_1 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
                return 0;
            }
        }

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

        gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

        if ( len == 2 ) {
            plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
            data_pos = pos + 2;
        } else {
            plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
            data_pos = pos + 1;
        }

        if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
            SetCuaseWord_4 ( CAUSE_Actterm );
        } else {
            SetCuaseWord_4 ( CAUSE_Actcon );
        }

        //Fill Table Header
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

        vsq.data = in_buf[pos];

        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

        memcpy ( data3, &in_buf[pos], in_len - pos  );

        data3len = in_len - pos ;

        data[0] = vsq.BsVsq.Number;

        data[1] = 0;

        //* ( uint16* ) &data[0] = vsq.BsVsq.Number;

        plb->pro_logic_buf.VSQ_Word.data = vsq.data;

        data_pos = 2;

        if ( vsq.BsVsq.SQ == 1 ) {//paser the sequence data

            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;

                memcpy ( &data_addr, &in_buf[pos], 3 );
            } else {
                gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
                memcpy ( &data_addr, &in_buf[pos], 2 );
            }

            para_len = gap;



            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                //fill the table header
                i_map = gb104.GetDataId ( data_addr + i );

                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                }

                //data_id  = data_id + i;
                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

                //* ( uint16* ) &data[2+3*i] = data_id + i;
                data[4+3*i] = 4;

                data_pos += 4;

                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

                data2len += para_len;
            }
        } else {//parser the unsequence data
            gap = ( in_len - pos ) / vsq.BsVsq.Number;

            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                    para_len = gap - 3;
                    memcpy ( &data_addr, &in_buf[pos], 3 );

                } else {
                    para_len = gap - 2;
                    memcpy ( &data_addr, &in_buf[pos], 2 );


                }

                //fill the header
                i_map = gb104.GetDataId ( data_addr );

                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                }

                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

                //* ( uint16* ) &data[2+3*i] = data_id;

                data[4+3*i] = 4;
                data_pos += 3;

                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len );
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
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
            SetCuaseWord_4 ( CAUSE_Actcon );
        } else {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
            SetCuaseWord_4 ( CAUSE_Deactcon );

        }

        //build the ack frame to station
        // BuildFrame1016 ( out_buf, out_len );
        //BuildFrame6816 ( C_BO_NC_1, out_buf, out_len, data3, data3len );
        Build_I_Frame ( C_BO_NC_1, out_buf, out_len, data3, data3len );

        return 1;
    }

    return 0;
}




//#define C_BO_TC_1  64//>∶＝32比特串带时标

static char Parser_C_BO_TC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  51//>∶＝32比特串
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
    CP56Time2a _t;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;


    memcpy ( ( char* ) &_t, &in_buf[in_len-7], 7 );

    if ( !CheckTimeValid ( _t ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the frame timeout,cmd cancel\n" );
        return 0;

    }

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb != 0 ) {
        if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
            if ( in_buf[pos] != C_BO_TC_1 ) {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
                return 0;
            }
        }

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

        gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

        if ( len == 2 ) {
            plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
            data_pos = pos + 2;
        } else {
            plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
            data_pos = pos + 1;
        }

        if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
            SetCuaseWord_4 ( CAUSE_Actterm );
        } else {
            SetCuaseWord_4 ( CAUSE_Actcon );
        }

        //Fill Table Header
        gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

        vsq.data = in_buf[pos];

        gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

        memcpy ( data3, &in_buf[pos], in_len - pos  );

        data3len = in_len - pos ;

        data[0] = vsq.BsVsq.Number;

        data[1] = 0;

        //* ( uint16* ) &data[0] = vsq.BsVsq.Number;

        plb->pro_logic_buf.VSQ_Word.data = vsq.data;

        data_pos = 2;

        if ( vsq.BsVsq.SQ == 1 ) {//paser the sequence data

            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;

                memcpy ( &data_addr, &in_buf[pos], 3 );
            } else {
                gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
                memcpy ( &data_addr, &in_buf[pos], 2 );
            }

            para_len = gap-7;



            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                //fill the table header
                i_map = gb104.GetDataId ( data_addr + i );

                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                }

                // data_id  = data_id + i;
                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

                //* ( uint16* ) &data[2+3*i] = data_id + i;
                data[4+3*i] = 4;

                data_pos += 4;

                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

                data2len += para_len;
            }
        } else {//parser the unsequence data
            gap = ( in_len - pos ) / vsq.BsVsq.Number;

            for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
                if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                    para_len = gap - 10;
                    memcpy ( &data_addr, &in_buf[pos], 3 );

                } else {
                    para_len = gap - 9;
                    memcpy ( &data_addr, &in_buf[pos], 2 );


                }

                //fill the header
                i_map = gb104.GetDataId ( data_addr );

                if ( i_map != 0 ) {
                    data_id = i_map->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                }

                memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

                //* ( uint16* ) &data[2+3*i] = data_id;

                data[4+3*i] = 4;
                data_pos += 3;

                //fill the data buf
                memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len);
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
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
            SetCuaseWord_4 ( CAUSE_Actcon );
        } else {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
            SetCuaseWord_4 ( CAUSE_Deactcon );

        }

        //build the ack frame to station
        // BuildFrame1016 ( out_buf, out_len );
        //BuildFrame6816 ( C_BO_NC_1, out_buf, out_len, data3, data3len );
        Build_I_Frame ( C_BO_TC_1, out_buf, out_len, data3, data3len );

        return 1;
    }

    return 0;
}



// 在监视方向的系统命令（站特定参数）
//static char parser_C_BO_NC_4(char *in_buf,uint16 in_len,char *out_buf,uint16 *out_len)//  150//>∶＝比特串
//{
// return 1;
//}
// 在控制方向的系统命令（站特定参数）
static char Pack_M_EI_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  70//>∶＝初始化结束
{
    char ret = 0;
    uint16 data_len = 0;
    char data[512];
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", in_len );
    ret = PrepareProData_4 ( in_buf, in_len, data, &data_len );

    if ( ret ) {
        // BuildFrame6816 ( M_EI_NA_1, out_buf, out_len, data, data_len );
        Build_I_Frame ( M_EI_NA_1, out_buf, out_len, data, data_len );
        return 1;
    }

    return 0;
}

static char Parser_C_IC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  100//>∶＝总召唤命令（包括分组召唤）
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[8];
    uint8 datalen = 0;
    uint8 class_data_type = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_IC_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    plb->pro_logic_buf.M_type = C_IC_NA_1;//告诉外部逻辑是总召唤，结束后请清除

    plb->pro_logic_buf.total_data_send_over = 1;//设置发送结束标志为未结束
    plb->pro_logic_buf.total_ask_begin = 1;//设置召唤起始标志
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
    else
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    plb->pro_logic_buf.QOI = in_buf[in_len-1];

    if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }

    data[datalen++] = in_buf[in_len-1];//召唤限定词

    SetCuaseWord_4 ( CAUSE_Actcon );

    if ( plb->pro_logic_buf.QOI == CAUSE_Introgen ) {//总召唤
        class_data_type = ACK_STATION_ASK_DATA;
    } else if ( plb->pro_logic_buf.QOI > CAUSE_Introgen && plb->pro_logic_buf.QOI < CAUSE_Intro16 + 1 ) {//分组召唤
        class_data_type = ACK_GROUP_ASK_DATA;
    } else {
        SetCuaseWord_4 ( CAUSE_Deact );
    }

    tab = gb104.SearchClassTable ( class_data_type );

    if ( tab == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no support the service No.%d service\n", class_data_type );
        return 0;
    }

    if ( tab->grade == CLASS1 ) {//判断总召数据采用1级还是2级数据应答

        gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, class_data_type );
    } else {
        gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, class_data_type );
    }

    //BuildFrame1016 ( out_buf, out_len );

    return NO_ASDU;
}

static char Parser_C_CI_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  101//>∶＝累计量召唤命令）
{
    return 1;
}

static char Parser_C_RD_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  102//>∶＝读命令
{
    uint8 pos;
    int i;
    uint8 len;
    uint8 ret = 0;
    char data3[512];
    uint16 data3len = 0;
    char data[256];
    uint32 data_addr = 0;
    // uint8 para_len = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    uint16 data_id = 0;
    uint8 type = 0;
    // uint8 gap = 0;
    ID_MAP *i_map = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RD_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    data[0] = vsq.BsVsq.Number;

    data[1] = 0;

    //* ( uint16* ) &data[0] = vsq.BsVsq.Number;

    data_pos = 2;

    if ( vsq.BsVsq.SQ == 1 ) {

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
            //gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;

            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            // gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {

            i_map = gb104.GetDataId ( data_addr + i );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            //fill the table header
            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id + i;
            data[4+3*i] = 0;

            data_pos += 3;

        }
    } else {
        // gap = ( in_len - pos - 1 ) / vsq.BsVsq.Number;

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                //para_len = gap - 3;
                pos += 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                // para_len = gap - 2;
                pos += 2;
            }

            i_map = gb104.GetDataId ( data_addr );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
                type = i_map->type;
                SetCuaseWord_4 ( CAUSE_Req );
            } else {
                type = C_RD_NA_1;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
                SetCuaseWord_4 ( CAUSE_Unknowndataaddr );

            }

            memcpy ( &data[2+3*i], ( char* ) &data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id;

            data[4+3*i] = 0;
            data_pos += 3;
        }
    }

    //memcpy ( in_buf, data, data_pos );
    protocol.member.data_type = type;

    if ( ProGetData ( data, data_pos, data3, &data3len ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", data3len );
        ret = PrepareProData_4 ( data3, data3len, out_buf, out_len );

        if ( ret ) {

            // BuildFrame6816 ( type, data3, &data3len , out_buf, *out_len );
            Build_I_Frame ( type, data3, &data3len , out_buf, *out_len );
            memcpy ( out_buf, data3, data3len );
            *out_len = data3len;
            return 1;
        } else
            return 0;
    } else {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "data not to be got,out_len=%d\n", *out_len );

        return 0;
    }
}

static char Parser_C_CS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  103//>∶＝时钟同步命令
{
    uint8 pos = 0;
    uint8 len = 0;
    PRO_DATA_CLASS_CFG *tab = 0;
    uint8 data_pos = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    CP56Time2a ct;
    char data[32];
    uint16 datalen = 0;
    timer.GetCP56Time ( &ct );
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d len=%d<%d>\n", pos, len, in_buf[pos] );

        if ( in_buf[pos] != C_CS_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    plb->pro_logic_buf.M_type = C_CS_NA_1;

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d len=%d<%02x%02x>\n", pos, len, in_buf[pos+1], in_buf[pos] );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;

    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d len=%d<%02x%02x>\n", pos, len, in_buf[pos+1], in_buf[pos] );

    if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
        //memset ( data, 0, 3 );
        data_pos = pos + 3;
        //datalen = 3;
    } else {
        //memset ( data, 0, 2 );
        data_pos = pos + 2;
        //datalen = 2;
    }

    memcpy ( ( char* ) &ct, &in_buf[data_pos], 7 );

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[%d]%d-%d-%d  %d:%d:%dms @week:%d  \n", data_pos
            , ct.year + 2000
            , ct.month
            , ct.day_week&0x1f
            , ct.hour_su&0x1f
            , ct.minute_iv&0x3f
            , ct.millisecond
            , ( ct.day_week >> 5 ) &0x07 );
#if 0
    timer.GetCP56Time ( &ct );
    memcpy ( &data[datalen], ( char* ) &ct, 7 );
    datalen += 7;
#endif
    ////////////////pack data for prosettime func//////////
    data[0] = 1;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 7;
    memcpy ( &data[5], &in_buf[data_pos], 7 );
    datalen = 12;
    ProSetTime ( data, datalen, data, &datalen );
    tab = gb104.SearchClassTable ( TIME_TB_DATA );

    if ( tab != 0 ) { //判断总召数据采用1级还是用2级数据应答
        if ( tab->grade == CLASS1 ) {
            gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS1, TIME_TB_DATA );
        } else {
            gb104.AddServiceBuf ( gb104.member.CurPortId, CLASS2, TIME_TB_DATA );
        }

        // BuildFrame1016 ( out_buf, out_len );

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "*out_len=%d\n\n\n", *out_len );

        return 0;
    }

    return 0;
}

static char Parser_C_TS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  104//>∶＝测试命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    //uint8 datalen = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_TS_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;

    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
        //plb->SendCause.BsCause.T = 1;
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data num is not 1\n" );
        return 0;
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data addr is not 0\n" );
        return 0;
    }

    memcpy ( &data[data_pos], &in_buf[pos], 2 );

    //* ( uint16* ) &data[data_pos] = ( * ( uint16* ) & in_buf[pos] );

    data_pos += 2;
    //BuildFrame6816 ( C_TS_NA_1 , out_buf, out_len , data, data_pos );
    Build_I_Frame ( C_TS_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}

static char Parser_C_RP_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  105//>∶＝复位进程命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    //uint8 datalen = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_RP_NA_1  ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "pos=%d,len=%d\n", pos, len );

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;

    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data num is not 1\n" );
        return 0;
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data addr is not 0\n" );
        return 0;
    }

    data[data_pos] = in_buf[pos];

    switch ( data[data_pos] ) {

    case 1://进程总复位
        //terminal reset;
        //ProRstTer(char * inbuf,uint16 inlen,char * outbuf,uint16 * outlen);
        //ProSendMsg(0xffffffff,MSG_YK,uint32 da,uint8 data);
        break;

    case 2://复位事件缓存带时间的信息
        //event by time bufer reset;
        //memset ( gb101.c1_data_buf, 0, sizeof ( gb101.c1_data_buf ) );
        //memset ( gb101.c2_data_buf, 0, sizeof ( gb101.c2_data_buf ) );
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "event buf has been reset by zero\n" );
        break;

    default:
        break;
    }

    data_pos += 1;

    //BuildFrame6816 ( C_RP_NA_1 , out_buf, out_len , data, data_pos );
    Build_I_Frame ( C_RP_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}

// 在控制方向的参数命令（站特定参数）
static char Parser_C_CD_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  106//>∶＝延时获得命令
{
    uint8 pos = 0;
    uint8 len = 0;
    char data[256];
    uint32 data_addr = 0;
    BSVSQ vsq;
    uint8 data_pos = 0;
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != C_CD_NA_1  ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;

    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];

    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    if ( vsq.BsVsq.Number != 1 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data num is not 1\n" );
        return 0;
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the data addr is not 0\n" );
        return 0;
    }

    CP56Time2a _t;

    timer.GetCP56Time ( &_t );
    memcpy ( &data[data_pos], ( char* ) &_t.millisecond, 2 );
    //* ( uint16* ) &data[data_pos] = _t.millisecond;
    data_pos += 2;

    //BuildFrame6816 ( C_CD_NA_1 , out_buf, out_len , data, data_pos );
    Build_I_Frame ( C_CD_NA_1 , out_buf, out_len , data, data_pos );
    return 1;
}

/**
*************************************************************************************
*  @brief  解析获得参数规约帧
*  @author  liht
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
char ParserGetPara_4 ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
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
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != type ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    data[0] = vsq.BsVsq.Number;

    data[1] = 0;


    //PFUNC ( TEM_INFO, DEBUG_PRO104, "num=%d,%d\n", data[0] + data[1]*256, vsq.BsVsq.Number );

    data_pos = 2;

    if ( vsq.BsVsq.SQ == 1 ) {

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;

            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 2 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }

        para_len = gap;



        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb104.GetDataId ( data_addr + i );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );

            data[4+3*i] = para_len;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

            data2len += para_len;
        }
    } else {
        gap = ( in_len - pos ) / vsq.BsVsq.Number;

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }

            i_map = gb104.GetDataId ( data_addr );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );

            data[4+3*i] = para_len;
            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len );
            pos += gap;
            data2len += para_len;
        }
    }

    memcpy ( in_buf, data, data_pos );

    memcpy ( &in_buf[data_pos], data2, data2len );
    in_len = data_pos + data2len;
    gb104.member.pc_mse_nx = type;
    protocol.member.pc_mse_nx = gb104.member.pc_mse_nx;

    if ( ProGetPara ( in_buf, in_len, data3, &data3len ) ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", data3len );
        ret = PrepareProData_4 ( data3, data3len, out_buf, out_len );
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d,%d\n", *out_len, ret );

        if ( ret ) {
            SetCuaseWord_4 ( CAUSE_Actcon );
            Build_I_Frame ( type, data3, &data3len , out_buf, *out_len );
           //PFUNC ( TEM_INFO, DEBUG_PRO104, "data3_len=%d\n", data3len );
            memcpy ( out_buf, data3, data3len );
			memcpy((char*)out_len,(char*)&data3len,2);
            return 1;
        }

        return 0;
    } else {

        ret = PrepareProData_4 ( data3, data3len, out_buf, out_len );
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", *out_len );
        if ( ret ) {
            SetCuaseWord_4 ( CAUSE_Deactcon );
            Build_I_Frame ( type, data3, &data3len , out_buf, *out_len );
            memcpy ( out_buf, data3, data3len );
			memcpy((char*)out_len,(char*)&data3len,2);
            return 1;
        }

        return 0;
    }
}

static char Parser_P_ME_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  110//>∶＝测量值参数，规一化值
{
    if ( ( in_buf[in_len-1]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara_4 ( P_ME_NA_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara_4 ( P_ME_NA_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}

static char Parser_P_ME_NB_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  111//>∶＝测量值参数，标度化值
{
    if ( ( in_buf[in_len-1]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara_4 ( P_ME_NB_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara_4 ( P_ME_NB_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}

static char Parser_P_ME_NC_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  112//>∶＝测量值参数，短浮点数
{
    if ( ( in_buf[in_len-1] & ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara_4 ( P_ME_NC_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara_4 ( P_ME_NC_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}

// 文件传输（站特定参数）
static char Parser_P_AC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  113//>∶＝参数激活
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
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "\n" );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len ) ) {
        if ( in_buf[pos] != P_AC_NA_1 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "type error\n" );
            return 0;
        }
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 ) {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1] * 256;
        data_pos = pos + 2;
    } else {
        plb->pro_logic_buf.M_SendCause.data = ( uint8 ) in_buf[pos];
        data_pos = pos + 1;
    }

    if ( plb->pro_logic_buf.M_SendCause.BsCause.code != CAUSE_Act ) {
        SetCuaseWord_4 ( CAUSE_Actterm );
    } else {
        SetCuaseWord_4 ( CAUSE_Actcon );
    }

    //Fill Table Header
    gb104.GetKeyWordPositionAndLen ( KEY_WORD_VSQ, &pos, &len );

    vsq.data = in_buf[pos];

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memcpy ( data3, &in_buf[in_len], in_len - pos - 2 );

    data3len = in_len - pos - 2;

    data[0] = vsq.BsVsq.Number;

    data[1] = 0;

    //* ( uint16* ) &data[0] = vsq.BsVsq.Number;

    plb->pro_logic_buf.VSQ_Word.data = vsq.data;

    data_pos = 2;

    if ( vsq.BsVsq.SQ == 1 ) {

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
            gap = ( in_len - pos - 4 ) / vsq.BsVsq.Number;

            memcpy ( &data_addr, &in_buf[pos], 3 );
        } else {
            gap = ( in_len - pos - 3 ) / vsq.BsVsq.Number;
            memcpy ( &data_addr, &in_buf[pos], 2 );
        }

        para_len = gap;

        i_map = gb104.GetDataId ( data_addr );

        if ( i_map != 0 ) {
            data_id = i_map->data_id;
        } else {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
        }

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            //fill the table header
            i_map = gb104.GetDataId ( data_addr + i );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            //data_id = data_id + i;
            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id + i;
            data[4+3*i] = 1;

            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+i*gap], para_len );

            data2len += para_len;
        }
    } else {
        gap = ( in_len - pos - 1 ) / vsq.BsVsq.Number;

        for ( i = 0;i < vsq.BsVsq.Number;i++ ) {
            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
                memcpy ( &data_addr, &in_buf[pos], 3 );
                para_len = gap - 3;
            } else {
                memcpy ( &data_addr, &in_buf[pos], 2 );
                para_len = gap - 2;
            }

            //fill the data buf header
            i_map = gb104.GetDataId ( data_addr );

            if ( i_map != 0 ) {
                data_id = i_map->data_id;
            } else {
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "data addr error %08x\n", data_addr );
            }

            memcpy ( &data[2+3*i], ( char* ) & data_id, 2 );

            //* ( uint16* ) &data[2+3*i] = data_id;

            data[4+3*i] = 1;
            data_pos += 3;

            //fill the data buf
            memcpy ( &data2[data2len], &in_buf[pos+gb104.member.pro_cfg.datasign_addr_size], para_len );
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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Actcon );
    }


    else {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d\n", in_len );
        SetCuaseWord_4 ( CAUSE_Deactcon );

    }

    //BuildFrame6816 ( P_AC_NA_1, out_buf, out_len, data3, data3len );
    Build_I_Frame ( P_AC_NA_1, out_buf, out_len, data3, data3len );

    //BuildFrame1016 ( out_buf, out_len );

    return 1;
}

static char Pack_F_FR_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  120//>∶＝文件准备就绪
{
    SetCuaseWord_4 ( CAUSE_File );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) return 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.Number = 1;

    if ( Build_I_Frame ( F_FR_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }

    return 0;
}

static char Pack_F_SR_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  121//>∶＝节准备就绪
{
    SetCuaseWord_4 ( CAUSE_File );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) return 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.Number = 1;

    if ( Build_I_Frame ( F_SR_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }

    return 0;
}

static char Parser_F_SC_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  122//>∶＝召唤目录，选择文件，召唤文件召唤节
{
    BSCAUSE Cause;
    SCQWORD scw;
    SRQWORD srw;
    FRQWORD fw;
    uint8 pos = 0;
    uint8 len = 0;
    uint32 type = 0;
    uint16 file_name = 0;
    PRO_LOGIC_BUF04 *plb = 0;
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
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "inlen=%d\n", in_len );
    bart = protocol.SearchBufAddrRegistTable ( gb104.member.CurPortId );

    if ( bart == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no send buf for port%d sending ,type=%d\n", gb104.member.CurPortId , F_SC_NA_1 );
        return 0;
    }

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );//选择对应传输口的逻辑缓存

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no config logic buf for port%d paraser ,type=%d\n", gb104.member.CurPortId , F_SC_NA_1 );
        return 0;
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_SENDCAUSE, &pos, &len );

    if ( len == 2 )
        Cause.data = * ( uint16* ) & in_buf[pos];
    else
        Cause.data = in_buf[pos];

    scw.data = in_buf[in_len-1];//品质描述词，倒数第三位

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_TYPE, &pos, &len );

    memcpy ( ( char* ) &type, &in_buf[pos], len );

    plb->pro_logic_buf.M_type = type;//保持文件传输标识

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

    memcpy ( data, &in_buf[pos], len );

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_addr=%02x%02x\n", data[1], data[0] );

    pos += len;

    datalen += len;

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "cause=%d\n", Cause.BsCause.code );

    if ( Cause.BsCause.code == CAUSE_Req ) {//目录用
        return 0; //SendDirectory();召唤目录不响应
    } else if ( Cause.BsCause.code == CAUSE_File ) { //召唤文件
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "scqword.ui=%d\n", scw.ScqWord.UI1 );

        switch ( scw.ScqWord.UI1 ) { //根据召唤限定词执行相应的文件传输

        case SelectFile:
            file_name = in_buf[pos] + in_buf[pos+1] * 256;//获取文件名
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "file_name=%04x\n", file_name );
            memcpy ( &data[datalen], &in_buf[pos], 2 );
            datalen += 2;
            //get file info to file the out buf

            if ( CreateFileInfo ( file_name, &plb->pro_logic_buf.FileInfo ) != 0 ) { //bug0024
                fw.FrqWord.BS = 0;//文件准备就绪限定词，肯定确认
            } else {
                fw.FrqWord.BS = 1;
            }

            //prepare ack data
            memcpy ( &data[datalen], &plb->pro_logic_buf.FileInfo.file_len, 3 );

            datalen += 3;

            data[datalen] = fw.data;

            datalen += 1;

            ret = Pack_F_FR_NA_4 ( data, datalen, out_buf, out_len );

            plb->pro_logic_buf.file_section_cnt = 1;

            plb->pro_logic_buf.file_segment_cnt = 0;

            plb->pro_logic_buf.FileCheckSum = 0;

            plb->pro_logic_buf.SectionCheckSum = 0;

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "Select file %d\n", SelectFile );

            return ret;

            break;

        case AskFile://召唤文件
            if ( plb->pro_logic_buf.FileInfo.file_name != ( uint8 ) in_buf[pos] + ( uint8 ) in_buf[pos+1]*256 ) { //选择的文件名和请求的文件名不同,否定回答            SRQ_Word.SrqWord.BS = 1;
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
                section_size = plb->pro_logic_buf.FileInfo.sect_segment_num * plb->pro_logic_buf.FileInfo.segment_size;
                data[datalen] = plb->pro_logic_buf.FileInfo.file_section_num;
                datalen += 1;
                memcpy ( &data[datalen], &section_size, 3 );
                datalen += 3;
                data[datalen] = srw.data;
                datalen += 1;

            }

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "ask file %d\n", AskFile );

            ret = Pack_F_SR_NA_4 ( data, datalen, out_buf, out_len );

            return ret;
            break;

        case DeactFile:
            plb->pro_logic_buf.file_section_cnt = 1;

            plb->pro_logic_buf.file_segment_cnt = 0;

            //plb->is_file_data = NO;//bug0062

            plb->pro_logic_buf.FileCheckSum = 0;

            plb->pro_logic_buf.SectionCheckSum = 0;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "DeactFile %d\n", DeactFile );

            return 0;

        case DeleteFile:
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "DeleteFile %d\n", DeleteFile );
            break;

        case SelectSection:
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "SelectSection %d\n", SelectSection );
            break;

        case AskSection://召唤节
            for ( i = 0;i < plb->pro_logic_buf.FileInfo.sect_segment_num;i++ ) {
                datalen = len;
                memcpy ( &data[datalen], ( char* ) & plb->pro_logic_buf.FileInfo.file_name, 2 );
                // * ( uint16* ) &data[datalen] = plb->FileInfo.file_name;
                datalen += 2;
                data[datalen++] = plb->pro_logic_buf.file_section_cnt;


                //build read file para
                file_para[file_para_len] = 1;
                file_para_len += 1;
                file_para[file_para_len] = 0;
                file_para_len += 1;
                //memcpy ( &file_para[file_para_len], &(plb->pro_logic_buf.FileInfo.file_name), 2 );
                * ( uint16* ) &file_para[file_para_len] = plb->pro_logic_buf.FileInfo.file_name;
                file_para_len += 2;
                file_para[file_para_len++] = 5;
                offset = ( i + plb->pro_logic_buf.FileInfo.sect_segment_num * ( plb->pro_logic_buf.file_section_cnt - 1 ) )
                         * plb->pro_logic_buf.FileInfo.segment_size;
                memcpy ( &file_para[file_para_len], ( char* ) &offset, 4 );

                file_para_len += 4;
                
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "file_name =%04x,off set= %d,len=%d\n", plb->pro_logic_buf.FileInfo.file_name,
                  //      offset, plb->pro_logic_buf.FileInfo.segment_size );
                ////////////////////////////////////////////////////

	               if ( ( i + ( plb->pro_logic_buf.file_section_cnt - 1 ) *plb->pro_logic_buf.FileInfo.sect_segment_num ) < plb->pro_logic_buf.FileInfo.file_seg_num -1) {
	                    file_para[file_para_len++] = plb->pro_logic_buf.FileInfo.segment_size;
	                    data[datalen++] = plb->pro_logic_buf.FileInfo.segment_size;
	                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "segment len= %d\n", plb->pro_logic_buf.FileInfo.segment_size );
		               } 
		           else if( ( i + ( plb->pro_logic_buf.file_section_cnt - 1 ) *plb->pro_logic_buf.FileInfo.sect_segment_num ) > plb->pro_logic_buf.FileInfo.file_seg_num - 1 ) {
	                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%x file send over\n", plb->pro_logic_buf.FileInfo.file_name );
	                    break;
					}
					else if(( i + ( plb->pro_logic_buf.file_section_cnt - 1 ) *plb->pro_logic_buf.FileInfo.sect_segment_num ) == plb->pro_logic_buf.FileInfo.file_seg_num - 1 ) {
	                    file_para[file_para_len++] = plb->pro_logic_buf.FileInfo.last_segment_size;
	                    data[datalen++] = plb->pro_logic_buf.FileInfo.last_segment_size;
					}
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "send segment =%d,datalen=%d\n", segment_len , datalen );
                    if ( ProReadFile ( file_para, file_para_len, segment_buf, &segment_len ) ) {
                        if ( segment_len > 238 ) {
                            return 0;
                    	}
                    	memcpy ( &data[datalen], segment_buf, segment_len );
                    	datalen += segment_len;
                    } else {
                        return 0;
                    }
					file_para_len=0;
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "datalen=%d,seg_len=%d\n", datalen, segment_len );

                    plb->pro_logic_buf.SectionCheckSum += GetSum ( ( uint8* ) segment_buf, segment_len );

                    ret = Pack_F_SG_NA_4 ( data, datalen, out_buf, out_len );
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "////////++++++++++++++send sg datalen =%d out_len=%d\n", datalen , *out_len );
            

		            if ( ret )  {
		                    //protocol.AddSendBuf ( bart->paddr, out_buf, *out_len );
		                    //protocol.CopySendFrame ( bart->paddr, out_buf, *out_len );
		                    gb104.SendFrame(bart->paddr,out_buf, *out_len);
		            }
		
		            //plb->file_segment_cnt++;
		
		             mdelay ( 100 );//间隔50ms发送1段
        	}

            //ack last segment
            datalen = len;

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "++++++++++++++send last segment =%d datalen=%d\n", segment_len , datalen );


            // * ( uint16* ) &data[datalen] = plb->FileInfo.file_name;
			* ( uint16* ) &data[datalen] = plb->pro_logic_buf.FileInfo.file_name;
            //memcpy ( &data[datalen], ( char* ) &plb->pro_logic_buf.FileInfo.file_name, 2 );

            datalen += 2;

            data[datalen++] = plb->pro_logic_buf.file_section_cnt;

            data[datalen++] = 4;//最后的段

            data[datalen++] = plb->pro_logic_buf.SectionCheckSum;

            plb->pro_logic_buf.SectionCheckSum = 0;

            ret = Pack_F_LS_NA_4 ( data, datalen, out_buf, out_len );

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "////////////////send last datalen =%d out_len=%d\n", datalen , *out_len );

            if ( ret )  {
                //protocol.AddSendBuf ( bart->paddr, out_buf, *out_len );
                //protocol.CopySendFrame ( bart->paddr, out_buf, *out_len );
				gb104.SendFrame(bart->paddr,out_buf, *out_len);
            }

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "AskSection %d\n", AskSection );

            return 0;

            break;
        case DeactSection:
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "DeactSection %d\n", DeactSection );
            break;

        default:
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "no option %d\n", 0 );
            break;
        }

    }

    return 0;
}

static char Pack_F_LS_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  123//>∶＝最后的节，最后的段
{
    SetCuaseWord_4 ( CAUSE_File );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) return 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.Number = 1;

    if ( Build_I_Frame ( F_LS_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }

    return 0;
}

static char Parser_F_AF_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  124//>∶＝认可文件，认可节
{
    uint8 ret = 0;
    uint8 pos = 0, len = 0;
    uint16 file_name;
    char data[32];
    uint8 datalen = 0;
    AFQWORD afw;
    SRQWORD srw;
    PRO_LOGIC_BUF04 *plb = 0;
    uint32 datasign = 0;
    afw.data = 0;
    srw.data = 0;
    uint32 section_size = 0;
    uint8 sec_name = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser <port_id=%d>\n", gb104.member.CurPortId );
        return 0;
    }

    gb104.GetKeyWordPositionAndLen ( KEY_WORD_DATA_START, &pos, &len );

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
     PFUNC ( TEM_WARNING,DEBUG_PRO104, "section name error sn=%d,tn=%d<port_id=%d>\n",sec_name,plb->file_section_cnt, gb101.CurPortId );
     return 0;
     }
     */
    afw.data = in_buf[in_len-1];
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "afq.data=%d\n", afw.data );

    switch ( afw.AfqWord .UI1 ) {

    case 1://文件肯定确认
        plb->pro_logic_buf.file_section_cnt = 1;
        plb->pro_logic_buf.file_segment_cnt = 0;
        //删除文件
        //传输目录
        plb->pro_logic_buf.FileCheckSum = 0;
        plb->pro_logic_buf.SectionCheckSum = 0;
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "file confirm<port_id=%d>\n", gb104.member.CurPortId );
        return 0;
        //      break;

    case 2://文件否定确认，不重发
        plb->pro_logic_buf.file_section_cnt = 1;
        plb->pro_logic_buf.file_segment_cnt = 0;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, " file nack no resend<port_id=%d>\n", gb104.member.CurPortId );
        break;

    case 3://节肯定确认
        plb->pro_logic_buf.FileCheckSum += plb->pro_logic_buf.SectionCheckSum;
        plb->pro_logic_buf.SectionCheckSum = 0;
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "station confirm ack section ok FileCheckSum=%x,SectionCheckSum=%x\n",
                //plb->pro_logic_buf.FileCheckSum, plb->pro_logic_buf.SectionCheckSum );

        if ( plb->pro_logic_buf.file_section_cnt >= plb->pro_logic_buf.FileInfo.file_section_num ) { //节发送完，文件发送结束
            plb->pro_logic_buf.file_segment_cnt = 0;
            data[datalen++] = plb->pro_logic_buf.file_section_cnt;
            data[datalen++] = 2;// LSQ = 2;//最后的节
            data[datalen++] = plb->pro_logic_buf.FileCheckSum;
            ret = Pack_F_LS_NA_4 ( data, datalen, out_buf, out_len );
            PFUNC ( TEM_INFO, DEBUG_PRO104, "section over\n" );
        } else {
            plb->pro_logic_buf.file_section_cnt++;//节序号++，起始序号为1
            srw.SrqWord.BS = 0;
            section_size = plb->pro_logic_buf.FileInfo.sect_segment_num * plb->pro_logic_buf.FileInfo.segment_size;
            data[datalen] = plb->pro_logic_buf.file_section_cnt;
            datalen += 1;
            memcpy ( &data[datalen], &section_size, 3 );
            datalen += 3;
            data[datalen] = srw.data;
            datalen += 1;

            ret = Pack_F_SR_NA_4 ( data, datalen, out_buf, out_len );

        }

        plb->pro_logic_buf.file_segment_cnt = 0;

        return ret;

        break;

    case 4://节否定确认,重发

        if ( afw.AfqWord.UI2 == 2 ) { //如果是校验出错，重发

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "station Ack Section Checksun Error <section:%d\n", plb->pro_logic_buf.file_section_cnt );
            plb->pro_logic_buf.file_section_cnt -= 1;
        }

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "section nack and resend <port_id=%d>\n", gb104.member.CurPortId );

        plb->pro_logic_buf.file_segment_cnt = 0;

        break;

    default:
        break;
    }

    return 0;
}

static char Pack_F_SG_NA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  125//>∶＝段
{
    SetCuaseWord_4 ( CAUSE_File );

    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) return 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.Number = 1;

    if ( Build_I_Frame ( F_SG_NA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }

    return 0;
}

static char Pack_F_DR_TA_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  126//>∶＝目录
{
    //暂不实现
    SetCuaseWord_4 ( CAUSE_Req );
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );
    //信息对象地址 2或三
    //1~n个文件////////////////
    //文件名或者子目录名
    //文件长度
    //SOF文件状态
    //7字节的CP56Time2a时间
    //////////////////////////////
    //gb104.member.pro_logic_buf[0].SOF_Word.SofWord.FA = 0;//0-文件等待传输，1-文件传输已激活
    //gb104.member.pro_logic_buf[0].SOF_Word.SofWord.LFD = 0;//0-后面还有目录文件，1-最后的目录文件
    //gb104.member.pro_logic_buf[0].SOF_Word.SofWord._FOR = 0;//0-定义文件名，1-定义子目录名
    //gb104.member.pro_logic_buf[0].SOF_Word.SofWord.STATUS = 0;//缺省为0

    if ( plb == 0 ) return 0;

    plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 1;

    plb->pro_logic_buf.VSQ_Word.BsVsq.Number = 64;

    if ( Build_I_Frame ( F_DR_TA_1, out_buf, out_len, in_buf, in_len ) ) {
        return 1;
    }

    return 0;
}

static char Parser_P_XX_XX_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len ) //  222//>∶＝终端参数块设置
{
    if ( ( in_buf[in_len-1]& ( char ) 0x80 ) == ( char ) 0x80 ) {
        if ( ParserGetPara_4 ( P_XX_XX_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ( ParserSetPara_4 ( P_XX_XX_1, in_buf, in_len, out_buf, out_len ) ) {
            return 1;
        } else {
            return 0;
        }
    }
}

/**
*************************************************************************************
*  @brief  根据类型查找解析函数
*  @author  liht
*  @param[in] uint8 type
*  @return  表指针
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PARSER_FUNC * Gb104::SearchParserFuncTable ( uint8 type )
{
    int i;

    for ( i = 0;i < MAX_PARSER04_FUNC_NUM;i++ ) {
        //PFUNC(TEM_INFO,DEBUG_PRO104,"[%d]intype=%d,lookup type=%d\n",i,type, parser_func[i].type);
        if ( member.parser_func[i].type == type ) {
            return &member.parser_func[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  根据类型查找打包函数
*  @author  liht
*  @param[in] uint8 type
*  @return  表指针
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PACK_FUNC * Gb104::SearchPackFuncTable ( uint8 type )
{
    int i;

    for ( i = 0;i < MAX_PACK04_FUNC_NUM;i++ ) {
        if ( member.pack_func[i].type == type ) {
            return &member.pack_func[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
ID_MAP *Gb104::GetDataId ( uint32 dataaddr )
{
    int i;
    //PFUNC ( TEM_INFO,DEBUG_PRO104, "in data address=%04x\n", dataaddr );

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

        //PFUNC ( TEM_INFO,DEBUG_PRO104, "in data address[%d]=%04x\n", i, member.yk_id_map[i].data_addr );
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


    for ( i = 0;i < MAX_STEP_NUM;i++ ) {
        if ( member.step_id_map[i].data_addr == dataaddr ) {
            return &member.step_id_map[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
ID_MAP *Gb104::GetDataAddr ( uint16 dataid )
{
    int i;
    // PFUNC(TEM_INFO,DEBUG_PRO104,"in data id=%02x\n", dataid);

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

    for ( i = 0;i < MAX_STEP_NUM;i++ ) {
        if ( member.step_id_map[i].data_id == dataid ) {
            return &member.step_id_map[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  解析入口函数
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
char Gb104::Parser ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    PARSER_FUNC *tab;
    char ret = 0;
    uint8 type=0;
    uint8 pos,len;    
    
    GetKeyWordPositionAndLen(KEY_WORD_TYPE,&pos,&len);
    type=in_buf[pos];
    tab = SearchParserFuncTable ( type);
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d,%x\n", type , tab );

    if ( tab != 0 ) {
        if ( tab->fp != 0 ) { //功能解析
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "inlen=%d \n", in_len );
            ret = ( *tab->fp ) ( in_buf, in_len , member.pb.buf, &member.pb.bytes );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d \n", member.pb.bytes );
        }

        if ( ret == 2 ) {
            if ( tab->pcb != 0 ) { ////函数处理
                ret = ( *tab->pcb ) ( member.pb.buf, member.pb.bytes, out_buf, out_len );
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d \n", member.pb.bytes );
            } else {
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d \n", member.pb.bytes );
                ret = 0;
            }
        } else if ( ret == 1 ) {
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "len=%d\n", member.pb.bytes );
            memcpy ( out_buf, member.pb.buf, member.pb.bytes );
            *out_len = member.pb.bytes;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d \n", *out_len );
        }
    }

    return ret;
}

/**
*************************************************************************************
*  @brief  组包入口函数
*  @author  liht
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
char Gb104::Pack ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    PACK_FUNC *tab;
    char ret = 0;
    tab = SearchPackFuncTable ( type );
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "%d\n", tab->type );

    if ( tab != 0 ) {
        if ( tab->fp != 0 ) {
            ret = ( *tab->fp ) ( in_buf, in_len , out_buf, out_len );
        }

        return ret;
    }

    return ret;
}

void Gb104::InitGb104 ( void )
{
    int i;
    ComBasePara *ComPara;
    Data < BaseDataType > data;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );
    user_data.GetData ( PARA_5139, &data );
    ComPara = ( ComBasePara* ) data.Data;
    member.TerSt.data = 0;//terminal status is initialized by zero
//read the pro cofig file fill the buf
    member.xml_para_cnt = 0;
    ReadCfg();//read para config file and fill the para buffer
    ParaCheck();//check the para is valid


    InitFunc();//config the datatype link to the function
//InitDataClassTable();if you won't to use the file outside ,you can config the buffer directly.
    InitIdMapTable();//config the datasign and data id map
    memset ( member.pro_logic_buf04, 0, sizeof ( member.pro_logic_buf04 ) );//reset the logic buffer by zero
    //InitDataClassTable();
    //////////////////////////////

    member.pro_logic_buf04[0].pro_logic_buf.linkaddr = ComPara->Data.LocalId;//config the link addr and host addr for the 4 port

    for ( i = 0;i < 4;i++ ) {
        member.pro_logic_buf04[i].pro_logic_buf.linkaddr = ComPara->Data.LocalId;
        member.pro_logic_buf04[i].pro_logic_buf.hostaddr = ComPara->Data.LocalId;
    }

    member.data_group_cfg[0].data_type = member.pro_data_type_cfg.syx_type;

    member.data_group_cfg[1].data_type = member.pro_data_type_cfg.yc_type;
    member.data_group_cfg[2].data_type = M_ST_NA_1;
    member.data_group_cfg[3].data_type = 0;
    member.data_group_cfg[4].data_type = M_BO_NA_1;
    member.data_group_cfg[5].data_type = 0;

//遥信遥测等分组配置
	InitGroup();
#if 0

    for ( i = 0;i < 8;i++ ) {
        if ( introx_info[data_group_cfg[0].start_group_id - 1 + i][0].flag == 1 )
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "[intro%d]%d\n", data_group_cfg[0].start_group_id + i,
                    introx_info[data_group_cfg[0].start_group_id - 1 + i][0].flag );
    }

#endif

    //PFUNC(TEM_WARNING,DEBUG_PRO104,"YC_GROUP data_cnt=%d\n",pro_yc_data_num);
#if 0
    for ( i = 0;i < 4;i++ ) {
        if ( introx_info[data_group_cfg[1].start_group_id - 1 + i][0].flag == 1 )
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "[intro%d]%d\n", data_group_cfg[1].start_group_id + i,
                    introx_info[data_group_cfg[1].start_group_id - 1 + i][0].flag );
    }

#endif
    //PFUNC(TEM_WARNING,DEBUG_PRO104,"YC_GROUP data_cnt=%d\n",pro_yc_data_num);
#if 0
    for ( i = 0;i < 2;i++ ) {
        if ( member.introx_info[member.data_group_cfg[2].start_group_id - 1 + i][0].flag == 1 )
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "[intro%d]%d\n", member.data_group_cfg[2].start_group_id + i,
                    member.introx_info[member.data_group_cfg[2].start_group_id - 1 + i][0].flag );
    }

#endif
#if 0
    for ( i = 0;i < 16;i++ ) {
        for ( j = 0;j < 8;j++ ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "intro[%d][%d] %d,<%d>\n", i, j, introx_info[i][j].flag, introx_info[i][j].typesign );
        }
    }

#endif

//复位1、2级数据缓存
    memset ( &member.c1_data_buf, 0, sizeof ( member.c1_data_buf ) );

    memset ( &member.c2_data_buf, 0, sizeof ( member.c2_data_buf ) );


//超时计数器初始化
    memset ( member.timeout104, 0, sizeof ( member.timeout104 ) );

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );

    member.InitOver = 1;

}

Gb104::Gb104 ( void )
{


}
void Gb104::InitGroup ( void )
{
    ConfigGroupData ( &member.data_group_cfg[0], &member.introx_info[member.data_group_cfg[0].start_group_id - 1], member.yx_id_map, member.pro_yx_data_num + member.pro_dyx_data_num, GROUP_YX_DATA );
    ////////////////配置遥测组内数据的分配一组多帧传输的配置////////////////////
    ConfigGroupData ( &member.data_group_cfg[1], &member.introx_info[member.data_group_cfg[1].start_group_id - 1], member.yc_id_map, member.pro_yc_data_num, GROUP_YC_DATA );
    ////////////////配置步位置组内数据的分配一组多帧传输的配置////////////////////
    ConfigGroupData ( &member.data_group_cfg[2], &member.introx_info[member.data_group_cfg[2].start_group_id - 1], member.step_id_map, member.pro_user_step_num, GROUP_STEP_DATA );
    ////////////////配置32位串组内数据的分配///////////////////
    ConfigGroupData ( &member.data_group_cfg[4], &member.introx_info[member.data_group_cfg[4].start_group_id - 1], member.bit32_id_map, 1, 0 );
}
/**
*************************************************************************************
*  @brief  设置1级数据事件
*  @author  liht
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
char Gb104::SetC1Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time )
{

    int i;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d,%d,%d\n", dataid, datasize, data_type );

    if ( data == 0 || time == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "error para addr is zero\n" );
        return 0;
    }

    for ( i = 0;i < protocol.GetBufAddrRegistNum();i++ ) {
        if ( protocol.member.buf_addr_regist_table[i].pro_id != PRO104 ) {
            continue;
        }

        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].port_id = protocol.member.buf_addr_regist_table[i].port_id;

        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].dataid = dataid;//GetDataAddr(dataid)->data_addr;
        memcpy ( member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].data, data, datasize );
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].datasize = datasize;
        //memset(&c1_data_buf.c1_data[c1_data_buf.in_cnt].read_flag,EVENT_NOREAD,buf_addr_regist_table.num);
        memcpy ( member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].time, time, 7 );
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].read_flag = EVENT_NOREAD;
        member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].data_type = data_type;
        //memcpy(c1_data_buf.c1_data.paddr,buf_addr_regist_table.paddr,sizeof(char*)*buf_addr_regist_table.num);
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "[%03d] %d,%04x,%d,%d,%d\n", member.c1_data_buf.in_cnt, protocol.member.buf_addr_regist_table[i].port_id
                , member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].dataid
                , member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].data
                , member.c1_data_buf.c1_data[member.c1_data_buf.in_cnt].datasize
                , time[0] );
        member.c1_data_buf.in_cnt++;

        if ( member.c1_data_buf.in_cnt >= MAX_C1_EVENT_NUM ) {
            member.c1_data_buf.in_cnt = 0;
        }

    }

    return 1;

}

/**
*************************************************************************************
*  @brief  获取1级数据事件
*  @author  liht
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
char Gb104:: GetC1Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag )
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
                // * ( uint16* ) &out_buf->buf[0] = out_buf->data_num;

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
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "111111111111=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );
                    return j;
                }

            }
        }

    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "111111111111=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );

    return j;
}

/**
*************************************************************************************
*  @brief  设置2级数据事件
*  @author  liht
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
char Gb104::SetC2Event ( uint16 dataid, char*data, uint8 datasize, uint8 data_type, char *time )
{
    int i;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d,%d,%d\n", dataid, datasize, data_type );

    if ( data == 0 || time == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "error para addr is zero\n" );
        return 0;
    }

    for ( i = 0;i < protocol.GetBufAddrRegistNum();i++ ) {
        if ( protocol.member.buf_addr_regist_table[i].pro_id != PRO104 ) {
            continue;
        }

        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].port_id = protocol.member.buf_addr_regist_table[i].port_id;

        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].dataid = dataid;//GetDataAddr(dataid)->data_addr;
        memcpy ( member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].data, data, datasize );
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].datasize = datasize;
        //memset(&c2_data_buf.c2_data[c2_data_buf.in_cnt].read_flag,EVENT_NOREAD,MAX_PARSER_BUF_NUM);
        memcpy ( member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].time, time, 7 );
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].read_flag = EVENT_NOREAD;
        member.c2_data_buf.c2_data[member.c2_data_buf.in_cnt].data_type = data_type;
        //memcpy(c2_data_buf.c2_data.paddr,buf_addr_regist_table.paddr,sizeof(char*)*MAX_PARSER_BUF_NUM);
        member.c2_data_buf.in_cnt++;

        if ( member.c2_data_buf.in_cnt >= MAX_C2_EVENT_NUM ) {
            member.c2_data_buf.in_cnt = 0;
        }
    }

    return 1;

}

/**
*************************************************************************************
*  @brief  获取2级数据事件
*  @author  liht
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
char Gb104::GetC2Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag )
{
    int i, k = 0, j = 0;
    out_buf->bytes = 0;
    out_buf->data_num = 0;
    int data_pos = 0;
    uint8 max_num = 0;

    for ( i = 0;i < MAX_C2_EVENT_NUM;i++ ) {
        if ( member.c2_data_buf.c2_data[i].port_id == port_id ) {
            if ( member.c2_data_buf.c2_data[i].read_flag == EVENT_NOREAD && member.c2_data_buf.c2_data[i].data_type == data_type ) {
                //c2_data_buf.c2_data[i].read_flag=EVENT_READ;
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
                // * ( uint16* ) &out_buf->buf[0] = out_buf->data_num;

                if ( out_buf->data_num >= max_num ) {
                    break;
                }

            }
        }
    }

    data_pos = k * 3 + 2;

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "k2222222222222====%d,<%d>(%d)\n", k, data_pos, port_id );
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
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "222222=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );
                    return j;
                }
            }
        }
    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "222222222=====%d,<%d>(%d),out_buf->bytes=%d\n", j, data_type, port_id, out_buf->bytes );

    return j;
}

/**
*************************************************************************************
*  @brief  规约定时器计时
*  @author  liht
*  @param[in] uint16 pn
*  @note 无。
*  @see  NULL
**************************************************************************************
*/

void Gb104::ProTimerCount ( uint16 pn )
{
    PRO_LOGIC_BUF04 *plb = 0;
    plb = SelectLogicBuf ( pn );

    if ( plb != 0 ) {

        if ( plb->no_asdu_con_timer.flag == START ) {
            plb->no_asdu_con_timer.cnt++;
        }

        if ( plb->recv_asdu_con_timer.flag == START ) {
            plb->recv_asdu_con_timer.cnt++;
        }

        if ( plb->send_test_timer.flag == START ) {
            plb->send_test_timer.cnt++;
        }

        if ( plb->link_idle_timer.flag == START ) {
            plb->link_idle_timer.cnt++;
        }

        if ( plb->send_asdu_con_timer.flag == START ) {
            plb->send_asdu_con_timer.cnt++;
        }
    }
}

/**
*************************************************************************************
*  @brief  规约定时器复位
*  @author  liht
*  @param[in]  uint16 pn
*  @return
*  @note 对对应端口的定时器进行复位。
*  @see  NULL
**************************************************************************************
*/
void Gb104::ProTimerReset ( uint16 pn )
{
    PRO_LOGIC_BUF04 *plb = 0;
    plb = SelectLogicBuf ( pn );

    if ( plb != 0 ) {
        plb->send_asdu_con_timer.cnt = 0;
        plb->recv_asdu_con_timer.cnt  = 0;
        plb->send_test_timer.cnt  = 0;
        plb->link_idle_timer.cnt  = 0;
        plb->recv_asdu_con_timer.flag = STOP;
        plb->send_test_timer.flag = STOP;
        plb->link_idle_timer.flag = STOP;
        plb->send_asdu_con_timer.flag = STOP;
        plb->no_asdu_con_timer.flag = STOP;
        plb->no_asdu_con_timer.cnt = 0;

    }
}

/**
*************************************************************************************
*  @brief  规约链路复位
*  @author  liht
*  @param[in] uint16 pn
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Gb104::ProLinkReset ( uint16 pn )
{
    char data[8];
    uint16 datalen = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = SelectLogicBuf ( pn );

    if ( plb != 0 ) {
        plb->SendSeqBack = 0;
        plb->RecvSeqBack = 0;
        plb->pro_logic_buf.connect_st = 0;
        plb->CounterPartAck = 0;
        ProTimerReset ( pn );
        plb->transfer_flag = STOP;
        plb->enable_send_flag = DISABLE;
        plb->ICtrlWord.data = 0;
        memcpy ( data, ( char* ) &pn, 2 );
        datalen = 2;
        ProRstLink ( data, datalen, data, &datalen );
        PFUNC ( TEM_WARNING, DEBUG_PRO104,"8\n");
		
    }

}

/**
*************************************************************************************
*  @brief  规约定时器超时处理
*  @author  liht
*  @param[in] uint16 port_num
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
short CheckTimeOut ( uint16 port_num )
{
    char data[8];
    uint16 datalen = 0;
    UCTRLWORD Uword;
    Uword.data = 3;
    PRO_LOGIC_BUF04 *plb = 0;
    BUF_ADDR_REGIST_TABLE *bart = 0;
    plb = gb104.SelectLogicBuf ( port_num );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to check timeout\n" );
        return 0;

    }

    bart = protocol.SearchBufAddrRegistTable ( port_num );

    if ( bart == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "the send buf is no valid\n" );
        return 0;
    }

    if ( plb->no_asdu_con_timer.cnt >= gb104.member.InerPara.T3/*NO_ASDU_CON_TIMEOUT*/ ) {
        plb->no_asdu_con_timer.flag = STOP;
        plb->no_asdu_con_timer.cnt = 0;
        plb->send_asdu_con_timer.flag = STOP;
        plb->send_asdu_con_timer.cnt = 0;
        plb->CtrlWordBack.data = 1;
        plb->CtrlWordBack.S_CtrlWord.RecvSeq = plb->ICtrlWord.I_CtrlWord.RecvSeq;
        gb104.member.ProtBuf.pack_frame_len = 0;
        Build_U_S_Frame ( gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
        //protocol.AddSendBuf ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
        gb104.SendFrame ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );

    }

    if ( plb->recv_asdu_con_timer.cnt >= gb104.member.InerPara.T1/*APDU_SEND_TIMEOUT*/ ) {
        //rst_link ( port_num );
        memcpy ( data, ( char* ) &port_num, 2 );
        datalen = 2;
        ProRstLink ( data, datalen, data, &datalen );
		PFUNC ( TEM_WARNING, DEBUG_PRO104,"2\n");
    }

    if ( plb->send_asdu_con_timer.cnt >= gb104.member.InerPara.T2/*ASDU_CON_TIMEOUT*/ ) {
        plb->send_asdu_con_timer.flag = STOP;
        plb->send_asdu_con_timer.cnt = 0;
        plb->no_asdu_con_timer.flag = STOP;
        plb->no_asdu_con_timer.cnt = 0;
        plb->CtrlWordBack.data = 1;
        plb->CtrlWordBack.S_CtrlWord.RecvSeq = plb->ICtrlWord.I_CtrlWord.RecvSeq;
        // add_pro104_header ( port_num );
        // SEND_DATA_404 ( port_num );
        gb104.member.ProtBuf.pack_frame_len = 0;
        Build_U_S_Frame ( gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
        gb104.SendFrame ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
        // protocol.AddSendBuf ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
    }

    if ( plb->send_test_timer.cnt >= gb104.member.InerPara.T1/*SEND_TEST_TIMEOUT*/ ) {

        //rst_link ( port_num );
        memcpy ( data, ( char* ) &port_num, 2 );
        datalen = 2;
        ProRstLink ( data, datalen, data, &datalen );
		PFUNC ( TEM_WARNING, DEBUG_PRO104,"3\n");
        /*
        if(bart->paddr->fun_reset!=0){
        bart->paddr->fun_reset(port_num);
        }
        */
    }

    if ( plb->link_idle_timer.cnt >= gb104.member.InerPara.T4/*IDLE_TIMEOUT*/ ) {
        plb->link_idle_timer.flag = STOP;
        Uword.U_CtrlWord.TESTFR = COMMAND04;
        plb->CtrlWordBack.data = Uword.data;

        //add_pro104_header ( port_num );
        //SEND_DATA_404 ( port_num );
        gb104.member.ProtBuf.pack_frame_len = 0;
        Build_U_S_Frame ( gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
        //protocol.AddSendBuf ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
        gb104.SendFrame ( bart->paddr, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
        plb->send_test_timer.flag = START;
        plb->send_test_timer.cnt = 0;
    }

    return FRAME_OK;
}

char Build_U_S_Frame ( char *out_buf, uint16 *out_len )
{
    PRO_LOGIC_BUF04 *plb = 0;
    *out_len = 0;
	uint16 i=0;
	memset((char*)out_len,0,2);
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "NO logic buf to build \"I\" frame \n" );
        return 0;
    }

    out_buf[i++] = 0x68;

    out_buf[i++] = 4;
    out_buf[i++] = ( char ) plb->CtrlWordBack.data;
    out_buf[i++] = ( char ) ( plb->CtrlWordBack.data >> 8 );
    out_buf[i++] = ( char ) ( plb->CtrlWordBack.data >> 16 );
    out_buf[i++] = ( char ) ( plb->CtrlWordBack.data >> 24 );
	memcpy((char*)out_len,(char*)&i,2);
	//dump_binary((uint8 * )out_len-1,4);
	//dump_binary((uint8 * )out_buf,i);
    //*out_len = 6;
    return 1;
}

char Build_I_Frame ( uint8 type , char *out_buf, uint16 *out_len, char *in_buf, uint16 in_len )
{
    PRO_LOGIC_BUF04 *plb = 0;
    uint16 k = 0;
    *out_len = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "NO logic buf to build \"I\" frame \n" );
        return 0;
    }

    out_buf[ k++] = 0x68;

    out_buf[ k ++] = 4;
    out_buf[ k ++] = ( char ) plb->ICtrlWord.data;
    out_buf[ k ++] = ( char ) ( plb->ICtrlWord.data >> 8 );
    out_buf[ k ++] = ( char ) ( plb->ICtrlWord.data >> 16 );
    out_buf[ k ++] = ( char ) ( plb->ICtrlWord.data >> 24 );
    out_buf[ k ++] = type;
    out_buf[ k ++] = plb->pro_logic_buf.VSQ_Word.data;

    if ( gb104.member.pro_cfg.send_cause_size == 2 ) {
        //*(uint16*)&out_buf[*out_len] = plb->pro_logic_buf.SendCause.data;
        memcpy ( &out_buf[k], ( char* ) &plb->pro_logic_buf.SendCause.data, 2 );
        k += 2;
    } else {
        out_buf[k] = ( char ) plb->pro_logic_buf.SendCause.data;
        k += 1;
    }

    if ( gb104.member.pro_cfg.link_addr_size == 2 ) {
        memcpy ( &out_buf[k], ( char* ) & plb->pro_logic_buf.linkaddr, 2 );
        //*(uint16*)&out_buf[*out_len] = plb->pro_logic_buf.linkaddr;
        k += 2;
    } else {
        out_buf[k] = ( char ) plb->pro_logic_buf.linkaddr;
        k += 1;
    }

    memcpy ( &out_buf[k], in_buf, in_len );

    k += in_len;
    out_buf[1] = k - 2;
    *out_len = k;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%x]:%d\n", out_buf, *out_len );
    return 1;

}

short U_FrameHandle ( BUF_MUX *buf, uint16 port_id )
{
    uint8 i;
    UCTRLWORD Uword;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( port_id );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser u_frame\n" );
        return 0;

    }

    Uword.data = 0;

    for ( i = CTRL_WORD04_LEN;i > 0;i-- ) {
        Uword.data = Uword.data << 8;
        Uword.data |= buf->frame.buf[CTRL_WORD04_POS+i-1];
    }

    if ( Uword.U_CtrlWord.TESTFR == COMMAND04 ) { //˕µ½²㋔ļ®
        plb->send_test_timer.flag = STOP;
        plb->pro_logic_buf.connect_st = 1;
        Uword.U_CtrlWord.TESTFR = CONFIRM04;

    } else if ( Uword.U_CtrlWord.TESTFR == CONFIRM04 ) { //˕µ½²㋔ȷɏ
        plb->send_test_timer.flag = STOP;
        plb->pro_logic_buf.connect_st = 1;
        Uword.U_CtrlWord.TESTFR = COMMAND04;

        return FRAME_OK;
    } else if ( Uword.U_CtrlWord.STOPDT == COMMAND04 ) { //˕µ½´«ˤֹͣļ®
        plb->transfer_flag = STOP;
        plb->enable_send_flag = DISABLE;
		plb->recv_asdu_con_timer.flag = STOP;
		plb->recv_asdu_con_timer.cnt = 0;
        Uword.U_CtrlWord.STOPDT = CONFIRM04;
    } else if ( Uword.U_CtrlWord.STOPDT == CONFIRM04 ) { //˕µ½´«ˤֹͣȷɏ
        plb->transfer_flag = STOP;
        plb->enable_send_flag = DISABLE;
        Uword.U_CtrlWord.STOPDT = COMMAND04;
        return FRAME_OK;
    } else if ( Uword.U_CtrlWord.STARTDT == COMMAND04 ) { //˕µ½´«ˤǴ¶¯ļ®
        plb->transfer_flag = START;
        plb->enable_send_flag = ENABLE;
        plb->pro_logic_buf.connect_st = 1;
        Uword.U_CtrlWord.STARTDT = CONFIRM04;
    } else if ( Uword.U_CtrlWord.STARTDT == CONFIRM04 ) { //˕µ½´«ˤǴ¶¯ȷɏ
        plb->transfer_flag = START;
        plb->enable_send_flag = ENABLE;
        plb->pro_logic_buf.connect_st = 1;
        Uword.U_CtrlWord.STARTDT = COMMAND04;
        return FRAME_OK;
    } else {
        return FRAME_ERR;
    }

    plb->UCtrlWord.data = Uword.data;

    plb->CtrlWordBack.data = Uword.data;
    // TransStack[port_num].datalen = APCI_FRAME_LEN;
    gb104.member.ProtBuf.pack_frame_len = 0;
    Build_U_S_Frame ( gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
    // protocol.AddSendBuf ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
    gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
    return FRAME_OK;
}

/*=======================================
函数名：short CheckFrameOk(uchar *pBuf,short len)
功能说明：对主站来的数据进行101-2002规约的帧格式检查，
输入参数：数据缓存首地址 数据长度
返回：
========================================*/


char Gb104::CheckFrame ( BUF_MUX *buf, uint16 port_id )
{
    short ret = 0;
    uint8 pos = 0;
    uint8 len = 0;
    char data[8];
    uint16 datalen = 0;
    uint16 linkaddr = 0;
    uint8 isu = 0;
    uint8 i = 0;
    volatile  ICTRLWORD Iword;
    PRO_LOGIC_BUF04 *plb = 0;

    if ( buf == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "comm buf addr is zero\n" );
        return -1;
    }

    if ( member.ProtBuf.valid_flag == 1 ) {
        member.ProtBuf.valid_flag = 0;
        plb = gb104.SelectLogicBuf ( port_id );

        if ( plb == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to check frame\n" );
            return -1;

        }

        Iword.data = 0;
        
        isu = member.ProtBuf.parser_buf[2] & 0x03;
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "CheckFrameOk  ok\n" );
        if (  member.ProtBuf.parser_frame_len == ( uint16 ) APCI_FRAME_LEN ) {//符合APCI帧格式
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "APCI FRAME 6 \n" );

            if ( isu == 1 ) { //是S格式的帧
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "S FRAME 6 \n" );

                for ( i = CTRL_WORD_LEN ;i > 0;i-- ) {
                    Iword.data = Iword.data << 8;
                    Iword.data |= ( unsigned long ) member.ProtBuf.parser_buf[CTRL_WORD04_POS+i-1];
                }

                //PFUNC ( TEM_WARNING, DEBUG_PRO104, "APCI[S]->TerSendSeq=%04d pcRecvSeq=%d\n",
                        //plb->ICtrlWord.I_CtrlWord.SendSeq, Iword.I_CtrlWord.RecvSeq );
                plb->CounterPartAck = ( uint16 ) Iword.I_CtrlWord.RecvSeq;

                plb->SendSeqBack = ( uint16 ) Iword.I_CtrlWord.RecvSeq;

                if ( ( ( uint16 ) plb->ICtrlWord.I_CtrlWord.SendSeq - plb->SendSeqBack < member.InerPara.K/*MAX_SEND_RECV_MATCH_NUM */ ) \
                        && ( plb->transfer_flag == START ) ) {
                    plb->enable_send_flag = ENABLE;
                } else {
                    plb->enable_send_flag = DISABLE;
                }

                //对方接收到本方发送的帧序号有误，重新连接初始化。

                if ( ( ( uint16 ) plb->ICtrlWord.I_CtrlWord.SendSeq ) < plb->CounterPartAck ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[S]seq err->TerSendSeq=%d pcRecvSeq=%d\n",
                            plb->ICtrlWord.I_CtrlWord.SendSeq, plb->CounterPartAck );
                    memcpy ( data, ( char* ) &port_id, 2 );
                    datalen = 2;
                    ProRstLink ( data, datalen, data, &datalen );
					PFUNC ( TEM_WARNING, DEBUG_PRO104,"4\n");
                    ret = RECV_SEQ_ERR;
                } else {
                    plb->recv_asdu_con_timer.flag = STOP;
					//PFUNC ( TEM_INFO, DEBUG_PRO104,"\n");
                    ret = S_TYPE_FRAME;
                }
            } else if ( isu == 3 ) {  //是U格式的帧
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "U FRAME 6 \n" );
                ret = U_TYPE_FRAME;
            } else {
                ret = FRAME_ERR;
            }
        } else if ( member.ProtBuf.parser_frame_len > APCI_FRAME_LEN ) {//是I格式的帧
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "[I]CheckFrameOk_APCI_FRAME_LEN ok\n" );
            plb->send_asdu_con_timer.flag = START;
            plb->send_asdu_con_timer.cnt = 0;
            plb->no_asdu_con_timer.flag = START;
            plb->no_asdu_con_timer.cnt = 0;

            GetKeyWordPositionAndLen ( KEY_WORD_LINKADDR, &pos, &len );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "link addr pos %d len %d\n", pos, len );

            if ( len == 2 ) {
                linkaddr = ( uint8 ) member.ProtBuf.parser_buf[pos] + ( uint8 ) member.ProtBuf.parser_buf[pos+1] * 256;

                if ( ( linkaddr != plb->pro_logic_buf.linkaddr ) && ( linkaddr != ( uint16 ) 0xffff ) ) {

                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[I]LinkAddr error %d\n", linkaddr );
                    return ASDU_ADDR_ERR;

                } else {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "[I]LinkAddr %d\n", linkaddr );
                }

            } else {
                linkaddr = ( uint8 ) member.ProtBuf.parser_buf[pos];

                if ( ( linkaddr != plb->pro_logic_buf.linkaddr ) && ( linkaddr != ( uint16 ) 0xff ) ) {


                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[I]LinkAddr error %d\n", linkaddr );
                    return ASDU_ADDR_ERR;
                } else {

                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[I]LinkAddr %d\n", linkaddr );
                }

            }


            isu = buf->frame.buf[CTRL_WORD04_POS] & 0x01;

            if ( isu == 0 ) {
                for ( i = CTRL_WORD_LEN ;i > 0;i-- ) {
                    Iword.data = Iword.data << 8;
                    Iword.data |= ( unsigned long ) member.ProtBuf.parser_buf[CTRL_WORD04_POS+i-1];
                }

//接收对方接收的帧序号错误，重新连接初始化。
                plb->CounterPartAck = ( uint16 ) Iword.I_CtrlWord.RecvSeq;

                if ( ( plb->CounterPartAck ) > ( uint16 ) plb->ICtrlWord.I_CtrlWord.SendSeq ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[I]seq err->TerSendSeq=%d pcRecvSeq=%d\n",
                            plb->ICtrlWord.I_CtrlWord.SendSeq, plb->CounterPartAck );
                    memcpy ( data, ( char* ) &port_id, 2 );
                    datalen = 2;
                    ProRstLink ( data, datalen, data, &datalen );
					PFUNC ( TEM_WARNING, DEBUG_PRO104,"5\n");
                    ret = RECV_SEQ_ERR;
                }

//接收对方发送的帧序号错误，重新连接初始化。

                else if ( ( ( uint16 ) plb->ICtrlWord.I_CtrlWord.RecvSeq ) > ( uint16 ) Iword.I_CtrlWord.SendSeq ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "[I]seq err->TerRecvSeq=%04d pcSendSeq=%d\n",
                            plb->ICtrlWord.I_CtrlWord.RecvSeq, ( uint16 ) Iword.I_CtrlWord.SendSeq );
                    memcpy ( data, ( char* ) &port_id, 2 );
                    datalen = 2;
                    ProRstLink ( data, datalen, data, &datalen );
					PFUNC ( TEM_WARNING, DEBUG_PRO104,"1\n");
                    ret = RECV_SEQ_ERR;
                }

//收到和发送的帧序号正常，进行规约解析。
                else {
                    plb->ICtrlWord.I_CtrlWord.RecvSeq++;
//当收到最大个数的I格式帧还没有给确认，发确认帧。
					//PFUNC ( TEM_INFO, DEBUG_PRO104,"plb->ICtrlWord.I_CtrlWord.RecvSeq:%d\n",plb->ICtrlWord.I_CtrlWord.RecvSeq);
                    if ( ( ( uint16 ) plb->ICtrlWord.I_CtrlWord.RecvSeq - plb->RecvSeqBack ) \
                            >= member.InerPara.W/* MAX_RECV_APDU_SEND_CON_NUM*/ ) {
                        plb->send_asdu_con_timer.flag = STOP;
                        plb->send_asdu_con_timer.cnt = 0;

                        plb->no_asdu_con_timer.flag = STOP;
                        plb->RecvSeqBack = ( uint16 ) plb->ICtrlWord.I_CtrlWord.RecvSeq;
                        plb->CtrlWordBack.S_CtrlWord.Res1 = 1;
                        plb->CtrlWordBack.S_CtrlWord.Res2 = 0;
                        plb->CtrlWordBack.S_CtrlWord.RecvSeq = plb->ICtrlWord.I_CtrlWord.RecvSeq;
                        member.ProtBuf.pack_frame_len = 0;
                        Build_U_S_Frame ( member.ProtBuf.pack_buf, &member.ProtBuf.pack_frame_len );
                        //protocol.AddSendBuf ( buf, member.ProtBuf.pack_buf, member.ProtBuf.pack_frame_len );
                        gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                        PFUNC ( TEM_WARNING, DEBUG_PRO104,"--------\n");

                    }

                    plb->SendSeqBack = ( uint16 ) Iword.I_CtrlWord.RecvSeq;

                    if ( ( uint16 ) plb->ICtrlWord.I_CtrlWord.SendSeq - plb->SendSeqBack < member.InerPara.K/*MAX_SEND_RECV_MATCH_NUM */\
                            && ( plb->transfer_flag == START ) ) {
                        plb->enable_send_flag = ENABLE;
                    } else {
                        plb->enable_send_flag = DISABLE;
                    }

                    plb->recv_asdu_con_timer.flag = STOP;

                    if ( plb->transfer_flag == START ) {  //如果没有收到控制站的启动传输命令，不进行解析数据

                        //ret = ApplyLayerHandle_4 ( pBuf, len, port_num );
                        ret = I_TYPE_FRAME;
                    } else {  //返回没传输不被允许
                        PFUNC ( TEM_WARNING, DEBUG_PRO104, "transfer_flag stop\n" );
                        ret = TRANS_DISABLE;
                    }
                }
            } else {
                ret = FRAME_ERR;
            }
        } else {
            ret = FRAME_ERR;
        }

        if ( ret != FRAME_ERR ) { //如果收到的帧格式正确，链路空闲倒计时。
            plb->link_idle_timer.flag = START;
            plb->link_idle_timer.cnt = 0;
        }
    }

    return ret;
}

/**
*************************************************************************************
*  @brief  104规约通信口检帧接口
*  @author  liht
*  @param[in] BUF_MUX *buf
*  @param[in] uint16 port_id
*  @param[out] BUF_MUX *buf
*  @return  输出在buf的结构体中
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
uint16 Gb104::DealComBuf ( BUF_MUX *buf, uint16 port_id )
{
    PRO_TIMEOUT_4 *tab = 0;
    uint16 bytes = 0;
    tab = SelectTimeOut ( port_id );

    if ( tab == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "port id error %d,please check the port is setting to pro\n", port_id );
        return 0;
    }

    while ( buf->recv.out_cnt != buf->recv.in_cnt ) {
        if ( tab->begin68 == 0 ) {
            if ( buf->recv.buf[buf->recv.out_cnt] == 0x68 ) {
                tab->begin68 = 1;
                buf->frame.in_cnt = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "68 frame\n" );
            }
        }


        if ( tab->begin68 == 1 ) {
            buf->frame.buf[buf->frame.in_cnt++] = buf->recv.buf[buf->recv.out_cnt];
            //PFUNC(TEM_WARNING,DEBUG_PRO104,"frame byte %02x begin68=%d\n",buf->recv.buf[buf->recv.out_cnt],tab->begin68);

            if ( buf->frame.in_cnt == ( ( uint8 ) ( buf->frame.buf[1] ) + 2 ) ) {
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "68 frame ok\n" );
                tab->begin68 = 0;
                memcpy ( member.ProtBuf.parser_buf, ( char* ) buf->frame.buf, buf->frame.in_cnt );
                member.ProtBuf.parser_frame_len = buf->frame.in_cnt;
                member.ProtBuf.valid_flag = 1;
                buf->frame.in_cnt = 0;
                return 1;
            }

            if ( buf->frame.in_cnt > 256 ) {
                tab->begin68 = 0;
                buf->frame.in_cnt = 0;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "frame len over flow 256 \n" );
            }
        }

        buf->recv.out_cnt++;

        if ( buf->recv.out_cnt >= BUFFER_SIZE ) {
            buf->recv.out_cnt = 0;
        }

        bytes++;

        if ( bytes > 255 ) {
            bytes = 0;
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "break cycle %d\n", bytes );
            break;
        }

        if ( ( tab->begin68 == 0 ) ) { //其他情况，由超时处理.
            tab->frame = 0;
            tab->frame_flag = 0;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "no wait\n" );
        } else {
            tab->frame_flag = 1;
        }
    }

    return 0 ;
}

/**
*************************************************************************************
*  @brief  规约解析时查找关键字的位置和长度
*  @author  liht
*  @param[in] uint8 keyword_id
*  @param[in] uint16 inlen
*  @param[out] uint8 *pos
*  @param[out] uint8 *len
*  @return  成功返回1，否则返回0，返回1代表找到关键字成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb104:: GetKeyWordPositionAndLen ( uint8 keyword_id, uint8 *pos, uint8 *len )
{
    char ret = 0;

    switch ( keyword_id ) {

    case KEY_WORD_LINKADDR:

    case KEY_WORD_COMADDR:

        if ( member.pro_cfg.send_cause_size == 2 ) {
            *pos = 10;
        } else {
            *pos = 9;
        }

        if ( member.pro_cfg.link_addr_size == 2 ) {
            *len = 2;
        } else {
            *len = 1;
        }

        ret = 1;

        break;

    case KEY_WORD_TYPE:

        *pos = 6;
        *len = 1;

        break;

    case KEY_WORD_VSQ:

        *pos = 7;
        *len = 1;
        break;

    case KEY_WORD_SENDCAUSE:

        *pos = 8;

        if ( member.pro_cfg.send_cause_size == 2 ) {
            *len = 2;
        } else {
            *len = 1;
        }

        break;

    case KEY_WORD_DATA_START:

        *pos = 8;

        if ( member.pro_cfg.send_cause_size == 2 ) {
            *pos += 2;
        } else {
            *pos += 1;
        }

        if ( member.pro_cfg.link_addr_size == 2 ) {
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

char Gb104::DealTimeOut ( BUF_MUX *buf, uint16 port_id )
{

    int i;
    PRO_LOGIC_BUF04 *plb = 0;
    DataType3 *ykseltime;
    Data < BaseDataType > data;
    user_data.GetData ( PARA_5091, &data );
    ykseltime = ( DataType3* ) data.Data;
        
    plb = gb104.SelectLogicBuf ( port_id );

    if ( plb == 0 ) {

        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to deal pack frame\n" );
        return 0;
    }

    for ( i = 0;i < MAX_PORT;i++ ) {
        // PFUNC ( TEM_INFO,DEBUG_PRO104, "%d,%d,%d,%d,%d\n", timeout104[i].frame_flag,
        //timeout104[i].byte_flag, timeout104[i].link_flag,
        // timeout104[i].resend_flag, timeout104[i].test_flag );

        if ( member.timeout104[i].en == 1 ) {

            if ( member.timeout104[i].frame_flag == 1 ) {//if the frame is timeout ,the process is here
                member.timeout104[i].frame++;

                if ( member.timeout104[i].frame > 3 ) {
                    member.timeout104[i].begin10 = 0;
                    member.timeout104[i].begin68 = 0;
                    member.timeout104[i].frame = 0;
                    member.timeout104[i].frame_flag = 0;
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "frame timeout\n" );
                }
            }

            if ( member.timeout104[i].byte_flag == 1 ) {
                member.timeout104[i].byte_flag = 0;
            }

            if ( member.timeout104[i].yk_en == 1 ) {//delay 30s send request link status
                member.timeout104[i].yk_cnt++;

                if ( member.timeout104[i].yk_cnt >= ykseltime->Data ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk timeout\n" );
                    member.timeout104[i].yk_cnt = 0;
                    member.timeout104[i].yk_en = 0;
                    member.timeout104[i].yk_timeout_flag = 1;

                }
            }

            if ( member.timeout104[i].btj_en == 1 ) {//delay 30s send request link status 步调节
                member.timeout104[i].btj_cnt++;

                if ( member.timeout104[i].btj_cnt >= ykseltime->Data ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "btj timeout\n" );
                    member.timeout104[i].btj_cnt = 0;
                    member.timeout104[i].btj_en = 0;
                    member.timeout104[i].btj_timeout_flag = 1;

                }
            }
        }
    }

    return 1;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
char Gb104::ConfigTimeOut ( uint16 port_id )
{
    static uint8 cnt = 0;
    int i;

    if ( port_id == 0 ) return 0;

    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.timeout104[i].port_id == port_id ) {
            member.timeout104[i].en = 1;
            return 1;
        }
    }

    if ( cnt >= MAX_PORT ) {
		cnt=0;
		return 0;

	}

    member.timeout104[cnt].en = 1;

    member.timeout104[cnt].port_id = port_id;

    cnt++;

    return 1;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
PRO_TIMEOUT_4* Gb104::SelectTimeOut ( uint16 port_id )
{
    int i;

    if ( port_id == 0 ) return 0;

    for ( i = 0;i < MAX_PORT;i++ ) {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "timeout104[%d]=%d,in_port_id=%d\n", i, member.timeout104[i].port_id, port_id );
        if ( member.timeout104[i].port_id == port_id ) {
            return &member.timeout104[i];
        }
    }

    return 0;
}

int Gb104::ParaCheck ( void )
{
    int i;
///////////////////////////////////protocol frame key word len and trans style check/////////////

    if ( member.pro_cfg.time_tag > CP56 ) {
        member.pro_cfg.time_tag = CP56;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Time Tag is Larger than 7,will be set by %d\n", member.pro_cfg.time_tag );
    }

    if ( member.pro_cfg.common_unit_addr_size > 2 ) {
        member.pro_cfg.common_unit_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "common_unit_addr_size is Larger than 2,will be set by %d\n", member.pro_cfg.common_unit_addr_size );
    } else if ( member.pro_cfg.common_unit_addr_size < 1 ) {
        member.pro_cfg.common_unit_addr_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "common_unit_addr_size is less than 1,will be set by %d\n", member.pro_cfg.common_unit_addr_size );
    }

    if ( member.pro_cfg.datasign_addr_size > 3 ) {
        member.pro_cfg.datasign_addr_size = 3;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "datasign_addr_size is Larger than 3,will be set by %d\n", member.pro_cfg.datasign_addr_size );
    } else if ( member.pro_cfg.datasign_addr_size < 2 ) {
        member.pro_cfg.datasign_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "datasign_addr_size is less than 2,will be set by %d\n", member.pro_cfg.datasign_addr_size );
    }

    if ( member.pro_cfg.link_addr_size > 2 ) {
        member.pro_cfg.link_addr_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "link_addr_size is Larger than 2,will be set by %d\n", member.pro_cfg.link_addr_size );
    } else if ( member.pro_cfg.link_addr_size < 1 ) {
        member.pro_cfg.link_addr_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "link_addr_size is less than 1,will be set by %d\n", member.pro_cfg.link_addr_size );
    }

    if ( member.pro_cfg.send_cause_size > 2 ) {
        member.pro_cfg.send_cause_size = 2;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "send_cause_size is Larger than 2,will be set by %d\n", member.pro_cfg.send_cause_size );
    } else if ( member.pro_cfg.send_cause_size < 1 ) {
        member.pro_cfg.send_cause_size = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "send_cause_size is less than 1,will be set by %d\n", member.pro_cfg.send_cause_size );
    }

    if ( member.pro_cfg.trans_style != EQ_TRANS && member.pro_cfg.trans_style != UNEQ_TRANS ) {
        member.pro_cfg.trans_style = UNEQ_TRANS;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "trans_style is invalid,will be set by %d\n", member.pro_cfg.trans_style );
    } else if ( member.pro_cfg.trans_style < 1 ) {
        member.pro_cfg.trans_style = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "trans_style is less than 1,will be set by %d\n", member.pro_cfg.trans_style );
    }

    if ( member.pro_cfg.enable_interrupt_process > 1 ) {
        member.pro_cfg.enable_interrupt_process = 1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "enable_interrupt_process is invalid,will be set by %d\n", member.pro_cfg.enable_interrupt_process );
    }

///////////////////////////////////////total ask data type check///////
    if ( member.pro_data_type_cfg.syx_type != M_SP_NA_1 && member.pro_data_type_cfg.syx_type != M_PS_NA_1 ) {
        member.pro_data_type_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "syx_type is invalid,will be set by %d\n", member.pro_data_type_cfg.syx_type );
    }

    if ( member.pro_data_type_cfg.dyx_type != M_DP_NA_1 ) {
        member.pro_data_type_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "dyx_type is invalid,will be set by %d\n", member.pro_data_type_cfg.dyx_type );
    }

    if ( member.pro_data_type_cfg.yc_type != M_ME_NA_1
            && member.pro_data_type_cfg.yc_type != M_ME_NB_1
            && member.pro_data_type_cfg.yc_type != M_ME_NC_1 ) {
        member.pro_data_type_cfg.yc_type = M_ME_NB_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "yc_type is invalid,will be set by %d\n", member.pro_data_type_cfg.yc_type );
    }

    if ( member.pro_data_type_cfg.yk_type != C_SC_NA_1
            && member.pro_data_type_cfg.yk_type != C_DC_NA_1 ) {
        member.pro_data_type_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "yk_type is invalid,will be set by %d\n", member.pro_data_type_cfg.yk_type );
    }

////////////////////////////////////////class 1 data type check///////////
    if ( member.class1_datatype_cfg.syx_type != M_SP_NA_1
            && member.class1_datatype_cfg.syx_type != M_PS_NA_1
            && member.class1_datatype_cfg.syx_type != M_SP_TA_1
            && member.class1_datatype_cfg.syx_type != M_SP_TB_1 ) {
        member.class1_datatype_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class1 syx_type is invalid,will be set by %d\n", member.class1_datatype_cfg.syx_type );
    }

    if ( member.class1_datatype_cfg.dyx_type != M_DP_NA_1
            && member.class1_datatype_cfg.syx_type != M_DP_TA_1
            && member.class1_datatype_cfg.syx_type != M_DP_TB_1 ) {
        member.class1_datatype_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class1 dyx_type is invalid,will be set by %d\n", member.class1_datatype_cfg.dyx_type );
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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class1 yc_type is invalid,will be set by %d\n", member.class1_datatype_cfg.yc_type );
    }

    if ( member.class1_datatype_cfg.yk_type != C_SC_NA_1
            && member.class1_datatype_cfg.yk_type != C_DC_NA_1 ) {
        member.class1_datatype_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class1 yk_type is invalid,will be set by %d\n", member.class1_datatype_cfg.yk_type );
    }


    ////////////////////////class2 data type check////////////////
    if ( member.class2_datatype_cfg.syx_type != M_SP_NA_1
            && member.class2_datatype_cfg.syx_type != M_PS_NA_1
            && member.class2_datatype_cfg.syx_type != M_SP_TA_1
            && member.class2_datatype_cfg.syx_type != M_SP_TB_1 ) {
        member.class2_datatype_cfg.syx_type = M_SP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class2 syx_type is invalid,will be set by %d\n", member.class2_datatype_cfg.syx_type );
    }

    if ( member.class2_datatype_cfg.dyx_type != M_DP_NA_1
            && member.class2_datatype_cfg.dyx_type != M_DP_TA_1
            && member.class2_datatype_cfg.dyx_type != M_DP_TB_1 ) {
        member.class2_datatype_cfg.dyx_type = M_DP_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class2 dyx_type is invalid,will be set by %d\n", member.class2_datatype_cfg.dyx_type );
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
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class2 yc_type is invalid,will be set by %d\n", member.class2_datatype_cfg.yc_type );
    }

    if ( member.class2_datatype_cfg.yk_type != C_SC_NA_1
            && member.class2_datatype_cfg.yk_type != C_DC_NA_1 ) {
        member.class2_datatype_cfg.yk_type = C_SC_NA_1;
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "class1 yk_type is invalid,will be set by %d\n", member.class2_datatype_cfg.yk_type );
    }

//////////////////////////////data class grade and priority check////////////
    for ( i = 0;i < 8;i++ ) {
        if ( member.ProDataClassCfgTable[i].grade != CLASS1 && member.ProDataClassCfgTable[i].grade != CLASS2 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "please confirm the data class is set by \"1\" or \"2\",you set [%d]\n", member.ProDataClassCfgTable[i].grade );
        }

        if ( member.ProDataClassCfgTable[i].pri == 0 )  {
            member.ProDataClassCfgTable[i].pri = i + 1;
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "The Priority is forbidden setting by zero\n", member.ProDataClassCfgTable[i].grade );
        }
    }

/////////////////////////data_group_cfg check//////////////////////

    for ( i = 0;i < 6;i++ ) {
        if ( member.data_group_cfg[i].start_group_id == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "start_group_id is forbidden setting by zero,\n" );
        }

        if ( ( member.data_group_cfg[i].max_group_num + member.data_group_cfg[i].start_group_id - 1 ) > 16 )  {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "The Group Id adding group Num is more than 16\n" );
        }
    }

    if ( member.InerPara.K <= 0 ) {
        member.InerPara.K = 12;
    }

    if ( member.InerPara.W <= 0 ) {
        member.InerPara.W = 8;
    }

    if ( member.InerPara.T0 <= 0 ) {
        member.InerPara.T0 = 10;
    }

    if ( member.InerPara.T1 <= 0 ) {
        member.InerPara.T1 = 12;
    }

    if ( member.InerPara.T2 <= 0 ) {
        member.InerPara.T2 = 5;
    }

    if ( member.InerPara.T3 <= 0 ) {
        member.InerPara.T3 = 15;
    }

    if ( member.InerPara.T4 <= 0 ) {
        member.InerPara.T4 = 8;
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  xml 节点内容向参数缓存转换
*  @author  liht
*  @param[in] xmlNodePtr node

*  @return  成功返回0，
*  @note 此函数递归调用，注意xml文件的子节点级数不能太深，要根据硬件资源来定。
*  @see  NULL
**************************************************************************************
*/
int Gb104:: ParaConfig ( xmlNodePtr node )
{

    while ( node ) {
        //////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"LinkAddrSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.link_addr_size = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "LinkAddrSize=%d\n", member.pro_cfg.link_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"CommonAddrSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.common_unit_addr_size = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "CommonAddrSize=%d\n", member.pro_cfg.common_unit_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"TransCauseSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.send_cause_size = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransCauseSize=%d\n", member.pro_cfg.send_cause_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"DataSignSize" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.datasign_addr_size = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "DataSignSize=%d\n", member.pro_cfg.datasign_addr_size );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        //////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TransType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.trans_style = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }


        if ( xmlStrcmp ( node->name, BAD_CAST"T0" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.T0 = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"T1" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.T1 = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"T2" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.T2 = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"T3" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.T3 = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"T4" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.T4 = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"K" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.K = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TransType=%d\n", member.pro_cfg.trans_style );
            xmlFree ( content );
            member.xml_para_cnt++;
        }


        if ( xmlStrcmp ( node->name, BAD_CAST"W" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.InerPara.W = atoi ( ( char* ) content );
            PFUNC ( TEM_INFO, DEBUG_PRO104, "member.InerPara.W=%d\n", member.InerPara.W );
            xmlFree ( content );
            member.xml_para_cnt++;
        }





        ////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TsYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.syx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TsYxType=%d\n", member.pro_data_type_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"TdYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.dyx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TdYxType=%d\n", member.pro_data_type_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"TYcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.yc_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TYcType=%d\n", member.pro_data_type_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"TYkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_data_type_cfg.yk_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TYkType=%d\n", member.pro_data_type_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }


        ////////////////////class 1 data type
        if ( xmlStrcmp ( node->name, BAD_CAST"C1sYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.syx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C1sYxType=%d\n", member.class1_datatype_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C1dYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.dyx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C1dYxType=%d\n", member.class1_datatype_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C1YcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.yc_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C1YcType=%d\n", member.class1_datatype_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C1YkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class1_datatype_cfg.yk_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C1YkType=%d\n", member.class1_datatype_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ////////////////////class 2 data type
        if ( xmlStrcmp ( node->name, BAD_CAST"C2sYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.syx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C2sYxType=%d\n", member.class2_datatype_cfg.syx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C2dYxType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.dyx_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C2dYxType=%d\n", member.class2_datatype_cfg.dyx_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C2YcType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.yc_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C2YcType=%d\n", member.class2_datatype_cfg.yc_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"C2YkType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.class2_datatype_cfg.yk_type = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "C2YkType=%d\n", member.class2_datatype_cfg.yk_type );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        //////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"TimeTag" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.time_tag = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TimeTag=%d\n", member.pro_cfg.time_tag );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"InitOverGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[0].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[0].type = INIT_OVER_DATA;
            member.ProDataClassCfgTable[0].fp = PackInitOverData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "InitOverGrade=%d\n", member.ProDataClassCfgTable[0].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"AckTotalAskGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[1].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[1].type = ACK_STATION_ASK_DATA;
            member.ProDataClassCfgTable[1].fp = PackAckSationAskData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "AckTotalAskGrade=%d\n", member.ProDataClassCfgTable[1].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YxChangeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[2].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[2].type = CHANGE_YX_DATA;
            member.ProDataClassCfgTable[2].fp = PackChangeYxData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YxChangeGrade=%d\n", member.ProDataClassCfgTable[2].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }


        if ( xmlStrcmp ( node->name, BAD_CAST"CtrlAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[3].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[3].type = CTRL_RET_DATA;
            member.ProDataClassCfgTable[3].fp = PackCtrlRetData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "CtrlAckGrade=%d\n", member.ProDataClassCfgTable[3].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"SoeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[4].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[4].type = SOE_DATA;
            member.ProDataClassCfgTable[4].fp = PackSoeData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "SoeGrade=%d\n", member.ProDataClassCfgTable[4].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        if ( xmlStrcmp ( node->name, BAD_CAST"TimeAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[5].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[5].type = TIME_TB_DATA;
            member.ProDataClassCfgTable[5].fp = PackTimeTbData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TimeAckGrade=%d\n", member.ProDataClassCfgTable[5].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"GroupAckGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[6].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[6].type = ACK_GROUP_ASK_DATA;
            member.ProDataClassCfgTable[6].fp = PackAckGroupAskData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "GroupAckGrade=%d\n", member.ProDataClassCfgTable[6].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        if ( xmlStrcmp ( node->name, BAD_CAST"YcChangeGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[7].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[7].type = CHANGE_YC_DATA;
            member.ProDataClassCfgTable[7].fp = PackChangeYcData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YcChangeGrade=%d\n", member.ProDataClassCfgTable[7].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StepAdjGrade" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[8].grade = atoi ( ( char* ) content );;
            member.ProDataClassCfgTable[8].type = STEP_RET_DATA;
            member.ProDataClassCfgTable[8].fp = PackStepRetData_4;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StepAdjGrade=%d\n", member.ProDataClassCfgTable[8].grade );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StepAdjPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[8].pri = atoi ( ( char* ) content );;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StepAdjPri=%d\n", member.ProDataClassCfgTable[8].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"InitOverPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[0].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "InitOverPri=%d\n", member.ProDataClassCfgTable[0].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        if ( xmlStrcmp ( node->name, BAD_CAST"AckTotalAskPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[1].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "AckTotalAskPri=%d\n", member.ProDataClassCfgTable[1].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YxChangePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[2].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YxChangePri=%d\n", member.ProDataClassCfgTable[2].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        if ( xmlStrcmp ( node->name, BAD_CAST"CtrlAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[3].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "CtrlAckPri=%d\n", member.ProDataClassCfgTable[3].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"SoePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[4].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "SoePri=%d\n", member.ProDataClassCfgTable[4].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }



        if ( xmlStrcmp ( node->name, BAD_CAST"TimeAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[5].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "TimeAckPri=%d\n", member.ProDataClassCfgTable[5].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"GroupAckPri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[6].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "GroupAckPri=%d\n", member.ProDataClassCfgTable[6].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YcChangePri" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.ProDataClassCfgTable[7].pri = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YcChangePri=%d\n", member.ProDataClassCfgTable[7].pri );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"ServiceType" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.pro_cfg.enable_interrupt_process = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "ServiceType=%d\n", member.pro_cfg.enable_interrupt_process );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        //////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YxGroupStartId=%d\n", member.data_group_cfg[0].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YxGroupNum=%d\n", member.data_group_cfg[0].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YxGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[0].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YxGroupDataNum=%d\n", member.data_group_cfg[0].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ///////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YcGroupStartId=%d\n", member.data_group_cfg[1].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YcGroupNum=%d\n", member.data_group_cfg[1].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"YcGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[1].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "YcGroupDataNum=%d\n", member.data_group_cfg[1].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StepGroupStartId=%d\n", member.data_group_cfg[2].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StepGroupNum=%d\n", member.data_group_cfg[2].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StepGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[2].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StepGroupDataNum=%d\n", member.data_group_cfg[2].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        //////////////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "BcdGroupStartId=%d\n", member.data_group_cfg[3].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "BcdGroupNum=%d\n", member.data_group_cfg[3].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"BcdGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[3].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "BcdGroupDataNum=%d\n", member.data_group_cfg[3].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        ///////////////////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StGroupStartId=%d\n", member.data_group_cfg[4].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StGroupNum=%d\n", member.data_group_cfg[4].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"StGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[4].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "StGroupDataNum=%d\n", member.data_group_cfg[4].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        //////////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupStartId" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].start_group_id = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "ResGroupStartId=%d\n", member.data_group_cfg[5].start_group_id );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].max_group_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "ResGroupNum=%d\n", member.data_group_cfg[5].max_group_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }

        if ( xmlStrcmp ( node->name, BAD_CAST"ResGroupDataNum" ) == 0 ) {
            xmlChar *content = xmlNodeGetContent ( node );
            member.data_group_cfg[5].group_data_num = atoi ( ( char* ) content );
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "ResGroupDataNum=%d\n", member.data_group_cfg[5].group_data_num );
            xmlFree ( content );
            member.xml_para_cnt++;
        }


        ///////////////////////
        if ( xmlStrcmp ( node->name, BAD_CAST"para" ) == 0 ) {
            if ( ParaConfig ( node->children ) != -1 ) {
                // //PFUNC ( TEM_INFO,DEBUG_PRO104, " 888digui //child name is \"%s\"\n", node->children->name );
            }
        }

        node = node->next;
    }

    if ( node == 0 ) {
        // //PFUNC ( TEM_INFO,DEBUG_PRO104, "@@@@@@@@@@@@@@@@@@ digui return\n" );
        return -1;
    }

    return 0;
}


/**
*************************************************************************************
*  @brief 从参数xml文件中读取参数
*  @author  liht
*  @return  成功返回0，
*  @note
*  @see  NULL
**************************************************************************************
*/
int  Gb104::ReadCfg ( void )
{

    xmlDocPtr doc;
    xmlNodePtr root;

	if(isFileExist(CONFIG_XML_PATH_4)==0)
    doc = xmlReadFile ( CONFIG_XML_PATH_4, "UTF-8", XML_PARSE_RECOVER );
    else{
    	copy(DEFAULT_CONFIG_XML_PATH_4,CONFIG_XML_PATH_4);
    }
	if(isFileExist(CONFIG_XML_PATH_4)==0)
    doc = xmlReadFile ( CONFIG_XML_PATH_4, "UTF-8", XML_PARSE_RECOVER );	
    else{
        PFUNC ( TEM_WARNING, DEBUG_PRO101, "Fail open %s\n", CONFIG_XML_PATH_4 );
    	return -1;
    }

    root = xmlDocGetRootElement ( doc );

    if ( root == NULL ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "Empty document\n" );
        xmlFreeDoc ( doc );
        return -1;
    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "root name is \"%s\"\n", root->name );

    while ( root ) {
        if ( xmlStrcmp ( root->name, BAD_CAST"para" ) == 0 ) {
            ParaConfig ( root->children );
        }

        root = root->next;
    }

    xmlFreeDoc ( doc );

    if (  member.xml_para_cnt != XML_PARA04_NUM ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "The Para Read Fail! respect num=%d,read num=%d\n", XML_PARA04_NUM, member.xml_para_cnt );
        return -1;
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
char Gb104::ConfigLogicBuf ( uint16 port_id )
{
    static uint8 cnt = 0;
    int i;

    if ( port_id == 0 ) return 0;

    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.pro_logic_buf04[i].pro_logic_buf.port_id == port_id ) {
            return 1;
        }
    }

    if ( cnt >= MAX_PORT ){
		cnt=0;
		return 0;

	}

    member.pro_logic_buf04[cnt].pro_logic_buf.port_id = port_id;

    cnt++;

    return 1;
}

/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
PRO_LOGIC_BUF04* Gb104::SelectLogicBuf ( uint16 port_id )
{
    int i;

    if ( port_id == 0 ) return 0;

    for ( i = 0;i < MAX_PORT;i++ ) {
        if ( member.pro_logic_buf04[i].pro_logic_buf.port_id == port_id ) {
            return &member.pro_logic_buf04[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  组帧和解析函数初始化列表
*  @author  liht
*  @param
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb104::InitFunc ( void )
{
//   控制方向过程信息命令//////////////////////////////////////////
    member.parser_func[0].type = C_SC_NA_1;
    member.parser_func[0].fp = Parser_C_SC_NA_4;
    member.parser_func[0].func_id = GET_DATA;
    member.parser_func[0].pcb = 0;
    member.parser_func[1].type = C_DC_NA_1;
    member.parser_func[1].fp = Parser_C_DC_NA_4;
    member.parser_func[1].func_id = YK_APP;
    member.parser_func[1].pcb = 0;
    member.parser_func[2].type = C_RC_NA_1;
    member.parser_func[2].fp = Parser_C_RC_NA_4;
    member.parser_func[2].func_id = YK_APP;
    member.parser_func[2].pcb = 0;
    member.parser_func[3].type = C_SE_NA_1;
    member.parser_func[3].fp = Parser_C_SE_NA_4;
    member.parser_func[3].func_id = ADJUST_APP;
    member.parser_func[3].pcb = 0;
    member.parser_func[4].type = C_SE_NB_1;
    member.parser_func[4].fp = Parser_C_SE_NB_4;
    member.parser_func[4].func_id = YK_APP;
    member.parser_func[4].pcb = 0;
    member.parser_func[5].type = C_SE_NC_1;
    member.parser_func[5].fp = Parser_C_SE_NC_4;
    member.parser_func[5].func_id = SET_PARA;
    member.parser_func[5].pcb = 0;
    member.parser_func[6].type = C_BO_NC_1;
    member.parser_func[6].fp = Parser_C_BO_NC_4;
    member.parser_func[6].func_id = SET_PARA;
    member.parser_func[6].pcb = 0;
//   在控制方向的系统命令（站特定参数)
    member.parser_func[7].type = C_IC_NA_1;
    member.parser_func[7].fp = Parser_C_IC_NA_4;
    member.parser_func[7].func_id = GET_DATA;
    member.parser_func[7].pcb = 0;
    member.parser_func[8].type = C_CI_NA_1;
    member.parser_func[8].fp = Parser_C_CI_NA_4;
    member.parser_func[8].func_id = GET_DATA;
    member.parser_func[8].pcb = 0;
    member.parser_func[9].type = C_RD_NA_1;
    member.parser_func[9].fp = Parser_C_RD_NA_4;
    member.parser_func[9].func_id = GET_DATA;
    member.parser_func[9].pcb = 0;
    member.parser_func[10].type = C_TS_NA_1;
    member.parser_func[10].fp = Parser_C_TS_NA_4;
    member.parser_func[10].func_id = 0;
    member.parser_func[10].pcb = 0;
    member.parser_func[11].type = C_RP_NA_1;
    member.parser_func[11].fp = Parser_C_RP_NA_4;
    member.parser_func[11].func_id = RESET_LINK;
    member.parser_func[11].pcb = 0;
    member.parser_func[12].type = C_CD_NA_1;
    member.parser_func[12].fp = Parser_C_CD_NA_4;
    member.parser_func[12].func_id = GET_TIME;
    member.parser_func[12].pcb = 0;
// 在控制方向的参数命令（站特定参数)////////////////////////////////////
    member.parser_func[13].type = P_ME_NA_1;
    member.parser_func[13].fp = Parser_P_ME_NA_4;
    member.parser_func[13].func_id = SET_PARA;
    member.parser_func[13].pcb = 0;
    member.parser_func[14].type = P_ME_NB_1;
    member.parser_func[14].fp = Parser_P_ME_NB_4;
    member.parser_func[14].func_id = SET_PARA;
    member.parser_func[14].pcb = 0;
    member.parser_func[15].type = P_ME_NC_1;
    member.parser_func[15].fp = Parser_P_ME_NC_4;
    member.parser_func[15].func_id = SET_PARA;
    member.parser_func[15].pcb = 0;
    member.parser_func[16].type = P_AC_NA_1;
    member.parser_func[16].fp = Parser_P_AC_NA_4;
    member.parser_func[16].func_id = ACTIVE_PARA;
    member.parser_func[16].pcb = 0;


// 文件传输（站特定参数)//////////////////////////////////////
    member.parser_func[17].type = F_SC_NA_1;
    member.parser_func[17].fp = Parser_F_SC_NA_4;
    member.parser_func[17].func_id = READ_FILE_INFO;
    member.parser_func[17].pcb = 0;
    member.parser_func[18].type = F_AF_NA_1;
    member.parser_func[18].fp = Parser_F_AF_NA_4;
    member.parser_func[18].func_id = READ_FILE;
    member.parser_func[18].pcb = 0;
    member.parser_func[19].type = C_CS_NA_1;
    member.parser_func[19].fp = Parser_C_CS_NA_4;
    member.parser_func[19].func_id = SET_TIME;
    member.parser_func[19].pcb = 0;
//////////////终端参数
    member.parser_func[20].type = P_XX_XX_1;
    member.parser_func[20].fp = Parser_P_XX_XX_4;
    member.parser_func[20].func_id = 0;
    member.parser_func[20].pcb = 0;


    member.parser_func[21].type = C_SE_TA_1;
    member.parser_func[21].fp = Parser_C_SE_TA_4;
    member.parser_func[21].func_id = 0;
    member.parser_func[21].pcb = 0;

    member.parser_func[22].type = C_SE_TB_1;
    member.parser_func[22].fp = Parser_C_SE_TB_4;
    member.parser_func[22].func_id = 0;
    member.parser_func[22].pcb = 0;

    member.parser_func[23].type = C_SE_TC_1;
    member.parser_func[23].fp = Parser_C_SE_TC_4;
    member.parser_func[23].func_id = 0;
    member.parser_func[23].pcb = 0;

    member.parser_func[24].type = C_DC_TA_1;
    member.parser_func[24].fp = Parser_C_DC_TA_4;
    member.parser_func[24].func_id = 0;
    member.parser_func[24].pcb = 0;

    member.parser_func[25].type = C_SC_TA_1;
    member.parser_func[25].fp = Parser_C_SC_TA_4;
    member.parser_func[25].func_id = 0;
    member.parser_func[25].pcb = 0;

    member.parser_func[26].type = C_BO_TC_1;
    member.parser_func[26].fp = Parser_C_BO_TC_4;
    member.parser_func[26].func_id = 0;
    member.parser_func[26].pcb = 0;

    member.parser_func[27].type = C_RC_TA_1;
    member.parser_func[27].fp = Parser_C_RC_TA_4;
    member.parser_func[27].func_id = 0;
    member.parser_func[27].pcb = 0;

//监视方向////////////////////////////////////////////
    member.pack_func[0].type = M_SP_NA_1;
    member.pack_func[0].fp = Pack_M_SP_NA_4;
    member.pack_func[1].type = M_SP_TA_1;
    member.pack_func[1].fp = Pack_M_SP_TA_4;
    member.pack_func[2].type = M_DP_NA_1;
    member.pack_func[2].fp = Pack_M_DP_NA_4;
    member.pack_func[3].type = M_DP_TA_1;
    member.pack_func[3].fp = Pack_M_DP_TA_4;
    member.pack_func[4].type = M_ME_NA_1;
    member.pack_func[4].fp = Pack_M_ME_NA_4;
    member.pack_func[5].type = M_ME_TA_1;
    member.pack_func[5].fp = Pack_M_ME_TA_4;
    member.pack_func[6].type = M_ME_NB_1;
    member.pack_func[6].fp = Pack_M_ME_NB_4;
    member.pack_func[7].type = M_ME_TB_1;
    member.pack_func[7].fp = Pack_M_ME_TB_4;
    member.pack_func[8].type = M_ME_NC_1;
    member.pack_func[8].fp = Pack_M_ME_NC_4;
    member.pack_func[9].type = M_ME_TC_1;
    member.pack_func[9].fp = Pack_M_ME_TC_4;
    member.pack_func[10].type = M_SP_TB_1;
    member.pack_func[10].fp = Pack_M_SP_TB_4;
    member.pack_func[11].type = M_DP_TB_1;
    member.pack_func[11].fp = Pack_M_DP_TB_4;
    member.pack_func[12].type = M_ME_TD_1;
    member.pack_func[12].fp = Pack_M_ME_TD_4;
    member.pack_func[13].type = M_ME_TE_1;
    member.pack_func[13].fp = Pack_M_ME_TE_4;
    member.pack_func[14].type = F_FR_NA_1;
    member.pack_func[14].fp = Pack_F_FR_NA_4;
    member.pack_func[15].type = F_SR_NA_1;
    member.pack_func[15].fp = Pack_F_SR_NA_4;
    member.pack_func[16].type = F_LS_NA_1;
    member.pack_func[16].fp = Pack_F_LS_NA_4;
    member.pack_func[17].type = F_SG_NA_1;
    member.pack_func[17].fp = Pack_F_SG_NA_4;
    member.pack_func[18].type = F_DR_TA_1;
    member.pack_func[18].fp = Pack_F_DR_TA_4;
    member.pack_func[19].type = M_BO_NA_1;
    member.pack_func[19].fp = Pack_M_BO_NA_4;

    member.pack_func[20].type = M_ST_NA_1;
    member.pack_func[20].fp = Pack_M_ST_NA_4;

    member.pack_func[21].type = M_ST_TA_1;
    member.pack_func[21].fp = Pack_M_ST_TA_4;
    return 1;
}

/**
*************************************************************************************
*  @brief  规约解析入口，供protocol调用
*  @author  liht
*  @param[in] BUF_MUX *buf
*  @param[in] uint16 port_id
*  @return  没有实际意义
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char deal_104 ( BUF_MUX *buf, uint16 port_id )
{
    static uint16 timer_id = 0;
    uint8 class_data_id = 0;
    char frame_type = 0;
    static uint8 flag = 0;
    char ret = 0;
    PRO_TIMEOUT_4 *to = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    char event_ret = 0;
    static uint16 test_cx_data1 = 0;//, test_cx_data2 = 220;
    to = gb104.SelectTimeOut ( port_id );
    plb = gb104.SelectLogicBuf ( port_id );
	gb104.member.CurPortId = port_id;
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
	 event_ret = gb104.ScanSpontBuf ( port_id );//事件判断
    if ( timer.IsTimesUp ( timer_id ) ) {//规约超时处理
        gb104.ProTimerCount ( port_id );
        gb104.DealTimeOut ( buf, port_id );
        CheckTimeOut ( port_id );
       


        if ( test_cx_data1 < 6 ) {
            test_cx_data1++;

            // ProSendMsg ( TASK_PRO, MSG_YX, 0x219, ( char* ) &test_cx_data1, 1 );//syx
            //ProSendMsg ( TASK_PRO, MSG_YX, 1, ( char* ) &test_cx_data1, 1 );
            //ProSendMsg ( TASK_PRO, MSG_YC, 0x4001, ( char* ) &test_cx_data2, 4 );

        }
    }

    gb104.member.ProtBuf.pack_frame_len = 0;

    if(gb104.DealComBuf ( buf, port_id ))//处理通信口数据
    frame_type = gb104.CheckFrame ( buf, port_id );//规约帧判断
    else
    frame_type = NO_TYPE_FRAME;	

    if ( frame_type == U_TYPE_FRAME ) {
        ret = U_FrameHandle ( buf, port_id );
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "ret=%d\n" , ret );

        if ( ret == FRAME_OK ) {
            ret = U_TYPE_FRAME;
        } else {
            ret = FRAME_ERR;
        }
    } else if ( frame_type == I_TYPE_FRAME ) {
        gb104.member.ProtBuf.pack_frame_len = 0;
        ret =  gb104.Parser ( gb104.member.ProtBuf.parser_buf, gb104.member.ProtBuf.parser_frame_len,
                              gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );

        if ( ret == NO_ASDU ) {
            plb->send_asdu_con_timer.flag = STOP;
            plb->send_asdu_con_timer.cnt = 0;
            plb->no_asdu_con_timer.flag = STOP;
            plb->CtrlWordBack.S_CtrlWord.Res1 = 1;
            plb->CtrlWordBack.S_CtrlWord.Res2 = 0;
            plb->CtrlWordBack.S_CtrlWord.RecvSeq = plb->ICtrlWord.I_CtrlWord.RecvSeq;
            gb104.member.ProtBuf.pack_frame_len = 0;
            Build_U_S_Frame ( gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
            gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
        } else if ( ret == FRAME_OK ) {
            //senddatahere;
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "the out data can be send directly\n" );
            gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
            gb104.member.ProtBuf.pack_frame_len = 0;
        }

    }

    // PFUNC ( TEM_WARNING,DEBUG_PRO104, "comming frame type =%d\n", frame_type );
    else {//主动上送突发数据，在初始化结束后
        if ( plb->pro_logic_buf.connect_st == 1 && plb->transfer_flag == START ) {
            mdelay ( 10 );
            gb104.member.ProtBuf.pack_frame_len = 0;
            gb104.GetServiceId ( port_id, CLASS1, &class_data_id );
            // PFUNC ( TEM_WARNING,DEBUG_PRO104, "send spont1 data %d\n", class_data_id );

            if ( class_data_id ) {
                SetCuaseWord_4 ( CAUSE_Spont );
                PackClass1Data_4 ( port_id, gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "self send, class 1 data\n" );
                //protocol.AddSendBuf ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                //protocol.CopySendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
            } else  {
                gb104.GetServiceId ( port_id, CLASS2, &class_data_id );
                //PFUNC ( TEM_WARNING,DEBUG_PRO104, "send spont2 data %d\n", class_data_id );

                if ( class_data_id ) {
                    SetCuaseWord_4 ( CAUSE_Spont );
                    PackClass2Data_4 ( port_id, gb104.member.ProtBuf.pack_buf, &gb104.member.ProtBuf.pack_frame_len );
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "self send, class 2 data\n" );
                    //protocol.AddSendBuf ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                    //protocol.CopySendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                    gb104.SendFrame ( buf, gb104.member.ProtBuf.pack_buf, gb104.member.ProtBuf.pack_frame_len );
                }
            }
        }

        //exit(0);
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  对1级数据进行组帧函数
*  @author  liht
*  @param[in] uint16 port_id
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackClass1Data_4 ( uint16 port_id, char *out_buf, uint16 *out_len )
{
    PRO_DATA_CLASS_CFG *tab;
    PRO_LOGIC_BUF04 *plb = 0;
    *out_len = 0;
    plb = gb104.SelectLogicBuf ( port_id );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to pack data\n" );
        return 0;
    }

    if ( gb104.member.LastDataClass1Id != 0 ) {
        tab = gb104.SearchClassTable ( gb104.member.LastDataClass1Id );

        if ( tab != 0 ) {
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "out_len addr=%x\n", out_len );

            if ( ( *tab->fp ) ( out_buf, out_len ) ) { //璿及数据应答主站召测的后续数据
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "len=%d,addr=%x\n", *out_len, out_len );
                return 1;
            }

            //
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  二级数据组帧
*  @author  liht
*  @param[in] uint16 port_id
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackClass2Data_4 ( uint16 port_id, char *out_buf, uint16 *out_len )
{
    PRO_DATA_CLASS_CFG *tab;
    *out_len = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( port_id );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to pack data\n" );
        return 0;
    }

    if ( gb104.member.LastDataClass2Id != 0 ) { //
        tab = gb104.SearchClassTable ( gb104.member.LastDataClass2Id );

        if ( tab != 0 ) {
            if ( ( *tab->fp ) ( out_buf, out_len ) ) { //璿级数据应答主站召测的后续数据
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "len=%d\n", *out_len );
                return 1;
            }
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  初始化结束组帧
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackInitOverData_4 ( char *out_buf, uint16 *out_len )
{

    PFUNC ( TEM_WARNING, DEBUG_PRO104, "XXXXXXXXXXXXXX%d\n", INIT_OVER_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;

    pdcc = gb104.SearchClassTable ( INIT_OVER_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, INIT_OVER_DATA, CP00 ) )
                ret = Pack_M_EI_NA_4 ( data.buf, data.bytes, out_buf, out_len );
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, INIT_OVER_DATA, CP00 ) )
                ret = Pack_M_EI_NA_4 ( data.buf, data.bytes, out_buf, out_len );
        }

        if ( ret != 0 ) {
            return 1;
        }
    }

    //gb101.LastDataClass1Id=0;

    return 0;
}

/**
*************************************************************************************
*  @brief  变化遥信数据组包
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackChangeYxData_4 ( char *out_buf, uint16 *out_len )
{
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "XXXXXXXXXXXXXX%d\n", CHANGE_YX_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    ID_MAP *imp = 0;
    PACK_FUNC *pf = 0;
    uint16 data_id = 0;
    pdcc = gb104.SearchClassTable ( CHANGE_YX_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, CHANGE_YX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb104.GetDataAddr ( data_id );

                if ( imp == 0 ) return 0;

                pf = gb104.SearchPackFuncTable ( imp->type );

                if ( pf == 0 ) return 0;

                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );

                PFUNC ( TEM_WARNING, DEBUG_PRO104, "syx id=%d,msg_type=%d,data_type=%d\n", data.buf[2] + data.buf[3]*256, imp->type, CHANGE_YX_DATA );

            } else if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, CHANGE_DYX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb104.GetDataAddr ( data_id );

                if ( imp == 0 ) return 0;

                pf = gb104.SearchPackFuncTable ( imp->type );

                if ( pf == 0 ) return 0;

                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );

                PFUNC ( TEM_WARNING, DEBUG_PRO104, "dyx id=%d,msg_type=%d,data_type=%d\n", data.buf[2] + data.buf[3]*256, imp->type, CHANGE_DYX_DATA );
            }

            //ret = Pack_M_SP_NA_1 ( data.buf, data.bytes, out_buf, out_len );
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, CHANGE_YX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb104.GetDataAddr ( data_id );

                if ( imp == 0 ) return 0;

                pf = gb104.SearchPackFuncTable ( imp->type );

                if ( pf == 0 ) return 0;

                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
            } else  if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, CHANGE_DYX_DATA, CP00 ) ) {
                memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                imp = gb104.GetDataAddr ( data_id );

                if ( imp == 0 ) return 0;

                pf = gb104.SearchPackFuncTable ( imp->type );

                if ( pf == 0 ) return 0;

                ret = pf->fp ( data.buf, data.bytes, out_buf, out_len );
            }

            //ret = Pack_M_SP_NA_1 ( data.buf, data.bytes, out_buf, out_len );
        }

        if ( ret != 0 ) {
            return 1;
        }
    }

    //gb101.LastDataClass1Id=0;

    return 0;
}

/**
*************************************************************************************
*  @brief  控制返回数据组包
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackCtrlRetData_4 ( char *out_buf, uint16 *out_len )
{


    char ret = 0;
    int8 suc=0;
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    uint16 data_id = 0;
    uint8 time_tag = CP00;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf tu pack data\n" );
        return 0;
    }

    if ( plb->pro_logic_buf.yk_cmd == C_SC_TA_1 || plb->pro_logic_buf.yk_cmd == C_DC_TA_1 ) {
        time_tag = CP56;

    }

    pdcc = gb104.SearchClassTable ( CTRL_RET_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, CTRL_RET_DATA, time_tag ) ) {

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
						suc=0;
					else
						suc=1;	
					cbi(data.buf[5],4)	;//清除SCO中RES中的自定义成功失败选择
				}	
                if ( plb->pro_logic_buf.yk_st == 1 ) {//遥控选择返回
                	if(suc)
                		SetCuaseWord_4 ( CAUSE_Actcon );
                	else
                		SetCuaseWord_4 ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因
                }	
                else if ( plb->pro_logic_buf.yk_st == 2 ) {//遥控执行返回
                	if(suc)
                		SetCuaseWord_4 ( CAUSE_Actcon );
                	else
                		SetCuaseWord_4 ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "control end \n" );
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    //ProSendMsg ( TASK_PRO, MSG_YK, data_id, &data.buf[4] , 1 );
                    //plb->pro_logic_buf.yk_st = 3;
                    protocol.member.YkPortNum = 0;
                } else if ( plb->pro_logic_buf.yk_st == 3 ) {
                    //SetCuaseWord_4 ( CAUSE_Actterm );
                    protocol.member.YkPortNum = 0;
                } else if ( plb->pro_logic_buf.yk_st == 4 ) {//遥控取消返回
                    SetCuaseWord_4 ( CAUSE_Deactcon );
                    plb->pro_logic_buf.yk_st = 0;
                    protocol.member.YkPortNum = 0;
                }

                ret = PrepareProData_4 ( data.buf, data.bytes, out_buf, out_len );

                if ( ret ) {
                    Build_I_Frame ( plb->pro_logic_buf.yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    //BuildFrame6816 ( plb->yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    memcpy ( out_buf, data.buf, data.bytes );
                    *out_len = data.bytes;
                    return 1;
                }
            }
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, CTRL_RET_DATA, time_tag ) ) {
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
						suc=0;
					else
						suc=1;	
					cbi(data.buf[5],4)	;//清除SCO中RES中的自定义成功失败选择
				}
                if ( plb->pro_logic_buf.yk_st == 1 ) {
                	if(suc)
                		SetCuaseWord_4 ( CAUSE_Actcon );
                	else
                		SetCuaseWord_4 ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因
                }				
                if ( plb->pro_logic_buf.yk_st == 2 ) {
                	if(suc)
                		SetCuaseWord_4 ( CAUSE_Actcon );
                	else
                		SetCuaseWord_4 ( CAUSE_Unknowncause );//这里根据在合肥局调试结果,返回未知原因                	
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "control end \n" );
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    //ProSendMsg ( TASK_PRO, MSG_YK, data_id, &data.buf[4], 1 );
                    //plb->pro_logic_buf.yk_st = 3;
                    protocol.member.YkPortNum = 0;
                } else if ( plb->pro_logic_buf.yk_st == 3 ) {
                    //SetCuaseWord_4 ( CAUSE_Actterm );
                    protocol.member.YkPortNum = 0;
                } else if ( plb->pro_logic_buf.yk_st == 4 ) {
                    SetCuaseWord_4 ( CAUSE_Deactcon );
                    plb->pro_logic_buf.yk_st = 0;
                    protocol.member.YkPortNum = 0;
                }

                ret = PrepareProData_4 ( data.buf, data.bytes, out_buf, out_len );

                if ( ret ) {
                    //BuildFrame6816 ( plb->yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    Build_I_Frame ( plb->pro_logic_buf.yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
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


static uint8 PackStepRetData_4 ( char *out_buf, uint16 *out_len )
{


    char ret = 0;
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    uint16 data_id = 0;
    uint8 time_tag = CP00;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf tu pack data\n" );
        return 0;
    }

    if ( plb->pro_logic_buf.btj_cmd == C_RC_TA_1 ) {
        time_tag = CP56;

    }

    pdcc = gb104.SearchClassTable ( STEP_RET_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, STEP_RET_DATA, time_tag ) ) {
                SetCuaseWord_4 ( CAUSE_Actcon );

                if ( plb->pro_logic_buf.btj_st == 2 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "btj end \n" );
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, data_id, &data.buf[4] , 1 );
                    plb->pro_logic_buf.btj_st = 3;
                } else if ( plb->pro_logic_buf.btj_st == 3 ) {
                    SetCuaseWord_4 ( CAUSE_Actterm );
                    protocol.member.BtjPortNum = 0;
                } else if ( plb->pro_logic_buf.btj_st == 4 ) {
                    SetCuaseWord_4 ( CAUSE_Deactcon );
                    plb->pro_logic_buf.btj_st = 0;
                    protocol.member.BtjPortNum = 0;
                }

                ret = PrepareProData_4 ( data.buf, data.bytes, out_buf, out_len );

                if ( ret ) {
                    Build_I_Frame ( plb->pro_logic_buf.btj_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    memcpy ( out_buf, data.buf, data.bytes );
                    *out_len = data.bytes;
                    return 1;
                }
            }
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, STEP_RET_DATA, time_tag ) ) {
                SetCuaseWord_4 ( CAUSE_Actcon );

                if ( plb->pro_logic_buf.btj_st == 2 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "btj end \n" );
                    memcpy ( ( char* ) &data_id, & data.buf[2], 2 );
                    ProSendMsg ( CTRL_MSG_TASK_ALL, MSG_STEP, data_id, &data.buf[4], 1 );
                    plb->pro_logic_buf.btj_st = 3;
                } else if ( plb->pro_logic_buf.yk_st == 3 ) {
                    SetCuaseWord_4 ( CAUSE_Actterm );
                    protocol.member.BtjPortNum = 0;
                } else if ( plb->pro_logic_buf.btj_st == 4 ) {
                    SetCuaseWord_4 ( CAUSE_Deactcon );
                    plb->pro_logic_buf.btj_st = 0;
                    protocol.member.BtjPortNum = 0;
                }

                ret = PrepareProData_4 ( data.buf, data.bytes, out_buf, out_len );

                if ( ret ) {
                    //BuildFrame6816 ( plb->yk_cmd, data.buf, &data.bytes, out_buf, *out_len );
                    Build_I_Frame ( plb->pro_logic_buf.btj_cmd, data.buf, &data.bytes, out_buf, *out_len );
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
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackChangeYcData_4 ( char *out_buf, uint16 *out_len )
{
    PFUNC ( TEM_WARNING, DEBUG_PRO104, "XXXXXXXXXXXXXX%d\n", CHANGE_YC_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    uint8 time_tag;
    time_tag = gb104.member.pro_cfg.time_tag;
    pdcc = gb104.SearchClassTable ( CHANGE_YC_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, CHANGE_YC_DATA, time_tag ) ) {
                if ( time_tag == CP24 ) {
                    ret = 0;//ret=Pack_M_ME_TA_1(data.buf,data.bytes,out_buf,out_len);
                } else if ( time_tag == CP56 ) {
                    if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TD_1 )
                        ret = Pack_M_ME_TD_4 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TE_1 )
                        ret = Pack_M_ME_TE_4 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb104.member.class1_datatype_cfg.yc_type == M_ME_TF_1 )
                        ret = Pack_M_ME_TF_4 ( data.buf, data.bytes, out_buf, out_len );
                }
            }
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, CHANGE_YC_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = 0;//ret=Pack_M_SP_TA_1(data.buf,data.bytes,out_buf,out_len);
                else if ( time_tag == CP56 ) {
                    if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TD_1 )
                        ret = Pack_M_ME_TD_4 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TE_1 )
                        ret = Pack_M_ME_TE_4 ( data.buf, data.bytes, out_buf, out_len );
                    else if ( gb104.member.class2_datatype_cfg.yc_type == M_ME_TF_1 )
                        ret = Pack_M_ME_TF_4 ( data.buf, data.bytes, out_buf, out_len );
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
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackTimeTbData_4 ( char *out_buf, uint16 *out_len )
{

    char data[100];
    uint8 datalen = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    CP56Time2a ct;
    PRO_DATA_CLASS_CFG *pdcc = 0;

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "\n" );
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }

    SetCuaseWord_4 ( CAUSE_Actcon );

    timer.GetCP56Time ( &ct );
	PFUNC ( TEM_WARNING, DEBUG_PRO101, "[%d]%d-%d-%d  %d:%d:%dms @week:%d  \n", 0
            , ct.year + 2000
            , ct.month
            , ct.day_week&0x1f
            , ct.hour_su&0x1f
            , ct.minute_iv&0x3f
            , ct.millisecond
            , ( ct.day_week >> 5 ) &0x07 );
    memcpy ( &data[datalen], ( char* ) &ct, 7 );
    // memset ( &data[datalen], 0x33, 7 );
    //dump_binary ( ( uint8* ) &ct, 7 );
    datalen += 7;

    // BuildFrame6816 ( C_CS_NA_1, out_buf, out_len, data, datalen );
    Build_I_Frame ( C_CS_NA_1, out_buf, out_len, data, datalen );

    pdcc = gb104.SearchClassTable ( TIME_TB_DATA );

    if ( pdcc == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "The Service %d not find\n", TIME_TB_DATA );
        return 0;
    }

    if ( pdcc->grade == CLASS1 ) {
        gb104.member.LastDataClass1Id = 0;
    } else {
        gb104.member.LastDataClass2Id = 0;

    }

    return 1;
}

/**
*************************************************************************************
*  @brief  Soe数据组包
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackSoeData_4 ( char *out_buf, uint16 *out_len )
{
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d\n", SOE_DATA );
    pack_buf_t data;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    char ret = 0;
    uint8 time_tag;
    time_tag = gb104.member.pro_cfg.time_tag;
    pdcc = gb104.SearchClassTable ( SOE_DATA );

    if ( pdcc != 0 ) {
        if ( pdcc->grade == CLASS1 ) {
            gb104.member.LastDataClass1Id = 0;

            if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, SOE_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_SP_TA_4 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_SP_TB_4 ( data.buf, data.bytes, out_buf, out_len );
            } else if ( gb104.GetC1Event ( gb104.member.CurPortId, &data, SOE2_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_DP_TA_4 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_DP_TB_4 ( data.buf, data.bytes, out_buf, out_len );
            }
        } else {
            gb104.member.LastDataClass2Id = 0;

            if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, SOE_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_SP_TA_4 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_SP_TB_4 ( data.buf, data.bytes, out_buf, out_len );
            } else if ( gb104.GetC2Event ( gb104.member.CurPortId, &data, SOE2_DATA, time_tag ) ) {
                if ( time_tag == CP24 )
                    ret = Pack_M_DP_TA_4 ( data.buf, data.bytes, out_buf, out_len );
                else if ( time_tag == CP56 )
                    ret = Pack_M_DP_TB_4 ( data.buf, data.bytes, out_buf, out_len );
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
*  @author  liht
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
static uint8 PackTotalAskNoData_4 ( uint8 qoi, uint8 coi, uint8 cw, char *out_buf, uint16 *out_len )
{
    char data[32];
    uint8 datalen;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to pack data\n" );
        return 0;
    }

    pdcc = gb104.SearchClassTable ( ACK_STATION_ASK_DATA );

    if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
        memset ( data, 0, 3 );
        datalen = 3;
    } else {
        memset ( data, 0, 2 );
        datalen = 2;
    }

    data[datalen++] = qoi;

    SetCuaseWord_4 ( coi );

    //BuildFrame6816 ( C_IC_NA_1, out_buf, out_len, data, datalen );
    Build_I_Frame ( C_IC_NA_1, out_buf, out_len, data, datalen );

    return 1;
}

/**
*************************************************************************************
*  @brief  总召唤组包
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackAckSationAskData_4 ( char *out_buf, uint16 *out_len )
{
    PRO_LOGIC_BUF04 *plb = 0;
    int k;
    PACK_FUNC *tab;
    PRO_DATA_CLASS_CFG *pdcc = 0;

    ID_MAP *im_tab;
    pdcc = gb104.SearchClassTable ( ACK_STATION_ASK_DATA );

    if ( pdcc == 0 ) return 0;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb != 0 ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d,%d,%d\n,", ACK_STATION_ASK_DATA, plb->pro_logic_buf.QOI, plb->pro_logic_buf.M_type );

        if ( plb->pro_logic_buf.total_ask_begin == 1  ) {
            plb->pro_logic_buf.total_ask_begin = 0;
            PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Actcon, S_ACK, out_buf, out_len );
            return 1;
        }

        *out_len = 0;

        if ( plb->pro_logic_buf.total_data_send_over == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "over station ask data\n" );

            if ( plb->pro_logic_buf.total_ask_begin == 0 ) {
                PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );

                if ( pdcc->grade == CLASS1 )
                    gb104.member.LastDataClass1Id = 0;
                else
                    gb104.member.LastDataClass2Id = 0;

                plb->pro_logic_buf.M_type = 0;

                return 1;
            }
        }

        if ( plb->pro_logic_buf.M_type == C_IC_NA_1 ) { //总召咿
            if ( plb->pro_logic_buf.QOI == CAUSE_Introgen ) {
                //PFUNC(TEM_WARNING,DEBUG_PRO104,"all::::intlen=%d,outlen=%d\n",*out_len,pb.bytes);
                SetCuaseWord_4 ( CAUSE_Introgen );

                while ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag == 0 ) {
                    //PFUNC(TEM_WARNING,DEBUG_PRO104,"col=%d,row=%d[%d]\n",plb->intro_col,plb->intro_row,gb101.introx_info[plb->intro_col][plb->intro_row].flag);
                    plb->pro_logic_buf.intro_row++;

                    if ( plb->pro_logic_buf.intro_row >= 8 ) {
                        plb->pro_logic_buf.intro_row = 0;
                        plb->pro_logic_buf.intro_col++;

                        if ( plb->pro_logic_buf.intro_col >= 16 ) {
                            plb->pro_logic_buf.intro_row = 0;
                            plb->pro_logic_buf.intro_col = 0;
                            //plb->M_type=0;
                            plb->pro_logic_buf.total_data_send_over = 0;
                            //PFUNC ( TEM_INFO, DEBUG_PRO104, "total:data Send over\n" );

                            if ( plb->pro_logic_buf.total_data_send_over == 0 ) {
                                //PFUNC ( TEM_INFO, DEBUG_PRO104, "over station ask data\n" );

                                if ( plb->pro_logic_buf.total_ask_begin == 0 ) {
                                    PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );
                                    plb->pro_logic_buf.M_type = 0;

                                    if ( pdcc->grade == CLASS1 )
                                        gb104.member.LastDataClass1Id = 0;
                                    else
                                        gb104.member.LastDataClass2Id = 0;

                                    return 1;
                                }
                            }

                            return 1;
                        }
                    }

                }

                //* ( uint16* ) &gb101.member.pb.buf[0] = gb101.member.introx_info[plb->intro_col][plb->intro_row].num;
                memcpy ( &gb104.member.pb.buf[0],
                         ( char* ) &gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].num, 1 );

                gb104.member.pb.buf[1] = 0;

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "pb.buf[0]=%02x%02x addr=%x\n",
                        //gb104.member.pb.buf[1], gb104.member.pb.buf[0], &gb104.member.pb.buf[0] );

                //PFUNC(TEM_WARNING,DEBUG_PRO104,"total:data num=%d\n",*(uint16*)&pb.buf[0]);


                //PFUNC ( TEM_INFO, DEBUG_PRO104, "111col=%d,row=%d[%d] data_num=%d\n",
                        //plb->pro_logic_buf.intro_col, plb->pro_logic_buf.intro_row, gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag,
                        //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].num );

                for ( k = 0;k < gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].num;k++ )

                {
                    im_tab = gb104.GetDataId ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].start_addr + k );//id

                    if ( im_tab != 0 ) {
                        //* ( uint16* ) &gb101.member.pb.buf[3*k+2] = im_tab->data_id;
                        memcpy ( &gb104.member.pb.buf[3*k+2], ( char* ) &im_tab->data_id, 2 );
                    } else {
                        PFUNC ( TEM_WARNING, DEBUG_PRO104, "error no addr's id %d\n",
                                gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].start_addr + k );
                    }

                    gb104.member.pb.buf[3*k+4] = 0;//size

                    //PFUNC(TEM_WARNING,DEBUG_PRO104,"[%d]%d,%d\n",k+1,*(uint16*)&pb.buf[3*k+2],pb.buf[3*k+4]);

                }

                gb104.member.pb.bytes = 3 * k + 2;

                //PFUNC(TEM_WARNING,DEBUG_PRO104,"inlen=%d\n",pb.bytes);
                protocol.member.data_type = gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].typesign;

                if ( ProGetData ( gb104.member.pb.buf, gb104.member.pb.bytes, out_buf, out_len ) ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d,typesign=%d\n", *out_len,
                            //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].typesign );

                    tab = gb104.SearchPackFuncTable ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].typesign );

                    if ( tab != 0 ) {
                        tab->fp ( out_buf, *out_len, gb104.member.pb.buf, &gb104.member.pb.bytes );
                        //PFUNC ( TEM_INFO, DEBUG_PRO104, "intlen=%d,outlen=%d\n", *out_len, gb104.member.pb.bytes );
                        memcpy ( out_buf, gb104.member.pb.buf, gb104.member.pb.bytes );
                        *out_len = gb104.member.pb.bytes;

                    }
                }

                plb->pro_logic_buf.intro_row++;

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "222col=%d,row=%d[%d]\n",
                        //plb->pro_logic_buf.intro_col, plb->pro_logic_buf.intro_row,
                        //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag );

                if ( plb->pro_logic_buf.intro_row >= 8 ) {
                    plb->pro_logic_buf.intro_row = 0;
                    plb->pro_logic_buf.intro_col++;

                    if ( plb->pro_logic_buf.intro_col >= 16 ) {
                        plb->pro_logic_buf.intro_row = 0;
                        plb->pro_logic_buf.intro_col = 0;
                        plb->pro_logic_buf.total_data_send_over = 0;
                        PFUNC ( TEM_WARNING, DEBUG_PRO104, "total:data Send over\n" );

                    }
                }

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "333col=%d,row=%d[%d]\n",
                        //plb->pro_logic_buf.intro_col, plb->pro_logic_buf.intro_row,
                        //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag );

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
*  @author  liht
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static uint8 PackAckGroupAskData_4 ( char *out_buf, uint16 *out_len )
{
    PRO_LOGIC_BUF04 *plb = 0;
    int k;
    PACK_FUNC *tab;
    PRO_DATA_CLASS_CFG *pdcc = 0;
    ID_MAP *im_tab;
    pdcc = gb104.SearchClassTable ( ACK_GROUP_ASK_DATA );

    if ( pdcc == 0 ) return 0;

    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb != 0 ) {
        plb->pro_logic_buf.intro_col = plb->pro_logic_buf.QOI - 21;

        if ( ( char ) plb->pro_logic_buf.intro_col < 0 ) {
            return 0;

        }

        if ( plb->pro_logic_buf.total_ask_begin == 1 ) {
            plb->pro_logic_buf.total_ask_begin = 0;

            if ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag == 0 ) {
                PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Deactcon, S_ACK, out_buf, out_len );
            } else {
                PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Actcon, S_ACK, out_buf, out_len );
            }

            plb->pro_logic_buf.intro_col = 0;

            return 1;
        }

        *out_len = 0;

        if ( plb->pro_logic_buf.total_data_send_over == 0 ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "over group ask data\n" );

            if ( plb->pro_logic_buf.total_ask_begin == 0 ) {
                PackTotalAskNoData_4 ( plb->pro_logic_buf.QOI, CAUSE_Actterm, S_ACK, out_buf, out_len );
                plb->pro_logic_buf.M_type = 0;

                if ( pdcc->grade == CLASS1 )
                    gb104.member.LastDataClass1Id = 0;
                else
                    gb104.member.LastDataClass2Id = 0;

                PFUNC ( TEM_WARNING, DEBUG_PRO104, "group:Ack Send over\n" );

                return 1;
            }
        }

        //PFUNC ( TEM_INFO, DEBUG_PRO104, "111col=%d,row=%d[%d]\n", plb->pro_logic_buf.intro_col, plb->pro_logic_buf.intro_row,
                //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag );

        if ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag == 1 ) {

            gb104.member.pb.buf[0] = gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].num;
            gb104.member.pb.buf[1] = 0;

            for ( k = 0;k < gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].num;k++ )

            {
                im_tab = gb104.GetDataId ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].start_addr + k );//id

                if ( im_tab != 0 ) {
                    memcpy ( &gb104.member.pb.buf[3*k+2], ( char* ) &im_tab->data_id, 2 );
                    //* ( uint16* ) &gb101.member.pb.buf[3*k+2] = im_tab->data_id;
                } else {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "error no addr's id %d\n",
                            gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].start_addr + k );
                }

                gb104.member.pb.buf[3*k+4] = 0;//size

                //    PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d]%d,%d\n", k + 1,
                //            gb104.member.pb.buf[3*k+2] + gb104.member.pb.buf[3*k+3]*256, gb104.member.pb.buf[3*k+4] );

            }

            gb104.member.pb.bytes = 3 * k + 2;

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "inlen=%d\n", gb104.member.pb.bytes );

            protocol.member.data_type = gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].typesign;

            if ( ProGetData ( gb104.member.pb.buf, gb104.member.pb.bytes, out_buf, out_len ) ) {
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "outlen=%d\n", *out_len );

                tab = gb104.SearchPackFuncTable ( gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].typesign );

                if ( tab != 0 ) {
                    SetCuaseWord_4 ( plb->pro_logic_buf.QOI );
                    tab->fp ( out_buf, *out_len, gb104.member.pb.buf, &gb104.member.pb.bytes );
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "intlen=%d,outlen=%d\n", *out_len, gb104.member.pb.bytes );
                    memcpy ( out_buf, gb104.member.pb.buf, gb104.member.pb.bytes );
                    *out_len = gb104.member.pb.bytes;
                }
            }

           //PFUNC ( TEM_INFO, DEBUG_PRO104, "pb.bytes=%d\n", gb104.member.pb.bytes );

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "333col=%d,row=%d[%d]\n",
                    //plb->pro_logic_buf.intro_col, plb->pro_logic_buf.intro_row,
                    //gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag );
            plb->pro_logic_buf.intro_row++;

            if ( plb->pro_logic_buf.intro_row >= 8 ||
                    gb104.member.introx_info[plb->pro_logic_buf.intro_col][plb->pro_logic_buf.intro_row].flag == 0 ) {
                plb->pro_logic_buf.total_data_send_over = 0;
                plb->pro_logic_buf.intro_row = 0;
                plb->pro_logic_buf.intro_col = 0;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "group:data Send over\n" );
            }
        }else{
		 plb->pro_logic_buf.total_data_send_over = 0;
         plb->pro_logic_buf.intro_row = 0;
         plb->pro_logic_buf.intro_col = 0;
         PFUNC ( TEM_WARNING, DEBUG_PRO104, "group:data Send over\n" );
		}

    }

    if ( *out_len != 0 )
        return 1;
    else
        return 0;

}

//事件数据扫描，并通知做相应处理

/**
*************************************************************************************
*  @brief  扫描对应端口的事件缓存
*  @author  liht
*  @param[in] uint16 port_id
*  @return  成功返回1或2，否则返回0，返回1代表成功有1级数据事件产生，2代表有2级数据事件产生
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char Gb104::ScanSpontBuf ( uint16 port_id )
{
    int i;

    member.CurPortId = port_id;

    for ( i = 0;i < MAX_C1_EVENT_NUM;i++ ) {

        if ( member.c1_data_buf.c1_data[i].port_id == port_id ) {
            if ( member.c1_data_buf.c1_data[i].read_flag == EVENT_NOREAD ) {
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
*  @brief  设置发送原因关键字
*  @author  liht
*  @param[in] uint8 cause
*  @return  成功返回1，否则返回0，返回1代表成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char SetCuaseWord_4 ( uint8 cause )
{
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );

    if ( plb != 0 ) {
        plb->pro_logic_buf.SendCause.BsCause.code = cause;
        plb->pro_logic_buf.SendCause.BsCause.P_N = 0;
        plb->pro_logic_buf.SendCause.BsCause.T = 0;
        plb->pro_logic_buf.SendCause.BsCause.res = 0;
        return 1;
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  获得数据服务等级ID
*  @author  liht
*  @param[in] uint16 port_id
*  @param[in] uint8 type
*  @param[out] uint8 *s_id
*  @return  成功返回1，否则返回0，返回1代表发送成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char  Gb104::GetServiceId ( uint16 port_id, uint8 type, uint8 *s_id )
{
    PRO_LOGIC_BUF04 *plb = 0;
    plb = SelectLogicBuf ( port_id );
    //PFUNC ( TEM_WARNING,DEBUG_PRO104, "type=%d[%d,%d]\n", type, CLASS1, CLASS2 );

    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf to parser\n" );
        return 0;
    }

    if ( type == CLASS1 ) {

        if ( plb->pro_logic_buf.Service1Buf.in_cnt != plb->pro_logic_buf.Service1Buf.out_cnt ) {
            *s_id = plb->pro_logic_buf.Service1Buf.id[plb->pro_logic_buf.Service1Buf.out_cnt];
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "before:L=%d,C=%d:%d\n", gb104.member.LastDataClass1Id, *s_id&0x0f, port_id );

            if ( ( !JudgeDataService_4 ( *s_id&0x0f, &gb104.member.LastDataClass1Id ) ) && gb104.member.LastDataClass1Id != 0 ) {//service no change and last serId is not zero
                *s_id = gb104.member.LastDataClass1Id;
                return 1;
            }

            if ( gb104.member.LastDataClass1Id == 0 ) {
                *s_id = 0;
                return 0;
            }

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "after:L=%d,C=%d:%d\n", gb104.member.LastDataClass1Id, *s_id&0x0f, port_id );

            plb->pro_logic_buf.Service1Buf.flag[plb->pro_logic_buf.Service1Buf.out_cnt] = EVENT_READ;
            plb->pro_logic_buf.Service1Buf.out_cnt++;

            if ( plb->pro_logic_buf.Service1Buf.out_cnt >= MAX_SERVICE_ID ) {
                plb->pro_logic_buf.Service1Buf.out_cnt = 0;
            }

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d][%d]pt=%d,type=%d,s_id=%d\n" ,
                    //plb->pro_logic_buf.Service1Buf.in_cnt, plb->pro_logic_buf.Service1Buf.out_cnt, port_id, type, *s_id&0x0f );

            return 1;
        }

        if ( gb104.member.LastDataClass1Id != 0 ) {
            *s_id = gb104.member.LastDataClass1Id;
            return 1;
        }
    } else if ( type == CLASS2 ) {
        if ( plb->pro_logic_buf.Service2Buf.in_cnt != plb->pro_logic_buf.Service2Buf.out_cnt ) {
            *s_id = plb->pro_logic_buf.Service2Buf.id[plb->pro_logic_buf.Service2Buf.out_cnt];
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "before:L=%d,C=%d:%d\n", gb104.member.LastDataClass2Id, *s_id&0x0f, port_id );

            if ( ( !JudgeDataService_4 ( *s_id&0x0f, &gb104.member.LastDataClass2Id ) ) && gb104.member.LastDataClass2Id != 0 ) {//service no change and last serId is not zero
                *s_id = gb104.member.LastDataClass2Id;
                return 1;
            }

            if ( gb104.member.LastDataClass2Id == 0 ) {
                *s_id = 0;
                return 0;
            }

            PFUNC ( TEM_WARNING, DEBUG_PRO104, "after:L=%d,C=%d:%d\n", gb104.member.LastDataClass2Id, *s_id&0x0f, port_id );

            plb->pro_logic_buf.Service2Buf.flag[plb->pro_logic_buf.Service2Buf.out_cnt] = EVENT_READ;
            plb->pro_logic_buf.Service2Buf.out_cnt++;

            if ( plb->pro_logic_buf.Service2Buf.out_cnt >= MAX_SERVICE_ID ) {
                plb->pro_logic_buf.Service2Buf.out_cnt = 0;
            }

            //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d][%d]pt=%d,type=%d,s_id=%d\n" ,
                    //plb->pro_logic_buf.Service1Buf.in_cnt, plb->pro_logic_buf.Service1Buf.out_cnt, port_id, type, *s_id&0x0f );

            return 2;
        }

        if ( gb104.member.LastDataClass2Id != 0 ) {
            *s_id = gb104.member.LastDataClass2Id;
            return 1;
        }
    }

    //PFUNC ( TEM_INFO,DEBUG_PRO104, "no new service produce\n" );

    return 0;
}

/**
*************************************************************************************
*  @brief  根据给定的数据类型查表，返回表指针
*  @author  liht
*  @param[in] uint8 data_type
*  @return  成功返回表指针，否则返回0
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
PRO_DATA_CLASS_CFG* Gb104::SearchClassTable ( uint8 data_type )
{
    int i;
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "in dt=%d\n", data_type );

    for ( i = 0;i < DATA_CLASS_TABLE_NUM;i++ ) {
        //PFUNC ( TEM_WARNING,DEBUG_PRO104, "my dt=%d\n",ProDataClassCfgTable[i].type );
        if ( member.ProDataClassCfgTable[i].type == data_type ) {

            return &member.ProDataClassCfgTable[i];
        }
    }

    return 0;
}

/**
*************************************************************************************
*  @brief  检查帧长度是否符合要求
*  @author  liht
*  @param[in] uint16 len
*  @return  成功返回1，否则返回0，返回1代表符合要求
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
char CheckDataLen_4 ( uint16 len )
{
    uint8 frame_fix_len = 6;

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "in len =%d fix len=%d\n" , len, frame_fix_len );

    if ( len > 256 - frame_fix_len ) {//帧长度是否超过最大的长度
        return 0;
    } else {
        return 1;
    }


}

/**
*************************************************************************************
*  @brief  检查发送的数据信息体地址是否是连续的，提供后续重新组帧的信息
*  @author  liht
*  @param[in] char *databuf
*  @param[in] uint16 buflen
*  @param[out] uint8  datanum
*  @return  成功返回1，否则返回0，返回1代表数据中的信息体地址连续
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char CheckAddrInOrder_4 ( char* databuf, uint16 buflen, uint8 datanum )
{
    int i = 0;
    uint8 gap = 0;
    uint32 addr = 0;

    if ( datanum == 0 || datanum == 1 ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_num=%d\n", datanum );
        return 0;
    }

    gap = buflen / datanum;//获得数据跨度

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "%d/%d[gap=%d]\n", buflen, datanum, gap );

    for ( i = 0;i < datanum - 1;i++ ) {
        addr = 0;

        if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {//信息体地址长度为3个字节
            memcpy ( ( char* ) &addr, &databuf[i*gap], 3 );
            addr += 1;
            //PFUNC(TEM_WARNING,DEBUG_PRO104,"[%02d]%02x%02x%02x|||%02x%02x%02x[gap=%d]\n",i,
            //addr>>16,addr>>8,(uint8)addr,databuf[(i+1)*gap+2],
            //databuf[(i+1)*gap+1],databuf[(i+1)*gap],gap);

            if ( memcmp ( &addr, &databuf[ ( i+1 ) *gap], 3 ) == 0 ) {//前一个与后一个比较
                continue;
            } else {
                return 0;
            }
        } else {//信息体地址长度为2个字节
            memcpy ( ( char* ) &addr, &databuf[i*gap], 2 );
            addr += 1;
            //PFUNC(TEM_WARNING,DEBUG_PRO104,"[%02d]%02x%02x|||%02x%02x[gap=%d]\n",
            //i,(uint8)(addr>>8),(uint8)addr,(uint8)databuf[(i+1)*gap+1],
            //(uint8)databuf[(i+1)*gap],gap);

            if ( memcmp ( ( char* ) &addr, &databuf[ ( i+1 ) *gap], 2 ) == 0 ) {//前一个与后一个比较
                continue;
            } else {
                return 0;
            }
        }
    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "addr is in order\n" );

    return 1;
}

/**
*************************************************************************************
*  @brief  根据信息体地址连续的返回结果，重新对数据帧进行组装
*  @author  liht
*  @param[in] char *inbuf
*  @param[in] uint16 *inlen
*  @param[out] uint16 num
*  @return  成功返回1，否则返回0，返回1代表数据重新组装成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char ReBuildDataBuf_4 ( char *inbuf, uint16 *inlen, uint8 num )
{
    int i = 0;
    uint8 gap = 0;
    char data[256];

    if ( num == 0 || inlen == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "num=%d,&inlen=%08x\n", num , inlen );
        return 0;
    }

    gap = *inlen / num;

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "num=%d,*inlen=%d\n", num, ( uint16 ) *inlen );
	*inlen = 0;
    if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {
        memcpy ( data, inbuf, 3 );

        for ( i = 0;i < num;i++ ) {
            memcpy ( &data[i* ( gap-3 ) +3], &inbuf[i*gap+3], gap - 3 );
        }

        *inlen = i * ( gap - 3 ) + 3;
    } else if ( gb104.member.pro_cfg.datasign_addr_size == 2 ) {
        memcpy ( data, inbuf, 2 );

        for ( i = 0;i < num;i++ ) {
            memcpy ( &data[i* ( gap-2 ) +2], &inbuf[i*gap+2], gap - 2 );
        }

        *inlen = i * ( gap - 2 ) + 2;
    }

    memcpy ( inbuf, data, *inlen );

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "ret len=%d\n", *inlen );
    return 1;

}

/**
*************************************************************************************
*  @brief  读应用数据后将数据向规约格式的数据转换
*  @author  liht
*  @param[in] char *inbuf
*  @param[in] uint16 inlen
*  @param[out] char *out_buf
*  @param[out] uint16 *out_len
*  @return  成功返回1，否则返回0，返回1代表数据转换成功
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char PrepareProData_4 ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len )
{
    uint16 i = 0;
    uint16 data_num = 0;
    uint16 data_id = 0;
    uint16 data_start_pos = 0;
    uint32 data_addr = 0;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = gb104.SelectLogicBuf ( gb104.member.CurPortId );
    ID_MAP *imp = 0;

    if ( plb != 0 ) {
        if ( !VerifyRecvFormalPara ( in_buf, in_len, out_buf, out_len ) ) { //校验参数
            return 0;
        }

        data_num = ( uint8 ) in_buf[0] + ( uint8 ) in_buf[1] * 256;

        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_num=%d\n", data_num );
        data_start_pos = data_num * 3 + 2;
        *out_len = 0;
        //PFUNC ( TEM_INFO,DEBUG_PRO104, "data_num=%d,data_start_pos=%d,\n", data_num, data_start_pos );
        /////////////////////数据向规约转化//////////////////

        for ( i = 0;i < data_num;i++ ) {
            memcpy ( ( char* ) &data_id, &in_buf[2+i*3], 2 );
            // data_id = * ( uint16* ) & in_buf[2+i*3];
            //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_id=0x%x\n", data_id );

            if ( data_id == 0 ) {//终端ID的数据，终端ID为0x0000
                data_addr = 0;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "this is terminal selfaddr data\n\n\n\n" );
            } else {
                imp = gb104.GetDataAddr ( data_id );

                if ( imp == 0 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "the dataid 0x%x not pair dataaddr \n" , data_id );
                    return 0;
                }

                data_addr = imp->data_addr;//根据数据的ID获得信息体地址

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_addr=%08x,data_id=%x\n", data_addr, data_id );
            }

            if ( gb104.member.pro_cfg.datasign_addr_size == 3 ) {//信息体地址为3个字节的数据组装
                memcpy ( &out_buf[ ( 3+ ( uint8 ) in_buf[4+i*3] ) *i], ( char* ) &data_addr, 3 );//组装信息体地址
                *out_len += 3;//组装数据游标
                /*
                PFUNC(TEM_WARNING,DEBUG_PRO104,"[%08x]%02x%02x%02x\n",data_addr,
                 (uint8)out_buf[(3+in_buf[4+i*3])*i+2],
                 (uint8)out_buf[(3+in_buf[4+i*3])*i+1],
                 (uint8)out_buf[(3+in_buf[4+i*3])*i]);
                 */
                memcpy ( &out_buf[ ( 3+ ( uint8 ) in_buf[4+i*3] ) *i + 3], &in_buf[data_start_pos], ( uint8 ) in_buf[4+i*3] );//组装数据
                data_start_pos += ( uint8 ) in_buf[4+i*3];//取数据游标
                *out_len += ( uint8 ) in_buf[4+i*3];//组装数据游标
                // PFUNC ( TEM_INFO, DEBUG_PRO104, "out_len=%d\n", *out_len );
            } else {//信息体地址为2个字节的数据组装
                memcpy ( &out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i], ( char* ) &data_addr, 2 );
                *out_len += 2;
                /*
                PFUNC(TEM_WARNING,DEBUG_PRO104,"[%08x]%02x%02x\n",data_addr,
                 (uint8)out_buf[(2+in_buf[4+i*3])*i+1],
                 (uint8)out_buf[(2+in_buf[4+i*3])*i]);
                */

                memcpy ( &out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 2], &in_buf[data_start_pos], ( uint8 ) in_buf[4+i*3] );
#if 0

                if ( in_buf[4+i*3] == 1 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "addr=%02x%02x,data=%02x\n",
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 1], ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i],
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 2] );
                } else if ( in_buf[4+i*3] == 2 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "addr=%02x%02x,data=%02x%02x\n",
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 1], ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i],
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 3], ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 2] );
                } else if ( in_buf[4+i*3] == 3 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "addr=%02x%02x,data=%02x%02x%02x\n",
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 1], ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i],
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 4], ( uint8 ) out_buf[ ( 2+in_buf[4+i*3] ) *i + 3],
                            ( uint8 ) out_buf[ ( 2+in_buf[4+i*3] ) *i + 2] );
                } else if ( in_buf[4+i*3] == 4 ) {
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "addr=%02x%02x,data=%02x%02x%02x%02x\n",
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 1], ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i],
                            ( uint8 ) out_buf[ ( 2+ ( uint8 ) in_buf[4+i*3] ) *i + 5], ( uint8 ) out_buf[ ( 2+in_buf[4+i*3] ) *i + 4],
                            ( uint8 ) out_buf[ ( 2+in_buf[4+i*3] ) *i + 3], ( uint8 ) out_buf[ ( 2+in_buf[4+i*3] ) *i + 2] );
                }

#endif
                data_start_pos += ( uint8 ) in_buf[4+i*3];

                *out_len += ( uint8 ) in_buf[4+i*3];

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "out_len=%d\n", *out_len );
            }

        }

        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", *out_len );

        ///////////////////填规约逻辑缓存的数据个数及顺序标志
        plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 0;
        plb->pro_logic_buf.VSQ_Word.BsVsq.Number = data_num;

        if ( CheckAddrInOrder_4 ( out_buf, *out_len, data_num ) ) {
            //PFUNC(TEM_WARNING,DEBUG_PRO104,"not in order\n");
            ReBuildDataBuf_4 ( out_buf, out_len, data_num );
            plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 1;
        }

        if ( plb->pro_logic_buf.SendCause.BsCause.code >= CAUSE_Introgen && plb->pro_logic_buf.SendCause.BsCause.code < CAUSE_Intro16 + 1 ) {
            plb->pro_logic_buf.VSQ_Word.BsVsq.SQ = 1;
        }

        //PFUNC ( TEM_INFO, DEBUG_PRO104, "data_len=%d\n", *out_len );

        ////////////////检查数据帧长度是否符合规约帧长度要求

        if ( !CheckDataLen_4 ( *out_len ) ) {
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_len=%d over flow\n", *out_len );
            return 0;
        }

        return 1;
    }

    return 0;
}


/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
char Gb104::AddServiceBuf ( uint16 port_id, uint8 type, uint8 s_id )
{
    int i;
    PRO_LOGIC_BUF04 *plb = 0;
    plb = SelectLogicBuf ( port_id );

    if ( plb == 0 ) return 0;

    //PFUNC ( TEM_INFO,DEBUG_PRO104, "[%d][%d]pt=%d,type=%d,s_id=%d Request Service!\n" , plb->Service1Buf.in_cnt, plb->Service1Buf.out_cnt, port_id, type, s_id );

    if ( type == CLASS1 ) {
        for ( i = 0;i < MAX_SERVICE_ID;i++ ) {
            if ( plb->pro_logic_buf.Service1Buf.id[i] == s_id )
                if ( plb->pro_logic_buf.Service1Buf.flag[i] == EVENT_NOREAD )
                    return 0;
        }

        plb->pro_logic_buf.Service1Buf.id[plb->pro_logic_buf.Service1Buf.in_cnt] = s_id;

        plb->pro_logic_buf.Service1Buf.flag[plb->pro_logic_buf.Service1Buf.in_cnt] = EVENT_NOREAD;
        plb->pro_logic_buf.Service1Buf.in_cnt++;

        if ( plb->pro_logic_buf.Service1Buf.in_cnt >= MAX_SERVICE_ID ) {
            plb->pro_logic_buf.Service1Buf.in_cnt = 0;
        }
    } else if ( type == CLASS2 ) {
        for ( i = 0;i < MAX_SERVICE_ID;i++ ) {
            if ( plb->pro_logic_buf.Service2Buf.id[i] == s_id )
                if ( plb->pro_logic_buf.Service2Buf.flag[i] == EVENT_NOREAD )
                    return 0;
        }

        plb->pro_logic_buf.Service2Buf.id[plb->pro_logic_buf.Service2Buf.in_cnt] = s_id;

        plb->pro_logic_buf.Service2Buf.flag[plb->pro_logic_buf.Service2Buf.in_cnt] = EVENT_NOREAD;
        plb->pro_logic_buf.Service2Buf.in_cnt++;

        if ( plb->pro_logic_buf.Service2Buf.in_cnt >= MAX_SERVICE_ID ) {
            plb->pro_logic_buf.Service2Buf.in_cnt = 0;
        }
    }


    return 1;
}

/**
*************************************************************************************
*  @brief  判断数据服务，并决定执行哪个服务
*  @author  liht
*  @param[in] uint8 class_data_id
*  @param[out] uint8 *last_class_data_id
*  @return  成功返回1或2，否则返回0，返回1代表先来先服务，2表示后来先服务
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
static char JudgeDataService_4 ( uint8 class_data_id, uint8 *last_class_data_id )
{
    PRO_DATA_CLASS_CFG *tab1, *tab;

    if ( class_data_id == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no insert service change1 L=%d,C=%d\n", *last_class_data_id, class_data_id );
        return 0;
    }

    if ( gb104.member.pro_cfg.enable_interrupt_process == 0 ) { //上一个过程不允许打断
        if ( *last_class_data_id == 0 ) {

            *last_class_data_id = class_data_id;
            PFUNC ( TEM_WARNING, DEBUG_PRO104, "no insert service change2 L=%d:C=%d\n", gb104.member.LastDataClass1Id, class_data_id );
            return 1;
        }
    } else { //允许打断上一个过程

        if ( *last_class_data_id != class_data_id ) {
            if ( *last_class_data_id == 0 ) {
                *last_class_data_id = class_data_id;
                PFUNC ( TEM_WARNING, DEBUG_PRO104, "occupy service change1 L=%d:C=%d\n", gb104.member.LastDataClass1Id, class_data_id );
                return 2;
            }

            tab = gb104.SearchClassTable ( class_data_id );

            tab1 = gb104.SearchClassTable ( *last_class_data_id );

            if ( tab != 0 && tab1 != 0 ) {
                if ( tab->pri < tab1->pri ) { //优先级比较
                    *last_class_data_id = class_data_id;
                    PFUNC ( TEM_WARNING, DEBUG_PRO104, "occupy service change2 L=%d:C=%d\n", gb104.member.LastDataClass1Id, class_data_id );
                    return 2;
                }
            }

        }
    }

    return 0;
}


/**
*************************************************************************************
*  @brief  解析32比特串规约帧
*  @author  liht
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
char Gb104::InitIdMapTable ( void )
{
    int i, j;
	Data<BaseDataType> *tb=0;
    //PFUNC ( TEM_INFO, DEBUG_PRO101, "Id Map Table Initial\n" );
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
	uint16 yx_num=0;
    for ( j = 0;j < PARA_50B7 - PARA_50AD + 1;j++ ) {
        user_data.GetData ( PARA_50AD + j, &data );
        para = ( YXMsgSet * ) data.Data;
        paranum = para->Data.Num;
       // PFUNC ( TEM_INFO, DEBUG_PRO101, "paraid %x [%d]\n", PARA_50AD + j, paranum );

        if ( paranum != 0 ) {
            for ( i = 0;i < paranum;i++ ) {
            	if(para->Data.Define[i].MsgId<0)continue;
                member.yx_id_map[yx_num].bytes = 1;
                member.yx_id_map[yx_num].data_addr = para->Data.Define[i].MsgId;
                member.yx_id_map[yx_num].data_id = para->Data.Define[i].DataId;

                if ( isDyx(para->Data.Define[i].DataId) ) {
                    member.yx_id_map[yx_num].type = member.pro_data_type_cfg.dyx_type;
                    member.pro_dyx_data_num += 1;

                } else {
                    member.yx_id_map[yx_num].type = member.pro_data_type_cfg.syx_type;
                    member.pro_yx_data_num += 1;

                }
				yx_num=member.pro_dyx_data_num + member.pro_yx_data_num;

                if ( yx_num>= MAXYXDATA ) {
                    break;
                }
                	

            }
        } else {
            break;
        }
    }

#if 0
    /////////////////////////////////////////////
    for ( i = 0;i < 60;i++ ) {
        member.yx_id_map[i].bytes = 1;
        member.yx_id_map[i].data_addr = 1 + i;
        member.yx_id_map[i].data_id = 1 + i;
        member.yx_id_map[i].type = member.pro_data_type_cfg.syx_type;
        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", yx_id_map[i].data_addr, yx_id_map[i].data_id );
    }

    for ( i = 60;i < 84;i++ ) {
        member.yx_id_map[i].bytes = 1;
        member.yx_id_map[i].data_addr = 1 + i;
        member.yx_id_map[i].data_id = 0x201 + i - 60;
        member.yx_id_map[i].type = member.pro_data_type_cfg.syx_type;
        // PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", yx_id_map[i].data_addr, yx_id_map[i].data_id );
    }

    member.pro_yx_data_num = 84;

//配置遥测信息体地址和ID映射表//////////////////////////////////////////////////////////////////////

    for ( i = 84;i < 86;i++ ) {
        member.yx_id_map[i].bytes = 1;
        member.yx_id_map[i].data_addr = 1 + i;
        member.yx_id_map[i].data_id = 0x219 + i - 84;
        member.yx_id_map[i].type = member.pro_data_type_cfg.dyx_type;
        // PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", yx_id_map[i].data_addr, yx_id_map[i].data_id );
    }

    member.pro_dyx_data_num = 2;

    for ( i = 86;i < MAXYXDATA;i++ ) {
        member.yx_id_map[i].bytes = 0;
        member.yx_id_map[i].data_addr = 0;
        member.yx_id_map[i].data_id = 0 ;
        member.yx_id_map[i].type = 0;
    }

#endif

    for ( j = 0;j < PARA_50AC - PARA_5098 + 1;j++ ) {
        user_data.GetData ( PARA_5098 + j, &data );
        ycPara = ( YCMsgSet * ) data.Data;
        paranum = ycPara->Data.Num;
        //PFUNC ( TEM_INFO, DEBUG_PRO101, "paraid %x [%d]\n", PARA_5098 + j, paranum );

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

               // PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_addr[%x] %x\n", member.yc_id_map[member.pro_yc_data_num].data_addr, member.yc_id_map[member.pro_yc_data_num].data_id );

                member.pro_yc_data_num += 1;

                if ( member.pro_yc_data_num >= MAXYCDATA ) {
                  //  PFUNC ( TEM_WARNING, DEBUG_PRO104, "the yc config num more than %d\n", MAXYCDATA );
                    break;
                }

            }
        } else {
            break;
        }
    }

#if 0
    for ( i = 0;i < 0x1F2;i++ ) {
        if ( member.pro_data_type_cfg.yc_type == M_ME_NC_1 )
            member.yc_id_map[i].bytes = 4;
        else
            member.yc_id_map[i].bytes = 2;

        member.yc_id_map[i].data_addr = 0x4001 + i;

        member.yc_id_map[i].data_id = 0x4001 + i;

        member.yc_id_map[i].type = member.pro_data_type_cfg.yc_type;

        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", member.yc_id_map[i].data_addr, member.yc_id_map[i].data_id );
    }

    for ( i = 0x1F2;i < 0x1F4;i++ ) {
        if ( member.pro_data_type_cfg.yc_type == M_ME_NC_1 )
            member.yc_id_map[i].bytes = 4;
        else
            member.yc_id_map[i].bytes = 2;

        member.yc_id_map[i].data_addr = 0x4001 + i;

        member.yc_id_map[i].data_id = 0x4401 + i - 0x1F2;

        member.yc_id_map[i].type = member.pro_data_type_cfg.yc_type;

        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", member.yc_id_map[i].data_addr, member.yc_id_map[i].data_id );
    }

    for ( i = 0x1F4;i < 0x1FB;i++ ) {
        if ( member.pro_data_type_cfg.yc_type == M_ME_NC_1 )
            member.yc_id_map[i].bytes = 4;
        else
            member.yc_id_map[i].bytes = 2;

        member.yc_id_map[i].data_addr = 0x4001 + i;

        member.yc_id_map[i].data_id = 0x4601 + i - 0x1F4;

        member.yc_id_map[i].type = member.pro_data_type_cfg.yc_type;

        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", member.yc_id_map[i].data_addr, member.yc_id_map[i].data_id );
    }


    member.pro_yc_data_num = i;

    for ( i = 0x1FB;i < MAXYCDATA;i++ ) {
        member.yc_id_map[i].bytes = 0;
        member.yc_id_map[i].data_addr = 0;
        member.yc_id_map[i].data_id = 0;
    }

#endif
//配置遥控信息体地址和ID映射表////////////////////////////////////////////////////////////////////
    for ( j = 0;j < 1;j++ ) {
        user_data.GetData ( PARA_50B8 + j, &data );
        ykPara = ( YKMsgSet * ) data.Data;
        paranum = ykPara->Data.Num;
       // PFUNC ( TEM_INFO, DEBUG_PRO104, "paraid %x [%d]\n", PARA_50B8 + j, paranum );

        if ( paranum != 0 ) {
            for ( i = 0;i < paranum;i++ ) {
            	if(ykPara->Data.Define[i].MsgId<0)continue;
                member.yk_id_map[i].bytes = 1;
                member.yk_id_map[member.pro_yk_data_num].data_addr = ykPara->Data.Define[i].MsgId;
                member.yk_id_map[member.pro_yk_data_num].data_id = ykPara->Data.Define[i].DataId;
                member.yk_id_map[member.pro_yk_data_num].type = member.pro_data_type_cfg.yk_type;
              //  PFUNC ( TEM_WARNING, DEBUG_PRO104, "data_addr[%x] %x\n", member.yk_id_map[member.pro_yk_data_num].data_addr, member.yk_id_map[member.pro_yk_data_num].data_id );
                member.pro_yk_data_num += 1;

                if ( member.pro_yk_data_num >= MAXYKDATA ) {
                 //   PFUNC ( TEM_WARNING, DEBUG_PRO104, "the yk config num more than %d\n", MAXYKDATA );
                    break;
                }

            }
        } else {
            break;
        }
    }


#if 0
    for ( i = 0;i < 4;i++ ) {
        member.yk_id_map[i].bytes = 1;
        member.yk_id_map[i].data_addr = 0x6001 + i;
        member.yk_id_map[i].data_id = 0x6001 + i;
        member.yk_id_map[i].type = member.pro_data_type_cfg.yk_type;
        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", yk_id_map[i].data_addr, yk_id_map[i].data_id );
    }

    for ( i = 4;i < 7;i++ ) {
        member.yk_id_map[i].bytes = 1;
        member.yk_id_map[i].data_addr = 0x6001 + i;
        member.yk_id_map[i].data_id = 0x6064 + i - 4;
        member.yk_id_map[i].type = member.pro_data_type_cfg.yk_type;
        //PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", yk_id_map[i].data_addr, yk_id_map[i].data_id );
    }

    member.pro_yk_data_num = i;

    for ( i = 8;i < MAXYKDATA;i++ ) {
        member.yk_id_map[i].bytes = 0;
        member.yk_id_map[i].data_addr = 0;
        member.yk_id_map[i].data_id = 0;
    }

#endif

//配置参数信息体地址和ID映射表////////////////////////////////////////////
	tb=user_data.GetDataTablePtr(MSG_PARA);
	if(tb==0){
		 PFUNC ( TEM_WARNING,DEBUG_PRO104, "Para Table Not Found\n");
		return 0;
	}
	for(i =0;i<user_data.GetDataTableSize(MSG_PARA);i++){
		member.para_id_map[i].data_addr = tb[i].Id;
        member.para_id_map[i].data_id = tb[i].Id;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
	}
#if 0
    for ( i = 0;i < 0x55;i++ ) {

        member.para_id_map[i].data_addr = 0x5001 + i;
        member.para_id_map[i].data_id = 0x5001 + i;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
        // PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", para_id_map[i].data_addr, para_id_map[i].data_id );
    }

    for ( i = 0x55;i < 0x8e;i++ ) {

        member.para_id_map[i].data_addr = 0x5080 + i - 0x55;
        member.para_id_map[i].data_id = 0x5080 + i - 0x55;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
        //  PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", para_id_map[i].data_addr, para_id_map[i].data_id );
    }

    for ( i = 0x8e;i < 0x95;i++ ) {

        member.para_id_map[i].data_addr = 0x5100 + i - 0x8e;
        member.para_id_map[i].data_id = 0x5100 + i - 0x8e;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
        // PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", para_id_map[i].data_addr, para_id_map[i].data_id );
    }

    for ( i = 0x95;i < 0xA8;i++ ) {

        member.para_id_map[i].data_addr = 0x5200 + i - 0x95;
        member.para_id_map[i].data_id = 0x5200 + i - 0x95;
        member.para_id_map[i].bytes = user_data.GetDataSize ( member.para_id_map[i].data_id );
        // PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", para_id_map[i].data_addr, para_id_map[i].data_id );
    }
#endif
    member.pro_user_para_num = i;

//配置32位串信息体地址与ID映射表////////////////////////////////////////////////////
	tb=user_data.GetDataTablePtr(MSG_STEP);
	if(tb==0){
		 PFUNC ( TEM_WARNING,DEBUG_PRO104, "Para Table Not Found\n");
		return 0;
	}
	for ( i = 0;i < user_data.GetDataTableSize(MSG_STEP);i++ ) {
		member.bit32_id_map[i].data_addr = tb[i].Id;
        member.bit32_id_map[i].data_id = tb[i].Id;
        member.bit32_id_map[i].bytes = user_data.GetDataSize ( member.bit32_id_map[i].data_id );
        member.bit32_id_map[i].type = M_BO_NA_1;
	}
#if 0
    for ( i = 0;i < MAX_BIT32_NUM;i++ ) {

        member.bit32_id_map[i].data_addr = 0x6801 + i;
        member.bit32_id_map[i].data_id = 0x6801 + i;
        member.bit32_id_map[i].bytes = user_data.GetDataSize ( member.bit32_id_map[i].data_id );
        member.bit32_id_map[i].type = M_BO_NA_1;
        //  PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x\n", bit32_id_map[i].data_addr, bit32_id_map[i].data_id );
    }
#endif
    member.pro_user_bit32_num = i;

    //配置步位置信息体地址与ID映射表////////////////////////////////////////////////////

    for ( i = 0;i < MAX_STEP_NUM;i++ ) {

        member.step_id_map[i].data_addr = 0x6601 + i;
        member.step_id_map[i].data_id = 0x6601 + i;
        member.step_id_map[i].bytes = user_data.GetDataSize ( member.step_id_map[i].data_id );
        member.step_id_map[i].type = M_ST_NA_1;
        //  PFUNC ( TEM_WARNING,DEBUG_PRO101, "data_addr[%x] %x,%d\n", member.step_id_map[i].data_addr, member.step_id_map[i].data_id ,member.step_id_map[i].type);
    }

    member.pro_user_step_num = i;

    return 1;
}

void Gb104::SendFrame ( BUF_MUX *m_buf, char* buf, uint16 len )
{
    PRO_LOGIC_BUF04 *plb = 0;
	if(m_buf==0){
		PFUNC ( TEM_WARNING, DEBUG_PRO104, "no send buf to be send \n" );
		return;
	}
    plb = gb104.SelectLogicBuf ( m_buf->port_id );
	
    if ( plb == 0 ) {
        PFUNC ( TEM_WARNING, DEBUG_PRO104, "no logic buf surport %d\n",m_buf ->port_id );
        return ;
    }

    if ( len > APCI_FRAME_LEN ) {
        plb->recv_asdu_con_timer.flag = START;
        plb->recv_asdu_con_timer.cnt = 0;
        plb->send_asdu_con_timer.flag = STOP;
        plb->send_asdu_con_timer.cnt = 0;
        plb->no_asdu_con_timer.flag = STOP;
        plb->no_asdu_con_timer.cnt = 0;
        plb->CtrlWordBack.data = plb->ICtrlWord.data;
        plb->RecvSeqBack = plb->ICtrlWord.I_CtrlWord.RecvSeq;
        plb->ICtrlWord.I_CtrlWord.SendSeq++;

        if ( ( plb->ICtrlWord.I_CtrlWord.SendSeq - plb->SendSeqBack )
                >= member.InerPara.K/*MAX_SEND_RECV_MATCH_NUM*/ ) {
            plb->enable_send_flag = 0;
        }
    }

    plb->link_idle_timer.flag = START;

    plb->link_idle_timer.cnt  = 0;
    protocol.AddSendBuf ( m_buf, buf, len );
    protocol.CopySendFrame ( m_buf, buf, len );
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "SEND_DATA_104 () ok\n" );
	//sleep(10);
}

/**
*************************************************************************************
*  @brief  组数据配置
*  @author  liht
*  @param[in] DATA_GROUP_CFG *pgf
*  @param[in] ID_MAP *pmap
*  @param[in] uint16 data_num
*  @param[in] uint8 data_type
*  @param[out] INTRO_INFO ii[16][8]
*  @note 无。
*  @see  NULL
**************************************************************************************
*/
void Gb104::ConfigGroupData ( DATA_GROUP_CFG *pgf, INTRO_INFO ii[16][8], ID_MAP *pmap, uint16 data_num, uint8 data_type )
{
    int data_cnt, i, j, k = 0, h = 0;
    uint16 data_num_per_group = 0;
    uint16 last_group_num = 0;
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
        group_data_num = 127;
        limit_num = 1016;
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

        limit_num = 320;


    }

    ////////////////配置组内数据的分配一组多帧传输的配置////////////////////
    data_cnt = data_num;

    //PFUNC ( TEM_INFO, DEBUG_PRO104, " data_cnt=%d\n", data_cnt );

    if ( data_cnt > limit_num ) { //每组平局分配
        //PFUNC ( TEM_INFO, DEBUG_PRO104, " data num larger than %d\n", limit_num );
        pgf->used_group_num = pgf->max_group_num;
        //PFUNC ( TEM_INFO, DEBUG_PRO104, " inuse group num %d\n", pgf->used_group_num );
        data_num_per_group = data_cnt / pgf->used_group_num;
        last_group_num = data_num_per_group + data_cnt % data_num_per_group;

        for ( i = 0;i < pgf->max_group_num;i++ ) {
            if ( i < pgf->max_group_num - 1 ) {
                data_cnt = data_num_per_group;
            } else {
                data_cnt = last_group_num;
            }

            j = 0;

            data_inuse_cnt = 0;
            frame_data_num = 0;
           // PFUNC ( TEM_INFO, DEBUG_PRO104, "group data_cnt =%d, last_group_num=%d ,max_frame_num=%d\n",
                   // data_cnt, last_group_num, max_frame_data_num );

#if 1

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
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "![%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                            //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
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
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "=[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                            //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
                    j++;
                    tmp_type = pmap[addr_pos].type;
                }

                frame_data_num++;

                h++;

                if ( j >= 8 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "the group max frame is reached =%d\n", j );
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
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "))))[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );

            }

#else
            while ( data_cnt - max_frame_data_num > 0 ) {

                ii[i][j].flag = 1;
                ii[i][j].num = max_frame_data_num;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                addr_pos += max_frame_data_num;
                ii[i][j].typesign = tmp_type;
                data_cnt -= max_frame_data_num;
                ii[i][j].size = datasize;


                //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
                j++;
            }

            if ( data_cnt > 0 ) {
                ii[i][j].flag = 1;
                ii[i][j].num = data_cnt;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                ii[i][j].typesign = pgf->data_type;
                ii[i][j].size = datasize;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
                addr_pos += data_cnt;
            }

#endif
        }
    } else {
        //PFUNC ( TEM_INFO, DEBUG_PRO104, " data num less than %d\n", limit_num );
        pgf->used_group_num = data_cnt / group_data_num + 1;
        //last_group_num=data_num_per_group+data_cnt%data_num_per_group;

        for ( i = 0;i < pgf->used_group_num;i++ ) {
            if ( i < pgf->used_group_num - 1 ) {
                data_num_per_group = group_data_num;
                data_cnt = data_num_per_group;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "group_num=%d\n",  data_cnt );
            } else {
                last_group_num = data_num % group_data_num;

                data_cnt = last_group_num;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "last_group_num=%d\n",  data_cnt );
            }

            j = 0;

            data_inuse_cnt = 0;
            frame_data_num = 0;
#if 1

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
                   //PFUNC ( TEM_INFO, DEBUG_PRO101, "![%d,%d] flag=%d,num=%d,start_addr=%x [%d]\n", i + 1, j,
                            //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr , ii[i][j].typesign );
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
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "=[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                            //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
                    j++;
                    tmp_type = pmap[addr_pos].type;
                }

                if ( j >= 8 ) {
                    //PFUNC ( TEM_INFO, DEBUG_PRO104, "the group max frame is reached =%d\n", j );
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
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );

            }

#else
            while ( data_cnt - max_frame_data_num > 0 ) {
                ii[i][j].flag = 1;
                ii[i][j].num = max_frame_data_num;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                addr_pos += max_frame_data_num;
                ii[i][j].typesign = pgf->data_type;
                ii[i][j].size = datasize;
                data_cnt -= max_frame_data_num;

                //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
                j++;

            }

            if ( data_cnt > 0 ) {
                ii[i][j].flag = 1;
                ii[i][j].num = data_cnt;
                ii[i][j].start_addr = pmap[addr_pos].data_addr;
                addr_pos += data_cnt;
                ii[i][j].typesign = pgf->data_type;
                ii[i][j].size = datasize;
                //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d,%d] flag=%d,num=%d,start_addr=%x\n", i + 1, j,
                        //ii[i][j].flag, ii[i][j].num, ii[i][j].start_addr );
            }

#endif
        }
    }

    //PFUNC ( TEM_INFO, DEBUG_PRO104, "[%d] type=%d,g_id=%d,dn=%d,ugn=%d\n", data_num,
            //pgf->data_type ,
            //pgf->start_group_id,
            //pgf->max_group_num,
            //pgf->used_group_num );
}

int Gb104::WriteCfg ( uint32 para_id , char *para )
{
    char paraname[32];
    char paraStr[8];
    char filename[128];
    char states = 0;
    sprintf ( filename, "%s", CONFIG_XML_PATH_4 );
    //PFUNC ( TEM_INFO, DEBUG_PRO104, "para len=%d\n" , user_data.GetDataSize ( para_id ) );

    if ( para_id == PARA_5101 ) {
        Pro104Para ext_para;

        memcpy ( ( char* ) &ext_para.Data, para, user_data.GetDataSize ( para_id ) );

        if ( ext_para.Data.MsgAddrLen != member.pro_cfg.datasign_addr_size ) {//хϢͥµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "DataSignSize" );
            sprintf ( paraStr, "%d", ext_para.Data.MsgAddrLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.CommAddresLen != member.pro_cfg.link_addr_size ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "LinkAddrSize" );
            sprintf ( paraStr, "%d", ext_para.Data.CommAddresLen );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.T0 != member.InerPara.T0 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "T0" );
            sprintf ( paraStr, "%d", ext_para.Data.T0 );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.T1 != member.InerPara.T1 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "T1" );
            sprintf ( paraStr, "%d", ext_para.Data.T1 );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.T2 != member.InerPara.T2 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "T2" );
            sprintf ( paraStr, "%d", ext_para.Data.T2 );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.T3 != member.InerPara.T3 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "T3" );
            sprintf ( paraStr, "%d", ext_para.Data.T3 );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.T4 != member.InerPara.T4 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "T4" );
            sprintf ( paraStr, "%d", ext_para.Data.T4 );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.K != member.InerPara.K ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "K" );
            sprintf ( paraStr, "%d", ext_para.Data.K );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( ext_para.Data.W != member.InerPara.W ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "W" );
            sprintf ( paraStr, "%d", ext_para.Data.W );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        states = 1;
    } else if ( para_id == PARA_5139 ) {
        ComBasePara com_para;
        memcpy ( ( char* ) &com_para.Data, para, user_data.GetDataSize ( para_id ) );

		 if ( com_para.Data.YcDataType == 1 ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
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

        if ( com_para.Data.PointsYCGroup != member.data_group_cfg[1].group_data_num ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
            sprintf ( paraname, "%s", "YcGroupDataNum" );
            sprintf ( paraStr, "%d", com_para.Data.PointsYCGroup );
            ModifyParaXmlFile ( filename, paraStr, paraname );
        }

        if ( com_para.Data.PointsYXGroup != member.data_group_cfg[0].group_data_num ) {//¹«¹²µ¥Ԫµٖ·³¤¶ƍ
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
        ProLinkReset ( gb104.member.CurPortId );
		PFUNC ( TEM_WARNING, DEBUG_PRO104,"11\n");
        InitGb104();
		//PFUNC ( TEM_INFO, DEBUG_PRO104, "init over\n", filename );

    } else if ( states == 2 ) {
        //PFUNC ( TEM_INFO, DEBUG_PRO104, "write xml file %s ok\n", filename );
        InitIdMapTable();
        InitGroup();
    }

    //reinitial the gb101 para and run from begin

    return 0;
}

uint32 Gb104::IsMyPara ( uint32 paraid )
{
    if ( paraid == 0 ) {
        return INVALID_PARA;
    }

    if ( paraid >= PARA_5098 && paraid <= PARA_50CB ) { //三遥属性配置参数
        return paraid;
    }

    if ( paraid == PARA_5101 || paraid == PARA_5139 ) { //规约参数
        return paraid;
    }
	 if ( paraid == PARA_5097 ) { //规约参数
        return paraid;
    }

    return INVALID_PARA;
}
