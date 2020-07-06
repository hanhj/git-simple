#ifndef __AT91_IOCTL_H__
#define __AT91_IOCTL_H__

#define GPIO_IOC_MAGIC   'G'
#define IOCTL_GPIO_SETOUTPUT              _IOW(GPIO_IOC_MAGIC, 0, int)                   
#define IOCTL_GPIO_SETINPUT               _IOW(GPIO_IOC_MAGIC, 1, int)
#define IOCTL_GPIO_SETVALUE               _IOW(GPIO_IOC_MAGIC, 2, int) 
#define IOCTL_GPIO_GETVALUE    		  _IOR(GPIO_IOC_MAGIC, 3, int)

#define BUZZER_IOC_MAGIC 'B'
#define IOCTL_BUZZER_BEEP		  _IO(BUZZER_IOC_MAGIC, 0)
#define IOCTL_BUZZER_MUTE		  _IO(BUZZER_IOC_MAGIC, 1) 


#define LED_IOC_MAGIC 'L'
#define IOCTL_LED_ON			  _IO(LED_IOC_MAGIC, 0)
#define IOCTL_LED_OFF			  _IO(LED_IOC_MAGIC, 1)


typedef struct {
	int pin;
	int data;
	int usepullup;
}at91_gpio_arg;

#endif //__AT91_IOCTL_H__

