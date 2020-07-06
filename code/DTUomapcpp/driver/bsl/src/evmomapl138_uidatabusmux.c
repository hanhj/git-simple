//-----------------------------------------------------------------------------
// \file    evmomapl138_uidatabusmux.h
// \brief   setup OMAPL138 and ui data bus for use of peripherals 
//          
//
//-----------------------------------------------------------------------------
#include "evmomapl138_support.h"
#if support_evmomapl138_uidatabusmux 
#include "types.h"
#include "stdlib.h"
#include "evmomapl138_uidatabusmux.h"
#include "stdarg.h"
#include "stdlib.h"
#include "evmomapl138.h"
#include "evmomapl138_pinmux.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_i2c_gpio.h"
#include "evmomapl138_emac.h"
//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
//determines which devices may be used together
static uint8_t compatibilityMatrix[10][10] =   
{
 {0,1,1,1,0,1,0,1,1,1},
 {1,0,1,1,0,0,1,0,0,1},
 {1,1,0,1,1,1,0,0,1,0},
 {1,1,1,0,1,0,1,1,1,0},
 {0,0,1,1,0,0,0,0,0,1},
 {1,0,1,0,0,0,1,0,0,0},
 {0,1,0,1,0,1,0,1,1,0},
 {1,0,0,1,0,0,1,0,0,0},
 {1,0,1,1,0,0,1,0,0,1},
 {1,1,0,0,1,0,0,0,1,0}
};

//-----------------------------------------------------------------------------
// Private Typedefs
//-----------------------------------------------------------------------------
typedef enum
{
	uidb_char_lcd = 0,
	uidb_adc = 6,
	uidb_camera = 5,
	uidb_none = 7,
	uidb_video = 3,
} uidb_decoder_e;



//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
	void UIDB_setDecoder(uidb_decoder_e device);
  uint32_t init_SVIDEO_IN(bool_e isInit);
  uint32_t init_CVIDEO_IN(bool_e isInit);
  uint32_t init_SVIDEO_OUT(bool_e isInit);
  uint32_t init_CVIDEO_OUT(bool_e isInit);
  uint32_t init_CAMERA(bool_e isInit);
  uint32_t init_CHAR_LCD(bool_e isInit);
  uint32_t init_ADC(bool_e isInit);
  uint32_t init_DAC(bool_e isInit);
  uint32_t init_RMII_ETH(bool_e isInit);
  uint32_t init_GRAPH_LCD(bool_e isInit);
  
//-----------------------------------------------------------------------------
// Function Definitoins 
//-----------------------------------------------------------------------------


//init or shutdown the a given device
uint32_t initUIDBdevice(ui_bus_devices_e device, bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
  static bool_e firstRun = true;
  static bool_e isActiveDeviceList[NUM_UIDB_DEVICES];
  uint8_t i;

  //clear the acvtive device list on first run
  if(firstRun)
  {
  	for(i = 0; i < NUM_UIDB_DEVICES; i++)
  	{
  		isActiveDeviceList[i] = false;
  	}	
  }

  //check compatibility of devices in list
  for(i = 1; i < NUM_UIDB_DEVICES;i++)
  {
  	if(isActiveDeviceList[i] && compatibilityMatrix[i][device] != 1)
    {
			//we have an issue return that the two devices are incompatible 
				return(ERR_UIDB_INCOMPAT_DEV);
		}
    	
  }
    
	//initialize the actual device
     switch(device)
      {
        case     UIDB_SVIDEO_IN:
                  rtn = init_SVIDEO_IN(isInit);            
						break;
                  
        case     UIDB_CVIDEO_IN:
                  rtn = init_CVIDEO_IN(isInit);
			            break;
                  
        case     UIDB_SVIDEO_OUT:
                  rtn = init_SVIDEO_OUT(isInit);
			            break;
                  
        case     UIDB_CVIDEO_OUT:
                  rtn = init_CVIDEO_OUT(isInit);
			            break;
                  
        case     UIDB_CAMERA:   
                  rtn = init_CAMERA(isInit);
			            break;
                  
        case     UIDB_CHAR_LCD:
                  rtn = init_CHAR_LCD(isInit);
			            break;
                  
        case     UIDB_ADC:     
                  rtn = init_ADC(isInit);
			            break;
                  
        case     UIDB_DAC:
                  rtn = init_DAC(isInit);
			            break;
                  
        case     UIDB_RMII_ETH:
                  rtn = init_RMII_ETH(isInit);
			            break;
                  
        case     UIDB_GRAPH_LCD:
                  rtn = init_GRAPH_LCD(isInit);
			            break;
                  
        default:  return ERR_UIDB_INVALID_DEVICE;
    	}
  
  //change the state of the device if everything completed correctly
  if(rtn == ERR_NO_ERROR) isActiveDeviceList[device] = isInit;
	return rtn;
}



uint32_t init_SVIDEO_IN(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_IN_REG_0, PINMUX_SVID_IN_MASK_0, PINMUX_SVID_IN_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_IN_REG_1, PINMUX_SVID_IN_MASK_1, PINMUX_SVID_IN_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_IN_REG_2, PINMUX_SVID_IN_MASK_2, PINMUX_SVID_IN_VAL_2);

		//BOARD CONFIGURATION
		UIDB_setDecoder(uidb_video);
	}	
	else
	{
		
	}
	return rtn;
}
 

uint32_t init_CVIDEO_IN(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_IN_REG_0, PINMUX_CVID_IN_MASK_0, PINMUX_CVID_IN_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_IN_REG_0, PINMUX_CVID_IN_MASK_0, PINMUX_CVID_IN_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_IN_REG_0, PINMUX_CVID_IN_MASK_0, PINMUX_CVID_IN_VAL_0);

		//BOARD CONFIGURATION
		UIDB_setDecoder(uidb_video);
		EMAC_init(EMAC_INTERFACE_RMII);			//isolate the RMII phy because it causes problems
		EMAC_phyPowerDown();

		rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);
		UIDB_setDecoder(uidb_video);
	}	
	else
	{
		
	}
	return rtn;
}


uint32_t init_SVIDEO_OUT(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_OUT_REG_0, PINMUX_SVID_OUT_MASK_0, PINMUX_SVID_OUT_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_OUT_REG_1, PINMUX_SVID_OUT_MASK_1, PINMUX_SVID_OUT_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_SVID_OUT_REG_2, PINMUX_SVID_OUT_MASK_2, PINMUX_SVID_OUT_VAL_2);

		//BOARD CONFIGURATION
		rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);
		
	}	
	else
	{
	
	}
	return rtn;
}
 
uint32_t init_CVIDEO_OUT(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_OUT_REG_0, PINMUX_CVID_OUT_MASK_0, PINMUX_CVID_OUT_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_OUT_REG_1, PINMUX_CVID_OUT_MASK_1, PINMUX_CVID_OUT_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_CVID_OUT_REG_2, PINMUX_CVID_OUT_MASK_2, PINMUX_CVID_OUT_VAL_2);
		
		//BOARD CONFIGURATION
		UIDB_setDecoder(uidb_video);
		rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);

	}	
	else
	{
	
	}
	return rtn;
}
#if 0
uint32_t init_CAMERA(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_CAMERA_REG_0, PINMUX_CAMERA_MASK_0, PINMUX_CAMERA_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_CAMERA_REG_1, PINMUX_CAMERA_MASK_1, PINMUX_CAMERA_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_CAMERA_REG_2, PINMUX_CAMERA_MASK_2, PINMUX_CAMERA_VAL_2);


		//BOARD CONFIGURATION
		EMAC_init(EMAC_INTERFACE_RMII);			//isolate the RMII phy
		EMAC_phyPowerDown();
		UIDB_setDecoder(uidb_char_lcd);
		rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);
	}	
	else
	{
	
	}
	return rtn;
}
#endif
uint32_t init_CHAR_LCD(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_0_REG, PINMUX_LCD_D_BUS_0_MASK, PINMUX_LCD_D_BUS_0_VALUE);
   		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_1_REG, PINMUX_LCD_D_BUS_1_MASK, PINMUX_LCD_D_BUS_1_VALUE);
   		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_2_REG, PINMUX_LCD_D_BUS_2_MASK, PINMUX_LCD_D_BUS_2_VALUE);
   		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_CONTROL_0_REG, PINMUX_LCD_CONTROL_0_MASK, PINMUX_LCD_CONTROL_0_VALUE);
   		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_CONTROL_1_REG, PINMUX_LCD_CONTROL_1_MASK, PINMUX_LCD_CONTROL_1_VALUE);
   		
		//BOARD CONFIGURATION
		EMAC_init(EMAC_INTERFACE_RMII);			//isolate the RMII phy
		EMAC_phyPowerDown();
		UIDB_setDecoder(uidb_char_lcd);
		rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);
		
	}
	else
	{
	
	}
	return rtn;
}

uint32_t init_ADC(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;

	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_ADC_REG_0, PINMUX_ADC_MASK_0, PINMUX_ADC_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_ADC_REG_1, PINMUX_ADC_MASK_1, PINMUX_ADC_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_ADC_REG_2, PINMUX_ADC_MASK_2, PINMUX_ADC_VAL_2);
		//BOARD CONFIGURATION
		EMAC_init(EMAC_INTERFACE_RMII);			//isolate the RMII phy
		EMAC_phyPowerDown();
		UIDB_setDecoder(uidb_adc);
		
	}
	else
	{
	
	}
	return rtn;
}


uint32_t init_DAC(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;

	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_DAC_REG_0, PINMUX_DAC_MASK_0, PINMUX_DAC_VAL_0);
		EVMOMAPL138_pinmuxConfig(PINMUX_DAC_REG_1, PINMUX_DAC_MASK_1, PINMUX_DAC_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_DAC_REG_2, PINMUX_DAC_MASK_2, PINMUX_DAC_VAL_2);
		EVMOMAPL138_pinmuxConfig(PINMUX_DAC_REG_3, PINMUX_DAC_MASK_3, PINMUX_DAC_VAL_3);

		//BOARD CONFIGURATION
		UIDB_setDecoder(uidb_adc);
		I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_HIGH);
    
	}
	else
	{
	}
	
	return rtn;
}
#if 0
uint32_t init_RMII_ETH(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;

	if(isInit)
	{

		//PINMUXING
   		EVMOMAPL138_pinmuxConfig(PINMUX_MDIO_REG, PINMUX_MDIO_MASK, PINMUX_MDIO_VAL);
    	EVMOMAPL138_pinmuxConfig(PINMUX_RMII_REG_0, PINMUX_RMII_MASK_0, PINMUX_RMII_VAL_0);
    	EVMOMAPL138_pinmuxConfig(PINMUX_RMII_REG_1, PINMUX_RMII_MASK_1, PINMUX_RMII_VAL_1);
		EVMOMAPL138_pinmuxConfig(PINMUX_MII_EN_REG, PINMUX_MMII_EN_MASK, PINMUX_MII_EN_VAL);		

		//Disable the MII MDIO clock line
    	GPIO_setDir(GPIO_BANK2, GPIO_PIN6, GPIO_OUTPUT);     
    	GPIO_setOutput(GPIO_BANK2, GPIO_PIN6, OUTPUT_HIGH);  
    
    	//Enable the RMII MDIO clock line
		I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_ETHER_PGM_ENn, OUTPUT_LOW);
   
		  
	}
	else
	{
	}
	    
	return rtn;
}
#endif
uint32_t init_GRAPH_LCD(bool_e isInit)
{
	uint32_t rtn = ERR_NO_ERROR;
	
	if(isInit)
	{
		//PINMUXING
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_0_REG, PINMUX_LCD_D_BUS_0_MASK, PINMUX_LCD_D_BUS_0_VALUE);
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_1_REG, PINMUX_LCD_D_BUS_1_MASK, PINMUX_LCD_D_BUS_1_VALUE);
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_D_BUS_2_REG, PINMUX_LCD_D_BUS_2_MASK, PINMUX_LCD_D_BUS_2_VALUE);
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_CONTROL_0_REG, PINMUX_LCD_CONTROL_0_MASK, PINMUX_LCD_CONTROL_0_VALUE);
		EVMOMAPL138_pinmuxConfig(PINMUX_LCD_CONTROL_1_REG, PINMUX_LCD_CONTROL_1_MASK, PINMUX_LCD_CONTROL_1_VALUE);
	}	
	else
	{
	
	}
	return rtn;
}

//--------------------------------------------------------------
// Set the gpio expander st. the appropriate device is enabled
//--------------------------------------------------------------
void UIDB_setDecoder(uidb_decoder_e device)
{
	uint8_t pinVal;
	pinVal = (uint8_t)(device & 0x01);	
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELA, pinVal);
	pinVal = (uint8_t)((device>>1)& 0x01);	
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELB, pinVal);
	pinVal = (uint8_t)((device>>2)& 0x01);
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELC, pinVal); 
}
#endif 
