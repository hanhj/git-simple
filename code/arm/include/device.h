#ifndef DEVICE_H
#define DEVICE_H

//网口设备
#define ETH1_NAME "eth0"
#define ETH2_NAME "eth1"
#define ETH3_NAME "eth2"
#define ETH4_NAME "eth3"
#define PPP_NAME "ppp0"
//串口设备
#define UART_CONTROL_DEV 	"/dev/ttyS0"//控制台						物理连接到的端口
#define UART1_DEV 			"/dev/ttyS1" //串口1						CPU板232/485 1(com1)
#define UART2_DEV 			"/dev/ttySU3" //串口2                   CPU板232/485 2(com2)
#define UART3_DEV 			"/dev/ttySU2" //串口3                   CPU板232/485 3(com3)
#define UART4_DEV 			"/dev/ttySU4" //串口4                   CPU板232/485 4(com4)
#define UART5_DEV 			"/dev/ttyS2" //串口5						液晶板(com5)
#define UART6_DEV 			"/dev/ttySU6" //串口6                   扩展板232/485 1(com6)              
#define UART7_DEV 			"/dev/ttySU7" //串口7                   扩展板232/485 2(com7)              
#define UART8_DEV 			"/dev/ttySU1" //串口8                   扩展板GPS(com8)                    
#define UART9_DEV 			"/dev/ttySU0" //串口9                   扩展板CPRS/CDMA(com9)                        
#define UART10_DEV 			"/dev/ttySU5" //串口10                  扩展板短距离无线（预留未用）(com10)           
#define UART11_DEV 			"/dev/mux1"	//GPRS信号检测      
#define UART12_DEV 			"/dev/mux2"	//SMS信号检测            
                            

#define DEV_GPIO "/dev/gpio"

#define	PWM_DEVICE	"/dev/pwm"


#define RTC_DEV		"/dev/rtc0"
#define LCD_DEV		"/dev/fb0"

#define TTY_USB_DEV	"/dev/ttyUSB" //7-9 for usb-serial ttyUSB0 -> ttyUSB2
#define TTY_752_DEV	"/dev/ttySC"


#define ADC_DEV		"/dev/adc"
#define PCF8574_0	"/dev/pcf328"		//LED
#define PCF8574_1	"/dev/pcf329"		//U406
#define PCF8574_2	"/dev/pcf330"		//U400
#define DEV_OMAPL138_MISC "/dev/omapl138_misc" //omapl138 misc device


#endif
