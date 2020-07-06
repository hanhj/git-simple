//-----------------------------------------------------------------------------
// \file    evmomapl138_uidatabusmux.h
// \brief   setup freon for use of peripherals on ui vpif_in/vpif_out busses
//          
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_UIDATABUSMUX
#define EVMOMAPL138_UIDATABUSMUX

#if defined (__cplusplus)
extern "C" {
#endif
//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------

#define NUM_UIDB_DEVICES 10
//define devices
typedef enum
{
  UIDB_SVIDEO_IN = 0,
  UIDB_CVIDEO_IN = 1,
  UIDB_SVIDEO_OUT= 2,
  UIDB_CVIDEO_OUT= 3,
  UIDB_CAMERA    = 4,
  UIDB_CHAR_LCD  = 5,
  UIDB_ADC       = 6,
  UIDB_DAC       = 7,
  UIDB_RMII_ETH  = 8,
  UIDB_GRAPH_LCD = 9

}ui_bus_devices_e;
 
//-----------------------------------------------------------------------------
// Public Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief  	intializes the bus for use of the specified device. 
//
// \param   ui_bus_devices_e device - the device to be initialized.
//
// \param   bool_e isInit - true: indicates that the bus is to be initialized for
//							use with the device. False: indicates that the device
//							is finished with the bus and should only be called after
//							the actual device has been shutdown.
//
// \return  uint32_t
//    ERR_NO_ERROR - everything is ok with the init
//    else - the device was not initialized or initialization failed. 
//-----------------------------------------------------------------------------
uint32_t initUIDBdevice(ui_bus_devices_e device, bool_e isInit);

#if defined (__cplusplus)
}
#endif
#endif

