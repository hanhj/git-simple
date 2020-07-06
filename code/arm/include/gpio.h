#ifndef GPIO_H
#define GPIO_H
#include "types.h"
/**GPIO管脚类
*/
//以下是调用ioctrl 方式控制管脚的定义

#define SET_OUTPUT_LOW 10
#define SET_OUTPUT_HIGH 11
#define GET_VALUE 12
#define SET_INPUT 13

//以下是应用定义
#define GPIO_DIR_IN 0
#define GPIO_DIR_OUT 1

//以下是应用管脚定义第一位是bank号,第2,3位是bank内索引号,每一个bank有16个管脚,数字用十进制表示.
#define FEED_DOG_PIN		602		/*feed dog*/               
#define ENABLE_DOG_PIN		401		/*enable feed dog*/        
#define BEEP	 			11		/*BEEP*/                   
#define GPS_WAKE			811 	/*gps wake up    */        
#define GPS_RESET			713 	/*gps reset    */          
#define WIRELESS_POWER 		711 	/*gprs power enable    */  
#define WIRELESS_ON_OFF 	714 	/*gprs on/off    */        
#define WIRELESS_RST 		600 	/*gprs reset    */         
#define WIRELESS_INDICATE 	712 	/*gprs indicate led    */  
#define RUN_PIN 			715 	/*运行指示灯         */         
#define LITTLE_WIRELESS_SLEEP 			1111 	/*this pin is not use tmp*/         
#define LITTLE_WIRELESS_RST 			1112 	/*this pin is not use tmp */         

class GPIO
{
private:
	int fd ;	
public:
	  GPIO();
	  ~GPIO();
    /**
     * 获取GPIO管脚电平值
     *
     * @param pin    GPIO管脚
     *
     * @return 0: 电平低
     *     1: 高电平
     *
     */
    int16 Read ( int16 PIN );
    /**
     * 设置GPIO电平
     *
     * @param pin    GPIO管脚
     * @param value  0: 低电平
     *               1: 高电平
     *
     * @return 0: 成功
     *         其他: 失败
     */
    int Write ( int16 PIN, int16 value );
    /**
     * 设置GPIO方向
     *
     * @param pin    GPIO管脚
     * @param dir    GPIO管脚方向 0:输入,1 输出
     * @param value    GPIO管脚定义为输出时,输出电平 0:低电平,1:高电平
     * @return 0: 成功
     *         其他: 失败
     *
     */
	int Init ( int16 PIN,int16 dir,int16 value );
    
};
extern GPIO gpio;
#endif
