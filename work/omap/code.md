dir	: fun   
/arch/arm/mach-davinci/			:special file for board   
/arch/arm/mach-davinci/include	:special peripherals devices head file   
/include/asm-generic/	:common files:gpio.h,errno_base.h  
/include/linux/			:common devices :etherdevice.h  
drivers/gpio/			:speacial and common of gpio fun:gpio-davinci.c  

in board_omapl138_lcdk.c:

|step|fun_name|fun|call fun |data in|
|-|-|-|-|-|
|step1|dog_init	|config and req dog pins||<img width=200>gpio(6,2),gpio(4,1)|
|step2|davinci_serial_init|config serial 1-3 pins,enable rx,tx,register serial as a platform device|call davinci_serial_init in  mach-davinci/serial.c|pins define in da850.c,device define in devices-da8xx.c|
|sttp3|omapl138_lcdk_config_emac|config emac pin,cofig rmii phy,reset lan8720,register_emac as a platform device |call dx8xx_register_emac in devices-da8xx.c|pins define in board,emac devices in devices-da8xx.c|
|step4|da850_register_edma|register edma as platform device|call da850_register_edma in da8xx_devices.c|edma devices define in devices-da8xx.c|
|step5|omapl138_lcdk_mmc_init|config mmc pins,request cd pin,register mmc device as a platform devices|call da8xx_register_mmcsd0 in devices-da8xx.c|pins define in board,device define in devices-da8xx.c|
|step6|omapl138_lcdk_usb_init|config and request usb pins,register usb platform|call ds8xx_register_usb20,da8xx_board_usb_init in mach-davinci/usb.c|pin define in board,device define in usb.c|
|step7|da8xx_register_watchdog()|register da8xx watchdog as a platform device|call da8xx_register_watchdog in devices-da8xx.c|device define in devices-da8xx.c|
|step8|omapl138_lcdk_i2c_init|config pins,register board i2c device |call i2c_register_board_info in drivers/i2c/i2c_board_info.c|pins define in da850.c(1,4)(1,5),i2c device define in board file|
|step 9|da850_evm_config_pru_suart|config pins,register PRU as a platform device|call da8xx_register_pru_suart in devices-da8xx.c|pins define in da850.c,device define in devices-da8xx.c|
|step 10|omapl138_lcdk_spi_init|config and request spi pins,register spi master as a platform device,register spi device|call da8xx_register_spi in devices-da8xx.c,spi_register_board_info in driver/spi/spi.c|spi master pin in da850.c,spi master device define in devices-da8xx.c \  spi device other pin define in board,spi device define in board|
|step 11|omapl138_lcdk_gpio_init|config and request gpio pins,register gpio device as  platform device||pins and device define in board|

------------

|文件名|路径|函数|
|----|----|----|
|da850.c|/arch/arm/mach-davinci/|mux config,|
|mux.h|/arch/arm/mach-davinci/include/mach/|enum of pins|
|devices-da850.c|/arch/arm/mach-davinci/|funs of soc devices|
|gpio.h|include/asm-generic/|define functions of gpio:\
				gpio_is_valid|
|errno_base.h|include/asm-generic/|defien error no|
|etherdevice.h|include/linux/|dev_hw_addr_random|
|gpio-davinci.c|driviers/gpio/|davinci-gpio_setup|

