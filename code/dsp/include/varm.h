#ifndef VARM_H
#define VARM_H
#include "baseapp.h" ///<用到baseapp类.
#define READY 1
#define SEND 2
/**
* /brief 消息类
*/
class Varm :public BaseApp{
private:
	//以下为syslink层的接口函数
//		int (*fun_write) (char *buf ,int16 len);//写数据
//		int (*fun_read)(char *buf ,int16 len);//读数据
//		int (*fun_checkmsg)(char **buf ,int16 *len);//读消息		
//		int (*fun_init)(void);//初始化
public:
		int8 MsgSendFlag;
		uint32 msg_len;

	Varm();
	~Varm();	
	/**
	*************************************************************************************  
	*  @brief  初始化本模块。 
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 
	int16 Init();	
	/**
	*************************************************************************************  
	*  @brief  运行模块。 
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/	
	int16 Run();	
	/**
	*************************************************************************************  
	*  @brief 检查Arm发来的消息处理函数. 
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  无
	**************************************************************************************
	*/				
	void CheckMsg();

	/**
	*************************************************************************************  
	*  @brief  检查是否有录波数据，并传输给Arm侧
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 	
	int16 OnGetWaveMsg();
};
extern Varm varm;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* varm_main(void *);

#endif
