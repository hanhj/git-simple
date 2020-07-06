
/**
*********************************************************************************************************************
*   @file bat.h
*   @brief 提供蓄电池的管理功能
*   @warning <Not thread safe!>
*   @param 无
*   @author  liht
*   @date  2013.07.23
*   @version  v1.0.0.0
*   @note
*   提供了数据和参数的读写接口，如果需要获得模块提供的消息，请使用消息注册接口向本模块注册，
*   需要使用的头文件为msg.h
**********************************************************************************************************************
*/

#ifndef _BAT__H_
#define _BAT__H_
#include "datatype.h"
#include "baseapp.h"

typedef struct _BattPara {
    DataType4   *CellLessLevel;///<蓄电池欠压切除阈值
    DataType4   *CellLowLevel;///<蓄电池低压告警阈值
    DataType2   *CellActTime;///<蓄电池活化充放电间隔时间
}BattPara;

typedef struct _BattRunData {
    DataType4 *CellVol;///<蓄电池电压
    int16 Run;//电池活化计时
    int8 LosePower;//交流失电状态
    int8 PowerError;//电源故障状态
	int8 AlarmLowerU2;//电源给出的电池欠压状态
	int8 AlarmLowerU1;//测量电压给出的电池欠压状态
	int8 CutBatt;//切除电池
	int8 BattActState;//电池活化状态    
}BattRunData;

/**
************************************************************************************************************
*  @class  CBAT  bat.h  电池管理模块头文件
*  @brief  提供电池管理模块的对外接口，内部独立完成电池管理的逻辑
*  @author  liht
*  @note  如果要写数据中的某一个数据，请先使用读接口read将数据读出，然后改变对应的参数，再write写回去。
*  detailed  待续
************************************************************************************************************
*/

class Batt: public BaseApp
{

private:
    void GetMsg();/**< 调用msg.h中的接口将消息发布出去  */
    void SendMsg ( char *msg_buf );/**< 调用msg.h中的接口将消息发布出去  */
    BattRunData bat_data;/**<  数据  */
    BattPara bat_para;/**<  参数  */

		uint16 cal_cmd;//保存标定命令
		int8 start_cal;
		
public:
    Batt ( void );/**< 初始化函数  */
    ~Batt ( void );
    int16 Init();
    int16 Run ( void*arg );
    int16 GetMsg ( MessageData<BaseDataType>message );
    /**
    *************************************************************************************
    *  @brief  电池管理模块的私有配置接口，管理文件/home/用户/rtdata/battrun.dat,内容为BattData
    *  @author  liht
    *  @return  写成功，返回1，否则返回0，返回1代表缓存中的数据已被成功写入。
    **************************************************************************************
    */

    int16 WriteCfg();
    int16 ReadCfg();
    int8 CalibrateAd(uint16 cmd);

protected:
};

extern Batt batt;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* batt_main ( void* );

#endif

